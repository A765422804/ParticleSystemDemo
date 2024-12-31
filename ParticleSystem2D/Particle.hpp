//
//  ParticleData.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/25.
//

#pragma once

#include <iostream>

struct GravityMode
{
    // X方向上的速度和Y方向上的速度
    float DirX;
    float DirY;
    
    float RadialAccel;
    float TangentialAccel;
};

struct RotationMode
{
    float Angle;
    float DeltaDegree;
    
    float Radius;
    float DeltaRadius;
};


class Particle
{
public:
    Particle();
    ~Particle() = default;
    
public:
    float _posX;
    float _posY;
    float _startPosX;
    float _startPosY;

    float _colorR;
    float _colorG;
    float _colorB;
    float _colorA;

    float _deltaColorR;
    float _deltaColorG;
    float _deltaColorB;
    float _deltaColorA;

    float _size;
    float _deltaSize;

    float _rotation; // degree
    float _deltaRotation; // degree

    float _timeToLive;
    
    GravityMode _gravityMode;
    RotationMode _rotationMode;
};
