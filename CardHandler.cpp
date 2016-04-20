#include "CardHandler.h"

CardHandler::CardHandler(std::string filePath)
{
    readXml(filePath);
}

std::vector<Card> CardHandler::getVecCard()
{
    return vecCard;
}

// loops over all the cards and then loops over all the events and handles them
void CardHandler::HandleEvents()
{
    for (int indo = 0; indo < vecCard.size(); indo++)
    {
        for (int indi = 0; indi < frameEvents.size(); indi++)
        {
            //removing an event if used so only one card can be affected by a specific event
            if(vecCard[indo].handleEvent(&frameEvents[indi]))
                frameEvents.erase(frameEvents.begin() + indi);
        }
    }
}

std::vector<SDL_Event> CardHandler::getFrameEvents()
{
    return frameEvents;
}

void CardHandler::addEvent(SDL_Event e)
{
    frameEvents.push_back(e);
}


bool CardHandler::readXml(std::string filePath)
{
    // getting the string through an ifstream
    std::ifstream ifs(filePath, std::ios::in);
    if(!ifs.is_open())
    {
        return false;
    }

    // making it the right format (char) for the parsing to xml doc
    std::string xml_str;
    xml_str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    // creating a xml_document and parsing the xml-file, making a DOM-tree
    rapidxml::xml_document<> doc;
    try
    {
        doc.parse<0>((char*)xml_str.c_str());
    }
    catch (...)
    {
        return false;
    }


    // accesing the first node in the file
    rapidxml::xml_node<>* content = doc.first_node("content");
    std::cout << "The '" << content->name() << "' is loaded. \n\n";

    // accesing all category nodes in content
    for (rapidxml::xml_node<>* second = content->first_node("category"); second <= content->last_node("category"); second = second->next_sibling())
    {
        // writes out information about the categpries to the terminal
        std::cout << "Node '" << second->name() <<"' has value: " << second->value() << " and is type: " << second->type() << "\n";
        for (rapidxml::xml_attribute<> * attr = second->first_attribute(); attr; attr = attr->next_attribute())
        {
            std::cout << "The node has attribute " << attr->name() << " ";
            std::cout << "with value " << attr->value() << "\n";
        }
        std::cout << "\n";
    }


    int i = 0;
    // accesing all media nodes
    for (rapidxml::xml_node<>* second = content->first_node("media"); second; second = second->next_sibling())
    {
        // writes out inforamtion about the media to the terminal and creates variables to use for creation of cards

        std::string mediaPath = second->first_attribute("path")->value();

        std::stringstream ss;
        int scaleExp;
        ss << second->first_attribute("scale_exp")->value();
        ss >> scaleExp;

        std::vector<std::string> cardCat;
        std::string seHeader;
        std::string seText;
        std::string enHeader;
        std::string enText;
        int j = 0;
        for (rapidxml::xml_node<>* inside = second->first_node(); inside ; inside = inside->next_sibling())
        {

            std::string b = inside->name();
            if (b == "category")
            {
                std::string cat = inside->first_attribute()->value();
                cardCat.push_back(cat);
                j++;
            }
            else if (b == "se")
            {
                seHeader = inside->first_node()->value() ;
                seText = inside->first_node()->next_sibling()->value();
            }
            else
            {
                enHeader = inside->first_node()->value() ;
                enText = inside->first_node()->next_sibling()->value();
            }

        }

        // create card with variables
        glm::vec3 position = glm::vec3(rand() % rand() % (SCREEN_WIDTH /2) + (SCREEN_WIDTH /4),rand() % (SCREEN_HEIGHT /2) + (SCREEN_HEIGHT /4),0);
        glm::vec2 velocity;
        int random = rand() % 4;
        if (random == 0)
        {
            velocity = glm::vec2((rand() % 4 + 1) , rand() % 4 + 1);
        }
        else if (random == 1)
        {
            velocity = glm::vec2(-(rand() % 4 + 1) , rand() % 4 + 1);
        }
        else if (random == 2)
        {
            velocity = glm::vec2((rand() % 4 + 1) , -(rand() % 4 + 1));
        }
        else
        {
            velocity = glm::vec2(-(rand() % 4 + 1) , -(rand() % 4 + 1));
        }


        vecCard.push_back(Card(cardCat, seHeader, seText, enHeader, enText, true, position, velocity, mediaPath));
        i++;
    }
    return true;
}
