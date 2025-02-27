//
//  PS3SizeOvertime.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/CurveRange.hpp"
#include "../PS3Particle.hpp"
#include "PS3OvertimeModule.hpp"

class PS3SizeOvertime : public PS3OvertimeModule
{
public:
    PS3SizeOvertime(CurveRangePtr size);
    PS3SizeOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z);
    ~PS3SizeOvertime() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt) override;
    
public:
    bool _separateAxes; // 是否在每个轴上独立控制粒子大小
    
    CurveRangePtr _size; // 非独立控制时，size随时间变化
    
    // 独立控制时，各个轴上的粒子大小
    CurveRangePtr _xSize; // x
    CurveRangePtr _ySize; // y
    CurveRangePtr _zSize; // z
};

using PS3SizeOvertimePtr = std::shared_ptr<PS3SizeOvertime>;
