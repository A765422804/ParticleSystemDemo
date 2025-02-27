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
    PS3ParticleSystemRendererBase(PS3ParticleSystem* ps, int maxParticleCount, bool useGPU);
    virtual ~PS3ParticleSystemRendererBase() = default;

public:
    virtual void UpdateRenderData(std::vector<PS3ParticlePtr> particles) = 0;
    virtual void Render() = 0;
    
    virtual void SetNewParticle(PS3ParticlePtr particle) = 0;
    
public:
    virtual void InitUniform() = 0;
    virtual void UpdateUniform() = 0;
    
public:
    GLuint GetShaderID()
    {
        return _model->_renderer->_shader->ID;
    }
    
public:
    virtual void Clear();
    
public:
    PS3ParticleSystem* _ps;
    PS3ParticleBatchModelPtr _model;
};

using PS3ParticleSystemRendererBasePtr = std::shared_ptr<PS3ParticleSystemRendererBase>;
