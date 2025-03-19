//
//  ParticleSystem.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "PS3ParticleSystem.hpp"

PS3ParticleSystem::PS3ParticleSystem(int maxParticleCount)
: _isPlaying(false)
, _isPaused(false)
, _isStopped(true)
, _isEmitting(false)
, _needToRestart(false)
, _needRefresh(true)
, _prewarm(true)
, _isSubEmitter(false)
, _capacity(maxParticleCount)
, _loop(true)
, _duration(2.0f)
, _simulationSpeed(1)
, _startDelay(nullptr)
//, _rateOverTime(nullptr)
//, _rateOverDistance(nullptr)
//, _renderer(nullptr)
, _shapeModule(nullptr)
, _texture(nullptr)
, _spaceMode(SpaceMode::WORLD)
//, _trailModule(nullptr)
//, _processor(nullptr)
, _inilizer(nullptr)
//, _useGPU(false)
, _generator(nullptr)
{
    // processor
//    if (_useGPU)
//    {
//        _processor = std::make_shared<PS3ParticleProcessorGPU>(this);
//    }
//    else
//    {
//        _processor = std::make_shared<PS3ParticleProcessorCPU>(this);
//    }
    
    // generator
    auto rateOverTime = CurveRange::CreateCurveByConstant(250);
    auto rateOverDistance = CurveRange::CreateCurveByConstant(0);
    _generator = std::make_shared<PS3ParticleGenerator>(this, rateOverTime ,rateOverDistance);
    
    // spatial info
    SetPosition3D(vec3(0, 0, 0));
    SetRotation(vec3(0,0,0));
    SetScale(vec3(1, 1, 1));
    
    // gravity
    _gravity = CurveRange::CreateCurveByConstant(0.3);
    
    // 定义一个公用的curve和curveRange for debug
    std::vector<float> time = {0.0f, 1.0f};
    KeyFrameValue keyFrame1 = {2.0f, 0.0f, 0.0f}; // (0, 0) 点，没有左切线
    KeyFrameValue keyFrame2 = {2.0f, 0.0f, 0.0f}; // (1, 1) 点，没有右切线
    std::vector<KeyFrameValue> value = {keyFrame1, keyFrame2};
    CurvePtr curve = Curve::CreateCurveByTimesAndValues(time, value);
    CurveRangePtr curveRange = CurveRange::CreateCurveByOneCurve(curve);
    
    _startDelay = CurveRange::CreateCurveByConstant(0);

//    _rateOverTime = CurveRange::CreateCurveByConstant(30);
//    _rateOverDistance = CurveRange::CreateCurveByConstant(0);
    
    // renderer
//    if (_useGPU)
//    {
//        _renderer = std::make_shared<PS3RendererGPU>(this, maxParticleCount);
//    }
//    else
//    {
//        _renderer = std::make_shared<PS3RendererCPU>(this, maxParticleCount);
//    }

    //_shapeModule = PS3BoxEmitter::CreateBoxEmitter(EmitLocation::VOLUME, this);
    _shapeModule = PS3ConeEmitter::CreateConeEmitter(EmitLocation::BASE, ArcMode::RANDOM, 0, 360, 0, 0.001, 1, 10, 10, 0.5, this);
//    _shapeModule = PS3CircleEmitter::CreateCircleEmitter(ArcMode::LOOP, 0, 360, CurveRange::CreateCurveByConstant(1), 2, 1,10, this);
    //_shapeModule = PS3SphereEmitter::CreateSphereEmitter(EmitLocation::VOLUME, 1, 1, this);
    //_shapeModule = PS3HemisphereEmitter::CreateHemisphereEmitter(EmitLocation::VOLUME, 1, 1, this);
    
    // velocity overtime
    auto xSpeed = CurveRange::CreateCurveByConstant(0.0f);
    auto ySpeed = CurveRange::CreateCurveByTwoConstant(0, 0);
    auto zSpeed = CurveRange::CreateCurveByConstant(0.0f);
    auto velocityOvertimeModule = std::make_shared<PS3VelocityOvertime>(xSpeed, ySpeed, zSpeed);
    _overtimeModules["velocityOvertime"] = velocityOvertimeModule;
    
    // force overtime
    auto xForce = CurveRange::CreateCurveByConstant(0.0f);
    auto yForce = CurveRange::CreateCurveByConstant(0.0f);
    auto zForce = CurveRange::CreateCurveByConstant(0.0f);
    auto forceOvertimeModule = std::make_shared<PS3ForceOvertime>(xForce, yForce, zForce);
    _overtimeModules["forceOvertime"] = forceOvertimeModule;
    
    // size overtime
    std::vector<float> sizeTime = {0.0f, 0.5f, 1.0f};
    KeyFrameValue sizeKeyFrame1 = {0.0f, 0.0f, 2.0f}; // (0, 0) 点，没有左切线
    KeyFrameValue sizeKeyFrame2 = {0.2f, 0.5f, -0.5f}; // (1, 1) 点，没有右切线
    KeyFrameValue sizeKeyFrame3 = {0.0f, -2.0f, 0.0f}; // (1, 1) 点，没有右切线
    std::vector<KeyFrameValue> sizeValue = {sizeKeyFrame1, sizeKeyFrame2, sizeKeyFrame3};
    CurvePtr sizeCurve = Curve::CreateCurveByTimesAndValues(sizeTime, sizeValue);
    CurveRangePtr sizeCurveRange = CurveRange::CreateCurveByOneCurve(sizeCurve);
    
    auto allSize = sizeCurveRange;
//    auto xSize = CurveRange::CreateCurveByConstant(0.1f);
//    auto ySize = curveRange;
//    auto zSize = CurveRange::CreateCurveByConstant(0.1f);
    _overtimeModules["sizeOvertime"] = std::make_shared<PS3SizeOvertime>(allSize);
    
    // color overtime
        ColorKey colorKey1 = {vec3(0.0f, 0.0f, 1.0f), 0.0f}; // 蓝色
    ColorKey colorKey2 = {vec3(1.0f, 0.6f, 0.0f), 0.1f}; // 黄色
        ColorKey colorKey3 = {vec3(1.0f, 0.5f, 0.0f), 0.2f}; // 橙色
        ColorKey colorKey4 = {vec3(1.0f, 0.0f, 0.0f), 0.8f}; // 红色
    ColorKey colorKey5 = {vec3(1.0f, 0.0f, 0.0f), 1.0f}; // 红色
        AlphaKey alphaKey1 = {0.0f, 0.0f}; // alpha time
        AlphaKey alphaKey2 = {0.5f, 0.1f};
    AlphaKey alphaKey3 = {0.1, 0.8f};
    AlphaKey alphaKey4 = {0.0, 1.0f};
        std::vector<ColorKey> colorKeys = {colorKey1, colorKey2, colorKey3, colorKey4, colorKey5};
        std::vector<AlphaKey> alphaKeys = {alphaKey1, alphaKey2, alphaKey3, alphaKey4};
        GradientPtr gradient = Gradient::CreateByColorKeyAndAlphaKey(colorKeys, alphaKeys);
        GradientRangePtr gradientRange = GradientRange::CreateByOneGradient(gradient);
    // auto randomColor = GradientRange::CreateByRandomColor();
    _overtimeModules["colorOvertime"] = std::make_shared<PS3ColorOvertime>(gradientRange);
    
    // rotate overtime
    auto xRot = CurveRange::CreateCurveByConstant(radians(0.0));
    auto yRot = CurveRange::CreateCurveByConstant(radians(0.0));
    auto zRot = CurveRange::CreateCurveByConstant(radians(0.0));
    auto rotationOvertimeModule = std::make_shared<PS3RotationOvertime>(xRot, yRot, zRot);
    _overtimeModules["rotationOvertime"] = rotationOvertimeModule;
    
    // burst
//    auto burstCount = CurveRange::CreateCurveByConstant(50);
//    auto burst = std::make_shared<PS3Burst>(0, 2, 2, burstCount);
//    _generator->AddBurst(burst);
//    _bursts.push_back(burst);
    
    // inilizer
    _inilizer = std::make_shared<PS3ParticleInitializer>(this);
    
    std::vector<float> frameTime = {0.0f, 1.0f};
    KeyFrameValue frameKeyFrame1 = {0.0f, 0.0f, 1.0f}; // (0, 0) 点，没有左切线
    KeyFrameValue frameKeyFrame2 = {1.0f, 1.0f, 0.0f}; // (1, 1) 点，没有右切线
    std::vector<KeyFrameValue> frameTimeValue = {frameKeyFrame1, frameKeyFrame2};
    CurvePtr frameCurve = Curve::CreateCurveByTimesAndValues(frameTime, frameTimeValue);
    CurveRangePtr frameCurveRange = CurveRange::CreateCurveByOneCurve(frameCurve);
    
    // texture animation
    auto startFrame = CurveRange::CreateCurveByTwoConstant(1, 4);
    auto textureAnimationModule = std::make_shared<PS3TextureAnimationModule>(2, 2, AnimationMode::WHOLE_SHEET, frameCurveRange, startFrame, 4, true, 0);
    _overtimeModules["textureAnimationOvertime"] = textureAnimationModule;
    _overtimeModules["textureAnimationOvertime"]->_enable = false;
    
    // texture
    _texture = std::make_shared<Texture2D>("/Users/evanbfeng/work/resource/textures/sequence_fire2.png");
    //_texture->BindToUniform("MainTexture", _renderer->_model->_renderer->_shader, _renderer->_model->_renderer->GetTextureUnit("MainTexture"));
    
    // 初始化renderer的uniform
    // _renderer->InitUniform();
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
        
        auto newParticles = _generator->Update(dt);
        if (newParticles.size() == 0)
            return;
        
        InitializeParticles(newParticles);
        
//        float loopDelta = Repeat(_time, _duration) / _duration; // 当前时间在一个周期内的占比
//        for (auto &particle : newParticles)
//        {
//            particle->Reset(); // 初始化粒子
//            
//            if (_shapeModule != nullptr && _shapeModule->_enable) // 使用发射器进行发射
//            {
//                _shapeModule->Emit(particle);
//            }
//            else // 不使用发射器
//            {
//                particle->_position = vec3(0.0, 0.0, 0.0);
//                particle->_velocity = vec3(0.0, 0.0, 0.0);
//            }
//            
//            _inilizer->InitializeParticle(particle, loopDelta);
//            
//            // 将粒子数据传递给渲染器
//            _renderer->SetNewParticle(particle);
//            
//            if (!_useGPU)
//                _particles.push_back(particle);
//            
//            // 通知子发射器发生了粒子出生事件
//            NotifySubEmitters(particle, EventType::SPAWN);
//        }
        
        _shapeModule->_lastTime = _time;
        
//        // emit by rateOverTime 基于时间的发射
//        _emitRateTimeCounter += _rateOverTime->Evaluate(_time / _duration, 1) * dt;
//        if (_emitRateTimeCounter > 1)
//        {
//            int emitNum = floor(_emitRateTimeCounter);
//            _emitRateTimeCounter -= emitNum;
//            EmitParticles(emitNum, dt);
//        }
//        
//        // emit by rateOverDistance 基于距离的发射
//        float curRateOverDistance = _rateOverDistance->Evaluate(_time / _duration, 1);
//        if (curRateOverDistance > 0)
//        {
//            _oldWorldPos = _curWorldPos;
//            _curWorldPos = GetWorldPosition();
//            float distance = glm::distance(_curWorldPos, _oldWorldPos);
//            _emitRateDistanceCounter += curRateOverDistance * distance;
//        }
//        if (_emitRateDistanceCounter > 1)
//        {
//            int emitNum = floor(_emitRateDistanceCounter);
//            _emitRateDistanceCounter -= emitNum;
//            EmitParticles(emitNum, dt);
//        }
        
        // burst
//        for (auto burst : _bursts)
//            burst->Update(this, dt);
    }
}

