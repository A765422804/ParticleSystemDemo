//
//  ParticleData.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/25.
//

#include "Particle.hpp"

Particle::Particle()
    : _posX(0.0f)
    , _posY(0.0f)
    , _startPosX(0.0f)
    , _startPosY(0.0f)
    , _colorR(1.0f)
    , _colorG(1.0f)
    , _colorB(1.0f)
    , _colorA(1.0f)
    , _deltaColorR(0.0f)
    , _deltaColorG(0.0f)
    , _deltaColorB(0.0f)
    , _deltaColorA(0.0f)
    , _size(1.0f)
    , _deltaSize(0.0f)
    , _rotation(0.0f)
    , _deltaRotation(0.0f)
    , _timeToLive(1.0f)
    , _gravityMode()
    , _rotationMode()
{
}
