//
//  main.cpp
//  En main-fil kanske...
//
//  Created by My Rydberg on 2016-02-08.
//  Copyright (c) 2016 My. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>


#include "rapidxml.hpp" // mac: add to project, windows: add headers to include and "" should be used, ending with .hpp
#include "Card.h"

bool readXml(std::string filePath, std::vector<Card> &vecCard);
bool initWindow();
void close();
bool sorting(Card a, Card b);



// global window and renderer to the window
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//const int SCREEN_WIDTH = 1200;
//const int SCREEN_HEIGHT = 800;




int main(int argc, char*args[])
{
    std::vector<Card> theCards;
    std::vector<SDL_Event> frameEvents;

    //vector keeping track of all touch positions(x ,y)
    std::vector<std::pair <int , int > > touchPos;

    // initiation of the cards, read the xml-file and save cards to theCards
    std::string xmlPath = "C:/Users/Oscar/demo/mediaTest.xml"; // change to correct path

    //std::string xmlPath = "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/demo/mediaaTest.xml";

    readXml(xmlPath, theCards);
    time_t theNow = time(0);


    if (!initWindow())
    {
        printf("Failed the window \n");
    }
    else
    {
        for (int i = 0; i < theCards.size(); i++)
        {
            theCards[i].loadTexture(gRenderer);
        }

        bool quit = false;


        while (!quit)
        {

            SDL_Event e;

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //store the events in a vector
                frameEvents.push_back(e);
            }

            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            //Handle button events
            for(int i = 0; i < theCards.size(); i++ )
            {
                for (int j = 0; j < frameEvents.size(); j++)
                {
                    if (theCards[i].handleEvent( &frameEvents[j] ))
                    {
                        // remove event after it has been handled
                        frameEvents.erase(frameEvents.begin()+j);
                    }

                }

            }


            // begin render
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0x33, 0x00, 0x85, 0xFF);
            SDL_RenderClear( gRenderer );

            std::vector<int> activeCards;
            // render normal Cards
            for (int i = 0; i < theCards.size(); i++)
            {
                if (theCards[i].getLifeTime() > time(0))
                {
                    activeCards.push_back(i);
                }
                else
                {
                    theCards[i].move(theNow);
                    theCards[i].render(gRenderer);
                }
            }

            // render active Cards
            for (int i = 0; i < activeCards.size(); i++)
            {
                theCards[activeCards[i]].renderActive(gRenderer);
            }

            //Update screen
            SDL_RenderPresent( gRenderer );
            // end of render

            frameEvents.clear();
        }
    }

    close();

    return 0;
}

