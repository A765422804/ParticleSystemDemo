//
//  Texture2D.hpp
//  openGL
//
//  Created by flowerdance on 2024/7/9.
//

#pragma once

#include "../Head.h"

class Texture2D
{
public:
    Texture2D(std::string path);
    ~Texture2D() = default;
    
public:
    void Bind(unsigned int slot = 0) const;
    
private:
    unsigned int textureID;
    std::string path;
    unsigned char* data;
    int width, height, nrComponents;
};

using Texture2DPtr = std::shared_ptr<Texture2D>;
