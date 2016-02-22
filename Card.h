#include <glm/glm.hpp>
#include <vector>
#include <string>

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

public:
    //constructors
    Card(); //default construtor
    Card(std::vector<std::string> inCat, std::string inSvH, std::string inSvT, std::string inEnH, std::string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel);

    //Transformation functions
    void move(glm::vec2 velocity);

    //getters
    int getHeight();
    int getWidth();
    double getLifeTime();
    std::vector<std::string> getCategories();
    std::string getSvHeader();
    std::string getSvText();
    std::string getEnHeader();
    std::string getEnText();
    bool getIsSwede();
    glm::vec3 getPos();
    glm::vec2 getVelocity();

    //setters
    void setHeight();
    void setWidth();
    void setLifeTime();
    void addCategory();
    void setPos();
    void setVelocity();

    void changeLang();
};

    //if eng ->set lang to swe
