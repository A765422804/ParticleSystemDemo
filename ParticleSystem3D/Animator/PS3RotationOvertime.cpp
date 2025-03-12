//
//  PS3RotationOvertime.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/8.
//

#include "PS3RotationOvertime.hpp"

PS3RotationOvertime::PS3RotationOvertime(CurveRangePtr rotate)
: _separateAxes(false)
, _zRotate(rotate)
{
    
}

PS3RotationOvertime::PS3RotationOvertime(CurveRangePtr x, CurveRangePtr y, CurveRangePtr z)
: _separateAxes(true)
, _xRotate(x)
, _yRotate(y)
, _zRotate(z)
{
    
}

void PS3RotationOvertime::Animate(PS3ParticlePtr p, float dt)
{
    float normalizeTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    float randZ = _zRotate->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::ROTATION) : 0;
    
    // 得到本次旋转的四元数
    if (!_separateAxes) // 只考虑z方向的旋转
    {
        p->_deltaEuler = vec3(0, 0, _zRotate->Evaluate(normalizeTime, randZ)) * dt;
        p->_deltaQuat = quat(p->_deltaEuler);
    }
    else
    {
        float randX = _xRotate->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::ROTATION) : 0;
        float randY = _yRotate->IsTwoValues() ? PseudoRandom(p->_randomSeed + ModuleRandSeed::ROTATION) : 0;
        
        p->_deltaEuler = vec3(_xRotate->Evaluate(normalizeTime, randX), _yRotate->Evaluate(normalizeTime, randY), _zRotate->Evaluate(normalizeTime, randZ)) * dt;
        p->_deltaQuat = quat(p->_deltaEuler);
    }
    
    // 得到本次旋转的矩阵，并得到累积旋转overtime矩阵
    p->_deltaMat = toMat4(p->_deltaQuat);
    p->_accuMat = p->_accuMat * p->_deltaMat;
    
    // 结合初始旋转和rotationOvertime得到最终旋转
    p->_ultimateMat = p->_startMat * p->_accuMat;
    p->_ultimateQuat = quat_cast(p->_ultimateMat);
    
}
