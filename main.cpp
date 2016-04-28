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

bool readXml(std::string filePath, std::vector<Card*> &vecCard);
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

            SDL_RenderClear( gRenderer );
            float time = clock() - startClock;
            //time = 100*time/CLOCKS_PER_SEC;
            time = time/CLOCKS_PER_SEC;
            startClock = clock();
            ch.sort();
            for (int i = ch.getVecCard().size() - 1; i >= 0; i--) {
                if (ch.getVecCard()[i]->getLifeTime() < clock()) {
                    ch.getVecCard()[i]->move(time);
                }
                ch.getVecCard()[i]->render(gRenderer); // theTextures[theCards[i].texIndex]
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

bool readXml(std::string filePath, std::vector<Card*> &vecCard) {
    // getting the string through an ifstream
    std::ifstream ifs(filePath, std::ios::in);
    if(!ifs.is_open()) {
        return false;
    }
    
    // making it the right format (char) for the parsing to xml doc
    std::string xml_str;
    xml_str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    
    // creating a xml_document and parsing the xml-file, making a DOM-tree
    rapidxml::xml_document<> doc;
    try {
        doc.parse<0>((char*)xml_str.c_str());
    }
    catch (...) {
        return false;
    }
    
    
    // accesing the first node in the file
    rapidxml::xml_node<>* content = doc.first_node("content");
    std::cout << "The '" << content->name() << "' is loaded. \n\n";
    
    // accesing all category nodes in content
    for (rapidxml::xml_node<>* second = content->first_node("category"); second <= content->last_node("category"); second = second->next_sibling()) {
        // writes out information about the categpries to the terminal
        std::cout << "Node '" << second->name() <<"' has value: " << second->value() << " and is type: " << second->type() << "\n";
        for (rapidxml::xml_attribute<> * attr = second->first_attribute(); attr; attr = attr->next_attribute())
        {
            std::cout << "The node has attribute " << attr->name() << " ";
            std::cout << "with value " << attr->value() << "\n";
        }
        std::cout << "\n";
    }
    
    
    int i = 0;
    // accesing all media nodes
    for (rapidxml::xml_node<>* second = content->first_node("media"); second; second = second->next_sibling()) {
        // writes out inforamtion about the media to the terminal and creates variables to use for creation of cards
        
        std::string mediaPath = second->first_attribute("path")->value();
        
        std::stringstream ss;
        int scaleExp;
        ss << second->first_attribute("scale_exp")->value();
        ss >> scaleExp;
        
        std::vector<std::string> cardCat;
        std::string seHeader;
        std::string seText;
        std::string enHeader;
        std::string enText;
        int j = 0;
        for (rapidxml::xml_node<>* inside = second->first_node(); inside ; inside = inside->next_sibling()) {
            
            std::string b = inside->name();
            if (b == "category") {
                std::string cat = inside->first_attribute()->value();
                cardCat.push_back(cat);
                j++;
            }
            else if (b == "se"){
                seHeader = inside->first_node()->value() ;
                seText = inside->first_node()->next_sibling()->value();
            }
            else {
                enHeader = inside->first_node()->value() ;
                enText = inside->first_node()->next_sibling()->value();
            }
            
        }
        
        // create card with variables
        glm::vec3 position = glm::vec3(rand() % rand() % (SCREEN_WIDTH /2) + (SCREEN_WIDTH /4),rand() % (SCREEN_HEIGHT /2) + (SCREEN_HEIGHT /4),0);
        glm::vec2 velocity;
        int random = rand() % 4;
        if (random == 0) {
            velocity = glm::vec2((rand() % 4 + 1) , rand() % 4 + 1);
        }
        else if (random == 1) {
            velocity = glm::vec2(-(rand() % 4 + 1) , rand() % 4 + 1);
        }
        else if (random == 2) {
            velocity = glm::vec2((rand() % 4 + 1) , -(rand() % 4 + 1));
        }
        else {
            velocity = glm::vec2(-(rand() % 4 + 1) , -(rand() % 4 + 1));
        }
        
        PhotoCard* newCard = new PhotoCard(cardCat, seHeader, seText, enHeader, enText, position, velocity, mediaPath, gRenderer);
        vecCard.push_back(newCard);
        //vecCard[i].setLifeTime(clock());
        i++;
    }
    return true;
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
