//
//  PS3SizeOvertime.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#include "PS3SizeOvertime.hpp"

PS3SizeOvertime::PS3SizeOvertime(CurveRangePtr size)
: _separateAxes(false)
, _size(size)
{
    
}

PS3SizeOvertime::PS3SizeOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z)
: _separateAxes(true)
, _xSize(x)
, _ySize(y)
, _zSize(z)
{
    
}

void PS3SizeOvertime::Animate(PS3ParticlePtr p, float dt)
{
    float normalizeTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    if (!_separateAxes) // 统一用size来改变大小
    {
        float rand = _size->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::SIZE) : 0;
        
        float size = _size->Evaluate(normalizeTime, rand);
        p->_size = p->_startSize * size;
    }
    else
    {
        float randX = _xSize->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::SIZE) : 0;
        float randY = _ySize->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::SIZE) : 0;
        float randZ = _zSize->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::SIZE) : 0;
        
        vec3 size = vec3(_xSize->Evaluate(normalizeTime, randX),
                        _ySize->Evaluate(normalizeTime, randY),
                        _zSize->Evaluate(normalizeTime, randZ));
        p->_size = p->_startSize * size;
    }
}
