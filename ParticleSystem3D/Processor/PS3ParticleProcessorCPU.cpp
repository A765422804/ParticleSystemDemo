//
//  PS3ParticleProcessorCPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3ParticleProcessorCPU.hpp"
#include "../PS3ParticleSystem.hpp"

PS3ParticleProcessorCPU::PS3ParticleProcessorCPU(PS3ParticleSystem* ps)
: SuperType(ps)
{
    
}

void PS3ParticleProcessorCPU::UpdateParticles(std::vector<PS3ParticlePtr>& particles, float dt)
{
    for (int i = 0 ; i < particles.size() ; ++i)
    {
        auto p = particles[i];
        if (!UpdateParticle(p, dt))
        {
            // 说明第i个粒子消亡
            particles.erase(particles.begin() + i);
            -- i;
        }
    }
}

bool PS3ParticleProcessorCPU::UpdateParticle(PS3ParticlePtr p, float dt)
{
    // 生命周期衰减
    p->_remainingLifeTime -= dt;
    
    if (p->_remainingLifeTime < 0.0)
    {
        // Trigger了死亡事件
        _ps->NotifySubEmitters(p, EventType::DEATH);
        
        if (_ps->_trailModule)
            _ps->_trailModule->RemoveParticle(p);
        
        // 移除死亡粒子
        return false; // 表示粒子死亡
    }
    
    // 重力使用
    if (_ps->_gravity != nullptr)
    {
        float normalizedTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
        float gravityEffect = -_ps->_gravity->Evaluate(normalizedTime, Random01()) * 9.8 * dt;
        if (_ps->_spaceMode == SpaceMode::LOCAL)
        {
            vec4 gravity = vec4(0, gravityEffect, 0, 1);
            gravity = transpose(toMat4(_ps->GetRotation()))  * gravity;
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
    for (auto [key, value] : _ps->_overtimeModules)
    {
        value->Animate(p, dt);
    }
    
    // 更新位置
    p->_position = p->_position + p->_ultimateVelocity * dt;
    
    // 更新粒子的拖尾
    if (_ps->_trailModule)
        _ps->_trailModule->Animate(p, dt);
    
    return true;
}
