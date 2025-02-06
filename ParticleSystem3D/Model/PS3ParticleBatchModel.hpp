//
//  PS3ParticleBatchModel.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#pragma once

#include "../PS3Common.h"
#include "../../Renderer/ParticleRenderer.hpp"

class PS3ParticleBatchModel
{
public:
    PS3ParticleBatchModel(int maxParticleCount);
    ~PS3ParticleBatchModel() = default;
    
public:
    void SetVertexAttributes(); // 设置顶点的属性
    void SetIndexData(); // 这是我自己的定义的，初始化索引数据
    
    void AddParticleVertexData(int index, PVData pvdata); // 写入粒子数据
    
    void RenderModel(int count); // 原版叫updateIA
    
public:
    ParticleRendererPtr _renderer; // 真正的renderer，不是PS3中的Renderer
    
    std::vector<float> _vDataF; // 存储所有的顶点数据
    std::vector<int> _iDataI; // 这是我自己定义的，存储indexdata，对应初始化函数是SetIndexData
    int _vertAttrsFloatCount = 16;
    
    int _capacity; // 最大粒子数量
    int _vertCount;
    int _indexCount;
    
    bool _enabled; // 是否启用
};

using PS3ParticleBatchModelPtr = std::shared_ptr<PS3ParticleBatchModel>;

// TODO: 我还需要一个函数去Resize我的VData和iData
