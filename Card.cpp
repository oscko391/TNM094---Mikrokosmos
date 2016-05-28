#include "Card.h"

Card::Card()
{
    
}

Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r)
: svHeader(inSvH)
, svText(inSvT)
, enHeader(inEnH)
, enText(inEnT)
, pos(inPos)
, velocity(inVel)
, lifeTime(clock())
, path(textPath)
, isReading(false)
, isTrans(false)
, categories(inCat)
{
    infoIndex = infoIndexGenerator++;
    loadingText(r);
    if (shadow == NULL) {
        shadow = loadShadow(r);
    }
    if (arrow == NULL) {
        arrow = loadArrow(r);
    }
}




/*-----------------Transformation functions-----------------*/
void Card::move(float timeStep){
    if (!isReading) {
        pos[0] += 0.66*velocity[0]*timeStep; // ((velocity[0] *curve)/10) ;
        pos[1] += 0.66*velocity[1]*timeStep; // (velocity[1] * curve2);
        angle += 0.1*sin(velocity[0]) + 0.1*sin(velocity[1]);
        if(pos[0] > (SCREEN_WIDTH-SCREEN_WIDTH/10))
        {
            velocity[0] -= 5;
        }
        else if (pos[0] < SCREEN_WIDTH/10-getWidth() )
        {
            velocity[0] += 5;
        }
        
        if(pos[1] > (SCREEN_HEIGHT-SCREEN_HEIGHT/10) - 60 )
        {
            velocity[1] -= 5;
        }
        else if (pos[1] < SCREEN_HEIGHT/10-getHeight())
        {
            velocity[1] += 5;
        }
        if (width > (SCREEN_WIDTH/7.0)) {
            width *= 0.99;
            height *= 0.99;
        }
    }
    
}

//touch funcs
void Card::scale(SDL_Event* e)
{
    SDL_Finger* finger = SDL_GetTouchFinger(e->mgesture.touchId, 0);
    
    double scaleFactor = (sqrt(pow(finger->x - e->mgesture.x,2) + pow(finger->y - e->mgesture.y, 2))+e->mgesture.dDist)
    /sqrt(pow(finger->x - e->mgesture.x,2) + pow(finger->y - e->mgesture.y, 2));
    
    width *= scaleFactor;
    height *= scaleFactor;
}
//checks if finger coordinated are within the borders of the card
bool Card::isInside(int x, int y)
{
    bool inside = true;
    
    //finger is left of the card
    if( x < - width/2 )
    {
        inside = false;
    }
    //finger is right of the card
    else if( x > (width/2))
    {
        inside = false;
    }
    //finger above the card
    else if( y < -height/2 )
    {
        inside = false;
    }
    //finger below the card
    else if( y > (height/2))
    {
        inside = false;
    }
    
    //if none of the above criteria is true, finger must be inside the card
    return inside;
}


