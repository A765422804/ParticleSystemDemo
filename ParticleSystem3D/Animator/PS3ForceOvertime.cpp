//
//  PS3ForceOvertime.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#include "PS3ForceOvertime.hpp"

PS3ForceOvertime::PS3ForceOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z)
: _xForce(x)
, _yForce(y)
, _zForce(z)
{
}

void PS3ForceOvertime::Animate(PS3ParticlePtr p, float dt)
{
    float normalizeTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    float randX = _xForce->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::FORCE) : 0;
    float randY = _yForce->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::FORCE) : 0;
    float randZ = _zForce->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::FORCE) : 0;
    
    vec3 force = vec3(_xForce->Evaluate(normalizeTime, randX),
                    _yForce->Evaluate(normalizeTime, randY),
                    _zForce->Evaluate(normalizeTime, randZ));
    p->_velocity += force * dt;
    p->_ultimateVelocity = p->_velocity;
}
