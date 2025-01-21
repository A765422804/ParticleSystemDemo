//
//  CurveRange.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "CurveRange.hpp"

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
