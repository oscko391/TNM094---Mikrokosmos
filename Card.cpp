#include "Card.h"

Card::Card()
{

}

Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel, std::string textPath)
    : svHeader(inSvH)
    , svText(inSvT)
    , enHeader(inEnH)
    , enText(inEnT)
    , isSwede(lang)
    , pos(inPos)
    , velocity(inVel)
    , imgPath(textPath)
{
}


/*-----------------Transformation functions-----------------*/
void Card::move( time_t now )
{

    time_t after = time(0);

    double curve  = sin ((after - now)*0.00735);
    double curve2  = sin((after - now) * 0.001);

    pos[0] += ((velocity[0] *curve)/10) ;
    pos[1] += (velocity[1] * curve2);

    if(pos[0] > (SCREEN_WIDTH-width) || pos[0] < 0 ) // bildsize är 170
    {
        velocity[0]  = (velocity[0] * (- 1));
    }

    if(pos[1] > (SCREEN_HEIGHT-height) || pos[1] < 0 )
    {
        velocity[1]  = (velocity[1]  * (- 1));
    }
}


bool Card::handleEvent( SDL_Event* e )
{
    std::string currentEvent;
    //If mouse event happened
    if( (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP))
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

                if (isTrans)
                {
                    pos.x = x - touchPos.x;
                    pos.y = y - touchPos.y;
                    setLifeTime(time(0) + 10); //add time before death
                }

                break;

            case SDL_MOUSEBUTTONDOWN:
                currentEvent = "BUTTON_SPRITE_MOUSE_DOWN";
                std::cout << currentEvent << std::endl;
                setLifeTime(time(0) + 10);

                if (touchPos.x == -1.0f)
                {
                    touchPos.x = x - pos.x;
                    touchPos.y = y - pos.y;
                    isTrans = true;
                    std::cout << "IF" << std::endl;
                }



                //touchPos.x = x - pos.x;
                //touchPos.y = y - pos.y;


                return true;

                break;

            case SDL_MOUSEBUTTONUP:
                //currentEvent = "BUTTON_SPRITE_MOUSE_UP";
                //std::cout <<  currentEvent << std::endl;
                touchPos = glm::vec2(-1.0f,-1.0f);
                isTrans = false;
                break;
            }
        }
    }
    // if target is touched, activate it
    if (e->type == SDL_FINGERMOTION || e->type == SDL_FINGERDOWN|| e->type == SDL_FINGERUP)
    {
        int x = e->tfinger.x * SCREEN_WIDTH;
        int y = e->tfinger.y * SCREEN_HEIGHT;

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
            case SDL_FINGERMOTION:


                break;

            case SDL_FINGERDOWN:
                currentEvent = "BUTTON_SPRITE_MOUSE_DOWN";
                std::cout << currentEvent << std::endl;
                setLifeTime(time(0) + 10);
                return true;

                break;

            case SDL_FINGERUP:
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
    SDL_DestroyTexture(cardTexture);
    cardTexture = NULL;
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

double Card::getLifeTime()
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

std::string Card::getImgPath()
{
    return imgPath;
}


std::string Card::getEnHeader()
{
    return enHeader;
}

std::string Card::getEnText()
{
    return enText;
}

bool Card::getIsSwede()
{
    return isSwede;
}

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

void Card::setImgPath(std::string s)
{
    imgPath = s;
}


void Card::setWidth(int w)
{
    width = w;
}

void Card::setLifeTime(double inLife)
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

void Card::changeLang()
{
    if(isSwede)
        isSwede = false;
    else
        isSwede = true;
}

/*----------------------------------------------------------------------------*/



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
}

void Card::render( SDL_Renderer* gRenderer) // Blir error atm
{
    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), width, height };

    SDL_RenderCopy( gRenderer, cardTexture, NULL, &renderQuad );
}

void Card::renderActive( SDL_Renderer* gRenderer) // Blir error atm
{
    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), static_cast<int>(floor(width*1.5f)), static_cast<int>(floor(height*1.5f)) };

    SDL_RenderCopy( gRenderer, cardTexture, NULL, &renderQuad );
}