bool Card::handleEvent( SDL_Event* e )
{
    bool isEvent = false;
    float lDist, dDist, dTheta, dist;
    /*-----------------------------MOUSE_EVENT-------------------------------------------*/
    
    if( (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL))
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        
        double new_x = x - (pos[0] + width/2);
        double new_y = y - (pos[1] + height/2);
        x = cos((M_PI * angle)/180)*new_x + sin((M_PI * angle)/180)*new_y;
        y = cos((M_PI * angle)/180)*new_y - sin((M_PI * angle)/180)*new_x;
        
        
        if (isInside(x, y))
        {
            //calculate new position for card
            //float newPosx = x - touchPos.x;
            //float newPosy = y - touchPos.y;
            
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    
                    /* if (isTrans) {
                     SDL_GetMouseState( &x, &y );
                     pos.x = x - touchPos.x;
                     pos.y = y - touchPos.y;
                     setLifeTime(clock() + 10*CLOCKS_PER_SEC/2);
                     } */
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    isEvent = true;
                    
                    setLifeTime(clock() + 10*CLOCKS_PER_SEC/2);
                    int x2, y2;
                    SDL_GetMouseState( &x2, &y2 );
                    touchPos.x = x2 - pos.x;
                    touchPos.y = y2 - pos.y;
                    isTrans = true;
                    
                    if (y < -height/2 + 9*width/50 && abs(x) < width/2 - width/25 ) {
                        isReading = !isReading;
                    }
                    else if (isReading && y > height/2 - width/10 && abs(x) < width/2 - width/25  ) {
                        isReading = false;
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
                    setLifeTime(clock() + 10*CLOCKS_PER_SEC/2);
                    
                    if (e->wheel.y < 0 )
                        scaleFactor = 0.98;
                    else if (e->wheel.y > 0)
                        scaleFactor = 1.02;
                    
                    if (width*scaleFactor < SCREEN_WIDTH/2) {
                        width *= scaleFactor;
                        height *= scaleFactor;
                    }
                    
                    isEvent = true;
                    
                    
                    break;
            }
        }
        
    }
    /*---------------------------------TOUCH_EVENT------------------------------------------*
     if (e->type == SDL_FINGERMOTION || e->type == SDL_FINGERDOWN|| e->type == SDL_FINGERUP)
     {
     double x = e->tfinger.x * SCREEN_WIDTH;
     double y = e->tfinger.y * SCREEN_HEIGHT;
     glm::vec2 lastP;
     glm::vec2 lastCentroid;
     glm::vec2 newCentroid;
     
     double new_x = x - (pos[0] + width/2);
     double new_y = y - (pos[1] + height/2);
     x = cos((M_PI * angle)/180)*new_x + sin((M_PI * angle)/180)*new_y;
     y = cos((M_PI * angle)/180)*new_y - sin((M_PI * angle)/180)*new_x;
     
     if (isInside(x, y))
     {
     isEvent = true;
     //save the touchfinger events in a variable
     SDL_TouchFingerEvent tf = e->tfinger;
     
     float newPosx = pos.x+e->tfinger.dx*SCREEN_WIDTH;
     float newPosy = pos.y+e->tfinger.dy*SCREEN_HEIGHT;
     
     switch( e->type )
     {
     
     case SDL_FINGERDOWN:
     
     if (numberFingers == 0 || fingerEvents[0].x == -1)
     {
     fingerEvents[0] = tf;
     numberFingers++;
     }
     else if (numberFingers == 1 || fingerEvents[1].x == -1)
     {
     fingerEvents[1] = tf;
     numberFingers++;
     }
     
     // std::cout << "nf: " << numberFingers << std::endl;
     
     setLifeTime(time(0) + 10);
     touchPos.x = x - pos.x;
     touchPos.y = y - pos.y;
     isTrans = true;
     
     isReading = !isReading;
     //                std::cout << isReading;
     
     //  std::cout << "number fingers: " << numberFingers << std::endl;
     
     break;
     case SDL_FINGERMOTION:
     
     //if only 1 finger move card
     if (numberFingers < 2)
     {
     fingerEvents[0] = tf;
     // makes sure cards can't be forced outside of screen
     if (isTrans )
     {
     //isEvent = true;
     pos.x = pos.x + tf.dx*SCREEN_WIDTH;
     pos.y = pos.y + tf.dy*SCREEN_HEIGHT;
     setLifeTime(time(0) + 10); //add time before death
     }
     }
     //otherwise it should be scaled and/or rotated
     else
     {
     SDL_TouchFingerEvent thatFinger;
     lastP.x = tf.x - tf.dx; //the fingers position
     lastP.y = tf.y - tf.dy;
     
     if (std::abs(fingerEvents[0].x - lastP.x) < EPSILON && std::abs(fingerEvents[0].y - lastP.y) < EPSILON)
     {
     thatFinger = fingerEvents[1];
     fingerEvents[0] = tf;
     }
     else if (std::abs(fingerEvents[1].x - (tf.x - tf.dx)) < EPSILON && std::abs(fingerEvents[1].y - (tf.y - tf.dy)) < EPSILON)
     {
     thatFinger = fingerEvents[0];
     fingerEvents[1] = tf;
     }
     
     lastCentroid.x = lastP.x - (lastP.x - thatFinger.x)/2;
     lastCentroid.y = lastP.y - (lastP.y - thatFinger.y)/2;
     
     newCentroid.x = lastCentroid.x + e->tfinger.dx/2;
     newCentroid.y = lastCentroid.y + e->tfinger.dy/2;
     
     glm::vec2 lv, v;
     
     lv.x = lastP.x - lastCentroid.x; //from center to last position
     lv.y = lastP.y - lastCentroid.y;
     
     lDist = sqrt(pow(lv.x,2) + pow(lv.y,2));
     
     v.x = tf.x - newCentroid.x; //from center to current pos
     v.y = tf.y - newCentroid.y;
     
     dist = sqrt(pow(v.x,2) + pow(v.y,2));
     
     dDist = (dist - lDist);
     
     //std::cout << "dDist " << dDist << std::endl;
     
     
     double fingerDist = sqrt(pow(thatFinger.x - lastP.x ,2)
     + pow(thatFinger.y - lastP.y ,2));
     
     
     double scaleFactor = (fingerDist + dDist)/fingerDist;
     
     //std::cout << "skal: " << scaleFactor << std::endl;
     if (std::abs(scaleFactor - 1.0) < 0.1)
     {
     width *= scaleFactor;
     height *= scaleFactor;
     }
     
     glm::vec2 p1 = glm::vec2(lastP.x - thatFinger.x, lastP.y - thatFinger.y);
     glm::vec2 p2 = glm::vec2(tf.x - thatFinger.x, tf.y - thatFinger.y);
     
     // arctan(v X u/v * u)
     dTheta = atan2(lv.x*v.y - lv.y*v.x,lv.x*v.x + lv.y*v.y)*180/3.14;
     //dTheta = acos((v.x*lv.x + v.y*lv.y)/(sqrt(pow(v.x,2) + sqrt(pow(v.y,2)))*sqrt(pow(lv.x,2)+pow(lv.y,2))));
     
     
     if (std::abs(dTheta) > 3.14/1080)
     {
     angle += dTheta;
     }
     
     
     }
     
     break;
     
     case SDL_FINGERUP:
     
     if (std::abs(tf.x - fingerEvents[0].x) < EPSILON && std::abs(tf.y - fingerEvents[0].y) < EPSILON)
     {
     fingerEvents[0].x = -1; std::cout << "finger 1 upp" << std::endl;
     }
     else if (std::abs(tf.x - fingerEvents[1].x) < EPSILON && std::abs(tf.y - fingerEvents[1].y) < EPSILON)
     {
     fingerEvents[1].x = -1;
     std::cout << "finger 2 upp" << std::endl;
     }
     
     touchPos = glm::vec2(-1.0f,-1.0f);
     if (isTrans)
     numberFingers--;
     
     if (numberFingers == 0)
     isTrans = false;
     
     //  std::cout << "up: " << std::abs(tf.x + tf.dx - fingerEvents[0].x) << std::endl;
     
     
     std::cout << "nf : " << numberFingers << std::endl;
     
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

bool Card::getReading(){
    return isReading;
}

SDL_Texture* Card::getHeader(){
    return headersSv[infoIndex];
}

double Card::getAngle() {
    return angle;
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

bool Card::loadingText(SDL_Renderer* r){
    // /Library/Fonts   Arial.ttf
    bool success = true;
    
    // Loading font
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    TTF_Font *gFont = NULL;
    gFont = TTF_OpenFont( "/Library/Fonts/Arial.ttf", 100 ); // Apple: "/Library/Fonts/Arial.ttf"
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    SDL_Color textColor = { 0, 0, 0};
    
    // Loading text
    SDL_Surface* textSurface = NULL;
    // Svenska header
    textSurface = TTF_RenderText_Blended( gFont, svHeader.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        headersSv.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( headersSv[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    // Svenska text
    textSurface = NULL;
    textSurface = TTF_RenderText_Blended_Wrapped( gFont, svText.c_str(), textColor, 3000);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        infoTextSv.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( infoTextSv[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    // Svenska kategorier
    textSurface = NULL;
    std::string catString = "Placeholder";
    /*for (int i = 0; i < categories.size(); i++) {
     if (i > 0) {
     catString += " - ";
     catString += categories[i].c_str() ;
     }
     else {
     catString = categories[i].c_str() ;
     }
     }*/
    textSurface = TTF_RenderText_Blended( gFont, catString.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        catTextSv.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( catTextSv[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    
    // English header
    textSurface = NULL;
    textSurface = TTF_RenderText_Blended( gFont, enHeader.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        headersEn.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( headersEn[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    // English text
    textSurface = NULL;
    textSurface = TTF_RenderText_Blended_Wrapped( gFont, enText.c_str(), textColor, 3000);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        infoTextEn.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( infoTextEn[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    // Svenska kategorier
    textSurface = NULL;
    catString = "Placeholder";
    /*for (int i = 0; i < categories.size(); i++) {
     if (i > 0) {
     catString += " - ";
     catString += categories[i].c_str() ;
     }
     else {
     catString = categories[i].c_str() ;
     }
     }*/
    textSurface = TTF_RenderText_Blended( gFont, catString.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        catTextEn.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( catTextEn[infoIndex] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    return success;
}


SDL_Texture* Card::loadShadow(SDL_Renderer* r) {
    SDL_Texture* newTexture = SDL_CreateTexture( r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100 , 100 );
    SDL_SetRenderTarget( r, newTexture );
    
    SDL_SetRenderDrawColor( r, 0x20, 0x20, 0x20, 0xFF );
    SDL_RenderFillRect( r , NULL );
    
    SDL_SetRenderTarget( r, NULL );
    return newTexture;
}

SDL_Texture* Card::loadArrow(SDL_Renderer* r) {
    SDL_Texture* imgTexture = NULL;
    
    //Load image at specified path
    //SDL_Surface* loadedSurface = IMG_Load( "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/icons/arrow.png" ); //My
     SDL_Surface* loadedSurface = IMG_Load( "/Users/madeleinerapp/Documents/LiU/Githubmappen/TNM094---Mikrokosmos/icons/arrow.png"); //Madde
    
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image arrow! SDL_image Error: %s\n", IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        imgTexture = SDL_CreateTextureFromSurface( r, loadedSurface );
        if( imgTexture == NULL )
        {
            printf( "Unable to create texture from arrow ...! SDL Error: %s\n",  SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return imgTexture;
}

void Card::render( SDL_Renderer* r, bool swede) // Blir error atm
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
