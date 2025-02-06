//
//  ParticleSystem.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "PS3ParticleSystem.hpp"

PS3ParticleSystem::PS3ParticleSystem(int maxParticleCount)
: _isEmitting(true)
, _isPlaying(true)
, _capacity(maxParticleCount)
, _loop(true)
, _duration(6.0f)
, _simulationSpeed(1)
, _startLifeTime(nullptr)
, _startColor(nullptr)
, _startDelay(nullptr)
, _startSpeed(nullptr)
, _startSizeX(nullptr)
, _startRotationZ(nullptr)
, _rateOverTime(nullptr)
, _rateOverDistance(nullptr)
, _processor(nullptr)
, _shapeModule(nullptr)
{
    _startLifeTime = CurveRange::CreateCurveByConstant(5);
    _startColor = GradientRange::CreateByOneColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    _startDelay = CurveRange::CreateCurveByConstant(0);
    _startSizeX = CurveRange::CreateCurveByConstant(0.1);
    _startRotationZ = CurveRange::CreateCurveByConstant(0);
    _rateOverTime = CurveRange::CreateCurveByConstant(10);
    _rateOverDistance = CurveRange::CreateCurveByConstant(0);
    _startSpeed = CurveRange::CreateCurveByConstant(1);
    
    _processor = std::make_shared<PS3RendererCPU>(maxParticleCount);
    _processor->_particleSystem = this;

//    _shapeModule = PS3ShapeModule::CreateBoxEmitter(EmitLocation::VOLUME, vec3(0.0f), this);
    _shapeModule = PS3ShapeModule::CreateConeEmitter(EmitLocation::BASE, ArcMode::RANDOM, 0, 360, 0, 2, 0, 30, 5, this);
    
    // burst
//    auto burstCount = CurveRange::CreateCurveByConstant(100);
//    auto burst = std::make_shared<PS3Burst>(3, 2, 1, burstCount);
//    _bursts.push_back(burst);
}

void PS3ParticleSystem::ToEmit(float dt) // 调用EmitParticles
{
    float startDelay = _startDelay->Evaluate(0, 1);
    if (_time > startDelay) // 超过了延迟时间
    {
        // 超过了总时间
        if (_time > (_duration + startDelay))
            if (!_loop)
                _isEmitting = false;
        
        if (!_isEmitting)
            return; // 不发射直接返回
        
        // emit by rateOverTime 基于时间的发射
        _emitRateTimeCounter += _rateOverTime->Evaluate(_time / _duration, 1) * dt;
        if (_emitRateTimeCounter > 1)
        {
            int emitNum = floor(_emitRateTimeCounter);
            _emitRateTimeCounter -= emitNum;
            EmitParticles(emitNum, dt);
        }
        
        // emit by rateOverDistance 基于距离的发射
        float curRateOverDistance = _rateOverDistance->Evaluate(_time / _duration, 1);
        if (curRateOverDistance > 0)
        {
            _oldWorldPos = _curWorldPos;
            _curWorldPos = GetWorldPosition();
            float distance = glm::distance(_curWorldPos, _oldWorldPos);
            _emitRateDistanceCounter += curRateOverDistance * distance;
        }
        if (_emitRateDistanceCounter > 1)
        {
            int emitNum = floor(_emitRateDistanceCounter);
            _emitRateDistanceCounter -= emitNum;
            EmitParticles(emitNum, dt);
        }
        
        // burst
        for (auto burst : _bursts)
            burst->Update(this, dt);
    }
}

