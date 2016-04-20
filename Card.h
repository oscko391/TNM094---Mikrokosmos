#ifndef CARD_H
#define CARD_H

//#include </Users/my/Documents/LiU/Kandidat/SDL_tutorial/glm/glm/glm.hpp>
#if defined (__APPLE_CC__) //if apple dator
   #include <SDL2/SDL.h>
   #include "glm.hpp"
   #include <SDL2/SDL_image.h>
#else //annars windows version
    #include <SDL.h>
    #include <SDL_image.h>
    #include <glm/glm.hpp>
#endif

#include <vector>
#include <string>
#include <stdio.h>
#include <string>
#include <cmath>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 700; // 1200 //Flyttas till main sen?
const int SCREEN_HEIGHT = 300; // 700


class Card
{
private:
    int height = 100;
    int width= 150;
    double lifeTime = -1;
    std::vector<std::string> categories;
    std::string svHeader;
    std::string svText;
    std::string enHeader;
    std::string enText;
    bool isSwede = true; // limiting us to 2 languages
    bool isTrans = false;
    glm::vec3 pos;
    double angle = 0.0;
    glm::vec2 velocity;
    glm::vec2 touchPos = glm::vec2(-1.0f,-1.0f);
    std::string imgPath;

    SDL_Texture* cardTexture = NULL;



public:
    //constructors
    Card(); //default construtor
    Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel, std::string textPath);
    ~Card();
    //Transformation functions
    void move( time_t now );
    bool handleEvent( SDL_Event* e );


    //getters
    int getHeight();
    int getWidth();
    double getLifeTime();
    std::vector<std::string> getCategories();
    std::string getSvHeader();
    std::string getSvText();
    std::string getEnHeader();
    std::string getEnText();
    std::string getImgPath();//png
    SDL_Texture* getCardTexture();

    bool getIsSwede();
    glm::vec3 getPos();
    glm::vec2 getVelocity();


    //setters
    void setHeight(int h);
    void setWidth(int w);
    void setLifeTime(double inLife);
    void addCategory( std::string inCat);
    void setPos(glm::vec3 inPos);
    void setVelocity(glm::vec2 inVel);
    void setImgPath(std::string s);

    void changeLang();
    bool loadTexture(SDL_Renderer* gRenderer);
    void render( SDL_Renderer* gRenderer);
    void renderActive( SDL_Renderer* gRenderer);

     //touch funcs
    void scale(SDL_Event* e);
    //checks if finger coordinated are within the borders of the card
    bool isInside(int x, int y);

};
#endif // CARD_H