//void PS3ParticleSystem::EmitParticles(int emitNum, float dt)
//{
//    float loopDelta = Repeat(_time, _duration) / _duration; // 当前时间在一个周期内的占比
//    
//    // TODO: 这里有一个_needRefresh
//    
//    // 发射粒子
//    for (int i = 0; i < emitNum ; ++i)
//    {
//        if (_useGPU)
//        {
//            if (_renderer->_model->_particleCountGPU >= _capacity)
//                return;
//        }
//        else
//        {
//            if (_particles.size() >= _capacity)
//                return;
//        }
//        
//        PS3ParticlePtr particle = std::make_shared<PS3Particle>(this);
//        particle->Reset(); // 初始化粒子
//        
//        if (_shapeModule != nullptr && _shapeModule->_enable) // 使用发射器进行发射
//        {
//            _shapeModule->Emit(particle);
//        }
//        else // 不使用发射器
//        {
//            particle->_position = vec3(0.0, 0.0, 0.0);
//            particle->_velocity = vec3(0.0, 0.0, 0.0);
//        }
//        
//        _inilizer->InitializeParticle(particle, loopDelta);
//        
//        // TODO: 可以设置particle的随机数种子和loopCount，但是意义不明
//        
//        // 将粒子数据传递给渲染器
//        _renderer->SetNewParticle(particle);
//        
//        if (!_useGPU)
//            _particles.push_back(particle);
//        
//        // 通知子发射器发生了粒子出生事件
//        NotifySubEmitters(particle, EventType::SPAWN);
//    }
//    _shapeModule->_lastTime = _time;
//}

