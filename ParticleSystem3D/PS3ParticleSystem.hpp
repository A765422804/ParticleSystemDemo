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
#include "Renderer/PS3RendererCPU.hpp"
#include "Emitter/PS3ShapeModule.hpp"
#include "Animator/PS3TextureAnimationModule.hpp"
#include "Animator/PS3VelocityOvertime.hpp"
#include "Animator/PS3ForceOvertime.hpp"
#include "Animator/PS3SizeOvertime.hpp"
#include "Animator/PS3ColorOvertime.hpp"
#include "Animator/PS3RotationOvertime.hpp"
#include "../Function/Texture2D.hpp"



class PS3ParticleSystem : public Node
{
public:
    struct SubEmitterConfig // 子发射器的配置
    {
        EventType TriggerType;
        std::shared_ptr<PS3ParticleSystem> TargetEmitter;
        
        // TODO: 继承父粒子系统的哪些属性
    };
    
public:
    PS3ParticleSystem(int maxParticleCount);
    ~PS3ParticleSystem() = default;
    
public:
    void ToEmit(float dt); // 调用EmitParticles
    void EmitParticles(int emitNum, float dt);
    
public:
    void Update(float dt);
    void Render(); // 在原版里是beforeRender
    
public:
    void PrewarmSystem(); // 系统预热
    void Stop();
    void Clear(); // 将所有粒子清除
    void Reset(); // 重置
    
public:
    int GetParticleCount();

public: // 子粒子系统
    void NotifySubEmitters(PS3ParticlePtr p, EventType event);
    void EmitSubParticles(PS3ParticlePtr p, std::shared_ptr<PS3ParticleSystem> ps);
    
    void AddSubEmitter(SubEmitterConfig config)
    {
        config.TargetEmitter->_mainEmitter = this;
        _subEmitters.push_back(config);
    }
    
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
    
    // gravity
    CurveRangePtr _gravity;
    
    // lifeTime
    CurveRangePtr _startLifeTime;
    
    // Modules
    PS3ShapeModulePtr _shapeModule; // 发射器模块
    PS3TextureAnimationModulePtr _textureAnimationModule; // 纹理动画模块
    PS3VelocityOvertimePtr _velocityOvertimeModule; // 速度变化模块
    PS3ForceOvertimePtr _forceOvertimeModule; // 加速度变化模块
    PS3SizeOvertimePtr _sizeOvertimeModule; // 大小变化模块
    PS3ColorOvertimePtr _colorOvertimeModule; // 颜色变化模块
    PS3RotationOvertimePtr _rotationOvertimeModule; // 随时间的旋转模块
    
    // 我自定的属性，更好的做法是用material
    Texture2DPtr _texture;
    
    // 子发射器
    std::vector<SubEmitterConfig> _subEmitters;
    bool _isSubEmitter;
    PS3ParticleSystem* _mainEmitter;
    
public: // 状态 - 动
    vec3 _oldWorldPos;
    vec3 _curWorldPos;
    
    float _time; // 实际运行时间
    
    bool _isPlaying; // 是否在运行
    bool _isPaused; // 是否暂停
    bool _isStopped; // 是否停止播放
    bool _isEmitting; // 是否正在发射
    bool _needRefresh; // 需要刷新
    
    float _emitRateTimeCounter; // 随时间的发射数量
    float _emitRateDistanceCounter; // 随距离的发射数量
};

using PS3ParticleSystemPtr = std::shared_ptr<PS3ParticleSystem>;
