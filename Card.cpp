#include "Card.h"

Card::Card()
{

}

Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r)
    : categories(inCat)
    , svHeader(inSvH)
    , svText(inSvT)
    , enHeader(inEnH)
    , enText(inEnT)
    , pos(inPos)
    , velocity(inVel)
    , lifeTime(clock())
    , path(textPath)
    , isReading(false)
    , isTrans(false)
{
    infoIndex = infoIndexGenerator++;
    load(r);
}


/*-----------------Transformation functions-----------------*/
void Card::move(float timeStep){
    if (!isReading) {
        pos[0] += velocity[0]*timeStep; // ((velocity[0] *curve)/10) ;
        pos[1] += velocity[1]*timeStep; // (velocity[1] * curve2);

        if(pos[0] > (SCREEN_WIDTH-SCREEN_WIDTH/6))
        {
            velocity[0] -= 5;
        }
        else if (pos[0] < SCREEN_WIDTH/6-getWidth() )
        {
            velocity[0] += 5;
        }

        if(pos[1] > (SCREEN_HEIGHT-SCREEN_HEIGHT/6) - 60 )
        {
            velocity[1] -= 5;
        }
        else if (pos[1] < SCREEN_HEIGHT/6-getHeight())
        {
            velocity[1] += 5;
        }
    }
}

//touch funcs
void Card::scale(SDL_Event* e){
    SDL_Finger* finger = SDL_GetTouchFinger(e->mgesture.touchId, 0);

    double scaleFactor = (sqrt(pow(finger->x - e->mgesture.x,2) + pow(finger->y - e->mgesture.y, 2))+e->mgesture.dDist)
    /sqrt(pow(finger->x - e->mgesture.x,2) + pow(finger->y - e->mgesture.y, 2));

    width *= scaleFactor;
    height *= scaleFactor;
}
//checks if finger coordinated are within the borders of the card
bool Card::isInside(int x, int y){
    bool inside = true;

    //finger is left of the card
    if( x < pos[0] )
    {
        inside = false;
    }
    //finger is right of the card
    else if( x > (pos[0] + width))
    {
        inside = false;
    }
    //finger above the card
    else if( y < pos[1] )
    {
        inside = false;
    }
    //finger below the card
    else if( y > (pos[1]+ height))
    {
        inside = false;
    }

    //if none of the above criteria is true, finger must be inside the card
    return inside;
}


bool Card::handleEvent( SDL_Event* e )
{
    bool isEvent = false;
    /*-----------------------------MOUSE_EVENT-------------------------------------------*/

    if( (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL))
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );


        if (isInside(x, y))
        {
            //calculate new position for card
            //float newPosx = x - touchPos.x;
            //float newPosy = y - touchPos.y;


            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:


                    /*if (isTrans  && newPosx < (SCREEN_WIDTH - width) && newPosx > 0 &&
                        newPosy < (SCREEN_HEIGHT - height) && newPosy > 0)
                    {
                        pos.x = newPosx;
                        pos.y = newPosy;
                        setLifeTime(clock() + CLOCKS_PER_SEC/2); //add time before death

                    }
                    */
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    isEvent = true;

                    setLifeTime(clock() + CLOCKS_PER_SEC/2);
                    touchPos.x = x - pos.x;
                    touchPos.y = y - pos.y;
                    isTrans = true;

                    /*if (touchPos.x == -1.0f)
                    {
                        touchPos.x = x - pos.x;
                        touchPos.y = y - pos.y;
                        isTrans = true;
                    }*/
                    // {static_cast<int>(getPos().x) + 3 , static_cast<int>(getPos().y) + 3 , getWidth() - 6 , getWidth() / 10}
                    if ((x > getPos().x + 3) && (x < getPos().x + getWidth() - 6 ) && (y > getPos().y + 3) && (y < getPos().y + getWidth() / 10 ) ) {
                        isReading = !isReading;
                    }

                    break;

                case SDL_MOUSEBUTTONUP:
                    //currentEvent = "BUTTON_SPRITE_MOUSE_UP";
                    //std::cout <<  currentEvent << std::endl;
                    touchPos = glm::vec2(-1.0f,-1.0f);
                    isTrans = false;
                    break;

                case SDL_MOUSEWHEEL:
                    double scaleFactor = 1.0;
                    setLifeTime(clock() + CLOCKS_PER_SEC/2);

                    if (e->wheel.y < 0 )
                        scaleFactor = 0.98;
                    else if (e->wheel.y > 0)
                        scaleFactor = 1.02;

                    width *= scaleFactor;
                    height *= scaleFactor;
                    isEvent = true;


                    break;
            }
        }
        /*if (isTrans) {
            SDL_GetMouseState( &x, &y );
            pos.x = x - touchPos.x;
            pos.y = y - touchPos.y;
        }*/
    }
    /*---------------------------------TOUCH_EVENT------------------------------------------*
    // if target is touched, activate it

    if (e->type == SDL_FINGERMOTION || e->type == SDL_FINGERDOWN|| e->type == SDL_FINGERUP)
    {
        int x = e->tfinger.x * SCREEN_WIDTH;
        int y = e->tfinger.y * SCREEN_HEIGHT;

        if (isInside(x, y))
        {
            float newPosx = pos.x+e->tfinger.dx*SCREEN_WIDTH;
            float newPosy = pos.y+e->tfinger.dy*SCREEN_HEIGHT;

            switch( e->type )
            {

                case SDL_FINGERDOWN:
                    setLifeTime(time(0) + 10);

                    touchPos.x = x - pos.x;
                    touchPos.y = y - pos.y;
                    isTrans = true;
                    isEvent = true;


                    break;
                case SDL_FINGERMOTION:

                    if (isTrans && newPosx < (SCREEN_WIDTH - width) && newPosx > 0 &&
                        newPosy < (SCREEN_HEIGHT - height) && newPosy > 0)
                    {
                        pos.x = pos.x+e->tfinger.dx*SCREEN_WIDTH;
                        pos.y = pos.y+e->tfinger.dy*SCREEN_HEIGHT;
                        setLifeTime(time(0) + 10); //add time before death
                    }

                    break;

                case SDL_FINGERUP:
                    touchPos = glm::vec2(-1.0f,-1.0f);
                    isTrans = false;

                    break;
            }
        }
    }*/
    return isEvent;
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

