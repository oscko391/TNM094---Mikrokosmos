#if defined (__APPLE_CC__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "Card.h"
#include <iostream>
#include <vector>

SDL_Window* gWindow = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init(Card &c);



int main(int argc, char* args[])
{
    // declarations
    std::vector<std::string> CatVec;
    CatVec.push_back("category1");

    std::string svHeader = ("Den här protein är för farlig");
    std::string svText = ("text om proteinet");
    std::string enHeader = ("this protein is too dangerous");
    std::string enText = ("text about the protein");

    bool isSwede = true;

    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec2 velocity = glm::vec2(0.5, 0.5);

    Card testCard = Card(CatVec, svHeader, svText, enHeader, enText, isSwede, pos, velocity);

    /*
    testCard.addCategory("what");
    testCard.addCategory("up!");

    testCard.changeLang();

    for (int i = 0; i < testCard.getCategories().size(); i++)
    {
        std::cout << "Category: " << testCard.getCategories()[i] << std::endl;
    }

    if (testCard.getIsSwede())
    {
        std::cout << "Svensk header: " << testCard.getSvHeader() << "\nSvensk Text :"
                  << testCard.getSvText() << "\nPosition : (" <<testCard.getPos().x
                  << ", " << testCard.getPos().y << ", " << testCard.getPos().z << ")"
                  << std::endl << "Velocity : (" << testCard.getVelocity().x
                  << ", " << testCard.getVelocity().y << " )";
    }
    else
    {
        std::cout << "\nEnglish Header :" << testCard.getEnHeader()
                  <<"\nEnglish text :" << testCard.getEnText() << std::endl;
    }

    testCard.setVelocity(glm::vec2(1.0f,3.0f));

    if (testCard.getIsSwede())
    {
        std::cout << "Svensk header: " << testCard.getSvHeader() << "\nSvensk Text :"
                  << testCard.getSvText() << "\nPosition : (" <<testCard.getPos().x
                  << ", " << testCard.getPos().y << ", " << testCard.getPos().z << ")"
                  << std::endl << "Velocity : (" << testCard.getVelocity().x
                  << ", " << testCard.getVelocity().y << " )";
    }*/

    init(testCard);
    testCard.setImgPath("pinch_close.png");
    testCard.loadMedia();

    std::cout << testCard.getImgPath() << std::endl;


    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }

        //Apply the PNG image
        SDL_BlitSurface( testCard.getPngSurface(), NULL, testCard.getScreenSurface(), NULL );

        //Update the surface
        SDL_UpdateWindowSurface( gWindow );




    }
    return 0;
}

bool init(Card &c)
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
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else
            {
                //Get window surface
                c.setgScreenSurface(SDL_GetWindowSurface( gWindow )) ;
            }
        }

    }


    return success;
}

