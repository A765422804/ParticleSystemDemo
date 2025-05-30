//
//  PS3ColorOvertime.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#include "PS3ColorOvertime.hpp"

PS3ColorOvertime::PS3ColorOvertime(GradientRangePtr color)
: _color(color)
{
    
}

void PS3ColorOvertime::Animate(PS3ParticlePtr p, float dt)
{
    float normalizeTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    float rand = _color->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::COLOR) : 0;
    p->_color = p->_startColor;
    vec4 color = _color->Evaluate(normalizeTime, rand);
    p->_color *= color;
}
