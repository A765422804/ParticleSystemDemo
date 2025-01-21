//
//  ParticleSystem.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "PS3Common.h"
#include "../Tool/GradientRange.hpp"
#include "../Tool/CurveRange.hpp"
#include "../Common/Node.hpp"
#include "PS3Burst.hpp"
#include "Renderer/PS3ParticleSystemRendererBase.hpp"
#include "Emitter/PS3ShapeModule.hpp"
#include "Animator/PS3TextureAnimationModule.hpp"

class PS3ParticleSystem : public Node
{
public:
    PS3ParticleSystem();
    ~PS3ParticleSystem();
    
public:
    void ToEmit(float dt); // 调用EmitParticles
    void EmitParticles(int emitNum, float dt);
    
public:
    void Update(float dt);
    void Render(); // 在原版里是beforeRender
    
public:
    void PrewarmSystem(); // 系统预热
    
public:
    int GetParticleCount();
    
public: // 属性 - 静
    int _capacity;
    PS3ParticleSystemRendererBasePtr _processor;
    
    CurveRangePtr _startDelay;
    float _duration; // 粒子系统运行时间
    bool _loop; // 是否循环运行
    float _simulationSpeed; // 粒子系统更新速度
    
    CurveRangePtr _rateOverTime; // 随时间的发射率
    CurveRangePtr _rateOverDistance; // 随距离的发射率
    
    CurveRangePtr _startSpeed; // 起始速度
    
    std::vector<std::shared_ptr<PS3Burst>> _bursts;
    
    SpaceMode _spaceMode; // 坐标系
    
    // rotation
    bool _startRotation3D; // 是否要修改粒子在三轴上的旋转
    CurveRangePtr _startRotationX;
    CurveRangePtr _startRotationY;
    CurveRangePtr _startRotationZ;
    
    // size
    bool _startSize3D;
    CurveRangePtr _startSizeX;
    CurveRangePtr _startSizeY;
    CurveRangePtr _startSizeZ;
    
    // color
    GradientRangePtr _startColor;
    
    // lifeTime
    CurveRangePtr _startLifeTime;
    
    // Modules
    PS3ShapeModulePtr _shapeModule; // 发射器模块
    PS3TextureAnimationModulePtr _textureAnimationModule; // 纹理动画模块
    
public: // 状态 - 动
    vec3 _oldWorldPos;
    vec3 _curWorldPos;
    
    float _time; // 实际运行时间
    
    bool _isEmitting; // 是否正在发射
    
    int _emitRateTimeCounter; // 随时间的发射数量
    int _emitRateDistanceCounter; // 随距离的发射数量
};

using PS3ParticleSystemPtr = std::shared_ptr<PS3ParticleSystem>;
