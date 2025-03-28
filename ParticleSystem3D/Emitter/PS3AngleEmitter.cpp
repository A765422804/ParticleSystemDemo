//
//  PS3AngleEmitter.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3AngleEmitter.hpp"
#include "../PS3ParticleSystem.hpp"

PS3AngleEmitter::PS3AngleEmitter()
{
    
}

float PS3AngleEmitter::GenerateArcAngle()
{
    if (_arcMode == ArcMode::RANDOM)
        return RandomRange(0, _arc);

    if (_arcMode == ArcMode::EVEN)
    {
        float angle = _totalAngle + _angleStep;
        _totalAngle = Repeat(angle, _arc);
        return angle;
    }

    float angle = _totalAngle + 2 * M_PI * _arcSpeed->Evaluate(_ps->_time, 1) * (_ps->_time - _lastTime); // 这一帧的角度 = 上一帧的角度 + 角度增量
    _totalAngle = angle;
    std::cout<<"totalAngle: "<<angle<<std::endl;
    if (_arcSpread != 0)
    {
        angle = floor(angle / (_arc * _arcSpread)) * _arc * _arcSpread;
    }
    switch (_arcMode)
    {
        case ArcMode::LOOP:
            return Repeat(angle, _arc);
        case ArcMode::PINGPONG:
            return PingPong(angle, _arc);
        default:
            AssertShouldNotReachHere();
    }
}
