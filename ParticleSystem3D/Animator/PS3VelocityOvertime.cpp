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
    float randX = _xVelocity->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::VELOCITY_X) : 0;
    float randY = _yVelocity->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::VELOCITY_Y) : 0;
    float randZ = _zVelocity->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::VELOCITY_Z) : 0;
    
    vec3 vel = vec3(_xVelocity->Evaluate(normalizeTime, randX),
                    _yVelocity->Evaluate(normalizeTime, randY),
                    _zVelocity->Evaluate(normalizeTime, randZ));
    
    p->_animatedVelocity = vel;
    p->_ultimateVelocity += p->_animatedVelocity;
}
