//
//  StoryCard.cpp
//  SDL_tutorial
//
//  Created by My Rydberg on 2016-04-17.
//  Copyright (c) 2016 My. All rights reserved.
//


#include "StoryCard.h"

StoryCard::StoryCard()
{
    Card();
}

StoryCard::StoryCard(std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, std::string textPath, SDL_Renderer* r)
: Card(std::vector<std::string>(), inSvH, inSvT, inEnH, inEnT, glm::vec3(), glm::vec2(), textPath, r)
{
    texIndex = tex_generator++;
    loadingTex(r, textPath);
    int w, h;
    SDL_QueryTexture(image[texIndex], NULL, NULL, &w, &h);
    double f = (2*SCREEN_WIDTH/5.0) / w;
    w *= f;
    h *= f;
    if (h > (2*SCREEN_HEIGHT/3.0)) {
        f = (2*SCREEN_HEIGHT/3.0) / h;
        w *= f;
        h *= f;
    }
    setWidth( w);
    setHeight( h);

}

StoryCard::~StoryCard()
{
    //Card::~Card();
}


int StoryCard::getTexIndex() const
{
    return texIndex;
}

void StoryCard::setTexIndex(int i)
{
    texIndex = i;
}

void StoryCard::render( SDL_Renderer* r, bool swede, bool flipp)
{
    //Set rendering space and render to screen
    
    SDL_Rect renderQuad = {SCREEN_WIDTH/2 - getWidth() - 10 , SCREEN_HEIGHT/2 - getHeight()/2 - 20 , getWidth(), getHeight() };
    SDL_Rect renderTextQuad = {SCREEN_WIDTH/2 + 10, SCREEN_HEIGHT/2 - getHeight()/2 - 20 , getWidth(), getHeight() };
    
    if (flipp) {
        SDL_RenderCopyEx( r, image[texIndex], NULL, &renderQuad, 180, NULL, SDL_FLIP_NONE );
        if (swede) {
            SDL_RenderCopyEx(r, swedish[texIndex], NULL, &renderTextQuad, 180, NULL, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopyEx(r, english[texIndex], NULL, &renderTextQuad, 180, NULL, SDL_FLIP_NONE);
        }

    }
    else {
        SDL_RenderCopyEx( r, image[texIndex], NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE );
        if (swede) {
            SDL_RenderCopy(r, swedish[texIndex], NULL, &renderTextQuad);
        }
        else {
            SDL_RenderCopy(r, english[texIndex], NULL, &renderTextQuad);
        }
    }
    

    

    
}

void StoryCard::loadingTex(SDL_Renderer* r, std::string path)
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
    
    // The image
    SDL_Texture* newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_Rect renderQuad = {w/50 , w/50, w, h };
    SDL_RenderCopyEx( r, imgTexture , NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE );
    
    image.push_back(newTexture);
    
    // Swedish text
    newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    int header_w, header_h;
    SDL_QueryTexture(headersSv[infoIndex], NULL, NULL, &header_w, &header_h);
    double f = 1.0;
    if (header_w > w - w/25) {
        f = (w - w/25)/static_cast<double>(header_w);;
        header_w *= f;
        header_h *= f;
    }
    if (header_h > w / 10) {
        f = (w / 10)/static_cast<double>(header_h);;
        header_w *= f;
        header_h *= f;
    }
    int smaller_w = ((w - w/25) - header_w)/2;
    int smaller_h = ((w/10) - header_h)/2;
    
    SDL_Rect headQuad = {w/25, w/25  , w - w/25  , w/10 + 5};
    //SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    //SDL_RenderFillRect( r, &headQuad );
    
    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, headersSv[infoIndex], NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    int text_w, text_h;
    SDL_QueryTexture(infoTextSv[infoIndex], NULL, NULL, &text_w, &text_h);
    if (text_w > w - w/25 - 20) {
        f = (w - w/25 - 20)/static_cast<double>(text_w);
        text_w *= f;
        text_h *= f;
    }
    if (text_h > h - w/25 - w/10 - w/20 - 20 ) {
        f = (h - w/25 - w/10 - w/20 - 20)/static_cast<double>(text_h);;
        text_w *= f;
        text_h *= f;
    }
    smaller_w = ((w - w/25- 20) - text_w)/2;//((w - w/25) - text_w)/2;
    
    SDL_Rect textQuad = {w/25 + 10 + smaller_w, w/25 + w/10 + w/20 + 10 , static_cast<int>(text_w)  , static_cast<int>(text_h)};
    SDL_RenderCopyEx(r, infoTextSv[infoIndex], NULL, &textQuad, 0, NULL, SDL_FLIP_NONE);
    
    swedish.push_back(newTexture);
    
    
    // English text
    newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_QueryTexture(headersEn[infoIndex], NULL, NULL, &header_w, &header_h);
    if (header_w > w - w/25) {
        f = (w - w/25)/static_cast<double>(header_w);;
        header_w *= f;
        header_h *= f;
    }
    if (header_h > w / 10) {
        f = (w / 10)/static_cast<double>(header_h);;
        header_w *= f;
        header_h *= f;
    }
    smaller_w = ((w - w/25) - header_w)/2;
    smaller_h = ((w/10) - header_h)/2;
    
    headQuad = {w/25, w/25  , w - w/25  , w/10 + 5};
    //SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    //SDL_RenderFillRect( r, &headQuad );
    
    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, headersEn[infoIndex], NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    SDL_QueryTexture(infoTextEn[infoIndex], NULL, NULL, &text_w, &text_h);
    if (text_w > w - w/25 - 20) {
        f = (w - w/25 - 20)/static_cast<double>(text_w);
        text_w *= f;
        text_h *= f;
    }
    if (text_h > h - w/25 - w/10 - w/20 - 20 ) {
        f = (h - w/25 - w/10 - w/20 - 20)/static_cast<double>(text_h);;
        text_w *= f;
        text_h *= f;
    }
    smaller_w = ((w - w/25- 20) - text_w)/2;//((w - w/25) - text_w)/2;
    
    textQuad = {w/25 + 10 + smaller_w, w/25 + w/10 + w/20 + 10 , static_cast<int>(text_w)  , static_cast<int>(text_h)};
    SDL_RenderCopyEx(r, infoTextEn[infoIndex], NULL, &textQuad, 0, NULL, SDL_FLIP_NONE);
    
    english.push_back(newTexture);
    

    
    SDL_SetRenderTarget( r, NULL );
    
}