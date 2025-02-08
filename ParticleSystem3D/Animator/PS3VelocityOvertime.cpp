//
//  PS3VelocityOvertime.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#include "PS3VelocityOvertime.hpp"

PS3VelocityOvertime::PS3VelocityOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z)
: _xVelocity(x)
, _yVelocity(y)
, _zVelocity(z)
{
}

void PS3VelocityOvertime::Animate(PS3ParticlePtr p, float dt)
{
    float normalizeTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    vec3 vel = vec3(_xVelocity->Evaluate(normalizeTime, Random01()),
                    _yVelocity->Evaluate(normalizeTime, Random01()),
                    _zVelocity->Evaluate(normalizeTime, Random01()));
    p->_animatedVelocity = vel;
    p->_ultimateVelocity += p->_animatedVelocity;
}
