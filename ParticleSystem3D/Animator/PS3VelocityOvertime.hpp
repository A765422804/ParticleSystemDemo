//
//  PS3VelocityOvertime.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/CurveRange.hpp"
#include "../PS3Particle.hpp"
#include "PS3OvertimeModule.hpp"

class PS3VelocityOvertime : public PS3OvertimeModule
{
public:
    PS3VelocityOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z);
    ~PS3VelocityOvertime() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt) override;
    
public:
    CurveRangePtr _xVelocity;
    CurveRangePtr _yVelocity;
    CurveRangePtr _zVelocity;
};

using PS3VelocityOvertimePtr = std::shared_ptr<PS3VelocityOvertime>;
