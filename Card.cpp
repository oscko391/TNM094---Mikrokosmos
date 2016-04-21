#include "Card.h"

Card::Card()
{

}

Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath)
    : svHeader(inSvH)
    , svText(inSvT)
    , enHeader(inEnH)
    , enText(inEnT)
    , pos(inPos)
    , velocity(inVel)
    , lifeTime(clock())
    , path(textPath)
{
    //lifeTime = clock();
}


/*-----------------Transformation functions-----------------*/
void Card::move(){
    
    pos[0] += 0.2*velocity[0]; // ((velocity[0] *curve)/10) ;
    pos[1] += 0.2*velocity[1]; // (velocity[1] * curve2);
    
    if(pos[0] > (SCREEN_WIDTH-SCREEN_WIDTH/6))
    {
        velocity[0] -= 0.05;
    }
    else if (pos[0] < SCREEN_WIDTH/6-width )
    {
        velocity[0] += 0.05;
    }
    
    if(pos[1] > (SCREEN_HEIGHT-SCREEN_HEIGHT/6) )
    {
        velocity[1] -= 0.05;
    }
    else if (pos[1] < SCREEN_HEIGHT/6-height)
    {
        velocity[1] += 0.05;
    }

}


bool Card::handleEvent( SDL_Event* e )
{
    std::string currentEvent;
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        
        //Check if mouse is in button
        bool inside = true;
        
        
        //Mouse is left of the button
        if( x < pos[0] )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > (pos[0] + width))
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < pos[1] )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > (pos[1]+ height))
        {
            inside = false;
        }
        // _______
        
        //Mouse is outside button
        if( !inside )
        {
            // currentEvent = "BUTTON_SPRITE_MOUSE_OUT";
            //std::cout << currentEvent << std::endl;
            
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    // currentEvent = "BUTTON_SPRITE_MOUSE_OVER_MOTION";
                    // std::cout << currentEvent << std::endl;
                    
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    currentEvent = "BUTTON_SPRITE_MOUSE_DOWN";
                    std::cout << currentEvent << std::endl;
                    //setLifeTime(time(0) + 10);
                    setLifeTime(clock() + CLOCKS_PER_SEC/2);
                    return true;
                    
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    //currentEvent = "BUTTON_SPRITE_MOUSE_UP";
                    //std::cout <<  currentEvent << std::endl;
                    
                    break;
            }
        }
    }
    return false;
}



// Destructor
Card::~Card()
{
  //SDL_DestroyTexture(cardTexture);
  //cardTexture = NULL;
}
/*----------------------------------------------------------*/

/* --------------------GETTERS------------------------*/
int Card::getHeight()
{
    return height;
}

int Card::getWidth()
{
    return width;
}

clock_t Card::getLifeTime() const 
{
    return lifeTime;
}

std::vector<std::string> Card::getCategories()
{
    return categories;
}

std::string Card::getSvHeader()
{
    return svHeader;
}

std::string Card::getSvText()
{
    return svText;
}

std::string Card::getPath() const
{
    return path;
}


std::string Card::getEnHeader()
{
    return enHeader;
}

std::string Card::getEnText()
{
    return enText;
}
/*
bool Card::getIsSwede()
{
    return isSwede;
}*/

glm::vec3 Card::getPos()
{
    return pos;
}

glm::vec2 Card::getVelocity()
{
    return velocity;
}



/*---------------------------SETTERS-----------------------------------------*/
void Card::setHeight(int h)
{
    height = h;
}

/*void Card::setImgPath(std::string s)
{
    imgPath = s;
}*/


void Card::setWidth(int w)
{
    width = w;
}

void Card::setLifeTime(clock_t inLife)
{
    lifeTime = inLife;
}

void Card::addCategory( std::string inCat)
{
    categories.push_back(inCat);
}

void Card::setPos(glm::vec3 inPos)
{
    pos = inPos;
}

void Card::setVelocity(glm::vec2 inVel)
{
    velocity = inVel;
}
/*
void Card::changeLang()
{
    if(isSwede)
        isSwede = false;
    else
        isSwede = true;
}*/

/*----------------------------------------------------------------------------*/


/*
bool Card::loadTexture(SDL_Renderer* gRenderer)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(imgPath.c_str());
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", imgPath.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", imgPath.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    cardTexture = newTexture;
    return cardTexture != NULL;
}*/

void Card::render( SDL_Renderer* gRenderer) // Blir error atm
{
    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), width, height };
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect( gRenderer, &renderQuad );
}

/*void Card::render( SDL_Renderer* gRenderer, SDL_Texture* texture) // Blir error atm
{
    //Set rendering space and render to screen
    
    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), width, height };
    SDL_RenderCopy( gRenderer, texture, NULL, &renderQuad );
}*/

