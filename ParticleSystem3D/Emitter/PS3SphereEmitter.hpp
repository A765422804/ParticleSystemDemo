//
//  PS3SphereEmitter.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ShapeModule.hpp"

class PS3SphereEmitter : public PS3ShapeModule
{
    using SuperType = PS3ShapeModule;
public:
    PS3SphereEmitter();
    ~PS3SphereEmitter() = default;
    
public:
    static std::shared_ptr<PS3SphereEmitter> CreateSphereEmitter(EmitLocation emitLocation, float radius, float radiusThickness, PS3ParticleSystem* ps);
    
public:
    void Emit(PS3ParticlePtr particle) override;
    void RenderEmitter() override;

public:
    float _radius;
    float _radiusThickness;
};

using PS3SphereEmitterPtr = std::shared_ptr<PS3SphereEmitter>;
