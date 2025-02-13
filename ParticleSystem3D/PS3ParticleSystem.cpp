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
, _isSubEmitter(false)
, _capacity(maxParticleCount)
, _loop(true)
, _duration(6.0f)
, _simulationSpeed(1)
, _startLifeTime(nullptr)
, _startColor(nullptr)
, _startDelay(nullptr)
, _startSpeed(nullptr)
, _startSize3D(false) // xyz 应用不同的size
, _startSizeX(nullptr)
, _startRotation3D(false) // xyz 应用不同的rotation
, _startRotationZ(nullptr)
, _rateOverTime(nullptr)
, _rateOverDistance(nullptr)
, _processor(nullptr)
, _shapeModule(nullptr)
, _velocityOvertimeModule(nullptr)
, _textureAnimationModule(nullptr)
, _texture(nullptr)
, _spaceMode(SpaceMode::LOCAL)
{
    // spatial info
    SetPosition3D(vec3(5, 0, 5));
    SetRotation(vec3(0,0,0));
    SetScale(vec3(1, 1, 1));
    
    // 定义一个公用的curve和curveRange for debug
    std::vector<float> time = {0.0f, 1.0f};
    KeyFrameValue keyFrame1 = {0.0f, 0.0f, 1.0f}; // (0, 0) 点，没有左切线
    KeyFrameValue keyFrame2 = {1.0f, 1.0f, 0.0f}; // (1, 1) 点，没有右切线
    std::vector<KeyFrameValue> value = {keyFrame1, keyFrame2};
    CurvePtr curve = Curve::CreateCurveByTimesAndValues(time, value);
    CurveRangePtr curveRange = CurveRange::CreateCurveByOneCurve(curve);
    
    // startSize
    _startSizeX = CurveRange::CreateCurveByConstant(0.5);
    
    // startSpeed
    _startSpeed = CurveRange::CreateCurveByConstant(1);
    
    // startRotation
    _startRotationZ = CurveRange::CreateCurveByConstant(0);
    
    // gravity
    _gravity = CurveRange::CreateCurveByConstant(0);
    
    // startColor
//    ColorKey colorKey1 = {vec3(1.0f, 0.0f, 0.0f), 0.0f};
//    ColorKey colorKey2 = {vec3(0.0f, 1.0f, 0.0f), 1.0f};
//    AlphaKey alphaKey1 = {1.0f, 0.0f};
//    AlphaKey alphaKey2 = {1.0f, 1.0f};
//    std::vector<ColorKey> colorKeys = {colorKey1, colorKey2};
//    std::vector<AlphaKey> alphaKeys = {alphaKey1, alphaKey2};
//    GradientPtr gradient = Gradient::CreateByColorKeyAndAlphaKey(colorKeys, alphaKeys);
//    GradientRangePtr gradientRange = GradientRange::CreateByOneGradient(gradient);
//    _startColor = gradientRange;
    _startColor = GradientRange::CreateByOneColor(vec4(vec3(1.0f), 0.5));
    
    _startLifeTime = CurveRange::CreateCurveByConstant(10);
    _startDelay = CurveRange::CreateCurveByConstant(0);

    _rateOverTime = CurveRange::CreateCurveByConstant(10);
    _rateOverDistance = CurveRange::CreateCurveByConstant(0);
    
    _processor = std::make_shared<PS3RendererCPU>(maxParticleCount);
    _processor->_particleSystem = this;

    _shapeModule = PS3ShapeModule::CreateBoxEmitter(EmitLocation::SHELL, vec3(0.0f), this);
//    _shapeModule = PS3ShapeModule::CreateConeEmitter(EmitLocation::VOLUME, ArcMode::RANDOM, 0, 360, 0, 0.5, 1, 30, 2, this);
//    _shapeModule = PS3ShapeModule::CreateCircleEmitter(ArcMode::RANDOM, 0, 360, 0, 1, 1, this);
    //_shapeModule = PS3ShapeModule::CreateSphereEmitter(EmitLocation::VOLUME, 1, 1, this);
    //_shapeModule = PS3ShapeModule::CreateHemisphereEmitter(EmitLocation::VOLUME, 1, 1, this);
    
    // velocity overtime
    auto xSpeed = CurveRange::CreateCurveByConstant(0.0f);
    auto ySpeed = CurveRange::CreateCurveByConstant(0.0f);
    auto zSpeed = CurveRange::CreateCurveByConstant(0.0f);
    _velocityOvertimeModule = std::make_shared<PS3VelocityOvertime>(xSpeed, ySpeed, zSpeed);
    
    // force overtime
    auto xForce = CurveRange::CreateCurveByConstant(0.0f);
    auto yForce = CurveRange::CreateCurveByConstant(0.0f);
    auto zForce = CurveRange::CreateCurveByConstant(0.0f);
    _forceOvertimeModule = std::make_shared<PS3ForceOvertime>(xForce, yForce, zForce);
    
    // size overtime
//    auto allSize = curveRange;
//    auto xSize = CurveRange::CreateCurveByConstant(0.1f);
//    auto ySize = curveRange;
//    auto zSize = CurveRange::CreateCurveByConstant(0.1f);
//    _sizeOvertimeModule = std::make_shared<PS3SizeOvertime>(curveRange);
    
    // color overtime
//        ColorKey colorKey1 = {vec3(1.0f, 0.0f, 0.0f), 0.0f};
//        ColorKey colorKey2 = {vec3(0.0f, 0.0f, 1.0f), 1.0f};
//        AlphaKey alphaKey1 = {1.0f, 0.0f};
//        AlphaKey alphaKey2 = {1.0f, 1.0f};
//        std::vector<ColorKey> colorKeys = {colorKey1, colorKey2};
//        std::vector<AlphaKey> alphaKeys = {alphaKey1, alphaKey2};
//        GradientPtr gradient = Gradient::CreateByColorKeyAndAlphaKey(colorKeys, alphaKeys);
//        GradientRangePtr gradientRange = GradientRange::CreateByOneGradient(gradient);
//    _colorOvertimeModule = std::make_shared<PS3ColorOvertime>(gradientRange);
    
    // rotate overtime
    auto xRot = CurveRange::CreateCurveByConstant(0);
    auto yRot = CurveRange::CreateCurveByConstant(0);
    auto zRot = CurveRange::CreateCurveByConstant(0);
    _rotationOvertimeModule = std::make_shared<PS3RotationOvertime>(xRot, yRot, zRot);
    
    // burst
//    auto burstCount = CurveRange::CreateCurveByConstant(30);
//    auto burst = std::make_shared<PS3Burst>(2, 2, 2, burstCount);
//    _bursts.push_back(burst);
    
    // texture animation
    
    auto startFrame = CurveRange::CreateCurveByConstant(0);
    _textureAnimationModule = std::make_shared<PS3TextureAnimationModule>(1, 1, AnimationMode::WHOLE_SHEET, curveRange, startFrame, 1, true, 0);
    
    // texture
    _texture = std::make_shared<Texture2D>("/Users/evanbfeng/work/resource/textures/white_effect.png");
    _texture->Bind();
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
        
        if (_isSubEmitter && _spaceMode == SpaceMode::LOCAL)
        {
            // 如果是子发射器且使用相对位置，更新位置为子发射器的位置
            particle->_position = GetPosition3D();
        }
            
        
        // TODO: textureAnimationModule
        
        // velocity
        float curStartSpeed = _startSpeed->Evaluate(loopDelta, Random01());
        particle->_velocity = particle->_velocity * curStartSpeed;
        
        if (_spaceMode == SpaceMode::WORLD)
        {
            // 根据坐标系更新位置
            mat4 worldMat;
            quat rotationQuat;
            worldMat = GetWorldTransform();
            rotationQuat = GetRotation();
            particle->_position = vec3(worldMat * vec4(particle->_position, 1.0f));
            
            // 根据坐标系更新速度方向
            auto rotationMat = toMat4(rotationQuat);
            particle->_velocity = vec3(rotationMat * vec4(particle->_velocity, 1.0f));
            
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
        particle->_startQuat = quat(radians(particle->_startEuler));
        particle->_startMat = toMat4(particle->_startQuat);
        particle->_ultimateQuat = particle->_startQuat;
        
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
        
        // 我自己写的，源码是写在TextureOvertime里
        if (_textureAnimationModule)
        {
            particle->_startRow = floor(Random01() * _textureAnimationModule->_numTilesY);
        }
        
        _processor->SetNewParticle(particle);
        
        // 通知子发射器发生了粒子出生事件
        NotifySubEmitters(particle, EventType::SPAWN);
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
        if (_processor->UpdateParticles(scaledDeltaTime) == 0 && ! _isEmitting && ! _isSubEmitter)
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
    
    // 更新子发射系统
    for (auto& sub : _subEmitters)
    {
        sub.TargetEmitter->Update(dt);
    }
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
    float dt = 1.0f;
    int cnt = _duration / dt;
    
    for (int i = 0 ; i < cnt; ++i)
    {
        _time += dt;
        ToEmit(dt);
        _processor->UpdateParticles(dt);
    }
}

void PS3ParticleSystem::Render()
{
    _processor->UpdateRenderData();
    _processor->Render();
    if (!_isSubEmitter)
        _processor->_model->_renderer->SetWorldTransform(GetWorldTransform()); // TODO: 这是一个非常hack的做法
    else
    {
        _processor->_model->_renderer->SetWorldTransform(_mainEmitter->GetWorldTransform());
    }
    
    // 渲染发射器的线框
    if (_shapeModule && !_isSubEmitter)
        _shapeModule->RenderEmitter();
    
    // 渲染子系统
    for (auto& sub : _subEmitters)
        sub.TargetEmitter->Render();
}

int PS3ParticleSystem::GetParticleCount()
{
    if (_processor)
        return _processor->GetParticleCount();
    else
        return 0;
}

void PS3ParticleSystem::NotifySubEmitters(PS3ParticlePtr p, EventType event)
{
    for (auto& sub : _subEmitters)
    {
        if (sub.TriggerType == event)
        {
            EmitSubParticles(p, sub.TargetEmitter);
        }
    }
}

void PS3ParticleSystem::EmitSubParticles(PS3ParticlePtr p, std::shared_ptr<PS3ParticleSystem> ps)
{
    // 得到子发射器的位置
    ps->SetPosition3D(p->_position); // 如果是worldSpace，此处获得worldPos，否则获得localPos
    
    ps->EmitParticles(5, 0);
}
