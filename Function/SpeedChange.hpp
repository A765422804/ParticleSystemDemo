//
//  SpeedChange.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/7/25.
//

#ifndef SpeedChange_hpp
#define SpeedChange_hpp

#include "../Common/Head.h"

class SpeedChange
{
public:
    //定义一个枚举类型来表示不同的衰减函数
    enum ChangeType
    {
        NORMAL,
        NORMAL_MIN_NORMAL,
        NORMAL_MAX_NORMAL,
        MAX_MIN_MAX,
        MIN_MAX_MIN,
        MAX
    };
    
    static float Change(ChangeType changeType, const float& curValue, const float& maxValue,const float &normalValue, const float &minValue = 0.1f)
    {
        switch (changeType)
        {
            case NORMAL:
                return std::max(Normal(normalValue), minValue);
            case NORMAL_MIN_NORMAL:
                return std::max(NormalMinNormal(curValue, maxValue, normalValue), minValue);
            case NORMAL_MAX_NORMAL:
                return std::max(NormalMaxNormal(curValue, maxValue, normalValue), minValue);
            case MAX_MIN_MAX:
                return std::max(MaxMinMax(curValue, maxValue, normalValue), minValue);
            case MIN_MAX_MIN:
                return std::max(MinMaxMin(curValue, maxValue, normalValue), minValue);
            case MAX:
                return std::max(Max(curValue, maxValue, normalValue), minValue);
            default:
                return minValue;
        }
    }
    
    static float Normal(const float &normalValue)
    {
        return normalValue;
    }
    
    static float NormalMinNormal(const float& curValue, const float& maxValue,const float &normalValue)
    {
        float x = curValue / maxValue;
        return normalValue * 4 * std::pow(x - 0.5, 2);
    }
    
    static float NormalMaxNormal(const float& curValue, const float& maxValue,const float &normalValue)
    {
        float x = curValue / maxValue;
        return normalValue * ((-4) * std::pow(x-0.5, 2) + 2);
    }
    
    static float MaxMinMax(const float& curValue, const float& maxValue,const float &normalValue)
    {
        float x = curValue / maxValue;
        return normalValue * 8 * std::pow(x - 0.5, 2);
    }
    
    static float MinMaxMin(const float& curValue, const float& maxValue,const float &normalValue)
    {
        float x = curValue / maxValue;
        return normalValue * ((-8) * pow(x - 0.5, 2) + 2);
    }
    
    static float Max(const float& curValue, const float& maxValue,const float &normalValue)
    {
        return 2 * normalValue;
    }
};


#endif /* SpeedChange_hpp */
