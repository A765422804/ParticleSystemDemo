//
//  Texture2D.cpp
//  openGL
//
//  Created by flowerdance on 2024/7/9.
//

#include "Texture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(std::string path)
: textureID(0), path(path), data(nullptr), width(0), height(0), nrComponents(0)
{
    // 加载图像
        stbi_set_flip_vertically_on_load(true); // 翻转Y轴
        data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            // 生成纹理
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            
            if (textureID == 0) {
                std::cerr << "Error: Failed to load texture!" << std::endl;
            }

            // 设置纹理参数
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 加载纹理数据
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            //glBindTexture(GL_TEXTURE_2D, 0);

            // 释放图像数据
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture at path: " << path << std::endl;
            stbi_image_free(data);
        }
}

Texture2D::Texture2D(const std::vector<float>& data, int width, int height,GLenum internalFormat, GLenum format)
: textureID(0), width(width), height(height), dataFloat(data)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    if (textureID == 0) {
        std::cerr << "Error: Failed to load texture!" << std::endl;
    }
    
    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 加载纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, data.data());
    
    // 生成mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::BindToUniform(const std::string& uniformName, ShaderPtr shader, GLuint textureUnit) const
{
    // 检查 OpenGL 上下文
    if (!glIsProgram(shader->ID)) {
        std::cerr << "Invalid OpenGL context or shader program." << std::endl;
        return;
    }

    // 绑定着色器程序
    shader->use();

    // 获取 Uniform 变量位置
    GLint loc = glGetUniformLocation(shader->ID, uniformName.c_str());
    if (loc == -1) {
        std::cerr << "Uniform variable '" << uniformName << "' not found or not used in shader." << std::endl;
        return;
    }

    // 检查纹理单元范围
    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    if (textureUnit >= maxTextureUnits) {
        std::cerr << "Texture unit " << textureUnit << " is out of range. Max supported: " << maxTextureUnits << std::endl;
        return;
    }

    // 检查纹理 ID 有效性
    if (textureID == 0 || !glIsTexture(textureID)) {
        std::cerr << "Texture ID is invalid. Did you forget to generate the texture?" << std::endl;
        return;
    }

    // 激活纹理单元并绑定纹理
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 设置 Uniform 变量
    glUniform1i(loc, textureUnit);

    // 检查 OpenGL 错误
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}
