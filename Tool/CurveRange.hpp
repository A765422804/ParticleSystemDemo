//
//  CurveRange.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "../Head.h"
#include "Curve.hpp"

enum RangeMode
{
    Constant = 0,
    Curve = 1,
    TwoCurves = 2,
    TwoConstants = 3,
};

class CurveRange
{
public:
    CurveRange();
    ~CurveRange() = default;
    
public:
    static std::shared_ptr<CurveRange> CreateCurveByConstant(float constant);
    
public:
    float Evaluate(float time, float rndRatio);
    
private:
    // Constant
    float _constant;
    
    // Curve
    CurvePtr _spline;
    
    // Two Curve
    CurvePtr _splineMin;
    CurvePtr _splineMax;
    
    // TwoConstants
    float _constantMin;
    float _constantMax;
    
    // mode
    RangeMode _mode;
    float _multiplier = 1; // 倍率系数
};

using CurveRangePtr = std::shared_ptr<CurveRange>;
