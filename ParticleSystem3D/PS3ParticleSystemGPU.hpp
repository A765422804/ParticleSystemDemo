//
//  PS3ParticleSystemGPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#pragma once
#include "PS3Common.h"
#include "PS3ParticleSystem.hpp"

class PS3ParticleSystemGPU : public PS3ParticleSystem
{
    using SuperType = PS3ParticleSystem;
public:
    PS3ParticleSystemGPU(int maxParticleCount);
    ~PS3ParticleSystemGPU() = default;
    
public:
    void InitializeParticles(std::vector<PS3ParticlePtr> &particles) override;
    
    void Render() override;
    
    void UpdateParticles(float dt) override;
    
    void Clear() override;
    
    int GetParticleCount() override; // TODO: both GPU and CPU
public:
    void NotifySubEmitters(vec3 position, EventType event);
    void EmitSubParticles(vec3 position, std::shared_ptr<PS3ParticleSystem> ps);
    
public:
    PS3RendererGPUPtr _renderer;
};

using PS3ParticleSystemGPUPtr = std::shared_ptr<PS3ParticleSystemGPU>;
