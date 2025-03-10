//
//  Burst.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "PS3Burst.hpp"
#include "PS3ParticleSystem.hpp"

PS3Burst::PS3Burst(float time, int repeatCount, float repeatInterval, CurveRangePtr count)
: _time(time)
, _repeatCount(repeatCount)
, _repeatInterval(repeatInterval)
, _count(count)
, _remainingCount(0)
, _curTime(0.0f)
{
}

void PS3Burst::Reset()
{
    _remainingCount = 0;
    _curTime = 0.0f;
}

int PS3Burst::Update(PS3ParticleSystem* ps, float dt)
{
    int emitCount = 0;
    if (_remainingCount == 0)
    {
        _remainingCount = _repeatCount;
        _curTime = _time;
    }
    
    if (_remainingCount > 0)
    {
        // 计算上一帧的时间点和当前帧的时间点
        float preFrameTime = Repeat(ps->_time - ps->_startDelay->Evaluate(0, 1), ps->_duration) - dt;
        preFrameTime = (preFrameTime > 0.0) ? preFrameTime : 0.0;
        float curFrameTime = Repeat(ps->_time - ps->_startDelay->Evaluate(0, 1), ps->_duration);
        // 如果curTime在这之间则执行burst
        if (_curTime >= preFrameTime && _curTime < curFrameTime)
        {
            emitCount = _count->Evaluate(_curTime / ps->_duration, 1);
            // ps->EmitParticles(_count->Evaluate(_curTime / ps->_duration, 1), dt - (curFrameTime - _curTime));
            _curTime += _repeatInterval;
            -- _remainingCount;
        }
    }
    
    return emitCount;
}
