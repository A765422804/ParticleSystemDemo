//
//  PS3HemisphereEmitter.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ShapeModule.hpp"

class PS3HemisphereEmitter : public PS3ShapeModule
{
    using SuperType = PS3ShapeModule;
public:
    PS3HemisphereEmitter();
    ~PS3HemisphereEmitter() = default;
    
public:
    static std::shared_ptr<PS3HemisphereEmitter> CreateHemisphereEmitter(EmitLocation emitLocation, float radius, float radiusThickness, PS3ParticleSystem* ps);
    
public:
    void Emit(PS3ParticlePtr particle) override;
    void RenderEmitter() override;

public:
    float _radius;
    float _radiusThickness;
};

using PS3HemisphereEmitterPtr = std::shared_ptr<PS3HemisphereEmitter>;
