//
//  InputAssembler.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "ParticleRenderer.hpp"

const GLuint MAX_TEXTURE_UNITS = 80;

ParticleRenderer::ParticleRenderer(bool useGPU)
: VAO(0)
, VBO(0)
, EBO(0)
, _shader(nullptr)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    _type = useGPU ? ParticleType::GPU : ParticleType::CPU;
    
    // 初始化shader
    if (_type == ParticleType::CPU)
    {
        _shader = std::make_shared<Shader>("./shader_file/ps3_shader_vertex.vs", "./shader_file/ps3_shader_fragment.fs");
    }
    else
    {
        _shader = std::make_shared<Shader>("./shader_file/ps3_shader_gpu_vertex.vs", "./shader_file/ps3_shader_gpu_fragment.fs");
    }
}

void ParticleRenderer::SetupVertexDesc()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    if (_type == ParticleType::CPU)
    {
        // 位置属性 (vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PVData), (void*)offsetof(PVData, Position));

        // 纹理坐标属性 (vec3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PVData), (void*)offsetof(PVData, Texcoord));

        // 大小属性 (vec3)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PVData), (void*)offsetof(PVData, Size));

        // 旋转属性 (vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PVData), (void*)offsetof(PVData, Rotation));

        // 颜色属性 (vec4)
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(PVData), (void*)offsetof(PVData, Color));
    }
    else
    {
        // 位置和起始时间属性 (vec4)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(PVDataGPU), (void*)offsetof(PVDataGPU, Position_StartTime));

        // 大小属性 (vec3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PVDataGPU), (void*)offsetof(PVDataGPU, Size));

        // 旋转属性 (vec4)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PVDataGPU), (void*)offsetof(PVDataGPU, Rotation));

        // 颜色属性 (vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PVDataGPU), (void*)offsetof(PVDataGPU, Color));

        // 方向和生命属性 (vec4)
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(PVDataGPU), (void*)offsetof(PVDataGPU, Dir_Life));

        // 纹理坐标属性 (vec2)
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(PVDataGPU), (void*)offsetof(PVDataGPU, Texcoord));
    }
    
    glBindVertexArray(0);
}

void ParticleRenderer::SetVertexData(std::vector<float> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
}

void ParticleRenderer::SetIndexData(std::vector<unsigned int> indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    _indexCount = (unsigned int)indices.size();
}

void ParticleRenderer::SetCamera(CameraPtr camera)
{
    _camera = camera;
}

void ParticleRenderer::Render()
{
    _shader->use();
    
    // 计算VP矩阵
    auto view = _camera->GetViewTransform();
    auto projection = _camera->GetProjTransform();
    auto VPTransform = projection * view;
    auto viewInverse = inverse(view);

    _shader->setMat4("VPTransform", VPTransform);
    _shader->setMat4("ViewInverse", viewInverse);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void ParticleRenderer::SetWorldTransform(mat4 worldTransform)
{
    _shader->use();
    _shader->setMat4("WorldTransform", worldTransform);
}

GLuint ParticleRenderer::GetTextureUnit(const std::string& uniformName)
{
    auto it = _uniforms.find(uniformName);
        if (it != _uniforms.end())
        {
            return it->second;
        }
        else
        {
            GLuint unit = _nextTextureUnit++;
            if (_nextTextureUnit >= MAX_TEXTURE_UNITS)
            {
                throw std::runtime_error("Exceeded maximum number of texture units");
            }
            _uniforms[uniformName] = unit;
            return unit;
        }
}
