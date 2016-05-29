#ifndef SDL_tutorial_CardHandler_h
#define SDL_tutorial_CardHandler_h



#include "Card.h"
#include "PhotoCard.h"
#include "StoryHandler.h"
#include "Category.h"
#include "Menu.h"

#include "rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */


class CardHandler
{
private:
    // variables
    std::vector<SDL_Event> frameEvents;
    Menu theMenu;
    float addedTime; // change category when home and change back to home when story
    
    // home
    std::vector<Category> vecCat;
    std::vector<std::vector<Card*>> catCard;
    std::vector<Card*> currentCards;
    
    // story
    bool isStory;
    StoryHandler allStories;
    std::vector<std::string> swedishNames;
    std::vector<std::string> englishNames;
    
    
    // init for cards and 
    bool readXml(std::string filePath, SDL_Renderer* r);
    bool readStoryXml(std::string filePath, SDL_Renderer* r);

public:
    CardHandler(std::string mediaPath, std::string storyPath, SDL_Renderer* r);

    std::vector<Card*> getCurrentCard();
    std::vector<Card*> getAll();
    std::vector<SDL_Event> getFrameEvents();
    Menu getMenu();

    void addEvent(SDL_Event e);

    void HandleEvents(bool &lang);
    void changeCat(float t);

    void sort();

    void render(SDL_Renderer* r, bool swede);
    
    void clearEvents();


};

#endif // CARDHANDLER_H_INC
