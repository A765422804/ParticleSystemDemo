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
    
    // 得到本次旋转的四元数
    if (!_separateAxes) // 只考虑z方向的旋转
    {
        p->_deltaEuler = vec3(0, 0, _zRotate->Evaluate(normalizeTime, Random01())) * dt;
        p->_deltaQuat = quat(radians(p->_deltaEuler));
    }
    else
    {
        p->_deltaEuler = vec3(_xRotate->Evaluate(normalizeTime, Random01()), _yRotate->Evaluate(normalizeTime, Random01()), _zRotate->Evaluate(normalizeTime, Random01())) * dt;
        p->_deltaQuat = quat(radians(p->_deltaEuler));
    }
    
    // 得到本次旋转的矩阵，并得到累积旋转overtime矩阵
    p->_deltaMat = toMat4(p->_deltaQuat);
    p->_accuMat = p->_accuMat * p->_deltaMat;
    
    // 结合初始旋转和rotationOvertime得到最终旋转
    p->_ultimateMat = p->_startMat * p->_accuMat;
    p->_ultimateQuat = quat_cast(p->_ultimateMat);
    
}
