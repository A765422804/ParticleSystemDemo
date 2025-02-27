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
#include "../../Renderer/Renderer.hpp"
#include "../../Common/Node.hpp"

class PS3ParticleSystem;

class PS3ShapeModule
{
public:
    PS3ShapeModule();
    virtual ~PS3ShapeModule() = default;
    
public:
    //void OnInit(std::shared_ptr<PS3ParticleSystem> ps); // TODO: 把发射形状作用到粒子系统
    virtual void Emit(PS3ParticlePtr particle) = 0;
    
public: // 各种形状的发射
    
public:
//    void ApplyBoxThickness(std::vector<float>& pos);
    
public: // 我自己的函数
    virtual void RenderEmitter() = 0;
    
public:
    EmitLocation _emitLocation; // 发射位置
    PS3ParticleSystem* _ps;
    
    bool _enable; // 是否启用
//    float _randomPositionAmount;

//    
public:
    float _lastTime; // 上一帧的时间
    
public:
    RendererPtr _emitterRenderer;
};

using PS3ShapeModulePtr = std::shared_ptr<PS3ShapeModule>;
