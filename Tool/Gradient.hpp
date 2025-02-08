//
//  Gradient.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/8.
//

#pragma once

#include "../Head.h"

enum Mode
{
    BLEND, // 颜色插值
    FIXED, // 返回最近的颜色
};

struct ColorKey
{
    vec3 Color;
    float Time;
};

struct AlphaKey
{
    float Alpha;
    float Time;
};

class Gradient
{
public:
    Gradient();
    ~Gradient() = default;
    
public:
    static std::shared_ptr<Gradient> CreateByColorKeyAndAlphaKey(std::vector<ColorKey> colorKeys, std::vector<AlphaKey> alphaKeys, Mode mode = Mode::BLEND);
    
public:
    void SortKeys(); // 对关键帧进行time升序排列
    vec4 Evaluate(float time);
    
private:
    vec3 GetRGB(float time);
    float GetAlpha(float time);
    
private:
    Mode _mode;
    std::vector<ColorKey> _colorKeys;
    std::vector<AlphaKey> _alphaKeys;
};

using GradientPtr = std::shared_ptr<Gradient>;
