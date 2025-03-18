//
//  PS3ParticleBatchModelGPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#pragma once
#include "../PS3Common.h"
#include "../../Renderer/ParticleGLInterface.hpp"
#include "../PS3Particle.hpp"

// GPU模型的逻辑

class PS3ParticleSystem;

class PS3ParticleBatchModelGPU
{
public:
    PS3ParticleBatchModelGPU(int maxParticleCount, PS3ParticleSystem* ps);
    ~PS3ParticleBatchModelGPU() = default;
    
public:
    void SetVertexAttributes(); // 设置顶点的属性 Both
    void SetIndexData(); // 这是我自己的定义的，初始化索引数据 Both
    
public:
    void AddParticleVertexData(PS3ParticlePtr p, int num, float time);
    
    void ClearParticles();
    
    void RenderModel();
    
    void UpdateParticles(float time, float dt);
public:
    PS3ParticleSystem *_ps;
    ParticleGLInterfacePtr _renderer;
    
    std::vector<float> _vDataF; // 存储所有的顶点数据
    std::vector<int> _iDataI; // 这是我自己定义的，存储indexdata，对应初始化函数是SetIndexData
    
    int _vertAttrsFloatCount = 22;
    int _vertCount = 4;
    int _startTimeOffset = 3;
    int _lifeTimeOffset = 19;
    int _positionOffset = 0;
    
    int _capacity; // 最大粒子数量
    
    int _particleCount; // TODO: 这是一个hack的做法，把gpu的粒子数量存在这个地方，在emit的时候+1，在kill的时候-1
    
    bool _enabled; // 是否启用
};

using PS3ParticleBatchModelGPUPtr = std::shared_ptr<PS3ParticleBatchModelGPU>;
