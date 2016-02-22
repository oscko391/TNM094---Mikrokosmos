# include <glm/glm.hpp>

class Card
{
private:
    int height = 200;
    int width= 120;
    double lifeTime = 30;
    std::vector<string> *categories;
    string svHeader;
    string svText;
    string enHeader;
    string enText;
    bool isSwede = true; // limiting us to 2 languages
    glm::vec3 pos;
    glm::vec2 velocity;

public:
    //constructors
    Card(); //default construtor
    Card(std::vector<string> inCat, string inSvH, string inSvT, string inEnH, string inEnT, bool lang, glm::vec3 inPos, glm::vec2 inVel);

    //Transformation functions
    void move(glm::vec2 velocity);

    //getters
    int getHeight();
    int getWidth();
    double getLifeTime();
    std::vector<string> getCategories();
    string getSvHeader();
    string getSvText();
    string getEnHeader();
    string getEnText();
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

    //if eng ->set lang to swe
    void changeLang();
};
