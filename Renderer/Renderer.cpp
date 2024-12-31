//
//  Renderer.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#include "Renderer.hpp"

Renderer::Renderer()
{
    // TODO: implement
}

RendererPtr Renderer::CreateRenderer()
{
    RendererPtr ret = std::make_shared<Renderer>();
    
    ret->InitOpenGLResources();
    ret->_camera = std::make_shared<Camera>();
    
    return ret;
}

void Renderer::InitOpenGLResources()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Renderer::SetupVertexDesc()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (void*)(offsetof(V3_C4_T2, Colors)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (void*)(offsetof(V3_C4_T2, TexCoords)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void Renderer::SetupShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    _shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    
    // 计算VP矩阵
    auto view = _camera->GetViewTransform();
    auto projection = _camera->GetProjTransform();
    
    _shader->use();
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
}

void Renderer::SetVertexData(std::vector<V3_C4_T2> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(V3_C4_T2), vertices.data(), GL_STATIC_DRAW);
}

void Renderer::SetIndexData(std::vector<unsigned int> indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    _indexCount = (unsigned int)indices.size();
}

void Renderer::Render()
{
    _shader->use();
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}
