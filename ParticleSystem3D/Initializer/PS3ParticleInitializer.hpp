//
//  PS3ParticleInitializer.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/CurveRange.hpp"
#include "../../Tool/GradientRange.hpp"
#include "../PS3Particle.hpp"

class PS3ParticleSystem;

class PS3ParticleInitializer
{
public:
    PS3ParticleInitializer(PS3ParticleSystem * ps);
    ~PS3ParticleInitializer() = default;
    
public:
    void InitializeParticle(PS3ParticlePtr p, float dt);
    
public: // start attribute
    PS3ParticleSystem* _ps; // 粒子系统
    
    // rotation
    bool _startRotation3D; // 是否要修改粒子在三轴上的旋转
    CurveRangePtr _startRotationX;
    CurveRangePtr _startRotationY;
    CurveRangePtr _startRotationZ;
    
    // size
    bool _startSize3D;
    CurveRangePtr _startSizeX;
    CurveRangePtr _startSizeY;
    CurveRangePtr _startSizeZ;
    
    // color
    GradientRangePtr _startColor;
    
    // lifeTime
    CurveRangePtr _startLifeTime;
    
    // velocity
    CurveRangePtr _startSpeed; // 起始速度
};

using PS3ParticleInitializerPtr = std::shared_ptr<PS3ParticleInitializer>;