bool readXml(std::string filePath, std::vector<Card> &vecCard)
{
    // getting the string through an ifstream
    std::ifstream ifs(filePath, std::ios::in);
    if(!ifs.is_open())
    {
        return false;
    }

    // making it the right format (char) for the parsing to xml doc
    std::string xml_str;
    xml_str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    // creating a xml_document and parsing the xml-file, making a DOM-tree
    rapidxml::xml_document<> doc;
    try
    {
        doc.parse<0>((char*)xml_str.c_str());
    }
    catch (...)
    {
        return false;
    }


    // accesing the first node in the file
    rapidxml::xml_node<>* content = doc.first_node("content");
    std::cout << "The '" << content->name() << "' is loaded. \n\n";

    // accesing all category nodes in content
    for (rapidxml::xml_node<>* second = content->first_node("category"); second <= content->last_node("category"); second = second->next_sibling())
    {
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
    for (rapidxml::xml_node<>* second = content->first_node("media"); second; second = second->next_sibling())
    {
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
        for (rapidxml::xml_node<>* inside = second->first_node(); inside ; inside = inside->next_sibling())
        {

            std::string b = inside->name();
            if (b == "category")
            {
                std::string cat = inside->first_attribute()->value();
                cardCat.push_back(cat);
                j++;
            }
            else if (b == "se")
            {
                seHeader = inside->first_node()->value() ;
                seText = inside->first_node()->next_sibling()->value();
            }
            else
            {
                enHeader = inside->first_node()->value() ;
                enText = inside->first_node()->next_sibling()->value();
            }

        }

        // create card with variables
        glm::vec3 position = glm::vec3(rand() % rand() % (SCREEN_WIDTH /2) + (SCREEN_WIDTH /4),rand() % (SCREEN_HEIGHT /2) + (SCREEN_HEIGHT /4),0);
        glm::vec2 velocity;
        int random = rand() % 4;
        if (random == 0)
        {
            velocity = glm::vec2((rand() % 4 + 1) , rand() % 4 + 1);
        }
        else if (random == 1)
        {
            velocity = glm::vec2(-(rand() % 4 + 1) , rand() % 4 + 1);
        }
        else if (random == 2)
        {
            velocity = glm::vec2((rand() % 4 + 1) , -(rand() % 4 + 1));
        }
        else
        {
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

bool sorting(Card a, Card b)
{
    return a.getLifeTime() < b.getLifeTime();
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


/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/
/*
//Using SDL, SDL_image, standard IO, and, strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <string.h>

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Creates blank texture
		bool createBlank( int width, int height, SDL_TextureAccess = SDL_TEXTUREACCESS_STREAMING );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Set self as render target
		void setAsRenderTarget();

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		void copyPixels( void* pixels );
		int getPitch();
		Uint32 getPixel32( unsigned int x, unsigned int y );

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		void* mPixels;
		int mPitch;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Screen dimensions
SDL_Rect gScreenRect = { 0, 0, 320, 240 };

//Scene textures
LTexture gPinchCloseTexture;
LTexture gPinchOpenTexture;
LTexture gRotateTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		SDL_Log( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_RGBA8888, NULL );
		if( formattedSurface == NULL )
		{
			SDL_Log( "Unable to convert loaded surface to display format! %s\n", SDL_GetError() );
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
			if( newTexture == NULL )
			{
				SDL_Log( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Enable blending on texture
				SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );

				//Lock texture for manipulation
				SDL_LockTexture( newTexture, &formattedSurface->clip_rect, &mPixels, &mPitch );

				//Copy loaded/formatted surface pixels
				memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

				//Get image dimensions
				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;

				//Get pixel data in editable format
				Uint32* pixels = (Uint32*)mPixels;
				int pixelCount = ( mPitch / 4 ) * mHeight;

				//Map colors
				Uint32 colorKey = SDL_MapRGB( formattedSurface->format, 0, 0xFF, 0xFF );
				Uint32 transparent = SDL_MapRGBA( formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00 );

				//Color key pixels
				for( int i = 0; i < pixelCount; ++i )
				{
					if( pixels[ i ] == colorKey )
					{
						pixels[ i ] = transparent;
					}
				}

				//Unlock texture to update
				SDL_UnlockTexture( newTexture );
				mPixels = NULL;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface( formattedSurface );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			SDL_Log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

bool LTexture::createBlank( int width, int height, SDL_TextureAccess access )
{
	//Create uninitialized texture
	mTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height );
	if( mTexture == NULL )
	{
		SDL_Log( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		mWidth = width;
		mHeight = height;
	}

	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

void LTexture::setAsRenderTarget()
{
	//Make self render target
	SDL_SetRenderTarget( gRenderer, mTexture );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if( mPixels != NULL )
	{
		SDL_Log( "Texture is already locked!\n" );
		success = false;
	}
	//Lock texture
	else
	{
		if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 )
		{
			SDL_Log( "Unable to lock texture! %s\n", SDL_GetError() );
			success = false;
		}
	}

	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if( mPixels == NULL )
	{
		SDL_Log( "Texture is not locked!\n" );
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture( mTexture );
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* LTexture::getPixels()
{
	return mPixels;
}

void LTexture::copyPixels( void* pixels )
{
	//Texture is locked
	if( mPixels != NULL )
	{
		//Copy to locked pixels
		memcpy( mPixels, pixels, mPitch * mHeight );
	}
}

int LTexture::getPitch()
{
	return mPitch;
}

Uint32 LTexture::getPixel32( unsigned int x, unsigned int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32*)mPixels;

    //Get the pixel requested
    return pixels[ ( y * ( mPitch / 4 ) ) + x ];
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		SDL_Log( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			SDL_Log( "Warning: Linear texture filtering not enabled!" );
		}

        //Get device display mode
        SDL_DisplayMode displayMode;
        if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
        {
            gScreenRect.w = displayMode.w;
            gScreenRect.h = displayMode.h;
        }

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
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
					SDL_Log( "SDL_image could not initialize! %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load scene textures
	if( !gPinchCloseTexture.loadFromFile( "pinch_close.png" ) )
	{
		SDL_Log( "Failed to load pinch close texture!\n" );
		success = false;
	}
	if( !gPinchOpenTexture.loadFromFile( "pinch_open.png" ) )
	{
		SDL_Log( "Failed to load pinch open texture!\n" );
		success = false;
	}
	if( !gRotateTexture.loadFromFile( "rotate.png" ) )
	{
		SDL_Log( "Failed to load rotate texture!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gPinchCloseTexture.free();
	gPinchOpenTexture.free();
	gRotateTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		SDL_Log( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			SDL_Log( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Touch variables
			SDL_Point touchLocation = { gScreenRect.w / 2, gScreenRect.h / 2 };
			LTexture* currentTexture = &gPinchOpenTexture;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//Window event
					else if( e.type == SDL_WINDOWEVENT )
					{
						//Window resize/orientation change
						if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
						{
							//Get screen dimensions
							gScreenRect.w = e.window.data1;
							gScreenRect.h = e.window.data2;

							//Update screen
							SDL_RenderPresent( gRenderer );
						}
					}
					//Multi touch gesture
					else if( e.type == SDL_MULTIGESTURE )
					{
						//Rotation detected
						if( fabs( e.mgesture.dTheta ) > 3.14 / 720.0 )
						{
							touchLocation.x = e.mgesture.x * gScreenRect.w;
							touchLocation.y = e.mgesture.y * gScreenRect.h;
							currentTexture = &gRotateTexture;
						}
						//Pinch detected
						else if( fabs( e.mgesture.dDist ) > 0.0002 )
						{
							touchLocation.x = e.mgesture.x * gScreenRect.w;
							touchLocation.y = e.mgesture.y * gScreenRect.h;

							//Pinch open
							if( e.mgesture.dDist > 0 )
							{
								currentTexture = &gPinchOpenTexture;
							}
							//Pinch close
							else
							{
								currentTexture = &gPinchCloseTexture;
							}
						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render touch texture
				currentTexture->render( touchLocation.x - currentTexture->getWidth() / 2, touchLocation.y - currentTexture->getHeight() / 2 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
*/
