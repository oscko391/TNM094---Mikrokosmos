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


#include "rapidxml.hpp" // mac: add to project, windows: add headers to lib and refrence with < >
#include "Card.h"

bool readXml(std::string filePath, std::vector<Card> &vecCard);
bool initWindow();
void close();


// global window and renderer to the window
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//const int SCREEN_WIDTH = 1200;
//const int SCREEN_HEIGHT = 800;



int main(int argc, char*args[])
{
    std::vector<Card> theCards;
    
    // initiation of the cards, read the xml-file and save cards to theCards
    std::string xmlPath = "/Users/madeleinerapp/Documents/LiU/Githubmappen/TNM094---Mikrokosmos/demo/mediaTest.xml"; // change to correct path

    //std::string xmlPath = "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/demo/mediaTest.xml";
    
    readXml(xmlPath, theCards);
    time_t theNow = time(0);
    
    
    if (!initWindow()) {
        printf("Failed the window \n");
    }
    else {
        for (int i = 0; i < theCards.size(); i++) {
            theCards[i].loadTexture(gRenderer);
        }
        
        bool quit = false;
        SDL_Event e;

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
                for(int i = theCards.size(); i >= 0; i-- )
                {
                    if (theCards[i].handleEvent( &e ))
                    {
                        break;
                    }
                    
                }

                
            }


            // begin render
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0x33, 0x00, 0x85, 0xFF);
            SDL_RenderClear( gRenderer );
            
            std::vector<int> activeCards;
            // render normal Cards
            for (int i = 0; i < theCards.size(); i++) {
                if (theCards[i].getLifeTime() > time(0)) {
                    activeCards.push_back(i);
                }
                else {
                    theCards[i].move(theNow);
                    theCards[i].render(gRenderer);
                }
            }
            
            // render active Cards
            for (int i = 0; i < activeCards.size(); i++) {
                theCards[activeCards[i]].renderActive(gRenderer);
            }
            
            //Update screen
            SDL_RenderPresent( gRenderer );
            // end of render
        }
    }

    close();
    
    return 0;
}

bool readXml(std::string filePath, std::vector<Card> &vecCard) {
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
        
        
        vecCard.push_back(Card(cardCat, seHeader, seText, enHeader, enText, true, position, velocity, mediaPath));
        i++;
    }
    return true;
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