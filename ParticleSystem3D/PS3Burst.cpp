//
//  Burst.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "PS3Burst.hpp"
#include "PS3ParticleSystem.hpp"

void PS3Burst::Update(PS3ParticleSystem* ps, float dt)
{
    // TODO: 初始化的一些函数写在构造里
//    this._remainingCount = this._repeatCount;
//    this._curTime = this._time;
    
    if (_remainingCount > 0)
    {
        // 计算上一帧的时间和当前帧的时间
        int preFrameTime = Repeat(ps->_time - ps->_startDelay->Evaluate(0, 1), ps->_duration) - dt;
        preFrameTime = (preFrameTime > 0.0) ? preFrameTime : 0.0;
        int curFrameTime = Repeat(ps->_time - ps->_startDelay->Evaluate(0, 1), ps->_duration);
        // 如果curTime在这之间则执行burst
        if (_curTime >= preFrameTime && _curTime < curFrameTime)
        {
            ps->EmitParticles(_count->Evaluate(_curTime / ps->_duration, 1), dt - (curFrameTime - _curTime));
            _curTime += _repeatInterval;
            -- _remainingCount;
        }
    }
}
