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

#include "settings.h"
#include "Card.h"
#include "PhotoCard.h"
#include "StoryCard.h"
#include "CardHandler.h"

bool readXml(std::string filePath, std::vector<Card*> &vecCard);
SDL_Texture* loadingT(std::string path);
bool initWindow();
void close();

bool sorting( Card* a,  Card* b) {return a->getLifeTime() < b->getLifeTime(); }
std::string pathToFiles ="/Users/madeleinerapp/Documents/LiU/Githubmappen/TNM094---Mikrokosmos"; //change for specific computer
//std::string pathToFiles ="/Users/my/Documents/LiU/Kandidat/SDL_tutorial"; //change for specific computer
//std::string pathToFiles ="C:/Users/Oscar"; //change for specific computer


// global window and renderer to the window
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//const int SCREEN_WIDTH = 1200;
//const int SCREEN_HEIGHT = 800;
int Card::infoIndexGenerator = 0;
std::vector<SDL_Texture*> Card::headersSv;
std::vector<SDL_Texture*> Card::headersEn;
std::vector<SDL_Texture*> Card::infoTextSv;
std::vector<SDL_Texture*> Card::infoTextEn;
std::vector<SDL_Texture*> Card::catTextSv;
std::vector<SDL_Texture*> Card::catTextEn;

int PhotoCard::tex_generator = 0;
std::vector<SDL_Texture*> PhotoCard::headSv;
std::vector<SDL_Texture*> PhotoCard::headEn;
std::vector<SDL_Texture*> PhotoCard::readSv;
std::vector<SDL_Texture*> PhotoCard::readEn;

int StoryCard::tex_generator = 0;
std::vector<SDL_Texture*> StoryCard::image;
std::vector<SDL_Texture*> StoryCard::swedish;
std::vector<SDL_Texture*> StoryCard::english;

SDL_Texture* Card::shadow = NULL;
SDL_Texture* Card::arrow = NULL;

int main(int argc, char*args[])
{
    bool isSwedish = true;
    
    // initiation of the cards, read the xml-file and save cards to theCards
/*    std::string xmlPath1 = pathToFiles + "/media/write.xml";
    std::string xmlStory1 = pathToFiles + "/stories/stories.xml";
    std::cout << "1 " << xmlPath1 << std::endl;*/
    
    std::string xmlPath = "/Users/madeleinerapp/Documents/LiU/Githubmappen/TNM094---Mikrokosmos/media/write.xml";
    std::string xmlStory = "/Users/madeleinerapp/Documents/LiU/Githubmappen/TNM094---Mikrokosmos/stories/stories.xml";
    
    
    clock_t startClock;
    if (!initWindow()) {
        printf("Failed the window \n");
    }
    else {
        SDL_SetWindowFullscreen(gWindow,SDL_WINDOW_FULLSCREEN);
        std::cout << " testing " << std::endl;
        CardHandler ch = CardHandler(xmlPath, xmlStory, gRenderer);
        bool quit = false;
        SDL_Event e;
        
        startClock = clock();
        float time;
        while (!quit) {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                
                ch.addEvent(e);
                
            }
            
            ch.HandleEvents(isSwedish);
            
            // begin render
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, ch.backgorundColor[0], ch.backgorundColor[1], ch.backgorundColor[2], ch.backgorundColor[3]); // Backgroundcolor generated from xml-file
            SDL_RenderClear( gRenderer );
            time = clock() - startClock;
            
            time = time/CLOCKS_PER_SEC;
            ch.changeCat(time);
            startClock = clock();
            
            ch.render(gRenderer, isSwedish);
            
            //Update screen
            SDL_RenderPresent( gRenderer );
            // end of render
            ch.clearEvents();
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
    TTF_Quit();
}
