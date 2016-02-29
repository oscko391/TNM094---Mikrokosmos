#include "Card.h"

Card::Card()
{

}

Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel)
    : svHeader(inSvH)
    , svText(inSvT)
    , enHeader(inEnH)
    , enText(inEnT)
    , isSwede(lang)
    , pos(inPos)
    , velocity(inVel)
{
}


/*-----------------Transformation functions-----------------*/
/*void move( glm::vec2 &vel,  std::time_t now ,glm::vec3 &position){
    
    std::time_t after = time(0);
    
    double curve  = abs(10* sin ( after - now));
    double curve2  = abs(10* cos ( after - now));
    int randomNumber = rand() % 10 + 1;
    
    
    position[0] += ((vel[0] *curve * randomNumber)/100) ;
    position[1] += ((vel[1] *curve2 * randomNumber)/100); // + fungerar inte av npgon anledning...
    // std::cout << std::time<< std::endl;
    
    
    if(position[0] > (SCREEN_WIDTH-170) || position[0] < 0 ) // bildsize är 170
    {
        vel[0]  = (vel[0] * (- 1));
    }
    
    if(position[1] > (SCREEN_HEIGHT-158) || position[1] < 0 )
    {
        vel[1]  = (vel[1]  * (- 1));
    }
}*/

// Destructor
Card::~Card()
{
    SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;
	SDL_FreeSurface( gScreenSurface);
	gScreenSurface = NULL;

	//Quit SDL subsystems
	//IMG_Quit();
	//SDL_Quit();
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

SDL_Surface* Card::getPngSurface()
{
    return gPNGSurface;
}

SDL_Surface* Card::getScreenSurface()
{
    return gScreenSurface;
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
bool Card::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load PNG surface
    gPNGSurface = loadSurface( imgPath );
    //setPngSurface(loadSurface(imgPath));
    if( gPNGSurface == NULL )
    {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }

    return success;
}



SDL_Surface* Card::loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, getScreenSurface()->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

void Card::setPngSurface(SDL_Surface* s)
{
    gPNGSurface = s;
}

void Card::setgScreenSurface(SDL_Surface* surf)
{
    gScreenSurface = surf;
}
