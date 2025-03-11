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

// TODO: 仅保留基于CPU的逻辑

class PS3ParticleSystem;

class PS3ParticleBatchModelCPU
{
public:
    PS3ParticleBatchModelCPU(int maxParticleCount, PS3ParticleSystem* ps);
    ~PS3ParticleBatchModelCPU() = default;
    
public:
    void SetVertexAttributes(); // 设置顶点的属性 Both
    void SetIndexData(); // 这是我自己的定义的，初始化索引数据 Both
    
    void AddParticleVertexData(int index, PVData pvdata); // 写入粒子数据 CPU
//    void AddGPUParticleVertexData(PS3ParticlePtr p, int num, float time); // 写入gpu粒子数据 GPU
    
//    void ClearGPUParticles(); // GPU
    
    void RenderModel(int count); // 原版叫updateIA CPU
//    void RenderModelGPU(); // GPU
//    
//    void UpdateGPUParticles(float time, float dt); // GPU
//    
//    vec3 CalculateDeathPos(int index, float interval); // GPU
    
public:
    PS3ParticleSystem *_ps; // BOTH
    
    ParticleRendererPtr _renderer; // 真正的renderer，不是PS3中的Renderer // BOTH
    
    std::vector<float> _vDataF; // 存储所有的顶点数据 // BOTH
    std::vector<int> _iDataI; // 这是我自己定义的，存储indexdata，对应初始化函数是SetIndexData // BOTH
    int _vertAttrsFloatCount = 17; // CPU
//    int _vertAttrsFloatGPUCount = 21; // GPU
//    int _vertCount = 4; // GPU
//    
//    int _startTimeOffset = 3; // GPU
//    int _lifeTimeOffset = 18; // GPU
//    int _positionOffset = 0; // GPU
     
    int _capacity; // 最大粒子数量 BOTH
    
    //int _particleCountGPU; // TODO: 这是一个hack的做法，把gpu的粒子数量存在这个地方，在emit的时候+1，在kill的时候-1 // GPU
    
    bool _enabled; // 是否启用 // BOTH
};

using PS3ParticleBatchModelCPUPtr = std::shared_ptr<PS3ParticleBatchModelCPU>;
