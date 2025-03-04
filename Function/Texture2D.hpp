//
//  Texture2D.hpp
//  openGL
//
//  Created by flowerdance on 2024/7/9.
//

#pragma once

#include "../Head.h"
#include "Shader.hpp"

class Texture2D
{
public:
    Texture2D(std::string path);
    
    Texture2D(const std::vector<float>& data, int width, int height,GLenum internalFormat = GL_RGBA32F, GLenum format = GL_RGBA);
    
    ~Texture2D()
    {
        glDeleteTextures(1, &textureID);
    }
    
public:
    int GetHeight();
    
public:
    void BindToUniform(const std::string& uniformName, ShaderPtr shader, GLuint textureUnit) const;
    
public:
    unsigned int textureID;
    std::string path;
    unsigned char* data;
    std::vector<float> dataFloat;
    int width, height, nrComponents;
};

using Texture2DPtr = std::shared_ptr<Texture2D>;

inline int Texture2D::GetHeight()
{
    return height;
}
