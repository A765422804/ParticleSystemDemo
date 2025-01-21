//
//  Curve.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "../Head.h"

// 上溢出和下溢出的情况如何求值
enum class ExtrapolationMode
{
    LINEAR,
    CLAMP,
    LOOP,
    PING_PONG,
};

// 曲线区间内插值方法
enum class InterpolationMode
{
    LINEAR,
    CONSTANT,
    CUBIC,
};

struct KeyFrameValue 
{
    float Value;
    float LeftTangent;
    float RightTangent;
    // TODO: 原版中还包括TangentWeight，暂略
    // TODO: 修改value的类型为Template
};

class Curve
{
public:
    
public:
    float Evaluate(float time);
    
    float EvalBetweenTwoKeyFrames(float preTime, KeyFrameValue preValue, float nextTime, KeyFrameValue nextValue, float ratio);
    
public:
    float LinearTrend(float preTime, float preValue, float nextTime, float nextValue, float time);
    
    float WrapRepeat(float time, float preTime, float nextTime);
    float WrapPingPong(float time, float preTime, float nextTime);
    
private:
    std::vector<float> _times;
    std::vector<KeyFrameValue> _values;
    
    ExtrapolationMode _preExtrapolation;
    ExtrapolationMode _postExtrapolation;
    InterpolationMode _interpolationMode;
};

using CurvePtr = std::shared_ptr<Curve>;
