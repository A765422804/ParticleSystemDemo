//
//  PS3CircleEmitter.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3AngleEmitter.hpp"

class PS3CircleEmitter : public PS3AngleEmitter
{
    using SuperType = PS3AngleEmitter;
public:
    PS3CircleEmitter();
    ~PS3CircleEmitter() = default;
    
public:
    static std::shared_ptr<PS3CircleEmitter> CreateCircleEmitter(ArcMode arcMode, float arcSpread, float arc, CurveRangePtr arcSpeed, float radius, float radiusThickness,float angleStep, PS3ParticleSystem* ps);
    
public:
    void Emit(PS3ParticlePtr particle) override;
    void RenderEmitter() override;
    
public:
    // TODO: 圆形发射器特有的属性：没有
};

using PS3CircleEmitterPtr = std::shared_ptr<PS3CircleEmitter>;
