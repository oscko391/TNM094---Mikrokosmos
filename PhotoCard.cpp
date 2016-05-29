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

PhotoCard::PhotoCard(std::vector<std::string> inSvCat, std::vector<std::string> inEnCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r)
    : Card(inSvCat, inEnCat, inSvH, inSvT, inEnH, inEnT, inPos, inVel, textPath, r)
{
    texIndex = tex_generator++;
    loadingTex(r, textPath);
    int w, h;
    SDL_QueryTexture(headSv[texIndex], NULL, NULL, &w, &h);
    double f = (SCREEN_WIDTH/7.0) / w;
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

void PhotoCard::render( SDL_Renderer* gRenderer, bool swede) // Blir error atm
{
    //Set rendering space and render to screen
    SDL_Rect shadowQuad = {static_cast<int>(getPos().x) - getWidth()/40, static_cast<int>(getPos().y) - getWidth()/40, getWidth() + getWidth()/20, getHeight() + getWidth()/20 };
    SDL_SetTextureBlendMode(shadow, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(shadow, 0x50);
    SDL_RenderCopyEx( gRenderer, shadow, NULL, &shadowQuad, getAngle(), NULL, SDL_FLIP_NONE );
    
    
    SDL_Rect renderQuad = {static_cast<int>(getPos().x) , static_cast<int>(getPos().y), getWidth(), getHeight() };
    
    if (swede) {
        if (getReading()) {
            SDL_RenderCopyEx( gRenderer, readSv[texIndex], NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE );

        }
        else {
            SDL_RenderCopyEx( gRenderer, headSv[texIndex], NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE );

        }

    }
    else {
        if (getReading()) {
            SDL_RenderCopyEx( gRenderer, readEn[texIndex], NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE );
            
        }
        else {
            SDL_RenderCopyEx( gRenderer, headEn[texIndex], NULL, &renderQuad, getAngle(), NULL, SDL_FLIP_NONE );
            
        }
    }
    

}

void PhotoCard::loadingTex(SDL_Renderer* r, std::string path)
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
    
    // Svedish with header
    SDL_Texture* newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_Rect renderQuad = {w/50 , w/50, w, h };
    SDL_RenderCopyEx( r, imgTexture , NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE );
    

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
    
    SDL_Rect headQuad = {w/25, w/25  , w - w/25  , w/10 + w/25};
    SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( r, &headQuad );
    
    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, headersSv[infoIndex], NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    SDL_Rect arrowQuad = {w/2, w/25 + w/10, w/25, w/25};
    SDL_RenderCopyEx(r, arrow, NULL, &arrowQuad, NULL, NULL, SDL_FLIP_NONE);
    
    headSv.push_back(newTexture);
    
    
    // Swedish with text
    newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_RenderCopyEx( r, imgTexture , NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE );
    
    headQuad = {w/25, w/25  , w - w/25  , h - w/25};
    SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( r, &headQuad );
    
    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, headersSv[infoIndex], NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_Rect line1 = { w/10, w/25 + w/10, w - w/5, 3};
    SDL_RenderFillRect( r, &line1 );
    
    int cat_w, cat_h;
    SDL_QueryTexture(catTextSv[infoIndex], NULL, NULL, &cat_w, &cat_h);
    if (cat_h > w/30) {
        f = (w/30)/static_cast<double>(cat_h);
        cat_w *= f;
        cat_h *= f;
    }
    int center = (w - w/5 - cat_w)/2;
    
    SDL_Rect catRect = {w/10 + center, w/25+ w/10 + w/180 + 3, static_cast<int>(cat_w), static_cast<int>(cat_h)};
    SDL_RenderCopyEx(r, catTextSv[infoIndex], NULL, &catRect, 0, NULL, SDL_FLIP_NONE);
    
    SDL_Rect line2 = { w/10, w/25 + w/10 + w/20, w - w/5, 3};
    SDL_RenderFillRect( r, &line2 );
    
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

    SDL_Rect arrowQuad2 = {w/2, h - w/20, w/25, w/25};
    SDL_RenderCopyEx(r, arrow, NULL, &arrowQuad2, NULL, NULL, SDL_FLIP_VERTICAL);

    readSv.push_back(newTexture);
    
    // English head
    newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_RenderCopyEx( r, imgTexture , NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE );
    
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
    
    headQuad = {w/25, w/25  , w - w/25  , w/10 + w/25};
    SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( r, &headQuad );
    
    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, headersEn[infoIndex], NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    SDL_RenderCopyEx(r, arrow, NULL, &arrowQuad, NULL, NULL, SDL_FLIP_NONE);
    
    headEn.push_back(newTexture);
    
    // English with text
    newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + w/25 , h + w/25 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_RenderCopyEx( r, imgTexture , NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE );
    
    headQuad = {w/25, w/25  , w - w/25  , h - w/25};
    SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( r, &headQuad );
    
    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, headersEn[infoIndex], NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderFillRect( r, &line1 );
    SDL_RenderFillRect(r, &line2);
    
    SDL_QueryTexture(catTextEn[infoIndex], NULL, NULL, &cat_w, &cat_h);
    if (cat_h > w/30) {
        f = (w/30)/static_cast<double>(cat_h);
        cat_w *= f;
        cat_h *= f;
    }
    center = (w - w/5 - cat_w)/2;
    
    catRect = {w/10 + center, w/25+ w/10 + w/180, static_cast<int>(cat_w), static_cast<int>(cat_h)};
    SDL_RenderCopyEx(r, catTextEn[infoIndex], NULL, &catRect, 0, NULL, SDL_FLIP_NONE);
        
    SDL_QueryTexture(infoTextEn[infoIndex], NULL, NULL, &text_w, &text_h);
    if (text_w > w - w/25 - 20) {
        f = (w - w/25 - 20)/static_cast<double>(text_w);;
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
    
    SDL_RenderCopyEx(r, arrow, NULL, &arrowQuad2, NULL, NULL, SDL_FLIP_VERTICAL);
    
    readEn.push_back(newTexture);
    
    SDL_SetRenderTarget( r, NULL );
    
}