//
//  PS3ParticleGenerator.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/5.
//

#include "PS3ParticleGenerator.hpp"
#include "PS3ParticleSystem.hpp"

PS3ParticleGenerator::PS3ParticleGenerator(PS3ParticleSystem* ps, CurveRangePtr rateOverTime, CurveRangePtr rateOverDistance)
: _ps(ps)
, _rateOverTime(rateOverTime)
, _rateOverDistance(rateOverDistance)
{
    
}

std::vector<PS3ParticlePtr> PS3ParticleGenerator::Update(float dt)
{
    BeforeGenerate(dt);
    return GenerateParticles();
}

std::vector<PS3ParticlePtr> PS3ParticleGenerator::GenerateParticles()
{
    int emitCount = min(_emitCount, max(_ps->_capacity -  _ps->GetParticleCount(), 0));
    if (emitCount == 0)
        return std::vector<PS3ParticlePtr>();
    
    // 随时间随距离发射的粒子
    std::vector<PS3ParticlePtr> particles;
    std::generate_n(std::back_inserter(particles), _emitCount, [this](){
        return std::make_shared<PS3Particle>(_ps);
    });
    
    return particles;
}

std::vector<PS3ParticlePtr> PS3ParticleGenerator::GenerateParticles(int count)
{
    int emitCount = min(count, max(_ps->_capacity -  _ps->GetParticleCount(), 0));
    if (emitCount == 0)
        return std::vector<PS3ParticlePtr>();
    
    // 随时间随距离发射的粒子
    std::vector<PS3ParticlePtr> particles;
    std::generate_n(std::back_inserter(particles), count, [this](){
        return std::make_shared<PS3Particle>(_ps);
    });
    
    return particles;
}

void PS3ParticleGenerator::Reset()
{
    _emitRateTimeCounter = 0.0f;
    _emitRateDistanceCounter = 0.0f;
    for (auto& burst : _bursts)
        burst->Reset();
}

void PS3ParticleGenerator::AddBurst(PS3BurstPtr burst)
{
    _bursts.push_back(burst);
}

void PS3ParticleGenerator::RemoveBurst(int idx)
{
    _bursts.erase(_bursts.begin() + idx);
}

void PS3ParticleGenerator::BeforeGenerate(float dt)
{
    _emitCount = 0;
    // emit by rateOverTime 基于时间的发射
    _emitRateTimeCounter += _rateOverTime->Evaluate(_ps->_time / _ps->_duration, 1) * dt;

    if (_emitRateTimeCounter > 1)
    {
        int emitNum = floor(_emitRateTimeCounter);
        _emitRateTimeCounter -= emitNum;
        _emitCount += emitNum;
    }
    
    // emit by rateOverDistance 基于距离的发射
    float curRateOverDistance = _rateOverDistance->Evaluate(_ps->_time / _ps->_duration, 1);
    if (curRateOverDistance > 0)
    {
        _ps->_oldWorldPos = _ps->_curWorldPos;
        _ps->_curWorldPos = _ps->GetWorldPosition();
        float distance = glm::distance(_ps->_curWorldPos, _ps->_oldWorldPos);
        _emitRateDistanceCounter += curRateOverDistance * distance;
    }
    if (_emitRateDistanceCounter > 1)
    {
        int emitNum = floor(_emitRateDistanceCounter);
        _emitRateDistanceCounter -= emitNum;
        _emitCount += emitNum;
        //EmitParticles(emitNum, dt);
    }
    
    // burst
    for (auto burst : _bursts)
    {
       _emitCount += burst->Update(_ps, dt);
    }
}
