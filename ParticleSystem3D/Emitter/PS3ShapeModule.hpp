//
//  PS3ShapeModule.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#pragma once

#include "../PS3Common.h"
#include "../PS3Particle.hpp"
#include "../../Tool/CurveRange.hpp"

class PS3ParticleSystem;

class PS3ShapeModule
{
public:
    void OnInit(std::shared_ptr<PS3ParticleSystem> ps); // TODO: 把发射形状作用到粒子系统
    void Emit(PS3ParticlePtr particle);
    
public: // 各种形状的发射
    void BoxEmit(vec3& pos, vec3& dir);
    void CircleEmit(vec3& pos, vec3& dir);
    void ConeEmit(vec3& pos, vec3& dir);
    void SphereEmit(vec3& pos, vec3& dir);
    void HemisphereEmit(vec3& pos, vec3& dir);
    
public:
    void ApplyBoxThickness(std::vector<float>& pos);
    
    float GenerateArcAngle(); // 计算当前时刻circle、cone的发射弧度

public:
    ShapeType _shapeType; // 发射器类型
    EmitLocation _emitLocation; // 发射位置
    PS3ParticleSystem* _ps;
    
    bool _enable; // 是否启用
    float _randomPositionAmount;
    
    vec3 _boxThickness; // box盒子三维上的厚度
    
    ArcMode _arcMode; // 圆曲线模式
    float _arcSpread; // 扩散步长，保证角度是步长的倍数
    float _arc; // TODO: 好像表示最大角度
    CurveRangePtr _arcSpeed; // 弧线变化速度
    float _radius; // 半径
    float _radiusThickness; // 半径厚度
    
    float _angle; // 轴与母线的夹角
    float _length; // 顶部截面到底面的长度
    
public:
    float _lastTime;
    float _totalAngle; // 当前角度
};

using PS3ShapeModulePtr = std::shared_ptr<PS3ShapeModule>;
