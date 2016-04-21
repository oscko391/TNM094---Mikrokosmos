//
//  PhotoCard.h
//  SDL_tutorial
//
//  Created by My Rydberg on 2016-04-17.
//  Copyright (c) 2016 My. All rights reserved.
//

#ifndef SDL_tutorial_PhotoCard_h
#define SDL_tutorial_PhotoCard_h

#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Card.h"

class PhotoCard : public Card
{
private:
    int texIndex;
    std::string imgPath;
    static std::vector<SDL_Texture*> theTextures;
    
    SDL_Texture* loadingTex(SDL_Renderer *r, std::string path);
    
public:
    PhotoCard();
    PhotoCard(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r);
    ~PhotoCard();
    
    int getTexIndex() const;
    
    //void setImgPath(std::string s);
    void setTexIndex(int i);
    
    void render( SDL_Renderer* gRenderer);
};

#endif
