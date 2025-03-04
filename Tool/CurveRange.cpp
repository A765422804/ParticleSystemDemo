//
//  CurveRange.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "CurveRange.hpp"

CurveRange::CurveRange()
{
    // TODO: do nothing 暂时
}

std::shared_ptr<CurveRange> CurveRange::CreateCurveByConstant(float constant)
{
    std::shared_ptr<CurveRange> ret = std::make_shared<CurveRange>();
    
    ret->_constant = constant;
    ret->_mode = RangeMode::Constant;
    
    return ret;
}

std::shared_ptr<CurveRange> CurveRange::CreateCurveByOneCurve(CurvePtr curve)
{
    std::shared_ptr<CurveRange> ret = std::make_shared<CurveRange>();
    
    ret->_spline = curve;
    ret->_mode = RangeMode::Curve;
    
    return ret;
}

float CurveRange::Evaluate(float time, float rndRatio)
{
    switch (_mode) {
        case RangeMode::Constant:
            return _constant;
        case RangeMode::Curve:
            return _spline->Evaluate(time) * _multiplier;
        case RangeMode::TwoCurves:
            return LinearInterpolate(_splineMin->Evaluate(time),_splineMax->Evaluate(time) , rndRatio) * _multiplier;
        case RangeMode::TwoConstants:
            return LinearInterpolate(_constantMin, _constantMax, rndRatio);
        default:
            AssertShouldNotReachHere();
    }
}

int CurveRange::EvaluateHeight()
{
    switch (_mode) {
        case RangeMode::TwoCurves:
            return 2;
        case RangeMode::TwoConstants:
            return 2;
        case RangeMode::Constant:
            return 1;
        case RangeMode::Curve:
            return 1;
        default:
            AssertShouldNotReachHere();
    }
}

float CurveRange::EvaluateCurve(float time, int index)
{
    switch (_mode) {
        case RangeMode::Constant:
            return _constant;
        case RangeMode::Curve:
            return _spline->Evaluate(time) * _multiplier;
        case RangeMode::TwoCurves:
            return index == 0 ? (_splineMin->Evaluate(time) * _multiplier) : (_splineMax->Evaluate(time) * _multiplier);
        case RangeMode::TwoConstants:
            return index == 0 ? _constantMin : _constantMax;
        default:
            AssertShouldNotReachHere();
    }
}

Texture2DPtr CurveRange::PackCurveRangeXYZ(int samples, CurveRangePtr x, CurveRangePtr y, CurveRangePtr z, bool discrete)
{
    int height = max(x->EvaluateHeight(), y->EvaluateHeight(), z->EvaluateHeight());
    int len = samples * height * 4;
    std::vector<float> newData(len, 0.0f);
    
    std::vector<CurveRangePtr> curves = {x, y, z};
    float interval = 1.0f / (samples - 1);
    
    for (int h = 0; h < height; ++h) // 每层
    {
        for (int i = 0 ; i < 3; ++i) //每条curve
        {
            auto cr = curves[i];
            float sum = 0.0f;
            float average = 0.0f;
            for (int j = 0 ; j < samples; ++j) // 对每条curve进行j次采样
            {
                float value = cr->EvaluateCurve(interval * j, h);
                if (discrete)
                {
                    average = value;
                }
                else
                {
                    sum += value;
                    average = sum / (j + 1);
                }
                int index = (h * samples + j) * 4 + i;
                newData[index] = average;
                //newData[index] = sum;
            }
        }
    }
    
    //  生成纹理
    Texture2DPtr newTex = std::make_shared<Texture2D>(std::move(newData), samples, height, GL_RGBA, GL_RGBA);
    
    return newTex;
}

Texture2DPtr CurveRange::PackCurveRangeXY(int samples, CurveRangePtr x, CurveRangePtr y, bool discrete)
{
    int height = max(x->EvaluateHeight(), y->EvaluateHeight());
    int len = samples * height * 4;
    std::vector<float> newData(len, 0.0f);
    
    std::vector<CurveRangePtr> curves = {x, y};
    float interval = 1.0f / (samples - 1);
    
    for (int h = 0; h < height; ++h) // 每层
    {
        for (int i = 0 ; i < 2; ++i) //每条curve
        {
            auto cr = curves[i];
            float sum = 0.0f;
            float average = 0.0f;
            for (int j = 0 ; j < samples; ++j) // 对每条curve进行j次采样
            {
                float value = cr->EvaluateCurve(interval * j, h);
                if (discrete)
                {
                    average = value;
                }
                else
                {
                    sum += value;
                    average = sum / (j + 1);
                }
                int index = (h * samples + j) * 4 + i;
                newData[index] = average;
            }
        }
    }
    
    // 生成纹理
    Texture2DPtr newTex = std::make_shared<Texture2D>(std::move(newData), samples, height, GL_RGBA, GL_RGBA);
    return newTex;
}

Texture2DPtr CurveRange::PackCurveRangeN(int samples, CurveRangePtr cr, bool discrete)
{
    int height = cr->EvaluateHeight();
    int len = samples * height * 4;
    std::vector<float> newData(len, 0.0f);
    
    float interval = 1.0 / (samples - 1);
    
    float sum = 0.0f;
    float average = 0.0f;
    int offset = 0;
    
    for (int h = 0; h < height; ++h)
    {
        sum = 0.0f;
        for (int j = 0 ; j < samples; ++j)
        {
            auto value = cr->Evaluate(interval * j, h);
            if (discrete)
            {
                average = value;
            }
            else
            {
                sum += value;
                average = sum / (j + 1);
            }
            
            newData[offset] = average;
            newData[offset + 1] = average;
            newData[offset + 2] = average;
            offset += 4;
        }
    }
    
    // 生成纹理
    Texture2DPtr newTex = std::make_shared<Texture2D>(std::move(newData), samples, height, GL_RGBA, GL_RGBA);
    
    return newTex;
}

Texture2DPtr CurveRange::PackCurveRangeZ(int samples, CurveRangePtr cr, bool discrete)
{
    int height = cr->EvaluateHeight();
    int len = samples * height * 4;
    std::vector<float> newData(len, 0.0f);
    
    float interval = 1.0 / (samples - 1);
    
    float sum = 0.0f;
    float average = 0.0f;
    int offset = 0;
    
    for (int h = 0; h < height; ++h)
    {
        sum = 0.0f;
        for (int j = 0 ; j < samples; ++j)
        {
            auto value = cr->Evaluate(interval * j, h);
            if (discrete)
            {
                average = value;
            }
            else
            {
                sum += value;
                average = sum / (j + 1);
            }
            int index = offset + 2; // 存在rgba中的b中
            newData[index] = average;
            offset += 4;
        }
    }
    
    //  生成纹理
    Texture2DPtr newTex = std::make_shared<Texture2D>(std::move(newData), samples, height, GL_RGBA, GL_RGBA);
    
    return newTex;
}
