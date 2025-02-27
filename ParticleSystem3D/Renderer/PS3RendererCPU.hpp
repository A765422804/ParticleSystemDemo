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
    using SuperType = PS3ParticleSystemRendererBase;
public:
    PS3RendererCPU(PS3ParticleSystem* ps, int maxParticleCount);
    ~PS3RendererCPU() = default;
    
public:
    //int UpdateParticles(float dt) override;
    
public:
    void UpdateRenderData(std::vector<PS3ParticlePtr> particles) override;
    void Render() override; // 在原版里是beforeRender
    void Clear() override; 
    
    void SetNewParticle(PS3ParticlePtr particle) override;
    
public:
    void InitUniform() override;
    void UpdateUniform() override;
    
public:
    void FillMeshData(PS3ParticlePtr particle, int idx, float fi); // 本来应该根据RenderMode有多种func，暂时只写FillMeshData
    
public:
    PVData _particleVertexData; // 顶点属性data
};

using PS3RendererCPUPtr = std::shared_ptr<PS3RendererCPU>;
