#include "StoryHandler.h"

StoryHandler::StoryHandler( SDL_Renderer* r )
{
    currentStory = 0;
    currentCard = 0;
    sideArrow = loadArrow(r);
}

SDL_Texture* StoryHandler::loadArrow(SDL_Renderer* r){
    SDL_Texture* imgTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/icons/arrow2.png" );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image arrow! SDL_image Error: %s\n", IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        imgTexture = SDL_CreateTextureFromSurface( r, loadedSurface );
        if( imgTexture == NULL )
        {
            printf( "Unable to create texture from arrow ...! SDL Error: %s\n",  SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return imgTexture;
}

bool StoryHandler::handleEvent(SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN ) {
        int x, y;
        SDL_GetMouseState( &x, &y );
        return internalEvent(x, y);
    }
    /*
     if (e->type == SDL_FINGERDOWN ) {
     int x = e->tfinger.x * SCREEN_WIDTH;
     int y = e->tfinger.y * SCREEN_HEIGHT;
     return internalEvent(x, y);
     }*/
    return false;
}

bool StoryHandler::internalEvent(int x, int y) {
    // right arrow {SCREEN_WIDTH/20, SCREEN_HEIGHT/2 - 20, 40, 40};
    /*if (isFlipp) {
        x = SCREEN_WIDTH - x;
    }*/
    if (currentCard > 0 && x < SCREEN_WIDTH/20 + 40 && x > SCREEN_WIDTH/20 && y > SCREEN_HEIGHT/2 - 20 && y < SCREEN_HEIGHT/2 + 40) {
        currentCard--;
        return true;
        
    }
    
    // left arrow {SCREEN_WIDTH - SCREEN_WIDTH/20 - 40, SCREEN_HEIGHT/2 - 20, 40, 40}
    else if (currentCard < sizeStory-1 && x < SCREEN_WIDTH - SCREEN_WIDTH/20 && x > SCREEN_WIDTH - SCREEN_WIDTH/20 - 40 && y > SCREEN_HEIGHT/2 - 20 && y < SCREEN_HEIGHT/2 + 40) {
        currentCard++;
        return true;
        
    }
    return false;
}


bool StoryHandler::readXml(std::string filePath, SDL_Renderer* r, std::vector<std::string> &svNames, std::vector<std::string> &enNames) {
    
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
    
    
    rapidxml::xml_node<>* content = doc.first_node("content");
    
    int i = 0;
    for (rapidxml::xml_node<>* storyNode = content->first_node("story"); storyNode != nullptr; storyNode = storyNode->next_sibling())
    {
        storyCards.push_back(std::vector<StoryCard*>());
        // Indicra alla variabler som category behöver
        std::string texSv;
        std::string texEn;
        
        std::stringstream ss1;
        std::stringstream ss2;
        ss1 << storyNode->first_attribute("tex_se")->value();
        getline( ss1, texSv);
        
        ss2 << storyNode->first_attribute("tex_en")->value();
        getline( ss2, texEn);
        
        svNames.push_back(texSv);
        enNames.push_back(texEn);
        
        for (rapidxml::xml_node<>* media = storyNode->first_node("media"); media != nullptr; media = media->next_sibling())
        {
            std::string mediaPath = media->first_attribute("path")->value();
            
            
            std::string seHeader;
            std::string seText;
            std::string enHeader;
            std::string enText;
            std::stringstream ssSv;
            std::stringstream ssEn;
            for (rapidxml::xml_node<>* inside = media->first_node(); inside ; inside = inside->next_sibling()) {
                
                std::string b = inside->name();
                if (b == "se")
                {
                    seHeader = inside->first_node()->value() ;
                    
                    ssSv << inside->first_node()->next_sibling()->value();
                    std::string get;
                    getline(ssSv, get);
                    seText.append(get);

                }
                else
                {
                    enHeader = inside->first_node()->value() ;
                    
                    ssEn << inside->first_node()->next_sibling()->value();
                    std::string get;
                    getline(ssEn, get);
                    enText.append(get);

                }
                
            }
            StoryCard* newCard = new StoryCard(seHeader, seText, enHeader, enText, mediaPath, r);
            storyCards[i].push_back(newCard);
            
        }
        i++;
    }

    sizeStory = storyCards[currentStory].size();
    return true;
}

void StoryHandler::render(SDL_Renderer* r, bool lang) {
    storyCards[currentStory][currentCard]->render(r, lang, isFlipp);
    if (currentCard < sizeStory - 1) {
        SDL_Rect leftArrow = {SCREEN_WIDTH - SCREEN_WIDTH/20 - 40, SCREEN_HEIGHT/2 - 20, 40, 40};
        SDL_RenderCopyEx(r, sideArrow, NULL, &leftArrow, 270, NULL, SDL_FLIP_NONE);
    }
    if (currentCard > 0) {
        SDL_Rect rightArrow = {SCREEN_WIDTH/20, SCREEN_HEIGHT/2 - 20, 40, 40};
        SDL_RenderCopyEx(r, sideArrow, NULL, &rightArrow, 90, NULL, SDL_FLIP_NONE);
    }

}

void StoryHandler::setStory(int s) {
    if (currentStory != s) {
        currentStory = s;
        currentCard = 0;
        sizeStory = storyCards[currentStory].size();
    }
}

void StoryHandler::resetStory() {
    currentStory = 0;
    currentCard = 0;
    sizeStory = storyCards[0].size();
}



bool& StoryHandler::getFlipp() {
    return isFlipp;
}

