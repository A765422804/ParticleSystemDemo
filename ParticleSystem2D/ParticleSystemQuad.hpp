//
//  ParticleSystemQuad.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#pragma once

#include "../Common/Head.h"
#include "ParticleSystem.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Common/VertexType.hpp"

class ParticleSystemQuad : public ParticleSystem
{
public:
    using SuperType = ParticleSystem;
    using ParticleSystemQuadPtr = std::shared_ptr<ParticleSystemQuad>;
    
public:
    ParticleSystemQuad();
    ~ParticleSystemQuad() = default;
    
public:
    static ParticleSystemQuadPtr Create();
    
    void Init();
    
    void AllocMemory(); // 为_quads和_indices分配空间
    void InitIndices(); // 构建索引indices
    
    void UpdateParticleQuads() override; // 更新粒子Quads
    void Draw() override;
    void UpdatePosWithParticle(int quadIndex);
    
private:
    std::vector<V3_C4_T2_Quad> _quads; // quads to be rendered
    std::vector<unsigned int> _indices; // indices
    
    RendererPtr _renderer; // 渲染器
};

using ParticleSystemQuadPtr = ParticleSystemQuad::ParticleSystemQuadPtr;
