#include "Card.h"

Card::Card()
{

}

Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel, std::string textPath)
    : categories(inCat)
    , svHeader(inSvH)
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

    if (std::abs(velocity.x > 2))
        velocity*=0.5;
}

void Card::scale(SDL_Event* e)
{
    SDL_Finger* finger = SDL_GetTouchFinger(e->mgesture.touchId, 0);

    double scaleFactor = (sqrt(pow(finger->x - e->mgesture.x,2) + pow(finger->y - e->mgesture.y, 2))+e->mgesture.dDist)
                              /sqrt(pow(finger->x - e->mgesture.x,2) + pow(finger->y - e->mgesture.y, 2));

    width *= scaleFactor;
    height *= scaleFactor;
}

bool Card::handleEvent( SDL_Event* e )
{
    /*-----------------------------MOUSE_EVENT-------------------------------------------*/

    if( (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP))
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );


        if (isInside(x, y))
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

                setLifeTime(time(0) + 10);


                if (touchPos.x == -1.0f)
                {
                    touchPos.x = x - pos.x;
                    touchPos.y = y - pos.y;
                    isTrans = true;
                }


                //return true;

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

    /*---------------------------------TOUCH_EVENT------------------------------------------*/
    // if target is touched, activate it
    /*
        if (e->type == SDL_FINGERMOTION || e->type == SDL_FINGERDOWN|| e->type == SDL_FINGERUP)
        {
            int x = e->tfinger.x * SCREEN_WIDTH;
            int y = e->tfinger.y * SCREEN_HEIGHT;

            if (isInside(x, y)
            {
                switch( e->type )
                {

                case SDL_FINGERDOWN:
                    setLifeTime(time(0) + 10);

                        touchPos.x = x - pos.x;
                        touchPos.y = y - pos.y;
                        isTrans = true;
                        std::cout << "IF" << std::endl;


                    break;
                case SDL_FINGERMOTION:

                    if (isTrans)
                    {
                        std::cout << pos.x << std::endl;
                        pos.x = pos.x+e->tfinger.dx;
                        pos.y = pos.y+e->tfinger.dy;
                        setLifeTime(time(0) + 10); //add time before death
                        std::cout << "efter:" << pos.x << std::endl;

                    }

                    break;

                case SDL_FINGERUP:
                    touchPos = glm::vec2(-1.0f,-1.0f);
                    isTrans = false;

                    break;
                }
            }
        }*/

    /*--------------------------------------------MULTI_TOUCH-------------------------------------------*/

    if ( e->type == SDL_MULTIGESTURE)
    {
        int x = e->mgesture.x * SCREEN_WIDTH;
        int y = e->mgesture.y * SCREEN_HEIGHT;

        if (isInside(x ,y))
        {
            // scaling if fingers are pinching
            if (fabs( e->mgesture.dDist ) > 0.0002)
            {
                scale(e);

            }
            if (fabs( e->mgesture.dTheta ) > 3.14 / 1080.0 )
            {
                angle += e->mgesture.dTheta * 180/3.14;

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

bool Card::loadFromRenderedText(SDL_Renderer* gRenderer )
{
    bool success = true;
    SDL_Texture* newTexture = NULL;


         //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //Open the font
    TTF_Font *gFont = NULL;
	gFont = TTF_OpenFont( "C:/Users/Eleonora/Documents/SDL_CodeBlocks/mikrokosmos_codeblocks/OpenSans-SemiboldItalic.ttf", 100 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}


	//Render text surface
	//SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, categories[0].c_str(), textColor );

	std::string temp="";

	for (int i = 0; i < categories.size(); i++)
    {
        temp.append(categories[i]);
        if (i < categories.size() - 1)
            temp.append(" - ");
    }

	// category
	SDL_Color textColor = { 0, 0, 0};
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( gFont, temp.c_str(), textColor, 1000);
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Create texture from surface pixels
        categoryTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( categoryTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}




	// main text
	textColor = { 0, 0, 0};
	textSurface = TTF_RenderText_Blended_Wrapped( gFont, svText.c_str(), textColor, 1000);
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Create texture from surface pixels
        textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( textTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}


	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;



	//Return success
	return true; //headingTexture != NULL;
}


void Card::render( SDL_Renderer* gRenderer) // Blir error atm
{
    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), width, height };

    SDL_RenderCopyEx( gRenderer, cardTexture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE );
}

void Card::renderActive( SDL_Renderer* gRenderer) // Blir error atm
{


    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), static_cast<int>(floor(width*1.1f)), static_cast<int>(floor(height*1.1f)) };

    SDL_RenderCopyEx( gRenderer, cardTexture, NULL, &renderQuad, angle , NULL, SDL_FLIP_NONE);


    // render categories
    SDL_Rect categoriesRenderQuad = {static_cast<int>(pos.x + 0.1 * width) , static_cast<int>(pos.y + height * 0.1), static_cast<int>(floor(width*0.8f)), static_cast<int>(floor(height*0.10f)) };
    SDL_RenderCopyEx( gRenderer, categoryTexture, NULL, &categoriesRenderQuad, angle , NULL, SDL_FLIP_NONE);


    // render main text
    SDL_Rect textRenderQuad = {static_cast<int>(pos.x + 0.1 * width) , static_cast<int>(pos.y + height * 0.25), static_cast<int>(floor(width*0.8f)), static_cast<int>(floor(height*0.75f)) };
    SDL_RenderCopyEx( gRenderer, textTexture, NULL, &textRenderQuad, angle , NULL, SDL_FLIP_NONE);

}
bool Card::isInside(int x, int y)
{
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

    return inside;

}
