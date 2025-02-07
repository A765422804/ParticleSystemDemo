//
//  Renderer.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#include "Renderer.hpp"

Renderer::Renderer()
: VAO(0)
, VBO(0)
, EBO(0)
, _indexCount(0)
, _shader(nullptr)
, _camera(nullptr)
, _isWireFrameEnable(false)
{
    InitOpenGLResources();
}

void Renderer::InitOpenGLResources()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Renderer::SetupVertexDescV3_C4_T2()
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

void Renderer::SetupVertexDescV3()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(V3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Renderer::SetupVertexDescVec3()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Renderer::SetupShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    _shader = std::make_shared<Shader>(vertexPath, fragmentPath);
}

void Renderer::SetVertexData(std::vector<V3_C4_T2> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(V3_C4_T2), vertices.data(), GL_STATIC_DRAW);
}

void Renderer::SetVertexData(std::vector<V3> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(V3), vertices.data(), GL_STATIC_DRAW);
}

void Renderer::SetVertexData(std::vector<vec3> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
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
    
    if (_isWireFrameEnable)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void Renderer::RenderPoint(float size, vec4 color)
{
    _shader->use();
    _shader->setVec4("pointColor", color);
    
    glBindVertexArray(VAO);
    glPointSize(size);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

void Renderer::RenderCircle(unsigned int segmentCount)
{
    _shader->use();
    
    if (_isWireFrameEnable)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, segmentCount + 1);
    glBindVertexArray(0);
}

void Renderer::RenderLines()
{
    _shader->use();
    
    // 计算VP矩阵
    auto view = _camera->GetViewTransform();
    auto projection = _camera->GetProjTransform();

    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, _indexCount, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void Renderer::SetUseTexture(bool useTexture)
{
    _shader->use();
    _shader->setBool("useTexture", useTexture);
}

void Renderer::SetWireFrameColor(vec4 wireFrameColor)
{
    _shader->use();
    _shader->setVec4("wireFrameColor", wireFrameColor);
}

void Renderer::SetIsWireframeEnable(bool isWireFrameEnable)
{
    _isWireFrameEnable = isWireFrameEnable;
}

void Renderer::SetCamera(CameraPtr camera)
{
    _camera = camera;
}
