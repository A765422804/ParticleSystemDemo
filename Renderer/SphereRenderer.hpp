//
//  SphereRenderer.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/17.
//

#pragma once
#include "../Head.h"
#include "../Function/Shader.hpp"
#include "../Common/Camera.hpp"
#include "../Common/VertexType.hpp"

class SphereRenderer
{
public:
    SphereRenderer(float radius, int sectors, int stacks);
    ~SphereRenderer() = default;
    
public:
    void SetWorldTransform(const mat4& worldTransform);
    void SetCamera(CameraPtr camera);
    void Render();
    
private:
    void GenerateSphereData(float radius, int sectors, int stacks);
    void SetupOpenGLResources();
    
private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

    ShaderPtr _shader;
    CameraPtr _camera;
    mat4 _worldTransform;
};

using SphereRendererPtr = std::shared_ptr<SphereRenderer>;
