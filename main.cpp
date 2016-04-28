//
//  main.cpp
//  En main-fil kanske...
//
//  Created by My Rydberg on 2016-02-08.
//  Copyright (c) 2016 My. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */


#include "rapidxml.hpp" // mac: add to project, windows: add headers to lib and refrence with < >
#include "Card.h"
#include "PhotoCard.h"
#include "CardHandler.h"

SDL_Texture* loadingT(std::string path);
bool initWindow();
void close();

bool sorting( Card* a,  Card* b) {return a->getLifeTime() < b->getLifeTime(); }


// global window and renderer to the window
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//const int SCREEN_WIDTH = 1200;
//const int SCREEN_HEIGHT = 800;
int Card::infoIndexGenerator = 0;
std::vector<SDL_Texture*> Card::headers;

int PhotoCard::tex_generator = 0;
std::vector<SDL_Texture*> PhotoCard::theTextures;


int main(int argc, char*args[])
{
    //std::vector<Card*> theCards;
    
    // initiation of the cards, read the xml-file and save cards to theCards
    //std::string xmlPath = "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/demo/mediaTest.xml"; // change to correct path
    
    std::string xmlPath = "/Users/madeleinerapp/Documents/LiU/Githubmappen/TNM094---Mikrokosmos/demo/mediaTest.xml";

    
    //readXml(xmlPath, theCards);
    //std::vector<SDL_Texture*> theTextures(theCards.size());
    
    
    clock_t startClock = clock();
    if (!initWindow()) {
        printf("Failed the window \n");
    }
    else {
        CardHandler ch = CardHandler(xmlPath, gRenderer);
        //readXml(xmlPath, theCards);
        bool quit = false;
        SDL_Event e;
        
        startClock = clock();
        while (!quit) {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                
                //Handle button events
                //for(int i = theCards.size(); i >= 0; i-- )
                //{
                //    if (theCards[i]->handleEvent( &e ))
                //    {
                //        break;
                //    }
                //}
                ch.addEvent(e);
                
            }
            /*if( e.type == SDL_QUIT )
             {
             quit = true;
             }*/

            
            ch.HandleEvents();
            // begin render
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0x61, 0x62, 0x60, 0xFF);
//            SDL_SetRenderDrawColor( gRenderer, 0x11, 0x22, 0x55, 0xFF);


            SDL_SetRenderDrawColor( gRenderer, 0x11, 0x22, 0x55, 0xFF);

            SDL_RenderClear( gRenderer );
            float time = clock() - startClock;
            //time = 100*time/CLOCKS_PER_SEC;
            time = time/CLOCKS_PER_SEC;
            startClock = clock();
            ch.sort();
            for (int i = ch.getCurrentCard().size() - 1; i >= 0; i--) {
                if (ch.getCurrentCard()[i]->getLifeTime() < clock()) {
                    ch.getCurrentCard()[i]->move(time);
                }
                ch.getCurrentCard()[i]->render(gRenderer); // theTextures[theCards[i].texIndex]
            }
            ch.renderMenu(gRenderer);
            //Update screen
            SDL_RenderPresent( gRenderer );
            // end of render
            ch.getFrameEvents().clear();
        }
    }
    
    close();
    
    return 0;
}



SDL_Texture* loadingT(std::string path)
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
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return newTexture;
}

bool initWindow()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_SetRenderDrawBlendMode( gRenderer, SDL_BLENDMODE_BLEND);
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    
    return success;
}


void close()
{
    
    
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
