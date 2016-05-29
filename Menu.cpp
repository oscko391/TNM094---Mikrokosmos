//
//  Menu.cpp
//
//
//  Created by My Rydberg on 2016-04-28.
//

#include "Menu.h"

Menu::Menu(int h, SDL_Renderer* r, std::vector<Category> cats, std::vector<std::string> sv, std::vector<std::string> en)
    : width(SCREEN_WIDTH - 2*h)
    , height(h)
    , posX(h)
    , posY(SCREEN_HEIGHT - 1.5*h)
{
    categories = cats.size();
    currentCat = 0;
    pixPerCat = (width - 5*height)/categories;
    
    stories = sv.size();
    currentStory = 0;
    pixPerStory = (width - 6.5*height)/stories;
    
    square = posX + 2.5*height;
    
    loadingText(r,cats, sv, en);
    loadingTexture(r);
    //isSwedish = true;
}

Menu::~Menu()
{
    //Deallocate

}

bool Menu::loadingText(SDL_Renderer* r, std::vector<Category> cats, std::vector<std::string> sv, std::vector<std::string> en){
    // /Library/Fonts   Arial.ttf
    bool success = true;
    //SDL_Texture* newTexture = NULL;
    SDL_Surface* textSurface = NULL;



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

    SDL_Color textColor = { 255, 255, 255};
    //std::string newText = "";

    for (int i = 0; i < categories; i++) {
        // Swedish text
        textSurface = TTF_RenderText_Blended( gFont, cats[i].getTextSv().c_str(), textColor);
        if( textSurface == NULL )
        {
            printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Create texture from surface pixels
            svCat.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
            if( svCat[i] == NULL )
            {
                printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            //Get rid of old surface
            SDL_FreeSurface( textSurface );
        }

        // English text
        textSurface = TTF_RenderText_Blended( gFont, cats[i].getTextEn().c_str(), textColor);
        if( textSurface == NULL )
        {
            printf( "Unable to render Eng text surface! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Create texture from surface pixels
            enCat.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
            if( enCat[i] == NULL )
            {
                printf( "Unable to create Eng texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            //Get rid of old surface
            SDL_FreeSurface( textSurface );
        }
    }
    
    for (int i = 0; i < stories; i++) {
        // Swedish text
        textSurface = TTF_RenderText_Blended( gFont, sv[i].c_str(), textColor);
        if( textSurface == NULL )
        {
            printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Create texture from surface pixels
            svStory.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
            if( svStory[i] == NULL )
            {
                printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            //Get rid of old surface
            SDL_FreeSurface( textSurface );
        }
        
        // English text
        textSurface = TTF_RenderText_Blended( gFont, en[i].c_str(), textColor);
        if( textSurface == NULL )
        {
            printf( "Unable to render Eng text surface! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Create texture from surface pixels
            enStory.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
            if( enStory[i] == NULL )
            {
                printf( "Unable to create Eng texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            //Get rid of old surface
            SDL_FreeSurface( textSurface );
        }
    }

    // Swedish text
    std::string newText = "English";
    textSurface = TTF_RenderText_Blended( gFont, newText.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        svCat.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( svCat[categories] == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    // English text
    newText = "Svenska";
    textSurface = TTF_RenderText_Blended( gFont, newText.c_str(), textColor);
    if( textSurface == NULL )
    {
        printf( "Unable to render Eng text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Create texture from surface pixels
        enCat.push_back( SDL_CreateTextureFromSurface( r, textSurface ));
        if( enCat[categories] == NULL )
        {
            printf( "Unable to create Eng texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    TTF_CloseFont( gFont );
    gFont = NULL;

    return success;
}

void Menu::loadingTexture(SDL_Renderer* r) {
    SDL_Texture* imgTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/icons/home.png" );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image home! SDL_image Error: %s\n", IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        imgTexture = SDL_CreateTextureFromSurface( r, loadedSurface );
        if( imgTexture == NULL )
        {
            printf( "Unable to create texture from home! SDL Error: %s\n", SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    homeIcon = imgTexture;
    
    SDL_Texture* imgTexture2 = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface2 = IMG_Load( "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/icons/story.png" );
    if( loadedSurface2 == NULL )
    {
        printf( "Unable to load image story! SDL_image Error: %s\n", IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        imgTexture2 = SDL_CreateTextureFromSurface( r, loadedSurface2 );
        if( imgTexture2 == NULL )
        {
            printf( "Unable to create texture from story! SDL Error: %s\n", SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface2 );
    }
    
    storyIcon = imgTexture2;
    
    SDL_Texture* imgTexture3 = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface3 = IMG_Load( "/Users/my/Documents/LiU/Kandidat/SDL_tutorial/icons/updown.png" );
    if( loadedSurface3 == NULL )
    {
        printf( "Unable to load image story! SDL_image Error: %s\n", IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        imgTexture3 = SDL_CreateTextureFromSurface( r, loadedSurface3 );
        if( imgTexture3 == NULL )
        {
            printf( "Unable to create texture from story! SDL Error: %s\n", SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface3 );
    }
    
    updownIcon = imgTexture3;

}



void Menu::renderMenu(SDL_Renderer* r, bool lang, bool story)
{
    SDL_SetRenderDrawColor( r, 0x35, 0x35, 0x30, 0xFF );
    
    SDL_Rect slide = {posX, posY, static_cast<int>(2*height) + 5, height};
    SDL_RenderFillRect(r, &slide);
    
    SDL_Rect firstSquare = {posX, posY, height, height };
    SDL_RenderCopy( r, homeIcon, NULL, &firstSquare );
    
    SDL_Rect secondSquare = {static_cast<int>(posX + height + 5), posY, height, height };
    SDL_RenderCopy( r, storyIcon, NULL, &secondSquare );
    
    SDL_Rect langQuad = {posX + width - 2*height, posY, 2*height, height};
    SDL_RenderFillRect(r, &langQuad);
    
    SDL_Rect mode {1,1,1,1};

    int text_w, text_h;
    double f;
    if (lang) {
        SDL_QueryTexture(svCat[categories], NULL, NULL, &text_w, &text_h);
    }
    else {
        SDL_QueryTexture(enCat[categories], NULL, NULL, &text_w, &text_h);
    }
    
    if (text_h > 2*height/3) {
        f = (2*height/3)/static_cast<double>(text_h);
        text_w *= f;
        text_h *= f;
    }
    if (text_w > 2*height - 10) {
        f = (2*height - 10)/static_cast<double>(text_w);
        text_w *= f;
        text_h *= f;
    }
    int smaller_w = ((2*height) - text_w)/2;
    int smaller_h = (height - text_h)/2;
    SDL_Rect button = {posX + width - 2*height+ smaller_w, posY + smaller_h, static_cast<int>(text_w) , static_cast<int>(text_h) };
    
    if (lang) {
        SDL_RenderCopy( r, svCat[categories], NULL, &button );
    }
    else {
        SDL_RenderCopy( r, enCat[categories], NULL, &button );
    }
    
    SDL_Rect menuQuad = {1, 1, 1, 1};
    SDL_Rect chosenQuad = {1,1,1,1};
    if (!story) {
        menuQuad = {static_cast<int>(posX + 2.5*height) , posY, static_cast<int>(width - 5*height), height};
        SDL_RenderFillRect( r, &menuQuad );
        
        chosenQuad = {square, posY - 5, pixPerCat, height + 10};
        mode = {posX - 5, posY - 5, height + 10, height + 10 };
        
        //int text_w, text_h;
        //double f = 1.0;
        for (int i = 0; i < categories; i++) {
            if (lang) {
                SDL_QueryTexture(svCat[i], NULL, NULL, &text_w, &text_h);
            }
            else {
                SDL_QueryTexture(enCat[i], NULL, NULL, &text_w, &text_h);
            }
            
            if (text_h > 2*height/3) {
                f = (2*height/3)/static_cast<double>(text_h);
                text_w *= f;
                text_h *= f;
            }
            if (text_w > pixPerCat) {
                f = pixPerCat/static_cast<double>(text_w);
                text_w *= f;
                text_h *= f;
            }
            
            int smaller_w = (pixPerCat - text_w)/2;
            int smaller_h = (height - text_h)/2;
            SDL_Rect button = {static_cast<int>(posX + 2.5*height) + pixPerCat*i + smaller_w, posY + smaller_h, static_cast<int>(text_w) , static_cast<int>(text_h) };
            
            if (lang) {
                SDL_RenderCopy( r, svCat[i], NULL, &button );
            }
            else {
                SDL_RenderCopy( r, enCat[i], NULL, &button );
            }
        }
        
    }
    else {
        menuQuad = {static_cast<int>(posX + 2.5*height) , posY, static_cast<int>(width - 6.5*height), height};
        SDL_RenderFillRect( r, &menuQuad );
        
        SDL_Rect flippQuad = {static_cast<int>(posX + width - 3.5*height), posY, height, height};
        SDL_RenderFillRect( r, &flippQuad );
        SDL_RenderCopy( r, updownIcon, NULL, &flippQuad );
        
        // posX + 3*height + pixPerStory*currentStory
        chosenQuad = {square, posY - 5, pixPerStory, height + 10};
        mode = {static_cast<int>(posX + height) , posY - 5, height + 10, height + 10 };
        
        
        //int text_w, text_h;
        //double f = 1.0;
        for (int i = 0; i < stories; i++) {
            if (lang) {
                SDL_QueryTexture(svStory[i], NULL, NULL, &text_w, &text_h);
            }
            else {
                SDL_QueryTexture(enStory[i], NULL, NULL, &text_w, &text_h);
            }
            
            if (text_h > 2*height/3) {
                f = (2*height/3)/static_cast<double>(text_h);
                text_w *= f;
                text_h *= f;
            }
            if (text_w > pixPerStory) {
                f = pixPerStory/static_cast<double>(text_w);
                text_w *= f;
                text_h *= f;
            }
            
            int smaller_w = (pixPerStory - text_w)/2;
            int smaller_h = (height - text_h)/2;
            SDL_Rect button = {static_cast<int>(posX + 2.5*height) + pixPerStory*i + smaller_w, posY + smaller_h, static_cast<int>(text_w) , static_cast<int>(text_h) };
            
            if (lang) {
                SDL_RenderCopy( r, svStory[i], NULL, &button );
            }
            else {
                SDL_RenderCopy( r, enStory[i], NULL, &button );
            }
        }
        SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0x60);
        SDL_RenderDrawRect(r, &flippQuad);
        
    }
    
    
    
    SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0x60);
    SDL_RenderDrawRect(r, &slide);
    SDL_RenderDrawRect(r, &menuQuad);
    SDL_RenderDrawRect(r, &langQuad);
    SDL_SetRenderDrawColor( r, 0xB0, 0xB0, 0xB0, 0x50 );
    SDL_RenderFillRect( r, &chosenQuad );
    SDL_RenderFillRect( r, &mode );

    

}


bool Menu::handleEvent( SDL_Event* e , bool &lang, bool &story, bool &flipp) {
    if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState( &x, &y );
        if (x > posX && x < posX + width && y > posY && y < posY + height) {
            switch( e->type )
            {
                case SDL_MOUSEBUTTONDOWN:
                    return downEvent(x, y, lang, story, flipp);
                    break;
                    
                case SDL_MOUSEMOTION:
                    if (!story && x > posX + 2.5*height && x < posX + width - 2.5*height) {
                        return moveEvent(x, y, story);
                    }
                    else if (story && x > posX + 2.5*height && x < posX + width - 4*height) {
                        return moveEvent(x, y, story);
                    }
                    else {
                        isMoving = false;
                        return false;
                    }
                    
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    return upEvent(story);
                    break;
            }
        }
        else {
            isMoving = false;
            if (!story) {
                square = posX + 2.5*height + pixPerCat*currentCat;
            }
            else {
                square = posX + 2.5*height + pixPerStory*currentStory;
            }
            pressed = 0;
        }

    }
    
    /*
     if (e->type == SDL_FINGERDOWN ) {
        int x = e->tfinger.x * SCREEN_WIDTH;
        int y = e->tfinger.y * SCREEN_HEIGHT;
        return downEvent(x, y, lang, story);
     }*/
    return false;
}

bool Menu::downEvent(int x, int y, bool &lang, bool &story, bool &flipp) {

    if (x < posX + height)
    {
        story = false;
        flipp = false;
        //currentStory = 0;
        //square = posX + 3*height;
        upEvent(story);
        return true;
    }
    else if (x < posX + 2*height + 5) {
        story = true;
        upEvent(story);
        return true;
    }
    else if (x > posX + width - 2*height) {
        lang = !lang;
        return true;
    }
    else {
        int p = posX + 2.5*height;
        if (!story) {
            for (int i = 0; i < categories; i++) {
                p += pixPerCat;
                if (x < p) {
                    isMoving = true;
                    currentCat = i;
                    square = posX + 2.5*height + pixPerCat*currentCat;
                    pressed = x;
                    return true;
                }
            }
        }
        else {
            for (int i = 0; i < stories; i++) {
                p += pixPerStory;
                if (x < p) {
                    isMoving = true;
                    currentStory = i;
                    square = posX + 2.5*height + pixPerStory*currentStory;
                    pressed = x;
                    return true;
                }
            }
            if (x < posX + width - 2.5*height) {
                flipp = !flipp;
                return true;
            }
        }
        
    }

    return false;
}

bool Menu::moveEvent(int x, int y, bool story) {
    if (isMoving) {
        square += (x - pressed);
        pressed = x;
        
        if (!story) {
            if (square < posX + 2.5*height ) {
                square = posX + 2.5*height;
                currentCat = 0;
                
            }
            else if (square > posX + 2.5*height + pixPerCat*(categories - 1)) {
                square = posX + 2.5*height + pixPerCat*(categories - 1);
                currentCat = categories - 1;
            }
            
            if (currentCat < (categories - 1) && square > posX + 2.5*height + pixPerCat*(currentCat+1) - pixPerCat/4) {
                currentCat++;
            }
            else if (currentCat > 0 && square < posX + 2.5*height + pixPerCat*(currentCat-1) + pixPerCat/4) {
                currentCat--;
            }
        }
        else {
            if (square < posX + 2.5*height ) {
                square = posX + 2.5*height;
                currentStory = 0;
                
            }
            else if (square > posX + 2.5*height + pixPerStory*(stories-1)) {
                square = posX + 2.5*height + pixPerStory*(stories-1);
                currentStory = (stories-1);
            }
            
            if (currentStory < (stories-1) && square > posX + 2.5*height + pixPerStory*(currentStory+1) - pixPerStory/4) {
                currentStory++;
            }
            else if (currentStory > 0 && square < posX + 2.5*height + pixPerStory*(currentStory-1) + pixPerStory/4) {
                currentStory--;
            }
        }
        return true;
    }
    return false;
}

bool Menu::upEvent(bool story) {
    isMoving = false;
    if (!story) {
        square = posX + 2.5*height + pixPerCat*currentCat;
    }
    else {
        square = posX + 2.5*height + pixPerStory*currentStory;
    }
    return true;
}


int Menu::getCat() {
    return currentCat;
}
int Menu::getStory() {
    return currentStory;
}

void Menu::changeCategory() {
    currentCat = (currentCat + 1)%categories;
    square = posX + 2.5*height + pixPerCat*currentCat;
}



