#ifndef CARD_H
#define CARD_H

//#include </Users/my/Documents/LiU/Kandidat/SDL_tutorial/glm/glm/glm.hpp>
#if defined (__APPLE_CC__) //if apple dator
    #include <SDL2/SDL.h>
    #include </Users/my/Documents/LiU/Kandidat/SDL_tutorial/glm/glm/glm.hpp>
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
#else //annars windows version
    #include <SDL.h>
    #include <SDL_image.h>
    #include <glm/glm.hpp>
    #include <SDL_ttf.h>
#endif

#include <vector>
#include <string>
#include <stdio.h>
#include <string>
#include <cmath>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <iostream>

//Screen dimension constants
/*const int SCREEN_WIDTH = 1000; // Flyttas till main sen?
const int SCREEN_HEIGHT = 500;
const double EPSILON = 10e-04;*/
#include "settings.h"


class Card
{
private:
    double height = 120;
    double width= 200;
    SDL_TouchFingerEvent fingerEvents[2];
    int numberFingers = 0;
    double centroid = 0;
    clock_t lifeTime;
    /*std::vector<std::string> categories;
    std::string svHeader;
    std::string svText;
    std::string enHeader;
    std::string enText; */
    //bool isSwede = true; // limiting us to 2 languages
    glm::vec3 pos;
    glm::vec2 velocity;
    std::string path;

    bool isTrans;
    double angle = 0.0;
    glm::vec2 touchPos = glm::vec2(-1.0f,-1.0f);
    bool isReading;
    bool loadingText(SDL_Renderer* r);
    bool loadingText(SDL_Renderer* r, std::string svHeader, std::string svText, std::string enHeader, std::string enText, std::vector<std::string> svCat, std::vector<std::string> enCat);
    
protected:


    int infoIndex;
    static int infoIndexGenerator;
    static std::vector<SDL_Texture*> headersSv;
    static std::vector<SDL_Texture*> headersEn;
    
    static std::vector<SDL_Texture*> infoTextSv;
    static std::vector<SDL_Texture*> infoTextEn;
    
    static std::vector<SDL_Texture*> catTextSv;
    static std::vector<SDL_Texture*> catTextEn;


    static SDL_Texture* shadow;
    static SDL_Texture* loadShadow(SDL_Renderer* r);

    //SDL_Texture* cardTexture = NULL;


public:
    //static SDL_Texture* loadShadow(SDL_Renderer* r);
    //static SDL_Texture* shadow;
    static SDL_Texture* loadArrow(SDL_Renderer* r);
    static SDL_Texture* arrow;
    //int texIndex = -1; // images
    //constructors
    Card(); //default construtor
    Card(std::vector<std::string> inSvCat, std::vector<std::string> inEnCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r);
    ~Card();
    //Transformation functions
    void move(float timeStep);
    bool handleEvent( SDL_Event* e );

    //getters
    int getHeight();
    int getWidth();
    clock_t getLifeTime() const;
    std::vector<std::string> getCategories();
    std::string getSvHeader();
    std::string getSvText();
    std::string getEnHeader();
    std::string getEnText();
    std::string getPath() const;
    bool getReading();
    SDL_Texture* getHeader();
    static SDL_Texture* getShadow();
    double getAngle();

    //bool getIsSwede();
    glm::vec3 getPos();
    glm::vec2 getVelocity();


    //setters
    void setHeight(int h);
    void setWidth(int w);
    void setLifeTime(clock_t inLife);
    void addCategory( std::string inCat); // behövs?!? alla kategorier läses in direkt
    void setPos(glm::vec3 inPos);
    void setVelocity(glm::vec2 inVel);
    //void setImgPath(std::string s); // for images

    //void changeLang();
    //bool loadTexture(SDL_Renderer* gRenderer);
    virtual void render( SDL_Renderer* r, bool swede);
    //void render( SDL_Renderer* gRenderer, SDL_Texture* texture); // for images


    //touch funcs
    void scale(SDL_Event* e);
    //checks if finger coordinated are within the borders of the card
    bool isInside(int x, int y);
};

    //if eng ->set lang to swe

#endif
