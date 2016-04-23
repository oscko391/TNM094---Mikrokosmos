#ifndef SDL_tutorial_CardHandler_h
#define SDL_tutorial_CardHandler_h



#include "Card.h"
#include "PhotoCard.h"
#include "Category.h"

#include "rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

class CardHandler
{
private:
    std::vector<Card*> vecCard;
    std::vector<Category> vecCat;
    std::vector<std::vector<Card*>> catCard;
    std::vector<Card*> currentCards;
    
    std::vector<SDL_Event> frameEvents;
    
    bool readXml(std::string filePath, SDL_Renderer* r);
    
    bool menuEvent(SDL_Event* e);
    int pixPerCat; //1100/(vecCat.size() + 1);
    
public:
    CardHandler(std::string filePath, SDL_Renderer* r);
    
    std::vector<Card*> getVecCard();
    std::vector<SDL_Event> getFrameEvents();
    
    void addEvent(SDL_Event e);
    
    void HandleEvents();
    
    void sort();
    
    void renderMenu(SDL_Renderer* r);
    
    
};

#endif // CARDHANDLER_H_INC