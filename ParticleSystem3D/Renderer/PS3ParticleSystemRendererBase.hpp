//
//  PS3ParticleSystemRendererBase.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#pragma once

#include "../PS3Common.h"
#include "../PS3Particle.hpp"
#include "../Model/PS3ParticleBatchModel.hpp"

class PS3ParticleSystem;

class PS3ParticleSystemRendererBase
{
public:
    // TODO: 这是个temp，方便写ParticleSystem类中的Emit方法
    virtual PS3ParticlePtr GetFreeParticle() = 0;
    virtual void SetNewParticle(PS3ParticlePtr particle) = 0;
    
public:
    virtual int UpdateParticles(float dt) = 0;
    virtual int GetParticleCount() = 0;
    
public:
    virtual void UpdateRenderData() = 0;
    virtual void Render() = 0;
    
public:
    void Clear();
    
public:
    PS3ParticleSystem* _particleSystem;
    PS3ParticleBatchModelPtr _model;
};

using PS3ParticleSystemRendererBasePtr = std::shared_ptr<PS3ParticleSystemRendererBase>;
