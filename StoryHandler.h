#ifndef SDL_tutorial_StoryHandler_h
#define SDL_tutorial_StoryHandler_h



#include "Card.h"
#include "PhotoCard.h"
#include "Category.h"
#include "Menu.h"
#include "StoryCard.h"

#include "rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */


class StoryHandler
{
private:
    std::vector<std::vector<StoryCard*>> storyCards;
    
    int currentStory;
    int currentCard;
    int sizeStory;
    
    bool isFlipp = false;
    
    bool internalEvent(int x, int y);
    SDL_Texture* loadArrow(SDL_Renderer* r);
    
    SDL_Texture* sideArrow;
    
public:
    StoryHandler(SDL_Renderer* r);

    void render(SDL_Renderer* r, bool lang);
    bool handleEvent(SDL_Event* e);
    bool readXml(std::string filePath, SDL_Renderer* r, std::vector<std::string> &svNames, std::vector<std::string> &enNames);
    
    void setStory(int s);
    bool& getFlipp();
    
};

#endif // CARDHANDLER_H_INC
