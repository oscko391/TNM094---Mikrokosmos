//
//  Category.cpp
//  Sprint1tagning2
//
//  Created by Madeleine Rapp on 2016-03-09.
//  Copyright (c) 2016 Madeleine Rapp. All rights reserved.
//

#include "Category.h"

Category::Category()
{
}

Category::Category(std::string name, std::string texSv, std::string texEn, std::string path)
: catName(name)
, textSv(texSv)
, textEn(texEn)
, catPath(path)
{
}


// Destructor
Category::~Category()
{
    
}

// Getters
std::string Category::getCatName()
{
    return catName;
}

std::string Category::getTextSv()
{
    return textSv;
}

std::string Category::getTextEn()
{
    return textEn;
}

std::string Category::getCatPath()
{
    return catPath;
}