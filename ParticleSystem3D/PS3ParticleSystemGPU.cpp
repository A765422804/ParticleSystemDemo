//
//  PS3ParticleSystemGPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#include "PS3ParticleSystemGPU.hpp"

const int SampleNum = 128;
const float SampleInterval = 1.0f / (SampleNum - 1);

PS3ParticleSystemGPU::PS3ParticleSystemGPU(int maxParticleCount)
: SuperType(maxParticleCount)
, _renderer(nullptr)
{
    _entity = std::make_shared<PS3EntityGPU>();
    _renderer = std::make_shared<PS3RendererGPU>(this, maxParticleCount);
    
    //_renderer->InitUniform();
    _entity->SetIsLocalSpace(_spaceMode == SpaceMode::LOCAL);
    
    // texture animation
    auto textureAnimation = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_overtimeModules["textureAnimationOvertime"]);
    if (textureAnimation)
    {
        _entity->SetFrameTile(vec2(textureAnimation->_numTilesX, textureAnimation->_numTilesY));
    }
    
    bool enable = false;
    auto textureOvertime = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_overtimeModules["textureAnimationOvertime"]);
    enable = textureOvertime ? textureOvertime->_enable : false;
    if (enable)
    {
        _entity->SetUseAnimationOvertime(true);
        
        _animationTexture = CurveRange::PackCurveRangeXY(SampleNum, textureOvertime->_startFrame, textureOvertime->_frameOvertime, true);
        _entity->SetAnimationTexture(_animationTexture);

        // 传递其他info
        vec3 animInfo = vec3(_animationTexture->GetHeight(), textureOvertime->_numTilesX * textureOvertime->_numTilesY, textureOvertime->_cycleCount);
        _entity->SetAnimInfo(animInfo);
        
        //shader->setInt("animation_mode", _animationTexture->GetHeight());
        _entity->SetAnimationMode(_animationTexture->GetHeight());
    }
    else
    {
        _entity->SetUseAnimationOvertime(false);
    }
    
    // velocity overtime
    auto velocityOvertime = std::dynamic_pointer_cast<PS3VelocityOvertime>(_overtimeModules["velocityOvertime"]);
    enable = velocityOvertime ? velocityOvertime->_enable : false;
    if (enable)
    {
        _entity->SetUseVelocityOvertime(true);
        
        _velocityTexture = CurveRange::PackCurveRangeXYZ(SampleNum, velocityOvertime->_xVelocity, velocityOvertime->_yVelocity, velocityOvertime->_zVelocity);
        _entity->SetVelocityTexture(_velocityTexture);

        _entity->SetVelocityMode(_velocityTexture->GetHeight());
    }
    else
    {
        _entity->SetUseVelocityOvertime(false);
    }

    // force overtime
    auto forceOvertime = std::dynamic_pointer_cast<PS3ForceOvertime>(_overtimeModules["forceOvertime"]);
    enable = forceOvertime ? forceOvertime->_enable : false;
    if (enable)
    {
        _entity->SetUseForceOvertime(true);
        
        _forceTexture = CurveRange::PackCurveRangeXYZ(SampleNum, forceOvertime->_xForce, forceOvertime->_yForce, forceOvertime->_zForce);
        _entity->SetForceTexture(_forceTexture);

        _entity->SetForceMode(_forceTexture->GetHeight());
    }
    else
    {
        _entity->SetUseForceOvertime(false);
    }

    // size overtime
    auto sizeOvertime = std::dynamic_pointer_cast<PS3SizeOvertime>(_overtimeModules["sizeOvertime"]);
    enable = sizeOvertime ? sizeOvertime->_enable : false;
    if (enable)
    {
        _entity->SetUseSizeOvertime(true);
        
        if (sizeOvertime->_separateAxes)
        {
            _sizeTexture = CurveRange::PackCurveRangeXYZ(SampleNum, sizeOvertime->_xSize, sizeOvertime->_ySize, sizeOvertime->_zSize, true);
        }
        else
        {
            _sizeTexture = CurveRange::PackCurveRangeN(SampleNum, sizeOvertime->_size, true);
        }
        _entity->SetSizeTexture(_sizeTexture);

        _entity->SetSizeMode(_sizeTexture->GetHeight());
    }
    else
    {
        _entity->SetUseSizeOvertime(false);
    }

    // color overtime
    auto colorOvertime = std::dynamic_pointer_cast<PS3ColorOvertime>(_overtimeModules["colorOvertime"]);
    enable = colorOvertime ? colorOvertime->_enable : false;
    if (enable)
    {
        _entity->SetUseColorOvertime(true);
        
        _colorTexture = GradientRange::PackGradientRange(SampleNum, colorOvertime->_color);
        _entity->SetColorTexture(_colorTexture);

        _entity->SetColorMode(_colorTexture->GetHeight());
    }
    else
    {
        _entity->SetUseColorOvertime(false);
    }

    // rotation overtime
    auto rotationOvertime = std::dynamic_pointer_cast<PS3RotationOvertime>(_overtimeModules["rotationOvertime"]);
    enable = rotationOvertime ? rotationOvertime->_enable : false;
    if (enable)
    {
        _entity->SetUseRotationOvertime(true);
        
        if (rotationOvertime->_separateAxes)
        {
            _rotationTexture = CurveRange::PackCurveRangeXYZ(SampleNum, rotationOvertime->_xRotate, rotationOvertime->_yRotate, rotationOvertime->_zRotate, true);
        }
        else
        {
            _rotationTexture = CurveRange::PackCurveRangeZ(SampleNum, rotationOvertime->_zRotate, true);
        }
        _entity->SetRotationTexture(_rotationTexture);

        _entity->SetRotationMode(_rotationTexture->GetHeight());
    }
    else
    {
        _entity->SetUseRotationOvertime(false);
    }
    
    // gravity
    _gravityTexture = CurveRange::PackCurveRangeN(SampleNum, _gravity);
    _entity->SetGravityTexture(_gravityTexture);
    _entity->SetGravityMode(_gravityTexture->GetHeight());
    
    vec2 sampleInfo = vec2(SampleNum, SampleInterval);
    _entity->SetSampleInfo(sampleInfo);
    
    _entity->SetTexture(_texture);
    
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
    
    auto particles = ps->_generator->GenerateParticles(36);
    if (particles.size() == 0)
        return;
    ps->InitializeParticles(particles);
}

int PS3ParticleSystemGPU::GetParticleCount()
{
    return _renderer->_model->_particleCount;
}
