//
//  Burst.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "PS3Common.h"
#include "../Tool/CurveRange.hpp"

class PS3ParticleSystem;

class PS3Burst
{
public:
    PS3Burst(float time, int repeatCount, float repeatInterval, CurveRangePtr count);
    ~PS3Burst() = default;
    
public:
    int Update(PS3ParticleSystem* ps, float dt);
    void Reset();
    
private: // 属性
    float _time; //  从开始运行到触发burst的时间
    int _repeatCount; // burst的触发次数
    float _repeatInterval; // 每次触发的间隔时间
    CurveRangePtr _count; // 发射的粒子数量
    
private: // 状态
    float _curTime; // 当前爆发的时间点
    int _remainingCount; // 还剩下几次burst
};

using PS3BurstPtr = std::shared_ptr<PS3Burst>;
