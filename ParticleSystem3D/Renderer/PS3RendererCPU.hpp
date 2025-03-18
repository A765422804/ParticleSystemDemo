//
//  PS3RendererCPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#pragma once

#include "../PS3Common.h"
#include "PS3ParticleSystemRendererBase.hpp"
#include "../Model/PS3ParticleBatchModelCPU.hpp"
#include "../RAC&RAS/PS3CPURAC.hpp"
#include "../RAC&RAS/PS3CPURAS.hpp"

class PS3ParticleSystemCPU;

class PS3RendererCPU
{
public:
    PS3RendererCPU(PS3ParticleSystemCPU* ps, int maxParticleCount);
    ~PS3RendererCPU() = default;
    
public:
    void UpdateRenderData(std::vector<PS3ParticlePtr> particles);
    void Render(); // 在原版里是beforeRender
    void Clear();
    
public:
    void UpdateUniform();
    
public:
    void FillMeshData(PS3ParticlePtr particle, int idx, float fi); // 本来应该根据RenderMode有多种func，暂时只写FillMeshData
    
public:
    PS3ParticleSystemCPU* _ps;
    PS3ParticleBatchModelCPUPtr _model;
    
    PVData _particleVertexData; // 顶点属性data
    
    PS3CPURACPtr _rac;
    PS3CPURASPtr _ras;
};

using PS3RendererCPUPtr = std::shared_ptr<PS3RendererCPU>;
