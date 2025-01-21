//
//  ParticleSystem.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/10.
//

#include "PS3ParticleSystem.hpp"


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
    float loopDelta = Repeat(_time, _duration) / _duration;
    
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
            particle->_velocity = vec3(0.0, 0.0, -1.0);
        }
        
        // TODO: textureAnimationModule
        
        // velocity
        float curStartSpeed = _startSpeed->Evaluate(loopDelta, Random01());
        particle->_velocity = particle->_velocity * curStartSpeed;
        
        if (_spaceMode == SpaceMode::WORLD)
        {
            // TODO: 根据坐标系更正位置和速度
        }
        particle->_ultimateVelocity = particle->_velocity; // TODO: 意义是？
        
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
    
    
}

void PS3ParticleSystem::PrewarmSystem()
{
    // TODO: impl
}

void PS3ParticleSystem::Render()
{
    _processor->UpdateRenderData();
    _processor->Render();
}

int PS3ParticleSystem::GetParticleCount()
{
    if (_processor)
        return _processor->GetParticleCount();
    else
        return 0;
}
