#ifndef SDL_tutorial_Card_h
#define SDL_tutorial_Card_h
#include </Users/my/Documents/LiU/Kandidat/SDL_tutorial/glm/glm/glm.hpp>
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
const int SCREEN_HEIGHT = 700;


class Card
{
private:
    int height = 100;
    int width= 200;
    clock_t lifeTime = -1;
    std::vector<std::string> categories;
    std::string svHeader;
    std::string svText;
    std::string enHeader;
    std::string enText;
    //bool isSwede = true; // limiting us to 2 languages
    glm::vec3 pos;
    glm::vec2 velocity;
    std::string path;

    //SDL_Texture* cardTexture = NULL;
    

public:
    //int texIndex = -1; // images
    //constructors
    Card(); //default construtor
    Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, glm::vec3 inPos, glm::vec2 inVel, std::string textPath);
    ~Card();
    //Transformation functions
    void move();
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
    virtual void render( SDL_Renderer* gRenderer);
    //void render( SDL_Renderer* gRenderer, SDL_Texture* texture); // for images
};

    //if eng ->set lang to swe

#endif
