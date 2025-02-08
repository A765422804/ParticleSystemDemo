//
//  Particle.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "PS3Common.h"

class PS3ParticleSystem;

class PS3Particle
{
public:
    PS3Particle();
    ~PS3Particle() = default;
    
public:
    void Reset();
    
public:
    PS3ParticleSystem* _particleSystem;
    
    vec3 _position;
    vec3 _velocity;
    vec3 _angularVelocity;
    vec3 _ultimateVelocity;
    vec3 _animatedVelocity;
    
    vec3 _startEuler; // 起始三轴的欧拉角
    vec3 _rotation; // 旋转角度
    
    vec3 _startSize;
    vec3 _size;
    
    vec4 _startColor;
    vec4 _color;
    
    unsigned int randomSeed;
    
    float _remainingLifeTime;
    float _startLifeTime;
    
    int _frameIndex; // 帧序列
};

using PS3ParticlePtr = std::shared_ptr<PS3Particle>;
