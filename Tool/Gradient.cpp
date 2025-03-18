//
//  Gradient.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/8.
//

#include "Gradient.hpp"

Gradient::Gradient()
{
    
}

std::shared_ptr<Gradient> Gradient::CreateByColorKeyAndAlphaKey(std::vector<ColorKey> colorKeys, std::vector<AlphaKey> alphaKeys, Mode mode)
{
    auto ret = std::make_shared<Gradient>();
    
    ret->_colorKeys = colorKeys;
    ret->_alphaKeys = alphaKeys;
    ret->_mode = mode;
    
    return ret;
}

void Gradient::SortKeys()
{
    std::sort(_colorKeys.begin(), _colorKeys.end(),
              [](const ColorKey& a, const ColorKey& b) -> bool {
                  return a.Time < b.Time;
              });

    std::sort(_alphaKeys.begin(), _alphaKeys.end(),
              [](const AlphaKey& a, const AlphaKey& b) -> bool {
                  return a.Time < b.Time;
              });
}

vec4 Gradient::Evaluate(float time)
{
    vec4 res = vec4(GetRGB(time), GetAlpha(time));
    return res;
}

vec3 Gradient::GetRGB(float time)
{
    int length = int(_colorKeys.size());
    if (length > 1)
    {
        time = Repeat(time, 1.0f + 1e-6);
        for (int i = 1; i < length; ++i)
        {
            float preTime = _colorKeys[i - 1].Time;
            float curTime = _colorKeys[i].Time;
            if (time >= preTime && time <= curTime)
            {
                if (_mode == Mode::FIXED)
                {
                    return _colorKeys[i].Color;
                }
                else
                {
                    float factor = (time - preTime) / (curTime - preTime);
                    return LinearInterpolate(_colorKeys[i - 1].Color, _colorKeys[i].Color, factor);
                }
            }
        }
        int lastIndex = length - 1;
        if (time < _colorKeys[0].Time)
            return LinearInterpolate(vec3(0, 0, 0), _colorKeys[0].Color, time / _colorKeys[0].Time);
        else if (time > _colorKeys[lastIndex].Time)
            return LinearInterpolate(_colorKeys[lastIndex].Color, vec3(0, 0, 0), (time - _colorKeys[lastIndex].Time) / (1 - _colorKeys[lastIndex].Time));
        else
            AssertShouldNotReachHere();
        
    }
    else if (length == 1)
        return _colorKeys[0].Color;
    else
        return vec3(1, 1, 1);
}

float Gradient::GetAlpha(float time)
{
    int length = int(_alphaKeys.size());
    if (length > 1)
    {
        time = Repeat(time, 1);
        for (int i = 1 ; i < length; ++i)
        {
            float preTime = _alphaKeys[i - 1].Time;
            float curTime = _alphaKeys[i].Time;
            if (time >= preTime && time <= curTime)
            {
                if (_mode == Mode::FIXED)
                {
                    return _alphaKeys[i].Alpha;
                }
                else
                {
                    float factor = (time - preTime) / (curTime - preTime);
                    return LinearInterpolate(_alphaKeys[i - 1].Alpha, _alphaKeys[i].Alpha, factor);
                }
            }
        }
        int lastIndex = length - 1;
        if (time < _alphaKeys[0].Time)
            return LinearInterpolate(0.0f, _alphaKeys[0].Alpha, time / _alphaKeys[0].Time);
        else if (time > _alphaKeys[lastIndex].Time)
            return LinearInterpolate(_alphaKeys[lastIndex].Alpha, 0.0f, (time - _alphaKeys[lastIndex].Time) / (1 - _alphaKeys[lastIndex].Time));
        else
            AssertShouldNotReachHere();
    }
    else if (length == 1)
        return _alphaKeys[0].Alpha;
    else
        return 1;
}
