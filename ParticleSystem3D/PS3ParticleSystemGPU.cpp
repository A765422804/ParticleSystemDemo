//
//  PS3ParticleSystemGPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#include "PS3ParticleSystemGPU.hpp"

PS3ParticleSystemGPU::PS3ParticleSystemGPU(int maxParticleCount)
: SuperType(maxParticleCount)
, _renderer(nullptr)
{
    _renderer = std::make_shared<PS3RendererGPU>(this, maxParticleCount);
    
    _renderer->InitUniform();
}

void PS3ParticleSystemGPU::InitializeParticles(std::vector<PS3ParticlePtr> &particles)
{
    SuperType::InitializeParticles(particles);
    
    _renderer->SetNewParticles(particles);
    
    for (auto p : particles)
    {
        NotifySubEmitters(p->_position, EventType::SPAWN);
    }
}

void PS3ParticleSystemGPU::Render()
{
    _renderer->Render();
    SuperType::Render();
}

void PS3ParticleSystemGPU::UpdateParticles(float dt)
{
    _renderer->_model->UpdateParticles(_time, dt);
}

void PS3ParticleSystemGPU::Clear()
{
    _renderer->_model->ClearParticles();
}

void PS3ParticleSystemGPU::NotifySubEmitters(vec3 position, EventType event)
{
    for (auto& sub : _subEmitters)
    {
        if (sub.TriggerType == event)
        {
            EmitSubParticles(position, sub.TargetEmitter);
        }
    }
}

void PS3ParticleSystemGPU::EmitSubParticles(vec3 position, std::shared_ptr<PS3ParticleSystem> ps)
{
    ps->SetPosition3D(position); // 如果是worldSpace，此处获得worldPos，否则获得localPos
    
    auto particles = ps->_generator->GenerateParticles(72);
    if (particles.size() == 0)
        return;
    ps->InitializeParticles(particles);
}

int PS3ParticleSystemGPU::GetParticleCount()
{
    return _renderer->_model->_particleCount;
}
