//
//  GradientRange.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#include "GradientRange.hpp"

GradientRange::GradientRange()
{
    // TODO: do nothing 暂时
}

std::shared_ptr<GradientRange> GradientRange::CreateByOneColor(vec4 color)
{
    std::shared_ptr<GradientRange> ret = std::make_shared<GradientRange>();
    
    ret->_color = color;
    ret->_mode = ColorMode::COLOR;
    
    return ret;
}

vec4 GradientRange::Evaluate(float time, float rndRatio)
{
    switch (_mode) {
        case ColorMode::COLOR:
            return _color;
        case ColorMode::TWO_COLORS:
            return LinearInterpolate(_colorMin, _colorMax, rndRatio);
        case ColorMode::RANDOM_COLOR:
            return vec4(Random01(), Random01(), Random01(), Random01());
        case ColorMode::GRADIENT:
            return _gradient->Evaluate(time);
        case ColorMode::TWO_GRADIENTS:
            return LinearInterpolate(_gradientMin->Evaluate(time), _gradientMax->Evaluate(time), rndRatio);
        default:
            AssertShouldNotReachHere();
    }
}
