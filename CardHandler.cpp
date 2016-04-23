#include "CardHandler.h"

CardHandler::CardHandler(std::string filePath, SDL_Renderer* r)
{
    readXml(filePath, r);
    currentCards = vecCard;
    pixPerCat = 1100/(vecCat.size() + 1);
}

std::vector<Card*> CardHandler::getVecCard()
{
    return currentCards;
}

// loops over all the cards and then loops over all the events and handles them
void CardHandler::HandleEvents()
{
    for (int indi = 0; indi < frameEvents.size(); indi++)
    {
        if (menuEvent(&frameEvents[indi])) {
            frameEvents.erase(frameEvents.begin() + indi);
        }
        else {
            for (int indo = 0; indo < currentCards.size(); indo++)
            {
                //removing an event if used so only one card can be affected by a specific event
                if(currentCards[indo]->handleEvent(&frameEvents[indi]))
                    frameEvents.erase(frameEvents.begin() + indi);
            }
        }
    }
}

bool CardHandler::menuEvent(SDL_Event* e){
    // SDL_Rect menuQuad = {x , y, 1100, 50 };
    if (e->type == SDL_MOUSEBUTTONDOWN ) {
        //int pixPerCat = 1100/(vecCat.size() + 1);
        int p = 50 + pixPerCat;
        int x, y;
        SDL_GetMouseState( &x, &y );
        if (x > 50 && x < 1150 && y > 640 && y < 690) {
            if (x < p) {
                currentCards = vecCard;
                return true;
            }
            for (int i = 0; i < vecCat.size(); i++) {
                p += pixPerCat;
                if (x < p) {
                    currentCards = catCard[i];
                    return true;
                }
            }
        }
    }
    if (e->type == SDL_FINGERDOWN ) {
        int pixPerCat = 1100/(vecCat.size() + 1);
        int p = 50 + pixPerCat;
        int x = e->tfinger.x * SCREEN_WIDTH;
        int y = e->tfinger.y * SCREEN_HEIGHT;
        if (x > 50 && x < 1150 && y > 640 && y < 690) {
            if (x < p) {
                currentCards = vecCard;
                return true;
            }
            for (int i = 0; i < vecCat.size(); i++) {
                p += pixPerCat;
                if (x < p) {
                    currentCards = catCard[i];
                    return true;
                }
            }
        }
    }
    return false;
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
        
        std::stringstream ss;
        ss << second->first_attribute("tex_se")->value();
        ss >> texSv;
        
        ss << second->first_attribute("tex_en")->value();
        ss >> texEn;
        
        ss << second->first_attribute("path")->value();
        ss >> catPath;
        
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
                seText = inside->first_node()->next_sibling()->value();
            }
            else
            {
                enHeader = inside->first_node()->value() ;
                enText = inside->first_node()->next_sibling()->value();
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
        vecCard.push_back(newCard);
        
        // adding the card to categories vector
        for (int i = 0; i < cardCat.size(); i++) {
            for (int j = 0; j < vecCat.size(); j++) {
                if (vecCat[j].getCatName() == cardCat[i]) {
                    catCard[j].push_back(newCard);
                }
            }
        }
    }
    return true;
}

bool compCard( Card* a,  Card* b) {return a->getLifeTime() > b->getLifeTime(); }
void CardHandler::sort(){
    std::sort(currentCards.begin(), currentCards.end(), compCard);
}

void CardHandler::renderMenu(SDL_Renderer* r)
{
    SDL_Rect menuQuad = {50 , 640, 1100, 50 };
    
    //SDL_RenderCopy( gRenderer, theTextures[texIndex], NULL, &renderQuad );
    
    SDL_SetRenderDrawColor( r, 0x30, 0x30, 0x30, 0xCC );
    SDL_RenderFillRect( r, &menuQuad );
    
    SDL_Rect edge = {50, 640, pixPerCat, 50 };
    SDL_SetRenderDrawColor( r, 0x00, 0x00, 0x30, 0xFF );
    SDL_RenderDrawRect(r, &edge);
    for (int i = 0; i < vecCat.size(); i++) {
        edge = {51 + i + pixPerCat*(i+1), 640, pixPerCat, 50 };
        SDL_RenderDrawRect(r, &edge);
    }
}
