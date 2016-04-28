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
    
    SDL_RenderCopyEx( gRenderer, theTextures[texIndex], NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE );

}

SDL_Texture* PhotoCard::loadingTex(SDL_Renderer* r, std::string path)
{
    //The final texture
    SDL_Texture* imgTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        imgTexture = SDL_CreateTextureFromSurface( r, loadedSurface );
        if( imgTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    int w, h;
    SDL_QueryTexture(imgTexture, NULL, NULL, &w, &h);
    
    SDL_Texture* newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_Rect renderQuad = {w/50 , w/50, w, h };
    SDL_RenderCopyEx( r, imgTexture , NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE );
    
    SDL_Rect headQuad = {w/25, w/25  , w - w/25  , w / 10};
    SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( r, &headQuad );
    SDL_RenderCopyEx(r, getHeader(), NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    SDL_SetRenderTarget( r, NULL );
    
    return newTexture;
}