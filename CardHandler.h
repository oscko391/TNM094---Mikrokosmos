#ifndef CARDHANDLER_H_INCLUDED
#define CARDHANDLER_H_INCLUDE



#include "Card.h"
#include "rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

class CardHandler
{
private:
    std::vector<Card> vecCard;
    std::vector<SDL_Event> frameEvents;
    bool readXml(std::string filePath);

public:
    CardHandler(std::string filePath);

    std::vector<Card> getVecCard();
    std::vector<SDL_Event> getFrameEvents();

    void addEvent(SDL_Event e);

    void HandleEvents();




};

#endif // CARDHANDLER_H_INC
