//
//  PS3RendererCPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "PS3RendererCPU.hpp"
#include "../PS3ParticleSystem.hpp"

PS3RendererCPU::PS3RendererCPU(int maxParticleCount)
{
    _model = std::make_shared<PS3ParticleBatchModel>(maxParticleCount);
}

void PS3RendererCPU::SetNewParticle(PS3ParticlePtr particle)
{
    _particles.push_back(particle);
}

PS3ParticlePtr PS3RendererCPU::GetFreeParticle()
{
    if (_particles.size() >= _particleSystem->_capacity)
        return nullptr;
    
    return std::make_shared<PS3Particle>();
}

int PS3RendererCPU::GetParticleCount()
{
    return int(_particles.size());
}

int PS3RendererCPU::UpdateParticles(float dt)
{
    // 更新scale和rotation
    // 注意，scale和rotation是要传给shader的uniform变量
    
    // TODO: 更新拖尾模块
    
    // 处理重力
    
    // 更新每个粒子
    for (int i = 0 ; i < _particles.size(); ++i)
    {
        // 生命周期衰减
        auto &p = _particles[i];
        p->_remainingLifeTime -= dt;
        
        if (p->_remainingLifeTime < 0.0)
        {
            // Trigger了死亡事件
            _particleSystem->NotifySubEmitters(p, EventType::DEATH);
            
            // 移除死亡粒子
            _particles.erase(_particles.begin() + i);
            -- i;
            continue;
        }
        
        // 重力使用
        if (_particleSystem->_gravity != nullptr)
        {
            float normalizedTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
            float gravityEffect = -_particleSystem->_gravity->Evaluate(normalizedTime, Random01()) * 9.8 * dt;
            if (_particleSystem->_spaceMode == SpaceMode::LOCAL)
            {
                vec4 gravity = vec4(0, gravityEffect, 0, 1);
                gravity = transpose(toMat4(_particleSystem->GetRotation()))  * gravity;
                p->_velocity.x += gravity.x;
                p->_velocity.y += gravity.y;
                p->_velocity.z += gravity.z;
            }
            else
            {
                p->_velocity.y += gravityEffect;
            }
        }
        
        // 更新粒子最终速度
        p->_ultimateVelocity = p->_velocity;
        
        // 应用动画
        _particleSystem->_velocityOvertimeModule->Animate(p, dt);
        _particleSystem->_forceOvertimeModule->Animate(p, dt);
        //_particleSystem->_sizeOvertimeModule->Animate(p, dt);
        //_particleSystem->_colorOvertimeModule->Animate(p, dt);
        _particleSystem->_rotationOvertimeModule->Animate(p, dt);
        _particleSystem->_textureAnimationModule->Animate(p, dt);
        
        // 更新位置
        p->_position = p->_position + p->_ultimateVelocity * dt;
        
        // 更新粒子的拖尾
    }
    
    
    return int(_particles.size());
}

void PS3RendererCPU::UpdateRenderData()
{
    int idx = 0;
    for (int i = 0; i < _particles.size(); ++i)
    {
        auto p = _particles[i];
        float fi = 0;
        auto textureModule = _particleSystem->_textureAnimationModule;
        if (textureModule && textureModule->_enable)
            fi = p->_frameIndex;
        idx = i * 4;
        FillMeshData(p, idx, fi);
    }
}

void PS3RendererCPU::FillMeshData(PS3ParticlePtr p, int idx, float fi)
{
    for (int j = 0; j < 4; ++ j)
    {
        _particleVertexData.Position = p->_position;
        vec3 tempAttribUV = vec3(uvs[2 * j], uvs[2 * j + 1], fi);
        _particleVertexData.Texcoord = tempAttribUV;
        _particleVertexData.Size = p->_size;
        p->_ultimateQuat = normalize(p->_ultimateQuat);
        _particleVertexData.Rotation = vec4(p->_ultimateQuat.x, p->_ultimateQuat.y, p->_ultimateQuat.z, p->_ultimateQuat.w);
        _particleVertexData.Color = p->_color;
        _model->AddParticleVertexData(idx ++, _particleVertexData);
    }
}

void PS3RendererCPU::Render()
{
    auto shader = _model->_renderer->_shader;
    shader->use();
    shader->setBool("IsLocalSpace", _particleSystem->_spaceMode == SpaceMode::LOCAL);
    shader->setVec2("FrameTile", vec2(_particleSystem->_textureAnimationModule->_numTilesX, _particleSystem->_textureAnimationModule->_numTilesY));
    
    _model->RenderModel(int(_particles.size()));
}
