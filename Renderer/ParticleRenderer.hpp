//
//  InputAssembler.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#pragma once
#include "../Head.h"
#include "../Function/Shader.hpp"
#include "../ParticleSystem3D/PS3Common.h"
#include "../Common/Camera.hpp"

class ParticleRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer() = default;
    
public:
    void SetupVertexDesc(); // 初始化顶点属性
    
public:
    void SetVertexData(std::vector<float> vertices); // 设置顶点数据
    void SetIndexData(std::vector<unsigned int> indices); // 设置索引数据
    void SetWorldTransform(mat4 worldTransform);
    
public:
    void SetCamera(CameraPtr camera);
    
public:
    void Render();
    
public:
    unsigned int VAO, VBO, EBO;
    unsigned int _indexCount;
    
    ShaderPtr _shader;
    CameraPtr _camera;
};

using ParticleRendererPtr = std::shared_ptr<ParticleRenderer>;

