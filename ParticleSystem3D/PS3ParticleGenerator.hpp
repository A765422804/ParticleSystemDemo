//
//  PS3ParticleGenerator.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/5.
//

#pragma once
#include "PS3Common.h"
#include "../Tool/CurveRange.hpp"
#include "PS3Burst.hpp"
#include "PS3Particle.hpp"

class PS3ParticleSystem;

class PS3ParticleGenerator
{
public:
    PS3ParticleGenerator(PS3ParticleSystem* ps, CurveRangePtr rateOverTime, CurveRangePtr rateOverDistance);
    ~PS3ParticleGenerator() = default;
    
public: // burst
    void AddBurst(PS3BurstPtr burst);
    void RemoveBurst(int idx);
    
public:
    std::vector<PS3ParticlePtr> Update(float dt);
    void Reset();
    
public:
    void BeforeGenerate(float dt);
    std::vector<PS3ParticlePtr> GenerateParticles();
    std::vector<PS3ParticlePtr> GenerateParticles(int count);
    
public:
    PS3ParticleSystem* _ps;
    
    CurveRangePtr _rateOverTime; // 随时间的发射率
    CurveRangePtr _rateOverDistance; // 随距离的发射率
    
    std::vector<std::shared_ptr<PS3Burst>> _bursts;
    
public:
    float _emitRateTimeCounter; // 随时间的发射数量
    float _emitRateDistanceCounter; // 随距离的发射数量
    
    int _emitCount; // 当前这次update的发射数量
};

using PS3ParticleGeneratorPtr = std::shared_ptr<PS3ParticleGenerator>;
