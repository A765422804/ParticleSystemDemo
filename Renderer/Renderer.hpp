//
//  Renderer.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#pragma once

#include "../Head.h"
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
    void SetVertexData(std::vector<V3_C4_T2> vertices);
    void SetVertexData(std::vector<V3> vertices);
    void SetVertexData(std::vector<vec3> vertices);
    void SetIndexData(std::vector<unsigned int> indices);
    
public:
    void Render();
    void RenderPoint(float size, vec4 color);
    void RenderPoints(float size, size_t vertexCount);
    void RenderCircle(unsigned int segmentCount);
    void RenderLines();
    
public: // shader
    void SetWorldTransform(mat4 worldTransform);
    void SetUseTexture(bool useTexture);
    void SetWireFrameColor(vec4 wireFrameColor);
    
public: // camera
    void SetCamera(CameraPtr camera);
    
    
public: // gl
    void SetIsWireframeEnable(bool isWireFrameEnable);
    
public:
    void SetupVertexDescV3_C4_T2();
    void SetupVertexDescV3();
    void SetupVertexDescVec3();
    
    void SetupShaderProgram(const char *vertexPath, const char *fragmentPath);
    
private:
    void InitOpenGLResources();
    
private:
    unsigned int VAO, VBO, EBO;
    unsigned int _indexCount;
    ShaderPtr _shader;
    CameraPtr _camera;
    bool _isWireFrameEnable;
};

using RendererPtr = Renderer::RendererPtr;

// CreateRenderer->SetupVertexDesc->SetupShaderProgram->SetVertexData->SetIndexData->Render

