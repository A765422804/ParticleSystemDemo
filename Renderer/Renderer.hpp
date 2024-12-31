//
//  Renderer.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#pragma once

#include "../Common/Head.h"
#include "../Common/VertexType.hpp"
#include "../Function/Shader.hpp"
#include "../Common/Camera.hpp"

class Renderer
{
public:
    using RendererPtr = std::shared_ptr<Renderer>;
    
public:
    Renderer();
    ~Renderer() = default;
    
public:
    static RendererPtr CreateRenderer();
    
public:
    void SetVertexData(std::vector<V3_C4_T2> vertices);
    void SetIndexData(std::vector<unsigned int> indices);
    void Render();
    
public:
    void SetupVertexDesc();
    void SetupShaderProgram(const char* vertexPath, const char* fragmentPath);
    
private:
    void InitOpenGLResources();
    
private:
    unsigned int VAO, VBO, EBO;
    unsigned int _indexCount;
    ShaderPtr _shader;
    CameraPtr _camera;
};

using RendererPtr = Renderer::RendererPtr;

// CreateRenderer->SetupVertexDesc->SetupShaderProgram->SetVertexData->SetIndexData->Render
