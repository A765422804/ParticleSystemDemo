//
//  PS3OvertimeModule.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "../PS3Particle.hpp"

class PS3OvertimeModule
{
public:
    PS3OvertimeModule();
    virtual ~PS3OvertimeModule() = default;
public:
    virtual void Animate(PS3ParticlePtr p, float dt) = 0;
    
public:
    bool _enable;
};

using PS3OvertimeModulePtr = std::shared_ptr<PS3OvertimeModule>;
