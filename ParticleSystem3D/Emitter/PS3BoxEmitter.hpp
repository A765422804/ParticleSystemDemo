//
//  PS3BoxEmitter.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ShapeModule.hpp"

class PS3BoxEmitter : public PS3ShapeModule
{
    using SuperType = PS3ShapeModule;
public:
    PS3BoxEmitter();
    ~PS3BoxEmitter() = default;
    
public:
    static std::shared_ptr<PS3BoxEmitter> CreateBoxEmitter(EmitLocation emitLocation, PS3ParticleSystem* ps);
    
public:
    void Emit(PS3ParticlePtr particle) override;
    void RenderEmitter() override;
    
    // TODO: ApplyBoxThickness()
public:
    // TODO: box thickness属性
};

using PS3BoxEmitterPtr = std::shared_ptr<PS3BoxEmitter>;
