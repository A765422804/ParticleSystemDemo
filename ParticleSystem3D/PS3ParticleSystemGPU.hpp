//
//  PS3ParticleSystemGPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#pragma once
#include "PS3Common.h"
#include "PS3ParticleSystem.hpp"
#include "Renderer/PS3EntityGPU.hpp"

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
    
    PS3EntityGPUPtr _entity;
    
    Texture2DPtr _animationTexture; // 1
    Texture2DPtr _velocityTexture; // 2
    Texture2DPtr _forceTexture; // 3
    Texture2DPtr _sizeTexture; // 4
    Texture2DPtr _colorTexture; // 5
    Texture2DPtr _rotationTexture; // 6
    
    Texture2DPtr _gravityTexture; // 7
};



using PS3ParticleSystemGPUPtr = std::shared_ptr<PS3ParticleSystemGPU>;
