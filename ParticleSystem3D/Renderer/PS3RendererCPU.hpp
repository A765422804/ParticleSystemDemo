//
//  PS3RendererCPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#pragma once

#include "../PS3Common.h"
#include "PS3ParticleSystemRendererBase.hpp"

class PS3RendererCPU : public PS3ParticleSystemRendererBase
{
public:
    PS3RendererCPU(int maxParticleCount);
    ~PS3RendererCPU() = default;
    
public:
    void SetNewParticle(PS3ParticlePtr particle) override;
    int UpdateParticles(float dt) override;
    PS3ParticlePtr GetFreeParticle() override;
    int GetParticleCount() override;
    
public:
    void UpdateRenderData() override;
    void Render() override; // 在原版里是beforeRender
    
public:
    void FillMeshData(PS3ParticlePtr particle, int idx, int fi); // 本来应该根据RenderMode有多种func，暂时只写FillMeshData
    
public:
    std::vector<PS3ParticlePtr> _particles; // 粒子池
    PVData _particleVertexData; // 顶点属性data
};

using PS3RendererCPUPtr = std::shared_ptr<PS3RendererCPU>;
