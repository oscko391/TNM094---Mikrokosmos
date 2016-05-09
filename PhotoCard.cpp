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
    createBackground(r);
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


void PhotoCard::createBackground( SDL_Renderer* r)
{

    // tanken var att denna funktionen skulle rendera bakgrundsbilden och ramarna till en textur


   /* backgroundTexture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(r, backgroundTexture);  // render to the backgroundTexture texture

    SDL_Rect renderQuad = {0 , 0, 1920, 1080 };
    SDL_Rect whiteQuad = {0 - 5, 0 - 5, 1920 + 10, 1080 + 10 };
    SDL_Rect blackQuad = {0 - 10, 0 - 10, 1920 + 20, 1080 + 20 };

    SDL_SetRenderDrawColor( r, 0x10, 0x20, 0x50, 0x35 );
    SDL_RenderFillRect( r, &blackQuad );

    SDL_SetRenderDrawColor( r, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( r, &whiteQuad );

    SDL_RenderCopy( r, theTextures[texIndex], NULL, &renderQuad );

    SDL_SetRenderTarget(r, NULL); */  // the the render target to the screen

}


void PhotoCard::render( SDL_Renderer* gRenderer) // Blir error atm
{
    //Set rendering space and render to screen
    const static int SCALEFACTOR = 14;

    SDL_Rect renderQuad = {static_cast<int>(getPos().x) , static_cast<int>(getPos().y), getWidth(), getHeight() };
    SDL_Rect whiteQuad = {static_cast<int>(getPos().x) - 5, static_cast<int>(getPos().y) - 5, getWidth() + 10, getHeight() + 10 };
    SDL_Rect blackQuad = {static_cast<int>(getPos().x) - 10, static_cast<int>(getPos().y) - 10, getWidth() + 20, getHeight() + 20 };
    SDL_Rect headQuad = {static_cast<int>(getPos().x) + getWidth()/60 , static_cast<int>(getPos().y) + getWidth()/60  , getWidth() - getWidth()/30  , getWidth() / 10};


    int w, h;
    SDL_QueryTexture(getHeader(), NULL, NULL, &w, &h);

    SDL_Rect headingQuad = {
        static_cast<int>(getPos().x) + getWidth()/60 ,
        static_cast<int>(getPos().y) + getWidth()/60  ,
        w / SCALEFACTOR ,
        h / SCALEFACTOR};
    int xoffset = (getWidth() - w/SCALEFACTOR) / 2; //centrerar text
    headingQuad.x += xoffset;
    int yoffset = (headQuad.h - h /SCALEFACTOR) / 2;
    headingQuad.y += yoffset;

    if (getReading()) {
        headQuad = {static_cast<int>(getPos().x) + getWidth()/60 , static_cast<int>(getPos().y) + getWidth()/60 , getWidth() - getWidth()/30 , getHeight() - getWidth()/30};
    }

    // render the pre-rendered background texture with the image and the borders
    //SDL_RenderCopy( gRenderer, backgroundTexture, NULL, &renderQuad );

    //SDL_Rect headText =
    SDL_SetRenderDrawColor( gRenderer, 0x10, 0x20, 0x50, 0x35 );
    SDL_RenderFillRect( gRenderer, &blackQuad );

    SDL_SetRenderDrawColor( gRenderer, 0xEE, 0xEE, 0xEE, 0xFF );
    SDL_RenderFillRect( gRenderer, &whiteQuad );

    SDL_RenderCopy( gRenderer, theTextures[texIndex], NULL, &renderQuad );  // renderar bakgrundsbilden

    SDL_SetRenderDrawColor( gRenderer, 0xDD, 0xDD, 0xDD, 0xCC );    // bakgrund runt texten
    SDL_RenderFillRect( gRenderer, &headQuad );

//    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x77 );    // röd halvtansparent bakgrund runt texten för test
//                                       max                halv
//    SDL_RenderFillRect( gRenderer, &headingQuad );

    //getHeader() är en funkton som hämtar rubrik-texturen från card-klassen.
    SDL_RenderCopyEx(gRenderer, getHeader(), NULL, &headingQuad, 0, NULL, SDL_FLIP_NONE);  // renderar rubrik-texturen.
    // SDL_RenderCopyEx(gRenderer, getHeader(), NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);
    if (getReading()) {
        int bodyW, bodyH;
        headQuad = {static_cast<int>(getPos().x) + getWidth()/60 , static_cast<int>(getPos().y) + getWidth()/60 , getWidth() - getWidth()/30 , getHeight() - getWidth()/30};
        SDL_QueryTexture(getMainTexture(), NULL, NULL, &bodyW, &bodyH);
        double aspectRatio = (double)bodyW / (double)bodyH; //att bredden och hjöjden har samma proportioner
        headQuad.y += h * 2/SCALEFACTOR;
        headQuad.x += 5;
        headQuad.h = headQuad.w / aspectRatio;


        if (getLanguage() == SWEDISH)
        {
            // TODO: Add SDL_SetClipRect i samma storleksom det vita rammen. Kräver Surface
            SDL_RenderCopyEx(gRenderer, getMainTexture(), NULL, &headQuad, 0, NULL, SDL_FLIP_NONE); //ritar brödtexten
        }
        else if (getLanguage() == ENGLISH)
        {
            SDL_RenderCopyEx(gRenderer, getMainTextureEN(), NULL, &headQuad, 0, NULL, SDL_FLIP_NONE); //ritar brödtexten
        }

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(gRenderer,
                           static_cast<int>(getPos().x) + getWidth()/20 ,
                           static_cast<int>(getPos().y) + getWidth()/10,
                           static_cast<int>(getPos().x) + getWidth() - getWidth()/16 ,
                           static_cast<int>(getPos().y) + getWidth()/10);

        SDL_RenderDrawLine(gRenderer,
                           static_cast<int>(getPos().x) + getWidth()/20 ,
                           static_cast<int>(getPos().y) + getWidth()/9,
                           static_cast<int>(getPos().x) + getWidth() - getWidth()/16 ,
                           static_cast<int>(getPos().y) + getWidth()/9);



         //SDL_Rect catergoriesQuad = {static_cast<int>(getPos().x) , static_cast<int>(getPos().y), getWidth(), getHeight() };
        // SDL_RenderCopyEx(gRenderer, getCategoryTexture(), NULL, &catergoriesQuad, 0, NULL, SDL_FLIP_NONE);

    }
}

/*
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

    int header_w, header_h;
    SDL_QueryTexture(getHeader(), NULL, NULL, &header_w, &header_h);
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

    SDL_Rect headQuad = {w/25, w/25  , w - w/25  , w / 10};
    SDL_SetRenderDrawColor( r, 0xDD, 0xDD, 0xDD, 0xCC );
    SDL_RenderFillRect( r, &headQuad );

    headQuad = {w/25 + smaller_w, w/25 + smaller_h , static_cast<int>(header_w)  , static_cast<int>(header_h)};
    SDL_RenderCopyEx(r, getHeader(), NULL, &headQuad, 0, NULL, SDL_FLIP_NONE);

    SDL_SetRenderTarget( r, NULL );

    return newTexture;
}
*/

SDL_Texture* PhotoCard::loadingTex(SDL_Renderer* r, std::string path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( r, loadedSurface );
        if ( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
