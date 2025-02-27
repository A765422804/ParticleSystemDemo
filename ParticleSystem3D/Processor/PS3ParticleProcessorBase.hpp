//
//  PS3ParticleProcessorBase.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "../PS3Particle.hpp"

class PS3ParticleSystem;

class PS3ParticleProcessorBase
{
public:
    PS3ParticleProcessorBase(PS3ParticleSystem* ps);
    virtual ~PS3ParticleProcessorBase() = default;
    
public:
    virtual void UpdateParticles(std::vector<PS3ParticlePtr>& particles, float dt) = 0;
    virtual bool UpdateParticle(PS3ParticlePtr p, float dt) = 0;
    
public:
    PS3ParticleSystem* _ps; // 父级粒子系统
};

using PS3ParticleProcessorBasePtr = std::shared_ptr<PS3ParticleProcessorBase>;
