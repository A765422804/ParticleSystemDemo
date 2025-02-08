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
