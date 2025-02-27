//
//  PS3ParticleProcessorGPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/26.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ParticleProcessorBase.hpp"

class PS3ParticleProcessorGPU : public PS3ParticleProcessorBase
{
    using SuperType = PS3ParticleProcessorBase;
public:
    PS3ParticleProcessorGPU(PS3ParticleSystem* ps);
    ~PS3ParticleProcessorGPU() = default;

public:
    void UpdateParticles(std::vector<PS3ParticlePtr>& particles, float dt) override;
    bool UpdateParticle(PS3ParticlePtr p, float dt) override;
};

using PS3ParticleProcessorGPUPtr = std::shared_ptr<PS3ParticleProcessorGPU>;
