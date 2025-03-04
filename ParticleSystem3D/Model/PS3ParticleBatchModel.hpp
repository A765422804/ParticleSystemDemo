//
//  PS3ParticleBatchModel.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#pragma once

#include "../PS3Common.h"
#include "../../Renderer/ParticleRenderer.hpp"
#include "../PS3Particle.hpp"

class PS3ParticleBatchModel
{
public:
    PS3ParticleBatchModel(int maxParticleCount, bool useGPU);
    ~PS3ParticleBatchModel() = default;
    
public:
    void SetVertexAttributes(); // 设置顶点的属性
    void SetIndexData(); // 这是我自己的定义的，初始化索引数据
    
    void AddParticleVertexData(int index, PVData pvdata); // 写入粒子数据
    void AddGPUParticleVertexData(PS3ParticlePtr p, int num, float time); // 写入gpu粒子数据
    
    void RenderModelCPU(int count); // 原版叫updateIA
    void RenderModelGPU();
    
    void UpdateGPUParticles(float time, float dt);
    
public:
    ParticleRendererPtr _renderer; // 真正的renderer，不是PS3中的Renderer
    
    std::vector<float> _vDataF; // 存储所有的顶点数据
    std::vector<int> _iDataI; // 这是我自己定义的，存储indexdata，对应初始化函数是SetIndexData
    int _vertAttrsFloatCount = 17;
    int _vertAttrsFloatGPUCount = 21;
    int _vertCount = 4;
    
    int _startTimeOffset = 3;
    int _lifeTimeOffset = 18;
    
    int _capacity; // 最大粒子数量
    
    int _particleCountGPU; // TODO: 这是一个hack的做法，把gpu的粒子数量存在这个地方，在emit的时候+1，在kill的时候-1
    
    bool _enabled; // 是否启用
};

using PS3ParticleBatchModelPtr = std::shared_ptr<PS3ParticleBatchModel>;
