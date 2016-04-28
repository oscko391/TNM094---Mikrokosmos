#ifndef SDL_tutorial_Card_h
#define SDL_tutorial_Card_h
#include "glm.hpp"
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <vector>
#include <string>
#include <stdio.h>
#include <cmath>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 1200; // Flyttas till main sen? 
const int SCREEN_HEIGHT = 700;


class Card
{
private:
    double height = 120;
    double width= 200;
    clock_t lifeTime;
    std::vector<std::string> categories;
    std::string svHeader;
    std::string svText;
    std::string enHeader;
    std::string enText;
    //bool isSwede = true; // limiting us to 2 languages
    glm::vec3 pos;
    glm::vec2 velocity;
    std::string path;
    
    bool isTrans;
    double angle = 0.0;
    glm::vec2 touchPos = glm::vec2(-1.0f,-1.0f);
    
    bool isReading;
    
    static std::vector<SDL_Texture*> headers;
    static std::vector<SDL_Texture*> infoText;
    int infoIndex;
    static int infoIndexGenerator;
    bool loadingText(SDL_Renderer* r);

    //SDL_Texture* cardTexture = NULL;
    

public:
    //int texIndex = -1; // images
    //constructors
    Card(); //default construtor
    Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath, SDL_Renderer* r);
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
    virtual void render( SDL_Renderer* r);
    //void render( SDL_Renderer* gRenderer, SDL_Texture* texture); // for images
    
    
    //touch funcs
    void scale(SDL_Event* e);
    //checks if finger coordinated are within the borders of the card
    bool isInside(int x, int y);
};

    //if eng ->set lang to swe

#endif
