//
//  Curve.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "Curve.hpp"

float Curve::Evaluate(float time)
{
    int nFrames = int(_times.size());
    float firstTime = _times[0];
    float lastTime = _times[nFrames - 1];
    
    if (time < firstTime) // 下溢
    {
        float preValue = _values[0].Value;
        if (_preExtrapolation == ExtrapolationMode::CLAMP || nFrames < 2)
            return preValue;
        switch (_preExtrapolation)
        {
            case ExtrapolationMode::LINEAR:
                return LinearTrend(firstTime, preValue, _times[1], _values[1].Value, time);
            case ExtrapolationMode::LOOP:
                time = WrapRepeat(time, firstTime, lastTime);
                break;
            case ExtrapolationMode::PING_PONG:
                time = WrapPingPong(time, firstTime, lastTime);
                break;
            default:
                AssertShouldNotReachHere();
        }
    }
    else if (time > lastTime) // 上溢
    {
        float preFrame = _values[nFrames - 1].Value;
        if (_postExtrapolation == ExtrapolationMode::CLAMP || nFrames < 2)
            return preFrame;
        switch (_postExtrapolation) {
            case ExtrapolationMode::LINEAR:
                return LinearTrend(lastTime, preFrame, _times[nFrames - 2], _values[nFrames - 2].Value, time);
                break;
            case ExtrapolationMode::LOOP:
                time = WrapRepeat(time, firstTime, lastTime);
            case ExtrapolationMode::PING_PONG:
                time = WrapPingPong(time, firstTime, lastTime);
            default:
                AssertShouldNotReachHere();
        }
    }
    
    auto lowerBound = std::lower_bound(_times.begin(), _times.end(), time);
    
    int index = int(std::distance(_times.begin(), lowerBound));
    if (_times[index] == time)
        return _values[index].Value;
    
    int preIndex = index - 1;
    int nextIndex = index;
    int preTime = _times[preIndex];
    int nextTime = _times[nextIndex];
    KeyFrameValue preValue = _values[preIndex];
    KeyFrameValue nextValue = _values[nextIndex];
    
    float dt = nextTime - preTime;
    float ratio = (time - preTime) / dt;
    return EvalBetweenTwoKeyFrames(preTime, preValue, nextTime, nextValue, ratio);
}

float Curve::EvalBetweenTwoKeyFrames(float preTime, KeyFrameValue preValue, float nextTime, KeyFrameValue nextValue, float ratio)
{
    switch (_interpolationMode) {
        case InterpolationMode::CONSTANT:
            return preValue.Value;
        case InterpolationMode::LINEAR:
            return LinearInterpolate(preValue.Value, nextValue.Value, ratio);
        case InterpolationMode::CUBIC:
        {
            float dt = nextTime - preTime;
            float ONE_THIRD = 1.0 / 3.0;
            float preTangent = preValue.LeftTangent;
            float nextTangent = nextValue.RightTangent;
            float p1 = preValue.Value + ONE_THIRD * preTangent * dt;
            float p2 = nextValue.Value - ONE_THIRD * nextTangent * dt;
            return BezierInterpolate(preValue.Value, p1, p2, nextValue.Value, ratio);
        }
        default:
            AssertShouldNotReachHere();
    }
}

float Curve::LinearTrend(float preTime, float preValue, float nextTime, float nextValue, float time)
{
    float slope = (nextValue - preValue) / (nextTime - preTime);
    return preValue + (time - preValue) * slope;
}

float Curve::WrapRepeat(float time, float preTime, float nextTime)
{
    return preTime + Repeat(time - preTime, nextTime - preTime);
}

float Curve::WrapPingPong(float time, float preTime, float nextTime)
{
    return preTime + PingPong(time - preTime, nextTime - preTime);
}
