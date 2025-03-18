//
//  PS3RendererGPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/25.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ParticleSystemRendererBase.hpp"
#include "../../Function/Texture2D.hpp"
#include "../Model/PS3ParticleBatchModelGPU.hpp"
#include "../RAC&RAS/PS3GPURAC.hpp"
#include "../RAC&RAS/PS3GPURAS.hpp"

class PS3ParticleSystemGPU;

class PS3RendererGPU
{
public:
    PS3RendererGPU(PS3ParticleSystemGPU* ps, int maxParticleCount);
    ~PS3RendererGPU() = default;
    
public:
    void Render();
    void Clear();
    
    void SetNewParticle(PS3ParticlePtr particle);
    void SetNewParticles(std::vector<PS3ParticlePtr> &particles);
    
public:
    //void InitUniform();
    void UpdateUniform();
    
public:
    PS3ParticleSystemGPU* _ps;
    PS3ParticleBatchModelGPUPtr _model;
    
    // 各种曲线的texture
//    Texture2DPtr _animationTexture; // 1
//    Texture2DPtr _velocityTexture; // 2
//    Texture2DPtr _forceTexture; // 3
//    Texture2DPtr _sizeTexture; // 4
//    Texture2DPtr _colorTexture; // 5
//    Texture2DPtr _rotationTexture; // 6
    
    PS3GPURACPtr _rac;
    PS3GPURASPtr _ras;
};

using PS3RendererGPUPtr = std::shared_ptr<PS3RendererGPU>;
