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

std::shared_ptr<GradientRange> GradientRange::CreateByOneGradient(GradientPtr gradient)
{
    std::shared_ptr<GradientRange> ret = std::make_shared<GradientRange>();
    
    ret->_gradient = gradient;
    ret->_mode = ColorMode::GRADIENT;
    
    return ret;
}

std::shared_ptr<GradientRange> GradientRange::CreateByRandomColor()
{
    std::shared_ptr<GradientRange> ret = std::make_shared<GradientRange>();
    
    ret->_mode = ColorMode::RANDOM_COLOR;
    
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

int GradientRange::EvaluateHeight()
{
    switch (_mode) {
        case ColorMode::TWO_COLORS:
            return 2;
        case ColorMode::TWO_GRADIENTS:
            return 2;
        default:
            return 1;
    }
}

bool GradientRange::IsTwoValues()
{
    return _mode == ColorMode::TWO_COLORS || _mode == ColorMode::TWO_GRADIENTS;
}

Texture2DPtr GradientRange::PackGradientRange(int samples, GradientRangePtr gr)
{
    int height = gr->EvaluateHeight();
    int len = samples * height * 4;
    std::vector<float> newData(len, 0.0f);
    
    float interval = 1.0f / (samples - 1);
    int offset = 0;
    
    for (int h = 0 ; h < height ; h++)
    {
        for (int i = 0 ; i < samples ; i++)
        {
            vec4 color = gr->Evaluate(i * interval, 0);
            newData[offset++] = color.r;
            newData[offset++] = color.g;
            newData[offset++] = color.b;
            newData[offset++] = color.a;
        }
    }
    
    // 生成纹理
    Texture2DPtr newTex = std::make_shared<Texture2D>(std::move(newData), samples, height, GL_RGBA, GL_RGBA);
    return newTex;
}
