//
//  PS3ConeEmitter.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3AngleEmitter.hpp"

class PS3ConeEmitter : public PS3AngleEmitter
{
    using SuperType = PS3AngleEmitter;
public:
    PS3ConeEmitter();
    ~PS3ConeEmitter() = default;
    
public:
    static std::shared_ptr<PS3ConeEmitter> CreateConeEmitter(EmitLocation emitLocation, ArcMode arcMode, float arcSpread, float arc, CurveRangePtr arcSpeed, float radius, float radiusThickness,float angleStep, float angle, float length, PS3ParticleSystem* ps);
    
public:
    void Emit(PS3ParticlePtr particle) override;
    void RenderEmitter() override;
    
public:
    float _angle; // 轴与母线的夹角，角度制
    float _length; // 顶部截面到底面的长度
};

using PS3ConeEmitterPtr = std::shared_ptr<PS3ConeEmitter>;
