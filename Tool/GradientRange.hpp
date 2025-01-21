//
//  GradientRange.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#pragma once

#include "../Head.h"
#include "Gradient.hpp"

enum class ColorMode
{
    COLOR, // 单色
    GRADIENT, // 梯度颜色
    TWO_COLORS, // 双色
    TWO_GRADIENTS, // 双梯度颜色
    RANDOM_COLOR, // 随机颜色
};

class GradientRange
{
public:
    GradientRange();
    ~GradientRange() = default;
    
public:
    vec4 Evaluate(float time,float rndRatio);
    
private:
    ColorMode _mode;
    
    // color
    vec4 _color;
    
    // two color
    vec4 _colorMin;
    vec4 _colorMax;
    
    // gradient
    GradientPtr _gradient;
    
    // two gradient
    GradientPtr _gradientMin;
    GradientPtr _gradientMax;
    
    
};

using GradientRangePtr = std::shared_ptr<GradientRange>;
