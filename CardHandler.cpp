#include "CardHandler.h"

CardHandler::CardHandler(std::string mediaPath, std::string storyPath, SDL_Renderer* r)
:allStories(StoryHandler(r))
{
    readXml(mediaPath, r);
    currentCards = catCard[0];
    allStories.readXml(storyPath, r, swedishNames, englishNames);
    theMenu = Menu(35, r, vecCat, swedishNames, englishNames);
    addedTime = 2.0;
    isStory = false;
}

std::vector<Card*> CardHandler::getCurrentCard()
{
    return currentCards;
}

std::vector<Card*> CardHandler::getAll()
{
    return catCard[0];
}

Menu CardHandler::getMenu()
{
    return theMenu;
}

// loops over all the cards and then loops over all the events and handles them
void CardHandler::HandleEvents(bool &lang)
{
    
    for (int indo = 0; indo < currentCards.size(); indo++)
    {
        for (int indi = 0; indi < frameEvents.size(); indi++) {
            //removing an event if used so only one card can be affected by a specific event
            if (theMenu.handleEvent(&frameEvents[indi], lang, isStory, allStories.getFlipp())) {
                currentCards = catCard[theMenu.getCat()];
                allStories.setStory(theMenu.getStory());
                frameEvents.erase(frameEvents.begin() + indi);
                addedTime = 2.0;
            }
            else if(!isStory && currentCards[indo]->handleEvent(&frameEvents[indi])) {
                frameEvents.erase(frameEvents.begin() + indi);
                addedTime = 2.0;
            }
            else if (isStory && allStories.handleEvent(&frameEvents[indi])) {
                frameEvents.erase(frameEvents.begin() + indi);
                addedTime = 2.0;
            }
            
        }
    }
}



std::vector<SDL_Event> CardHandler::getFrameEvents()
{
    return frameEvents;
}

void CardHandler::addEvent(SDL_Event e)
{
    frameEvents.push_back(e);
}


bool CardHandler::readXml(std::string filePath, SDL_Renderer* r)
{
    // getting the string through an ifstream
    std::ifstream ifs(filePath, std::ios::in);
    if(!ifs.is_open())
    {
        return false;
    }
    
    // making it the right format (char) for the parsing to xml doc
    std::string xml_str;
    xml_str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    
    // creating a xml_document and parsing the xml-file, making a DOM-tree
    rapidxml::xml_document<> doc;
    try
    {
        doc.parse<0>((char*)xml_str.c_str());
    }
    catch (...)
    {
        return false;
    }
    
    
    // accesing the first node in the file
    rapidxml::xml_node<>* content = doc.first_node("content");
    
    ///--------CATEGORY----------///
    // accesing all category nodes in content
    for (rapidxml::xml_node<>* second = content->first_node("category"); second <= content->last_node("category"); second = second->next_sibling())
    {
        std::string cName = second->first_attribute("name")->value();
        
        // Indicra alla variabler som category behöver
        std::string texSv;
        std::string texEn;
        std::string catPath;
        
        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;
        ss1 << second->first_attribute("tex_se")->value();
        getline( ss1, texSv);
        
        ss2 << second->first_attribute("tex_en")->value();
        getline( ss2, texEn);
        
        ss3 << second->first_attribute("path")->value();
        ss3 >> catPath;
        
        vecCat.push_back(Category(cName, texSv, texEn, catPath));
        catCard.push_back(std::vector<Card*>());
    }
    
    
    ///--------------MEDIA--------------///
    // accesing all media nodes
    for (rapidxml::xml_node<>* second = content->first_node("media"); second; second = second->next_sibling())
    {
        // writes out inforamtion about the media to the terminal and creates variables to use for creation of cards
        
        std::string mediaPath = second->first_attribute("path")->value();
        
        std::stringstream ss;
        int scaleExp;
        ss << second->first_attribute("scale_exp")->value();
        ss >> scaleExp;
        
        std::vector<std::string> cardCat;
        std::string seHeader;
        std::string seText;
        std::string enHeader;
        std::string enText;
        std::stringstream sSv;
        std::stringstream sEn;
        for (rapidxml::xml_node<>* inside = second->first_node(); inside ; inside = inside->next_sibling())
        {
            
            std::string b = inside->name();
            if (b == "category")
            {
                std::string cat = inside->first_attribute()->value();
                cardCat.push_back(cat);
            }
            else if (b == "se")
            {
                seHeader = inside->first_node()->value() ;
                
                sSv << inside->first_node()->next_sibling()->value();
                std::string get;
                getline(sSv, get);
                seText.append(get);
                seText.append("\n\n");
                getline(sSv, get);
                seText.append(get);
            }
            else
            {
                enHeader = inside->first_node()->value() ;
                
                sEn << inside->first_node()->next_sibling()->value();
                std::string get;
                getline(sEn, get);
                enText.append(get);
                enText.append("\n\n");
                getline(sEn, get);
                enText.append(get);
            }
            
        }
        
        // create card with variables
        glm::vec3 position = glm::vec3(rand() % rand() % (SCREEN_WIDTH /2) + (SCREEN_WIDTH /4),rand() % (SCREEN_HEIGHT /2) + (SCREEN_HEIGHT /4),0);
        glm::vec2 velocity;
        int random = rand() % 4;
        int fact = 100;
        if (random == 0)
        {
            velocity = glm::vec2(fact*(rand() % 4 + 1) , fact*(rand() % 4 + 1));
        }
        else if (random == 1)
        {
            velocity = glm::vec2(-fact*(rand() % 4 + 1), fact*(rand() % 4 + 1));
        }
        else if (random == 2)
        {
            velocity = glm::vec2(fact*(rand() % 4 + 1) , -fact*(rand() % 4 + 1));
        }
        else
        {
            velocity = glm::vec2(-fact*(rand() % 4 + 1), -fact*(rand() % 4 + 1));
        }
        
        
        PhotoCard* newCard = new PhotoCard(cardCat, seHeader, seText, enHeader, enText, position, velocity, mediaPath, r);
        //catCard[0].push_back(newCard); // Insert all cards in catCard[0]
        
        // adding the card to categories vector
        for (int i = 0; i < cardCat.size(); i++) {
            for (int j = 0; j < vecCat.size(); j++) { // j=1 because catCard[0] contains all cards
                if (vecCat[j].getCatName() == cardCat[i]) {
                    catCard[j].push_back(newCard);
                }
            }
        }
    }
    return true;
}



