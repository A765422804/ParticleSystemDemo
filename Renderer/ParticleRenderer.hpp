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

enum ParticleType
{
    CPU,
    GPU,
};

class ParticleRenderer
{
public:
    ParticleRenderer(bool useGPU);
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
    GLuint GetTextureUnit(const std::string& uniformName);
    
public:
    unsigned int VAO, VBO, EBO;
    unsigned int _indexCount;
    
    ShaderPtr _shader;
    CameraPtr _camera;
    ParticleType _type;
    
    std::unordered_map<std::string, GLuint> _uniforms;
    GLuint _nextTextureUnit = 0;
};

using ParticleRendererPtr = std::shared_ptr<ParticleRenderer>;

