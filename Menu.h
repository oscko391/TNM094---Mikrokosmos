//
//  Menu.h
//
//
//  Created by My Rydberg on 2016-04-28.

#ifndef SDL_tutorial_Menu_h
#define SDL_tutorial_Menu_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>// ?
#include <string> // ?
#include <vector>
#if defined (__APPLE_CC__) //if apple dator
#include <SDL2/SDL.h>
//#include </Users/my/Documents/LiU/Kandidat/SDL_tutorial/glm/glm/glm.hpp> //My
#include "glm.hpp" //Madde
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else //annars windows version
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <SDL_ttf.h>
#endif

#include "settings.h"
#include "Category.h"
#include "StoryHandler.h"


class Menu
{
private:
    int posX;
    int posY;
    int width;
    int height;
    
    bool isMoving = false;
    int square;
    int pressed = 0;
    
    int categories;
    int pixPerCat;
    int currentCat;
    
    int stories;
    int pixPerStory;
    int currentStory;
    
    std::vector<SDL_Texture*> svCat;
    std::vector<SDL_Texture*> enCat;
    std::vector<SDL_Texture*> svStory;
    std::vector<SDL_Texture*> enStory;
    
    SDL_Texture* homeIcon;
    SDL_Texture* storyIcon;
    SDL_Texture* updownIcon;
    
    //bool isSwedish;
    
    bool loadingText(SDL_Renderer* r, std::vector<Category> cats, std::vector<std::string> sv, std::vector<std::string> en);
    void loadingTexture(SDL_Renderer* r);
    
    bool downEvent(int x, int y, bool &lang, bool &story, bool &flipp);
    bool moveEvent(int x, int y, bool story);
    bool upEvent(bool story);
    
public:
    //Initializes variables
    Menu(){};
    Menu(int h, SDL_Renderer* r, std::vector<Category> cats, std::vector<std::string> sv, std::vector<std::string> en);
    
    //Deallocates memory
    ~Menu();
    
    void renderMenu(SDL_Renderer* r, bool lang, bool story);
    
    bool handleEvent( SDL_Event* e , bool &lang );
    bool handleEvent( SDL_Event* e , bool &lang, bool &story, bool &flipp);
    
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    int getCat();
    int getStory();
    void changeCategory();
    //bool getSwedish();
    
    
    
};

#endif