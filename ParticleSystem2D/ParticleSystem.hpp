//
//  ParticleSystem.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/25.
//

#pragma once

#include "Head.h"

#include "Particle.hpp"
#include "../Function/Texture2D.hpp"

struct GravityModeAttribute
{
    glm::vec2 Gravity;
    
    float Speed;
    float SpeedVar;
    
    float RadialAccel;
    float RadialAccelVar;
    
    float TangentialAccel;
    float TangentialAccelVal;
    
    bool RotationIsDir; // MARK: to be reviewed 暂不用
};

struct RotationModeAttribute
{
    float StartRadius;
    float StartRadiusVar;
    
    float EndRadius;
    float EndRadiusVar;
    
    float RotatePerSecond;
    float RotatePersecondVar;
    
    // 注：angle属性在ParticleSystem中定义
};

enum EmitterMode
{
    GRAVITY,
    ROTATION,
};

#pragma mark -

class ParticleSystem
{
public:
    ParticleSystem();
    ~ParticleSystem() = default;
    
public:
    void Init();
    
    void AddParticles(int count);
    
    void Update(float dt);
    
    void StopSystem();
    
    void CopyParticle(int p1, int p2); // 把p2换到p1
    
    virtual void UpdateParticleQuads() = 0;
    virtual void Draw() = 0;
    
#pragma mark ParticleSystem
protected:
    float _elapsed; // 粒子系统累积时间
    Texture2DPtr _texture;
    
    // TODO: 设置一个变量表示是否启用预乘，目前不启用

#pragma mark EmitterAttribute
protected:
    float _isActive;
    float _isPaused;
    
    float _emissionRate;
    EmitterMode _emitterMode;
    
    float _timePerEmit; // 每次update的发射时间
    float _duration; // 发射器持续运行时间

#pragma mark ParticleAttribute
protected:
    // pos
    glm::vec2 _sourcePosition; // TODO: when to init?
    glm::vec2 _positionVar;
    
    // size
    float _startSize;
    float _startSizeVar;
    float _endSize;
    float _endSizeVar;
    
    // color
    glm::vec4 _startColor;
    glm::vec4 _startColorVar;
    glm::vec4 _endColor;
    glm::vec4 _endColorVar;
    
    // rotate 粒子自旋的角度degree
    float _startSpin;
    float _startSpinVar;
    float _endSpin;
    float _endSpinVar;
    
    // angle 粒子发射时的角度degree
    float _angle;
    float _angleVar;
    
    // life
    float _lifeTime;
    float _lifeTimeVar;
    
    // move mode
    GravityModeAttribute _gravityAttribute;
    RotationModeAttribute _rotationAttribute;
    
#pragma mark ParticlePool
protected:
    std::vector<Particle> _particles;
    int _particleCount;
    int _maxParticleCount;
};

// 注: attribute + variance -> [attri - var, attrib + var]

using ParticleSystemPtr = std::shared_ptr<ParticleSystem>;