SDL_Texture* Card::getMainTexture() {
    return textTexture;
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

SDL_Texture* Card::getMainTextureEN() {
    return enTextTexture;
}

SDL_Texture* Card::getCategoryTexture() {
    return categoryTexture;
}


int Card::getLanguage()
{
    return language;
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

bool Card::getReading(){
    return isReading;
}

SDL_Texture* Card::getHeader(){
    return headers[infoIndex];
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

// var loadingText förut
bool Card::load(SDL_Renderer* r){
    // /Library/Fonts   Arial.ttf
    bool success = true;
    //SDL_Texture* newTexture = NULL;
    SDL_Surface* textSurface = NULL;

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    gFont = TTF_OpenFont( "OpenSans-SemiboldItalic.ttf", 100 );
    if( gFont == NULL )
    {
        printf( "Failed to load the font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    SDL_Color textColor = { 0, 0, 0};
    //std::string newText = "";

    textSurface = TTF_RenderText_Blended( gFont, svHeader.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        headers.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( headers[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    // main text
	textColor = { 0, 0, 0};
	textSurface = TTF_RenderText_Blended_Wrapped( gFont, svText.c_str(), textColor, 2000);
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Create texture from surface pixels
        textTexture = SDL_CreateTextureFromSurface( r, textSurface );
		if( textTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	// english main text
	textColor = { 255, 0, 0};
	textSurface = TTF_RenderText_Blended_Wrapped( gFont, enText.c_str(), textColor, 2000);
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Create texture from surface pixels
        enTextTexture = SDL_CreateTextureFromSurface( r, textSurface );
		if( textTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

    // create categories texture
	std::string temp=" ";

	for (int i = 0; i < categories.size(); i++)
    {
        temp.append(categories[i]);
        if (i < categories.size() - 1)
            temp.append(" - ");
    }

	// category
	textColor = { 255, 0, 0};
	textSurface = TTF_RenderText_Blended_Wrapped( gFont, temp.c_str(), textColor, 2000);
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Create texture from surface pixels
        categoryTexture = SDL_CreateTextureFromSurface( r, textSurface );
		if( categoryTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}





    TTF_CloseFont( gFont );
    gFont = NULL;



    // Render averyting to one texture



    return success;
}

void Card::render( SDL_Renderer* r) // Blir error atm
{
    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), static_cast<int>(width), static_cast<int>(height) };
    SDL_SetRenderDrawColor( r, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect( r, &renderQuad );
}

/*void Card::render( SDL_Renderer* gRenderer, SDL_Texture* texture) // Blir error atm
{
    //Set rendering space and render to screen

    SDL_Rect renderQuad = {static_cast<int>(pos.x) , static_cast<int>(pos.y), width, height };
    SDL_RenderCopy( gRenderer, texture, NULL, &renderQuad );
}*/
