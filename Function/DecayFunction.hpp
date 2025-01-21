//
//  DecayFunction.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/7/24.
//

#ifndef DecayFunction_hpp
#define DecayFunction_hpp

#include "../Head.h"

class DecayFunction
{
public:
    //定义一个枚举类型来表示不同的衰减函数
    enum DecayType
    {
        LINEAR,
        FAST_THEN_SLOW,
        SLOW_THEN_FAST,
        FAST_SLOW_FAST,
        SLOW_FAST_SLOW
    };
    
    static float Decay(DecayType decayType, const float& curValue, const float& maxValue, const float &defaultValue = 1.0f)
    {
        switch (decayType)
        {
            case LINEAR:
                return std::min(Linear(curValue, maxValue), defaultValue);
            case FAST_THEN_SLOW:
                return std::min(FastThenSlow(curValue, maxValue), defaultValue);
            case SLOW_THEN_FAST:
                return std::min(SlowThenFast(curValue, maxValue), defaultValue);
            case FAST_SLOW_FAST:
                return std::min(FastSlowFast(curValue, maxValue), defaultValue);
            case SLOW_FAST_SLOW:
                return std::min(SlowFastSlow(curValue, maxValue), defaultValue);
            default:
                return 1.0f;
        }
    }
    
    static float Linear(const float& curValue, const float& maxValue)
    {
        if (curValue > maxValue)
            return 1.0f;
        return curValue / maxValue;
    }
    
    static float FastThenSlow(const float& curValue, const float& maxValue)
    {
        if (curValue > maxValue)
            return 1.0f;
        float x = curValue / maxValue;
        return std::pow(x, 2);
    }
    
    static float SlowThenFast(const float& curValue, const float& maxValue)
    {
        if (curValue > maxValue)
            return 1.0f;
        float x = curValue / maxValue;
        return std::pow(x, 0.5);
    }
    
    static float FastSlowFast(const float& curValue, const float& maxValue)
    {
        if (curValue > maxValue)
            return 1.0f;
        float x = curValue / maxValue;
        if (x >= 0.5)
        {
            return 1 - pow(0.5,0.5) * pow(1 - x,0.5);
        }
        else if (x >= 0)
        {
            return pow(0.5, 0.5) * pow(x, 0.5);
        }
        else
        {
            return 0;
        }
    }
    
    static float SlowFastSlow(const float& curValue, const float& maxValue)
    {
        if (curValue > maxValue)
            return 1.0f;
        float x = curValue / maxValue;
        if (x >= 0.5)
        {
            return 1 - 2 * pow(1 - x, 2);
        }
        else if (x >= 0)
        {
            return 2 * pow(x, 2);
        }
        else
        {
            return 0;
        }
    }
};

#endif /* DecayFunction_hpp */
