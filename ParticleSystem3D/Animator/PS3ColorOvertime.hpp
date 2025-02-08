//
//  PS3ColorOvertime.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/GradientRange.hpp"
#include "../PS3Particle.hpp"

class PS3ColorOvertime
{
public:
    PS3ColorOvertime(GradientRangePtr color);
    ~PS3ColorOvertime() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt);
    
public:
    bool _enable;
    GradientRangePtr _color;
};

using PS3ColorOvertimePtr = std::shared_ptr<PS3ColorOvertime>;
