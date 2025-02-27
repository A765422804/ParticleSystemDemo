//
//  TrailRenderer.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/17.
//

#include "TrailRenderer.hpp"

TrailRenderer::TrailRenderer()
: VAO(0)
, VBO(0)
, EBO(0)
, _shader(nullptr)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // 初始化shader
    _shader = std::make_shared<Shader>("./shader_file/trail_shader_vertex.vs", "./shader_file/trail_shader_fragment.fs");
    
    SetupVertexDesc();
}

void TrailRenderer::SetupVertexDesc()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // Position属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TrailData), (void*)offsetof(TrailData, Position));

    // IndexAndSize属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TrailData), (void*)offsetof(TrailData, IndexAndSize));

    // Texcoord属性
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TrailData), (void*)offsetof(TrailData, Texcoord));

    // Velocity属性
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(TrailData), (void*)offsetof(TrailData, Velocity));

    // Color属性
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(TrailData), (void*)offsetof(TrailData, Color));
    
    glBindVertexArray(0);
}

void TrailRenderer::SetVertexData(std::vector<float> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
}

void TrailRenderer::SetIndexData(std::vector<unsigned int> indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    _indexCount = (unsigned int)indices.size();
}

void TrailRenderer::SetCamera(CameraPtr camera)
{
    _camera = camera;
}

void TrailRenderer::Render()
{
    _shader->use();
    
    // 计算VP矩阵
    auto view = _camera->GetViewTransform();
    auto projection = _camera->GetProjTransform();
    auto VPTransform = projection * view;
    auto cameraPos = _camera->GetPosition();

    _shader->setMat4("VPTransform", VPTransform);
    _shader->setVec3("CameraPos", cameraPos);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void TrailRenderer::SetWorldTransform(mat4 worldTransform)
{
    _shader->use();
    _shader->setMat4("WorldTransform", worldTransform);
}