void PS3ParticleSystem::Update(float dt)
{
    float scaledDeltaTime = dt * _simulationSpeed;
    
    _dt = scaledDeltaTime;
    
    if (_isPlaying)
    {
        _time += scaledDeltaTime; // 更新总时间
        
        // std::cout<<"update time: " <<_time<<std::endl;
        
        // 发射粒子
        ToEmit(scaledDeltaTime);
        
        // 更新粒子
       // _processor->UpdateParticles(_particles, scaledDeltaTime);

        UpdateParticles(scaledDeltaTime);
        
        // 粒子全部死光则停止
        if (GetParticleCount() == 0 && ! _isEmitting && ! _isSubEmitter)
            Stop();
        
        // 更新粒子
//        if (_renderer->UpdateParticles(scaledDeltaTime) == 0 && ! _isEmitting && ! _isSubEmitter)
//        {
//            // 停止播放粒子
//            Stop();
//        }
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

void PS3ParticleSystem::UpdateParticles(float dt)
{
//    if (_useGPU)
//    {
//        _renderer->_model->UpdateGPUParticles(_time, dt);
//    }
//    else
//    {
//        for (int i = 0 ; i < _particles.size() ; ++i)
//        {
//            auto p = _particles[i];
//            if (!UpdateParticle(p, dt))
//            {
//                // 说明第i个粒子消亡
//                _particles.erase(_particles.begin() + i);
//                -- i;
//            }
//        }
//    }
}

//bool PS3ParticleSystem::UpdateParticle(PS3ParticlePtr p, float dt)
//{
//    // 生命周期衰减
//    p->_remainingLifeTime -= dt;
//    
//    if (p->_remainingLifeTime < 0.0)
//    {
//        // Trigger了死亡事件
//        NotifySubEmitters(p, EventType::DEATH);
//        
////        if (_trailModule)
////            _trailModule->RemoveParticle(p);
//        
//        // 移除死亡粒子
//        return false; // 表示粒子死亡
//    }
//    
//    // 重力使用
//    if (_gravity != nullptr)
//    {
//        float normalizedTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
//        float gravityEffect = -_gravity->Evaluate(normalizedTime, Random01()) * 9.8 * dt;
//        if (_spaceMode == SpaceMode::LOCAL)
//        {
//            vec4 gravity = vec4(0, gravityEffect, 0, 1);
//            gravity = transpose(toMat4(GetRotation()))  * gravity;
//            p->_velocity.x += gravity.x;
//            p->_velocity.y += gravity.y;
//            p->_velocity.z += gravity.z;
//        }
//        else
//        {
//            p->_velocity.y += gravityEffect;
//        }
//    }
//    
//    // 更新粒子最终速度
//    p->_ultimateVelocity = p->_velocity;
//    
//    // 应用动画
//    for (auto [key, value] : _overtimeModules)
//    {
//        if (value && value->_enable)
//            value->Animate(p, dt);
//    }
//    
//    // 更新位置
//    p->_position = p->_position + p->_ultimateVelocity * dt;
//    
//    // 更新粒子的拖尾
////    if (_trailModule)
////        _trailModule->Animate(p, dt);
//    
//    return true;
//}

void PS3ParticleSystem::Play()
{
    if (_needToRestart)
    {
        Reset();
        _needToRestart = false;
    }
    if (_isPaused)
    {
        _isPaused = false;
    }
    if (_isStopped)
    {
        _isStopped = false;
    }
    
    _isPlaying = true;
    if (!_isSubEmitter)
        _isEmitting = true;
    
    ResetPosition();
    
    if (_prewarm)
    {
        PrewarmSystem();
    }
    
//    if (_trailModule)
//    {
//        // TODO: 这里其实是基于trailModule的model去enable，对我而言不需要
//    }
    
//    if (_renderer)
//    {
//        _renderer->_model->_enabled = true;
//    }
    
    // MARK: 这里是我添加的逻辑，就是如果有子发射器话，让子发射器也执行对应的play函数
    for (auto sub : _subEmitters)
        sub.TargetEmitter->Play();
}

void PS3ParticleSystem::ResetPosition()
{
    _oldWorldPos = _curWorldPos = GetPosition3D();
}

void PS3ParticleSystem::Pause()
{
    if (_isStopped)
    {
        AssertShouldNotReachHere();
    }
    if (_isPlaying)
    {
        _isPlaying = false;
    }
    
    _isPaused = true;
    
    // MARK: 我自己加的，暂停所有的子发射器
    for (auto sub : _subEmitters)
        sub.TargetEmitter->Pause();
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
    
    // MARK: 我自己添加,子粒子系统也全部stop
    for (auto sub : _subEmitters)
        sub.TargetEmitter->Stop();
}

void PS3ParticleSystem::Restart()
{
    Stop();
    Play();
}

void PS3ParticleSystem::Clear()
{
//    if (!_useGPU)
//        _particles.clear();
//    else
//        _renderer->_model->ClearGPUParticles();
//    
//    if (_trailModule)
//        _trailModule->Clear();
}

void PS3ParticleSystem::Reset()
{
    _time = 0.0f;
    _generator->Reset();
//    _emitRateTimeCounter = 0.0f;
//    _emitRateDistanceCounter = 0.0f;
    ResetPosition();
    
    _generator->Reset();
}

void PS3ParticleSystem::PrewarmSystem()
{
    if (_time > _duration)
        return;
    
    _startDelay->_mode = RangeMode::Constant;
    _startDelay->_constant = 0.0f;
    float dt = 0.01f;
    // int cnt = _duration / dt;
    // _isEmitting = true;
    
//    for (int i = 0 ; i < cnt; ++i)
//    {
    while(_time < _duration)
    {
        _time += dt;
        ToEmit(dt);
        UpdateParticles(dt);
    }
    //}
    
    _time = _duration;
}

void PS3ParticleSystem::Render()
{
    //_renderer->UpdateRenderData(_particles);
    // _renderer->Render();
    
//    if (_trailModule)
//    {
//        _trailModule->UpdateRenderData();
//        _trailModule->Render();
//    }
    
    // 渲染发射器的线框
    if (_shapeModule && !_isSubEmitter)
        _shapeModule->RenderEmitter();
    
    // 渲染子系统
    for (auto& sub : _subEmitters)
        sub.TargetEmitter->Render();
}

//int PS3ParticleSystem::GetParticleCount()
//{
////    if (!_useGPU)
////        return int(_particles.size());
////    else
////        return _renderer->_model->_particleCountGPU;
//    return 0;
//}

//void PS3ParticleSystem::NotifySubEmitters(PS3ParticlePtr p, EventType event)
//{
//    for (auto& sub : _subEmitters)
//    {
//        if (sub.TriggerType == event)
//        {
//            EmitSubParticles(p, sub.TargetEmitter);
//        }
//    }
//}
//
//void PS3ParticleSystem::NotifySubEmitters(vec3 position, EventType event)
//{
//    for (auto& sub : _subEmitters)
//    {
//        if (sub.TriggerType == event)
//        {
//            EmitSubParticles(position, sub.TargetEmitter);
//        }
//    }
//}
//
//void PS3ParticleSystem::EmitSubParticles(vec3 position, std::shared_ptr<PS3ParticleSystem> ps)
//{
//    ps->SetPosition3D(position); // 如果是worldSpace，此处获得worldPos，否则获得localPos
//    
//    auto particles = ps->_generator->GenerateParticles(20);
//    if (particles.size() == 0)
//        return;
//    ps->InitializeParticles(particles);
//}

void PS3ParticleSystem::InitializeParticles(std::vector<PS3ParticlePtr> &particles)
{
    float loopDelta = Repeat(_time, _duration) / _duration; // 当前时间在一个周期内的占比
    for (auto &particle : particles)
    {
        particle->Reset(); // 初始化粒子
        
        if (_shapeModule != nullptr && _shapeModule->_enable) // 使用发射器进行发射
        {
            _shapeModule->Emit(particle);
        }
        else // 不使用发射器
        {
            particle->_position = vec3(0.0, 0.0, 0.0);
            particle->_velocity = vec3(0.0, 0.0, 0.0);
        }
        
        _inilizer->InitializeParticle(particle, loopDelta);
        
        // 将粒子数据传递给渲染器
        //_renderer->SetNewParticle(particle); // GPU 特有
        
//        if (!_useGPU)
//            _particles.push_back(particle); // CPU 特有
        
        // 通知子发射器发生了粒子出生事件
        //NotifySubEmitters(particle, EventType::SPAWN);
    }
    
    //_particles.insert(_particles.end(), particles.begin(), particles.end());
}

//void PS3ParticleSystem::EmitSubParticles(PS3ParticlePtr p, std::shared_ptr<PS3ParticleSystem> ps)
//{
//    // 得到子发射器的位置
//    ps->SetPosition3D(p->_position); // 如果是worldSpace，此处获得worldPos，否则获得localPos
//    
//    auto particles = ps->_generator->GenerateParticles(20);
//    if (particles.size() == 0)
//        return;
//    ps->InitializeParticles(particles);
//    // ps->EmitParticles(72, 0);
//}

//void PS3ParticleSystem::SetTrailModule(PS3TrailPtr trail)
//{
////    _trailModule = trail;
////    _trailModule->_ps = this;
//}