void PS3ParticleSystem::EmitParticles(int emitNum, float dt)
{
    float loopDelta = Repeat(_time, _duration) / _duration; // 当前时间在一个周期内的占比
    
    // TODO: 这里有一个_needRefresh
    
    // 坐标变换
    if (_spaceMode == SpaceMode::WORLD)
    {
        // TODO: 得到世界坐标下的矩阵和旋转
    }
    
    // 发射粒子
    for (int i = 0; i < emitNum ; ++i)
    {
        PS3ParticlePtr particle = _processor->GetFreeParticle();
        if (particle == nullptr)
            return;
        particle->_particleSystem = this;
        particle->Reset();
        
        if (_shapeModule != nullptr && _shapeModule->_enable) // 使用发射器进行发射
        {
            _shapeModule->Emit(particle);
        }
        else // 不使用发射器
        {
            particle->_position = vec3(0.0, 0.0, 0.0);
            particle->_velocity = vec3(0.0, 0.0, 0.0);
        }
        
        // TODO: textureAnimationModule
        
        // velocity
        float curStartSpeed = _startSpeed->Evaluate(loopDelta, Random01());
        particle->_velocity = particle->_velocity * curStartSpeed;
        
        if (_spaceMode == SpaceMode::WORLD)
        {
            // TODO: 根据坐标系更正位置和速度
        }
        particle->_ultimateVelocity = particle->_velocity;
        
        // 应用起始rotation
        if (_startRotation3D)
        {
            particle->_startEuler = vec3(_startRotationX->Evaluate(loopDelta, Random01()), _startRotationY->Evaluate(loopDelta, Random01()), _startRotationZ->Evaluate(loopDelta, Random01()));
        }
        else
        {
            particle->_startEuler = vec3(0, 0, _startRotationZ->Evaluate(loopDelta, Random01()));
        }
        particle->_rotation = particle->_startEuler;
        
        // 应用起始size
        if (_startSize3D)
        {
            particle->_startSize = vec3(_startSizeX->Evaluate(loopDelta, Random01()), _startSizeY->Evaluate(loopDelta, Random01()), _startSizeZ->Evaluate(loopDelta, Random01()));
        }
        else
        {
            particle->_startSize = vec3(_startSizeX->Evaluate(loopDelta, RandomM11()));
        }
        particle->_size = particle->_startSize;
        
        // 应用起始color
        particle->_startColor = _startColor->Evaluate(loopDelta, Random01());
        particle->_color = particle->_startColor;
        
        // 应用起始lifeTime
        particle->_startLifeTime = _startLifeTime->Evaluate(loopDelta, Random01()) + dt;
        particle->_remainingLifeTime = particle->_startLifeTime;
        
        // TODO: 设置particle的随机数种子和loopCount，但是我不知道意义何在
        
        _processor->SetNewParticle(particle);
    }
}

void PS3ParticleSystem::Update(float dt)
{
    float scaledDeltaTime = dt * _simulationSpeed;
    
    if (_isPlaying)
    {
        _time += scaledDeltaTime;
        
        // 发射粒子
        ToEmit(scaledDeltaTime);
        
        // 更新粒子
        if (_processor->UpdateParticles(scaledDeltaTime) == 0 && ! _isEmitting)
        {
            // 停止播放粒子
            Stop();
        }
    }
    else
    {
        // TODO: 调用processor同样更新rotation和scale
    }
    
    // TODO: attach model to scene
    
    // TODO: GPU和Trail的相关逻辑
}

void PS3ParticleSystem::Stop()
{
    if (this->_isPlaying || this->_isPaused) {
        this->Clear();
    }
    if (this->_isPlaying) {
        this->_isPlaying = false;
    }
    if (this->_isPaused) {
        this->_isPaused = false;
    }
    if (this->_isEmitting) {
        this->_isEmitting = false;
    }

    this->_isStopped = true;

    // if stop emit modify the refresh flag to true
    this->_needRefresh = true;

    this->Reset();
}

void PS3ParticleSystem::Clear()
{
    _processor->Clear();
    // TODO: clear拖尾的部分
}

void PS3ParticleSystem::Reset()
{
    _time = 0.0f;
    _emitRateTimeCounter = 0.0f;
    _emitRateDistanceCounter = 0.0f;
    // TODO: 重置Postion
    
    for (auto& burst : _bursts)
        burst->Reset();
}

void PS3ParticleSystem::PrewarmSystem()
{
    // TODO: impl
}

void PS3ParticleSystem::Render()
{
    _processor->UpdateRenderData();
    _processor->Render();
    
    // 渲染发射器的线框
    if (_shapeModule)
        _shapeModule->RenderEmitter();
}

int PS3ParticleSystem::GetParticleCount()
{
    if (_processor)
        return _processor->GetParticleCount();
    else
        return 0;
}


