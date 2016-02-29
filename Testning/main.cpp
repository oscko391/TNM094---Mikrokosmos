

/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL and standard IO
//#include <SDL2/SDL.h>
//#include <stdio.h>
/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include "glm.hpp"
#include "trigonometric.hpp"
#include <string>
#include <SDL2_image/SDL_image.h>
#include <cmath>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <iostream>


// KORT 1
glm::vec2 velocity;
glm::vec3 pos;

// KORT 2
glm::vec2 velocity2;
glm::vec3 pos2;

int a;
int b;
int width = 170;
int height = 158;

int a2;
int b2;
int width2 = 200;
int height2 = 149;


double curve;
double curve2;
int randomNumber;

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

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
    
    //Deallocates texture
    void free();
    
    //Renders texture at given point
    void render( int x, int y);
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    
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

//Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;
LTexture farTexture;
LTexture filmTexture;


LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
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
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    mTexture = newTexture;
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
    }
}

void LTexture::render( int x, int y)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool init()
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

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load Foo' texture
    if( !gFooTexture.loadFromFile( "//Users/madeleinerapp/Desktop/gris.jpg" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    
    if( !farTexture.loadFromFile( "//Users/madeleinerapp/Desktop/far.jpg" ) )
    {
        printf( "Failed to load Far' texture image!\n" );
        success = false;
    }
    
    /*   if( !filmTexture.loadFromFile( "//Users/madeleinerapp/Desktop/IMG_5814.MOV" ) )
     {
     printf( "Failed to load Film' texture image!\n" );
     success = false;
     }*/
    
    //Load background texture
    if( !gBackgroundTexture.loadFromFile( "/Users/madeleinerapp/Documents/LiU/TNM094 Kandidaten/projekt/SDL_tutorial/SDL_tutorial/space.bmp" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    
    
    
    return success;
}


void move( glm::vec2 vel,  std::time_t now ,glm::vec3 &position, int &aNum, int &bNum) //  void move(glm::vec2 velocity);
{
    //    pos = glm::vec3(200, 200, 200);
    //pos[1] = pos[1] + 1;
    
    
    std::time_t after = time(0);
    
    curve  = abs(10* sin ( after - now));
    curve2  = abs(10* cos ( after - now));
    randomNumber = rand() % 10 + 1;
    
    
    position[0] += ((vel[0] * aNum *curve * randomNumber)/100) ;
    position[1] += ((vel[1] * bNum *curve2 * randomNumber)/100); // + fungerar inte av npgon anledning...
    // std::cout << std::time<< std::endl;
    
    
    if(position[0] > (SCREEN_WIDTH-170) || position[0] < 0 ) // bildsize är 170
    {
        aNum = (aNum * (- 1));
    }
    
    if(position[1] > (SCREEN_HEIGHT-158) || position[1] < 0 )
    {
        bNum = (bNum * (- 1));
    }
    
    
}


/*
 bool collision(glm::vec3 &position, glm::vec3 &position2 )
 {
 //The sides of the rectangles
 int leftA, leftB;
 int rightA, rightB;
 int topA, topB;
 int bottomA, bottomB;
 
 //Calculate the sides of rect A
 leftA =     position[0];
 rightA =    position[0] + width;
 topA =    position[1];
 bottomA = position[1] + height;
 
 //Calculate the sides of rect B
 leftB = position2[0];
 rightB = position2[0] + width2;
 topB = position2[1];
 bottomB = position2[1] + height2;
 
 //If any of the sides from A are outside of B
 if( bottomA <= topB )
 {
 return false;
 }
 
 if( topA >= bottomB )
 {
 return false;
 }
 
 if( rightA <= leftB )
 {
 return false;
 }
 
 if( leftA >= rightB )
 {
 return false;
 }
 
 //If none of the sides from A are outside B
 return true;
 
 
 }*/



void close()
{
    //Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();
    farTexture.free();
    filmTexture.free();
    
    
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] ) // ------------------------main------------------
{
    velocity = glm::vec2(2.0, 3.0);
    velocity2 = glm::vec2(4.0, 1.0);
    
    //std::cout << velocity[0] << std::endl;
    
    
    pos = glm::vec3(133, 310, 200);
    pos = glm::vec3(500, 110, 100);
    
    a = 1;
    b = 1;
    a2 = 1;
    b2 = 1;
    std::time_t now = time(0);
    
    
    
    
    
    
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
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
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }
                
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                //Render background texture to screen
                // bool col = collision(pos, pos2 );
                
                gBackgroundTexture.render( 0, 0);
                move(velocity , now, pos, a, b );
                move(velocity2 , now, pos2, a2, b2);
                
                
                //Render Foo' to the screen
                gFooTexture.render( pos[0], pos[1]);
                farTexture.render( pos2[0], pos2[1]);
                
                
                
                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    
    
    //Free resources and close SDL
    close();
    
    return 0;
}


/// >>>>>>>>>>>>>>>> TEST <<<<<<<<<<<<<<<<

/*
 //Create default card in existing window and make the card move
 kortDef = Card(); //default constructor
 vec2 vel = [ 1, 2];
 kortDef.move(vel);
 
 //Create card with parameters in existing window and make the card move
 kortPar = Card(?);
 kortPar.move(vel);
 
 //Test read
 //test XML-file with 5 cards
 //test different move
 */


/// >>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<

