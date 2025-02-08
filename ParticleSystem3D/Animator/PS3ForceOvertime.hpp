//
//  PS3ForceOvertime.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/CurveRange.hpp"
#include "../PS3Particle.hpp"

class PS3ForceOvertime
{
public:
    PS3ForceOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z);
    ~PS3ForceOvertime() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt);
    
public:
    bool _enable;
    
    CurveRangePtr _xForce;
    CurveRangePtr _yForce;
    CurveRangePtr _zForce;
};

using PS3ForceOvertimePtr = std::shared_ptr<PS3ForceOvertime>;
