//
//  PS3ParticleSystemCPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#pragma once
#include "PS3Common.h"
#include "PS3ParticleSystem.hpp"

class PS3ParticleSystemCPU : public PS3ParticleSystem
{
    using SuperType = PS3ParticleSystem;
public:
    PS3ParticleSystemCPU(int maxParticleCount);
    ~PS3ParticleSystemCPU() = default;
    
public:
    void InitializeParticles(std::vector<PS3ParticlePtr> &particles) override; // 初始化粒子池
    
    void Render() override; // 在原版里是beforeRender
    
    void UpdateParticles(float dt) override;
    
    bool UpdateParticle(PS3ParticlePtr p, float dt);
    
    void Clear() override; // 将所有粒子清除
    
    void SetTrailModule(PS3TrailPtr trail);
    
    int GetParticleCount() override;
    
public:
    void NotifySubEmitters(PS3ParticlePtr p, EventType event);
    void EmitSubParticles(PS3ParticlePtr p, std::shared_ptr<PS3ParticleSystem> ps);
    
public:
    // 粒子池
    std::vector<PS3ParticlePtr> _particles; // CPU 特有
    
    PS3TrailPtr _trailModule; // CPU 特有
    
    PS3RendererCPUPtr _renderer;
};

using PS3ParticleSystemCPUPtr = std::shared_ptr<PS3ParticleSystemCPU>;
