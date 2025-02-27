//
//  PS3RendererGPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/25.
//

#include "PS3RendererGPU.hpp"
#include "../PS3ParticleSystem.hpp"

const int SampleNum = 32;
const float SampleInterval = 1.0f / (SampleNum - 1);

PS3RendererGPU::PS3RendererGPU(PS3ParticleSystem* ps, int maxParticleCount)
: SuperType(ps, maxParticleCount, true)
{
    
}

void PS3RendererGPU::UpdateRenderData(std::vector<PS3ParticlePtr> particles)
{
    // MARK: do nothing
}

void PS3RendererGPU::Clear()
{
    SuperType::Clear();
}

void PS3RendererGPU::InitUniform()
{
    auto shader = _model->_renderer->_shader;
    shader->use();
    
    // space mode
    shader->setBool("IsLocalSpace", _ps->_spaceMode == SpaceMode::LOCAL);
    
    
    // texture animation
    auto textureAnimation = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_ps->_overtimeModules["textureAnimationOvertime"]);
    if (textureAnimation)
    {
        shader->setVec2("FrameTile", vec2(textureAnimation->_numTilesX, textureAnimation->_numTilesY));
    }
    
    // 初始化overtime的texture2D
    bool enable = false;
    // texture overtime
    auto textureOvertime = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_ps->_overtimeModules["textureAnimationOvertime"]);
    enable = textureOvertime ? textureOvertime->_enable : false;
    if (enable)
    {
        shader->setBool("UseAnimationTexture", true);
        
        _animationTexture = CurveRange::PackCurveRangeXY(SampleNum, textureOvertime->_startFrame, textureOvertime->_frameOvertime, true);
        // 启用这个texture
        //_animationTexture->BindToUniform("texture_animation_tex0", GetShaderID(),_renderer->_model->_renderer->GetTextureUnit("texture_animation_tex0"));
        
        // 传递其他info
        vec3 animInfo = vec3(_animationTexture->GetHeight(), textureOvertime->_numTilesX * textureOvertime->_numTilesY, textureOvertime->_cycleCount);
        shader->setVec3("AnimInfo", animInfo);
    }
    else
    {
        shader->setBool("UseAnimationTexture", false);
    }
    
    // velocity overtime
    auto velocityOvertime = std::dynamic_pointer_cast<PS3VelocityOvertime>(_ps->_overtimeModules["velocityOvertime"]);
    enable = velocityOvertime ? velocityOvertime->_enable : false;
    if (enable)
    {
        shader->setBool("UseVelocityOvertime", true);
        
        _velocityTexture = CurveRange::PackCurveRangeXYZ(SampleNum, velocityOvertime->_xVelocity, velocityOvertime->_yVelocity, velocityOvertime->_zVelocity);
        // 启用这个texture
        //_velocityTexture->BindToUniform("texture_velocity_tex0", GetShaderID());
        
        // TODO: velocity有两个参数，分别是space和mode
    }
    else
    {
        shader->setBool("UseVelocityOvertime", false);
    }
    
    // force overtime
    auto forceOvertime = std::dynamic_pointer_cast<PS3ForceOvertime>(_ps->_overtimeModules["forceOvertime"]);
    enable = forceOvertime ? forceOvertime->_enable : false;
    if (enable)
    {
        shader->setBool("UseForceOvertime", true);
        
        _forceTexture = CurveRange::PackCurveRangeXYZ(SampleNum, forceOvertime->_xForce, forceOvertime->_yForce, forceOvertime->_zForce);
        // 启用这个texture
       // _forceTexture->BindToUniform("texture_force_tex0", GetShaderID());
        
        // TODO: force有两个参数，分别是space和mode
    }
    else
    {
        shader->setBool("UseForceOvertime", false);
    }
    

    // size overtime
    auto sizeOvertime = std::dynamic_pointer_cast<PS3SizeOvertime>(_ps->_overtimeModules["sizeOvertime"]);
    enable = sizeOvertime ? sizeOvertime->_enable : false;
    if (enable)
    {
        shader->setBool("UseSizeOvertime", true);
        
        if (sizeOvertime->_separateAxes)
        {
            _sizeTexture = CurveRange::PackCurveRangeXYZ(SampleNum, sizeOvertime->_xSize, sizeOvertime->_ySize, sizeOvertime->_zSize, true);
        }
        else
        {
            _sizeTexture = CurveRange::PackCurveRangeN(SampleNum, sizeOvertime->_size, true);
        }
        
        // 启用这个texture
        _sizeTexture->BindToUniform("texture_size_tex0", _model->_renderer->_shader,_model->_renderer->GetTextureUnit("texture_size_tex0"));

        
        // TODO: size有一个参数mode
    }
    else
    {
        shader->setBool("UseSizeOvertime", false);
    }

    
    // color overtime
    auto colorOvertime = std::dynamic_pointer_cast<PS3ColorOvertime>(_ps->_overtimeModules["colorOvertime"]);
    enable = colorOvertime ? colorOvertime->_enable : false;
    if (enable)
    {
        shader->setBool("UseColorOvertime", true);
        
        _colorTexture = GradientRange::PackGradientRange(SampleNum, colorOvertime->_color);
        // 启用这个texture
        //_colorTexture->BindToUniform("texture_color_tex0", GetShaderID(), _model->_renderer->GetTextureUnit("texture_color_tex0"));
        
        // TODO: color有一个参数mode
    }
    else
    {
        shader->setBool("UseColorOvertime", false);
    }
    
    // rotation overtime
    auto rotationOvertime = std::dynamic_pointer_cast<PS3RotationOvertime>(_ps->_overtimeModules["rotationOvertime"]);
    enable = rotationOvertime ? rotationOvertime->_enable : false;
    if (enable)
    {
        shader->setBool("UseRotationOvertime", true);
        
        if (!rotationOvertime->_separateAxes)
        {
            _rotationTexture = CurveRange::PackCurveRangeXYZ(SampleNum, rotationOvertime->_xRotate, rotationOvertime->_yRotate, rotationOvertime->_zRotate, true);
        }
        else
        {
            _rotationTexture = CurveRange::PackCurveRangeZ(SampleNum, rotationOvertime->_zRotate, true);
        }
        // 启用这个texture
        //_rotationTexture->BindToUniform("texture_rotation_tex0", GetShaderID());
        
        // TODO: rotation有一个参数mode
    }
    else
    {
        shader->setBool("UseRotationOvertime", false);
    }
    
    // sampleInfo
    vec2 sampleInfo = vec2(SampleNum, SampleInterval);
    shader->setVec2("SampleInfo", sampleInfo);
    
    // mainTexture
    _ps->_texture->BindToUniform("MainTexture", _model->_renderer->_shader, _model->_renderer->GetTextureUnit("MainTexture"));
}

void PS3RendererGPU::UpdateUniform()
{
    auto shader = _model->_renderer->_shader;
    shader->use();
    
    if (!_ps->_isSubEmitter)
        _model->_renderer->SetWorldTransform(_ps->GetWorldTransform());
    else
    {
        // 子发射器的粒子的世界矩阵，使用的是父发射器
        _model->_renderer->SetWorldTransform(_ps->_mainEmitter->GetWorldTransform());
    }
    
    // time and delta
    vec2 timeDelta = vec2(_ps->_time, _ps->_dt);
    shader->setVec2("Time_Delta", timeDelta);
}

void PS3RendererGPU::Render()
{
    UpdateUniform();
    _model->RenderModelGPU();
}

void PS3RendererGPU::SetNewParticle(PS3ParticlePtr p)
{
    // 把新粒子的数据传给model
    _model->AddGPUParticleVertexData(p, _model->_particleCountGPU, _ps->_time);
    ++ _model->_particleCountGPU;
}
