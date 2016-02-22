    #include "Card.h"

  /*  int main()
    {
        Card test = Card();
        return 0;
    }*/

    Card::Card(){

    }

    Card::Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel)
        : svHeader(inSvH)
        , svText(inSvT)
        , enHeader(inEnH)
        , enText(inEnT)
        , isSwede(lang)
        , pos(inPos)
        , velocity(inVel)
    { }



    /*-----------------Transformation functions-----------------*/
    void Card::move(glm::vec2 velocity)
    {
       ;
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


