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
    quat _startQuat; // 起始旋转的四元数
    mat4 _startMat; // 起始旋转的矩阵
    quat _ultimateQuat; // 最终旋转四元数，传递给shader的值
    mat4 _ultimateMat = mat4(1.0f); // 最终旋转的矩阵
    vec3 _deltaEuler; // 变化的欧拉角
    quat _deltaQuat; // 变化的四元数
    mat4 _deltaMat; // 变化的矩阵
    mat4 _accuMat = mat4(1.0f); // 累积rotationOvertime矩阵
    
    vec3 _startSize;
    vec3 _size;
    
    vec4 _startColor;
    vec4 _color;
    
    unsigned int randomSeed;
    
    float _remainingLifeTime;
    float _startLifeTime;
    
    float _frameIndex; // 帧序列
    int _startRow; // 随机初始化的使用纹理动画的行号
};

using PS3ParticlePtr = std::shared_ptr<PS3Particle>;
