//
//  StoryCard.h
//  SDL_tutorial
//
//  Created by My Rydberg on 2016-04-17.
//  Copyright (c) 2016 My. All rights reserved.
//


#ifndef SDL_tutorial_StoryCard_h
#define SDL_tutorial_StoryCard_h



#include <string>
#include <vector>
#include <SDL2_ttf/SDL_ttf.h>
#include "Card.h"

class StoryCard : public Card
{
private:
    int texIndex;
    static int tex_generator;
    
    static std::vector<SDL_Texture*> image;
    static std::vector<SDL_Texture*> swedish;
    static std::vector<SDL_Texture*> english;
    
    void loadingTex(SDL_Renderer *r, std::string path);
    
public:
    StoryCard();
    StoryCard(std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, std::string textPath, SDL_Renderer* r);
    ~StoryCard();
    
    int getTexIndex() const;
    
    void setTexIndex(int i);
    
    void render( SDL_Renderer* r, bool swede, bool flipp);
};

#endif
