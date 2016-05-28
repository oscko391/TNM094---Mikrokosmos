//
//  PhotoCard.h
//  SDL_tutorial
//
//  Created by My Rydberg on 2016-04-17.
//  Copyright (c) 2016 My. All rights reserved.
//

#ifndef SDL_tutorial_PhotoCard_h
#define SDL_tutorial_PhotoCard_h



#include <string>
#include <vector>
#include <SDL2_ttf/SDL_ttf.h>
#include "Card.h"

class PhotoCard : public Card
{
private:
    int texIndex;
    static int tex_generator;
    
    static std::vector<SDL_Texture*> headSv;
    static std::vector<SDL_Texture*> headEn;
    static std::vector<SDL_Texture*> readSv;
    static std::vector<SDL_Texture*> readEn;
    
    void loadingTex(SDL_Renderer *r, std::string path);
    
public:
    PhotoCard();
    PhotoCard(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r);
    ~PhotoCard();
    
    int getTexIndex() const;
    
    void setTexIndex(int i);
    
    void render( SDL_Renderer* gRenderer, bool swede);
};

#endif
