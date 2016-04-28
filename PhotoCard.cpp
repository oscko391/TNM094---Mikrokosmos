//
//  PhotoCard.cpp
//  SDL_tutorial
//
//  Created by My Rydberg on 2016-04-17.
//  Copyright (c) 2016 My. All rights reserved.
//


#include "PhotoCard.h"

PhotoCard::PhotoCard()
{
    Card();
}

PhotoCard::PhotoCard(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r)
    : Card(inCat, inSvH, inSvT, inEnH, inEnT, inPos, inVel, textPath, r)
{
    texIndex = tex_generator++;
    theTextures.push_back(loadingTex(r, textPath));
    int w, h;
    SDL_QueryTexture(theTextures[texIndex], NULL, NULL, &w, &h);
    double f = 170.0 / w;
    setWidth(f * w);
    setHeight(f * h);
}

PhotoCard::~PhotoCard()
{
    //Card::~Card();
}


int PhotoCard::getTexIndex() const
{
    return texIndex;
}

void PhotoCard::setTexIndex(int i)
{
    texIndex = i;
}

void PhotoCard::render( SDL_Renderer* gRenderer) // Blir error atm
{
    //Set rendering space and render to screen
    
    SDL_Rect renderQuad = {static_cast<int>(getPos().x) , static_cast<int>(getPos().y), getWidth(), getHeight() };
    SDL_Rect whiteQuad = {static_cast<int>(getPos().x) - 5, static_cast<int>(getPos().y) - 5, getWidth() + 10, getHeight() + 10 };
    SDL_Rect blackQuad = {static_cast<int>(getPos().x) - 10, static_cast<int>(getPos().y) - 10, getWidth() + 20, getHeight() + 20 };
    SDL_Rect headQuad = {static_cast<int>(getPos().x) + getWidth()/60 , static_cast<int>(getPos().y) + getWidth()/60  , getWidth() - getWidth()/30  , getWidth() / 10};

    if (getReading()) {
        headQuad = {static_cast<int>(getPos().x) + getWidth()/60 , static_cast<int>(getPos().y) + getWidth()/60 , getWidth() - getWidth()/30 , getHeight() - getWidth()/30};
    }
    
    //SDL_Rect headText =
    SDL_SetRenderDrawColor( gRenderer, 0x10, 0x20, 0x50, 0x90 );
    
//    SDL_SetRenderDrawColor( gRenderer, 0x10, 0x20, 0x50, 0x35 );
    SDL_RenderFillRect( gRenderer, &blackQuad );
    
    SDL_SetRenderDrawColor( gRenderer, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( gRenderer, &whiteQuad );
    
    SDL_RenderCopy( gRenderer, theTextures[texIndex], NULL, &renderQuad );
    
    SDL_SetRenderDrawColor( gRenderer, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( gRenderer, &headQuad );
    
    SDL_RenderCopyEx(gRenderer, getHeader(), NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
}

SDL_Texture* PhotoCard::loadingTex(SDL_Renderer* r, std::string path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( r, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return newTexture;
}