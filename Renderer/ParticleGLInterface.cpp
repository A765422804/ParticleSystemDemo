//
//  InputAssembler.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "ParticleGLInterface.hpp"
#include "../ParticleSystem3D/PS3ParticleSystemGPU.hpp"

const GLuint MAX_TEXTURE_UNITS = 80;
const GLuint MAX_PARTICLES = 100000;

ParticleGLInterface::ParticleGLInterface(bool useGPU, PS3ParticleSystem* ps)
: _ps(ps)
, VAO(0)
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
        
        // 创建 Transform Feedback 缓冲区
            glGenBuffers(1, &_deadParticleBuffer);
            glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _deadParticleBuffer);
            glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(vec3) * MAX_PARTICLES, nullptr, GL_STATIC_READ);
            
            // 创建 Transform Feedback 对象
            glGenTransformFeedbacks(1, &_TFBO);
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _TFBO);
        
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _deadParticleBuffer);
        
            // 设置 Transform Feedback 输出变量
            const char* varyings[] = { "DeadParticlePosition" };
            glTransformFeedbackVaryings(_shader->ID, 1, varyings, GL_SEPARATE_ATTRIBS);
            
            // 重新链接着色器程序
            glLinkProgram(_shader->ID);
    }
}

void ParticleGLInterface::SetupVertexDesc()
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

void ParticleGLInterface::SetVertexData(std::vector<float> vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
}

void ParticleGLInterface::SetIndexData(std::vector<unsigned int> indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()* sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    _indexCount = (unsigned int)indices.size();
}

void ParticleGLInterface::SetCamera(CameraPtr camera)
{
    _camera = camera;
}

void ParticleGLInterface::Render()
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
    
    if (_type == ParticleType::GPU)
    {
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _deadParticleBuffer);
        
        // 开始 Transform Feedback
        glBeginTransformFeedback(GL_TRIANGLES);
         
         // 绘制粒子
         glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
        
         // 检查错误
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }
         
         // 结束 Transform Feedback
         glEndTransformFeedback();
        
         // 读取捕获的数据
         glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, _deadParticleBuffer);
         vec3* deadParticlePositions = (vec3*)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY);
        
         // 处理死亡粒子的位置
         for (int i = 0; i < _indexCount; i++)
         {
             if (!std::isinf(deadParticlePositions[i].x))
             {
                 // std::cout<<deadParticlePositions[i]<<std::endl;
                 // 触发子发射器发射新粒子
                 dynamic_cast<PS3ParticleSystemGPU*>(_ps)->NotifySubEmitters(deadParticlePositions[i], EventType::DEATH);
             }
         }
         
         // 解除映射
         glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void ParticleGLInterface::SetWorldTransform(mat4 worldTransform)
{
    _shader->use();
    _shader->setMat4("WorldTransform", worldTransform);
}

GLuint ParticleGLInterface::GetTextureUnit(const std::string& uniformName)
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
