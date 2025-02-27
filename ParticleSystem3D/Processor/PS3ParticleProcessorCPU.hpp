//
//  PS3ParticleProcessorCPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ParticleProcessorBase.hpp"

class PS3ParticleProcessorCPU : public PS3ParticleProcessorBase
{
    using SuperType = PS3ParticleProcessorBase;
public:
    PS3ParticleProcessorCPU(PS3ParticleSystem* ps);
    ~PS3ParticleProcessorCPU() = default;

public:
    void UpdateParticles(std::vector<PS3ParticlePtr>& particles, float dt) override;
    bool UpdateParticle(PS3ParticlePtr p, float dt) override;
};

using PS3ParticleProcessorCPUPtr = std::shared_ptr<PS3ParticleProcessorCPU>;
