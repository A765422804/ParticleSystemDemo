//
//  PS3RotationOvertime.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/CurveRange.hpp"
#include "../PS3Particle.hpp"
#include "PS3OvertimeModule.hpp"

class PS3RotationOvertime : public PS3OvertimeModule
{
public:
    PS3RotationOvertime(CurveRangePtr rotate);
    PS3RotationOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z);
    ~PS3RotationOvertime() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt) override;
    
public:
    bool _separateAxes; // 是否在三个轴分开计算旋转
    
    CurveRangePtr _xRotate;
    CurveRangePtr _yRotate;
    CurveRangePtr _zRotate;
};

using PS3RotationOvertimePtr = std::shared_ptr<PS3RotationOvertime>;
