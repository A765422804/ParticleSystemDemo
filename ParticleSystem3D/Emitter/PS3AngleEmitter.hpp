//
//  PS3AngleEmitter.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#pragma once
#include "../PS3Common.h"
#include "PS3ShapeModule.hpp"

class PS3AngleEmitter : public PS3ShapeModule
{
    using SuperType = PS3ShapeModule;
    
public:
    PS3AngleEmitter();
    virtual ~PS3AngleEmitter() = default;
    
public:
    float GenerateArcAngle();
    
public: // 全部角度制
    ArcMode _arcMode; // 圆曲线模式
    float _arcSpread; // 扩散步长，保证角度是步长的倍数
    float _arc; // 表示最大角度
    CurveRangePtr _arcSpeed; // 弧线变化速度
    float _radius; // 半径
    float _radiusThickness; // 半径厚度
    float _angleStep; // 角度增量
    
public:
    float _totalAngle; // 当前角度

};

using PS3AngleEmitterPtr = std::shared_ptr<PS3AngleEmitter>;
