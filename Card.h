#include "glm.hpp"
#include <vector>
#include <string>
#include <stdio.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>

#include <cmath>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 1200; // Flyttas till main sen? 
const int SCREEN_HEIGHT = 800;


class Card
{
private:
    int height = 200;
    int width= 120;
    double lifeTime = 30.0;
    std::vector<std::string> categories;
    std::string svHeader;
    std::string svText;
    std::string enHeader;
    std::string enText;
    bool isSwede = true; // limiting us to 2 languages
    glm::vec3 pos;
    glm::vec2 velocity;
    std::string imgPath;

    SDL_Texture* cardTexture = NULL;


public:
    //constructors
    Card(); //default construtor
    Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel, std::string textPath);
    ~Card();
    //Transformation functions
//    void move(glm::vec2 velocity);
    void move( std::time_t now );
    
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
};

    //if eng ->set lang to swe
