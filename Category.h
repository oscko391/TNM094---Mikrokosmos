//
//  Category.h
//  Sprint1tagning2
//
//  Created by Madeleine Rapp on 2016-03-09.
//  Copyright (c) 2016 Madeleine Rapp. All rights reserved.
//
#ifndef SDL_tutorial_Category_h
#define SDL_tutorial_Category_h

#include <string>



class Category // Class med varje categori
{
private:
    std::string catName;
    std::string textSv;
    std::string textEn;
    std::string catPath; // path to the image who belongs to the category

    
public:
    //constructors
    Category(); //default construtor
    Category(std::string name, std::string texSv, std::string texEn, std::string path);
    ~Category();
    
    //getters
    std::string getCatName();
    std::string getTextSv();
    std::string getTextEn();
    std::string getCatPath();
};

#endif
