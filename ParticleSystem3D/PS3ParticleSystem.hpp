//
//  ParticleSystem.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#pragma once

#include "PS3Common.h"
#include "../Tool/GradientRange.hpp" // 可以删了
#include "../Tool/CurveRange.hpp" // 可以删了
#include "../Common/Node.hpp"
#include "PS3Burst.hpp" // 可以删了
#include "Renderer/PS3ParticleSystemRendererBase.hpp"
#include "Renderer/PS3RendererCPU.hpp"
#include "Renderer/PS3RendererGPU.hpp"
#include "Emitter/PS3ShapeModule.hpp"
#include "Animator/PS3TextureAnimationModule.hpp"
#include "Animator/PS3VelocityOvertime.hpp"
#include "Animator/PS3ForceOvertime.hpp"
#include "Animator/PS3SizeOvertime.hpp"
#include "Animator/PS3ColorOvertime.hpp"
#include "Animator/PS3RotationOvertime.hpp"
#include "../Function/Texture2D.hpp"
#include "Renderer/PS3Trail.hpp"
#include "Emitter/PS3BoxEmitter.hpp"
#include "Emitter/PS3CircleEmitter.hpp"
#include "Emitter/PS3ConeEmitter.hpp"
#include "Emitter/PS3SphereEmitter.hpp"
#include "Emitter/PS3HemisphereEmitter.hpp"
#include "Processor/PS3ParticleProcessorBase.hpp"
#include "Processor/PS3ParticleProcessorCPU.hpp"
#include "Processor/PS3ParticleProcessorGPU.hpp"
#include "Initializer/PS3ParticleInitializer.hpp"
#include "PS3ParticleGenerator.hpp"

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
    //void EmitParticles(int emitNum, float dt);
    virtual void InitializeParticles(std::vector<PS3ParticlePtr> &particles); // 初始化粒子
    
public:
    void Update(float dt);
    virtual void Render(); // 在原版里是beforeRender
    
public: // updateParticle
    virtual void UpdateParticles(float dt) = 0;
    // bool UpdateParticle(PS3ParticlePtr p, float dt); // CPU特有
    
public:
    void PrewarmSystem(); // 系统预热
    void Play();
    void Pause();
    void Stop();
    virtual void Clear() = 0; // 将所有粒子清除
    void Reset(); // 重置粒子系统运行时的累积数据
    void ResetPosition(); // 重置position，将oldPosition和newPostion都重置为当前粒子系统的位置
    void Restart(); // 重新开始，MARK: 我自己添加的函数
    
public:
    virtual int GetParticleCount() = 0;

public: // 子粒子系统
    // 在子类中分为CPU和GPU两种实现
//    void NotifySubEmitters(PS3ParticlePtr p, EventType event);
//    void EmitSubParticles(PS3ParticlePtr p, std::shared_ptr<PS3ParticleSystem> ps);
//    void NotifySubEmitters(vec3 position, EventType event);
//    void EmitSubParticles(vec3 position, std::shared_ptr<PS3ParticleSystem> ps);
    
    void AddSubEmitter(SubEmitterConfig config)
    {
        config.TargetEmitter->_mainEmitter = this;
        _subEmitters.push_back(config);
    }
    
public: // trail module
    // CPU特有
    //void SetTrailModule(PS3TrailPtr trail);
    
public: // 属性 - 静
    // TODO: 对于一些初始属性，思考如何添加随机性
    
    // 粒子池
    // std::vector<PS3ParticlePtr> _particles; // CPU 特有
    
    int _capacity;
    // PS3ParticleSystemRendererBasePtr _renderer; // 分开来写
    //PS3ParticleProcessorBasePtr _processor;
    
    CurveRangePtr _startDelay;
    float _duration; // 粒子系统运行时间
    bool _loop; // 是否循环运行
    float _simulationSpeed; // 粒子系统更新速度
    
    PS3ParticleGeneratorPtr _generator; // 粒子生成器
    
//    CurveRangePtr _rateOverTime; // 随时间的发射率
//    CurveRangePtr _rateOverDistance; // 随距离的发射率
    
//    CurveRangePtr _startSpeed; // 起始速度
    
    //std::vector<std::shared_ptr<PS3Burst>> _bursts;
    
    SpaceMode _spaceMode; // 坐标系
    
//    // rotation
//    bool _startRotation3D; // 是否要修改粒子在三轴上的旋转
//    CurveRangePtr _startRotationX;
//    CurveRangePtr _startRotationY;
//    CurveRangePtr _startRotationZ;
    
//    // size
//    bool _startSize3D;
//    CurveRangePtr _startSizeX;
//    CurveRangePtr _startSizeY;
//    CurveRangePtr _startSizeZ;
    
//    // color
//    GradientRangePtr _startColor;
    
    // gravity
    CurveRangePtr _gravity;
    
//    // lifeTime
//    CurveRangePtr _startLifeTime;
    
    // Modules
    PS3ShapeModulePtr _shapeModule; // 发射器模块
    
    // overtime module
    std::unordered_map<std::string, PS3OvertimeModulePtr> _overtimeModules;
    
    // initializer module
    PS3ParticleInitializerPtr _inilizer;
    
    // trail 模块
    // PS3TrailPtr _trailModule; // CPU 特有
    
    // 我自定的属性，更好的做法是用material
    Texture2DPtr _texture;
    
    // 子发射器
    std::vector<SubEmitterConfig> _subEmitters;
    bool _isSubEmitter;
    PS3ParticleSystem* _mainEmitter;
    
    // 是否需要预热
    bool _prewarm;
    
    // 是否使用GPU粒子
    
public: // 状态 - 动
    vec3 _oldWorldPos;
    vec3 _curWorldPos;
    
    float _time; // 实际运行时间
    
    bool _isPlaying; // 是否在运行
    bool _isPaused; // 是否暂停
    bool _isStopped; // 是否停止播放
    bool _isEmitting; // 是否正在发射
    bool _needRefresh; // 需要刷新
    bool _needToRestart; // 是否需要重新开始
    
//    float _emitRateTimeCounter; // 随时间的发射数量
//    float _emitRateDistanceCounter; // 随距离的发射数量
    
    // 当前帧时间
    float _dt;
};

using PS3ParticleSystemPtr = std::shared_ptr<PS3ParticleSystem>;
