//
//  CurveRange.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "../Head.h"
#include "Curve.hpp"
#include "../Function/Texture2D.hpp"

enum RangeMode
{
    Constant = 0,
    Curve = 1,
    TwoCurves = 2,
    TwoConstants = 3,
};

class PS3ParticleSystem;

class CurveRange
{
    friend class PS3ParticleSystem;
    using CurveRangePtr = std::shared_ptr<CurveRange>;
public:
    CurveRange();
    ~CurveRange() = default;
    
public:
    static std::shared_ptr<CurveRange> CreateCurveByConstant(float constant);
    static std::shared_ptr<CurveRange> CreateCurveByOneCurve(CurvePtr curve);
    static std::shared_ptr<CurveRange> CreateCurveByTwoConstant(float constantMin, float constantMax);
    
public:
    float Evaluate(float time, float rndRatio);
    int EvaluateHeight();
    float EvaluateCurve(float time, int index);
    
public:
    bool IsTwoValues();
    
public: // 生成纹理
    static Texture2DPtr PackCurveRangeXYZ(int samples, CurveRangePtr x, CurveRangePtr y, CurveRangePtr z, bool discrete = false);
    static Texture2DPtr PackCurveRangeXY(int samples, CurveRangePtr x, CurveRangePtr y, bool discrete = false);
    static Texture2DPtr PackCurveRangeN(int samples, CurveRangePtr cr, bool discrete = false);
    static Texture2DPtr PackCurveRangeZ(int samples, CurveRangePtr cr, bool discrete = false);
    
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