bool CardHandler::readXmlSettings(std::string filePath, SDL_Renderer* r)
{
    std::cout << "xml" << std::endl;
    // getting the string through an ifstream
    std::ifstream ifs(filePath, std::ios::in);
    if(!ifs.is_open())
    {
        return false;
    }
    
    // making it the right format (char) for the parsing to xml doc
    std::string xml_str;
    xml_str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    
    // creating a xml_document and parsing the xml-file, making a DOM-tree
    rapidxml::xml_document<> doc;
    try
    {
        doc.parse<0>((char*)xml_str.c_str());
    }
    catch (...)
    {
        return false;
    }
    
    
    // accesing the first node in the file
    rapidxml::xml_node<>* content = doc.first_node("content");
    
    ///-------SETTINGS------------///
    for (rapidxml::xml_node<>* second = content->first_node("CardHandler"); second <= content->last_node("CardHandler"); second = second->next_sibling())
    {
        // Backgorundcolors
        unsigned int bColorR;
        std::stringstream sR;
        sR << std::hex << second->first_attribute("background_color_r")->value();
        sR >> std::hex  >> bColorR;
        backgorundColor[0] = bColorR;
        
        unsigned int bColorG;
        std::stringstream sG;
        sG << std::hex << second->first_attribute("background_color_g")->value();
        sG >> std::hex >> bColorG;
        backgorundColor[1] = bColorG;
        
        unsigned int bColorB;
        std::stringstream sB;
        sB << std::hex << second->first_attribute("background_color_b")->value();
        sB >> std::hex >> bColorB;
        backgorundColor[2] = bColorB;
        
        unsigned int   bColorA;
        std::stringstream sA;
        sA << std::hex << second->first_attribute("background_color_a")->value() << std::hex;
        sA >> std::hex >>bColorA;
        backgorundColor[3] = bColorA;
    }
    
    return true;
} //------


void CardHandler::changeCat(float t) {
    
    addedTime += t;
    
    if (static_cast<int>(addedTime)%20 == 0) {
        isStory = false;
        theMenu.changeCategory();
        currentCards = catCard[theMenu.getCat()];
        allStories.setStory(0);
        addedTime = 2.0;
    }
}

bool compCard( Card* a,  Card* b) {return a->getLifeTime() > b->getLifeTime(); }
void CardHandler::sort(){
    std::sort(currentCards.begin(), currentCards.end(), compCard);
}

void CardHandler::render(SDL_Renderer* r, bool swede) {
    if (!isStory) {
        sort();
        for (int i = currentCards.size() - 1; i >= 0; i--) {
            if (currentCards[i]->getLifeTime() < clock()) {
                currentCards[i]->move(0.001); // (start - clock())/(CLOCKS_PER_SEC*2)
            }
            currentCards[i]->render(r, swede); // theTextures[theCards[i].texIndex]
        }
    }
    else {
        allStories.render(r, swede);
    }
    
    theMenu.renderMenu(r, swede, isStory);
    
}


void CardHandler::clearEvents()
{
    frameEvents.clear();
}
