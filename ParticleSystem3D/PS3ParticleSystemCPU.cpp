//
//  PS3ParticleSystemCPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#include "PS3ParticleSystemCPU.hpp"

PS3ParticleSystemCPU::PS3ParticleSystemCPU(int maxParticleCount)
: SuperType(maxParticleCount)
, _trailModule(nullptr)
, _renderer(nullptr)
{
    _entity = std::make_shared<PS3EntityCPU>();
    _renderer = std::make_shared<PS3RendererCPU>(this, maxParticleCount);
    
    // 初始化
    _entity->SetIsLocalSpace(_spaceMode == SpaceMode::LOCAL);
    auto textureAnimation = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_overtimeModules["textureAnimationOvertime"]);
    if (textureAnimation)
    {
        _entity->SetFrameTile(vec2(textureAnimation->_numTilesX, textureAnimation->_numTilesY));
    }
    
//    auto shader = _model->_renderer->_shader;
//    shader->use();
//    
//    // space mode
//    shader->setBool("IsLocalSpace", _ps->_spaceMode == SpaceMode::LOCAL);
//    
//    // texture animation
//    auto textureAnimation = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_ps->_overtimeModules["textureAnimationOvertime"]);
//    if (textureAnimation)
//    {
//        shader->setVec2("FrameTile", vec2(textureAnimation->_numTilesX, textureAnimation->_numTilesY));
//    }
    
    _entity->SetTexture(_texture);
    
    // texture
//    _texture->BindToUniform("MainTexture", _renderer->_model->_renderer->_shader, _renderer->_model->_renderer->GetTextureUnit("MainTexture"));
}

void PS3ParticleSystemCPU::InitializeParticles(std::vector<PS3ParticlePtr> &particles)
{
    SuperType::InitializeParticles(particles);
    
    _particles.insert(_particles.end(), particles.begin(), particles.end());
    
}

void PS3ParticleSystemCPU::Render()
{
    _renderer->UpdateRenderData(_particles);
    _renderer->Render();
    SuperType::Render();
    
    if (_trailModule)
    {
        _trailModule->UpdateRenderData();
        _trailModule->Render();
    }
}

void PS3ParticleSystemCPU::UpdateParticles(float dt)
{
    for (int i = 0 ; i < _particles.size() ; ++i)
    {
        auto p = _particles[i];
        if (!UpdateParticle(p, dt))
        {
            // 说明第i个粒子消亡
            _particles.erase(_particles.begin() + i);
            -- i;
        }
    }
    // std::cout<<_particles.size()<<std::endl;
}

bool PS3ParticleSystemCPU::UpdateParticle(PS3ParticlePtr p, float dt)
{
    // 生命周期衰减
    p->_remainingLifeTime -= dt;
    
    if (p->_remainingLifeTime < 0.0)
    {
        // Trigger了死亡事件
        NotifySubEmitters(p, EventType::DEATH);
        
        if (_trailModule)
            _trailModule->RemoveParticle(p);
        
        // 移除死亡粒子
        return false; // 表示粒子死亡
    }
    
    // 重力使用
    if (_gravity != nullptr)
    {
        float normalizedTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
        float gravityEffect = -_gravity->Evaluate(normalizedTime, Random01()) * 9.8 * dt;
        if (_spaceMode == SpaceMode::LOCAL)
        {
            vec4 gravity = vec4(0, gravityEffect, 0, 1);
            gravity = transpose(toMat4(GetRotation()))  * gravity;
            p->_velocity.x += gravity.x;
            p->_velocity.y += gravity.y;
            p->_velocity.z += gravity.z;
        }
        else
        {
            p->_velocity.y += gravityEffect;
        }
    }
    
    // 更新粒子最终速度
    p->_ultimateVelocity = p->_velocity;
    
    // 应用动画
    for (auto [key, value] : _overtimeModules)
    {
        if (value && value->_enable)
            value->Animate(p, dt);
    }
    
    // 更新位置
    p->_position = p->_position + p->_ultimateVelocity * dt;
    
    // 更新粒子的拖尾
    if (_trailModule)
        _trailModule->Animate(p, dt);
    
    return true;
}

void PS3ParticleSystemCPU::Clear()
{
    _particles.clear();
    
    _trailModule->Clear();
}

void PS3ParticleSystemCPU::SetTrailModule(PS3TrailPtr trail)
{
    _trailModule = trail;
    _trailModule->_ps = this;
}

void PS3ParticleSystemCPU::NotifySubEmitters(PS3ParticlePtr p, EventType event)
{
    for (auto& sub : _subEmitters)
    {
        if (sub.TriggerType == event)
        {
            EmitSubParticles(p, sub.TargetEmitter);
        }
    }
}

void PS3ParticleSystemCPU::EmitSubParticles(PS3ParticlePtr p, std::shared_ptr<PS3ParticleSystem> ps)
{
    // 得到子发射器的位置
    ps->SetPosition3D(p->_position); // 如果是worldSpace，此处获得worldPos，否则获得localPos
    
    auto particles = ps->_generator->GenerateParticles(72);
    if (particles.size() == 0)
        return;
    ps->InitializeParticles(particles);
}

int PS3ParticleSystemCPU::GetParticleCount()
{
    return _particles.size();
}
