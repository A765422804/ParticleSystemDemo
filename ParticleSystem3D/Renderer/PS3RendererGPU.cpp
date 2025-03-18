//
//  PS3RendererGPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/25.
//

#include "PS3RendererGPU.hpp"
#include "../PS3ParticleSystemGPU.hpp"

//const int SampleNum = 128;
//const float SampleInterval = 1.0f / (SampleNum - 1);

PS3RendererGPU::PS3RendererGPU(PS3ParticleSystemGPU* ps, int maxParticleCount)
: _ps(ps)
, _model(std::make_shared<PS3ParticleBatchModelGPU>(maxParticleCount,ps))
{
    _rac = std::make_shared<PS3GPURAC>(ps);
    _ras = std::make_shared<PS3GPURAS>(_ps->_entity->_property);
}

void PS3RendererGPU::Clear()
{
    if (_model)
        _model->_enabled = false;
    // 清空粒子
    _model->_iDataI.clear();
    _model->_vDataF.clear();
}

//void PS3RendererGPU::InitUniform()
//{
////    auto shader = _model->_renderer->_shader;
////    shader->use();
//    
//    // space mode
//    //shader->setBool("IsLocalSpace", _ps->_spaceMode == SpaceMode::LOCAL);
//    
//    // texture animation
////    auto textureAnimation = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_ps->_overtimeModules["textureAnimationOvertime"]);
////    if (textureAnimation)
////    {
////        shader->setVec2("FrameTile", vec2(textureAnimation->_numTilesX, textureAnimation->_numTilesY));
////    
////    }
//    
//    // 初始化overtime的texture2D
//    bool enable = false;
//    // texture overtime
//    auto textureOvertime = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_ps->_overtimeModules["textureAnimationOvertime"]);
//    enable = textureOvertime ? textureOvertime->_enable : false;
//    if (enable)
//    {
//        shader->setBool("UseAnimationOvertime", true);
//        
//        _animationTexture = CurveRange::PackCurveRangeXY(SampleNum, textureOvertime->_startFrame, textureOvertime->_frameOvertime, true);
//
//        // 传递其他info
//        vec3 animInfo = vec3(_animationTexture->GetHeight(), textureOvertime->_numTilesX * textureOvertime->_numTilesY, textureOvertime->_cycleCount);
//        shader->setVec3("AnimInfo", animInfo);
//        
//        shader->setInt("animation_mode", _animationTexture->GetHeight());
//    }
//    else
//    {
//        shader->setBool("UseAnimationOvertime", false);
//    }
//    
//    // velocity overtime
//    auto velocityOvertime = std::dynamic_pointer_cast<PS3VelocityOvertime>(_ps->_overtimeModules["velocityOvertime"]);
//    enable = velocityOvertime ? velocityOvertime->_enable : false;
//    if (enable)
//    {
//        shader->setBool("UseVelocityOvertime", true);
//        
//        _velocityTexture = CurveRange::PackCurveRangeXYZ(SampleNum, velocityOvertime->_xVelocity, velocityOvertime->_yVelocity, velocityOvertime->_zVelocity);
//
//        // TODO: velocity有个参数，space
//        shader->setInt("velocity_mode", _velocityTexture->GetHeight());
//    }
//    else
//    {
//        shader->setBool("UseVelocityOvertime", false);
//    }
//    
//    // force overtime
//    auto forceOvertime = std::dynamic_pointer_cast<PS3ForceOvertime>(_ps->_overtimeModules["forceOvertime"]);
//    enable = forceOvertime ? forceOvertime->_enable : false;
//    if (enable)
//    {
//        shader->setBool("UseForceOvertime", true);
//        
//        _forceTexture = CurveRange::PackCurveRangeXYZ(SampleNum, forceOvertime->_xForce, forceOvertime->_yForce, forceOvertime->_zForce);
//        
//        // TODO: force有个参数，space
//        shader->setInt("force_mode", _forceTexture->GetHeight());
//    }
//    else
//    {
//        shader->setBool("UseForceOvertime", false);
//    }
//    
//    // size overtime
//    auto sizeOvertime = std::dynamic_pointer_cast<PS3SizeOvertime>(_ps->_overtimeModules["sizeOvertime"]);
//    enable = sizeOvertime ? sizeOvertime->_enable : false;
//
//    if (enable)
//    {
//        shader->setBool("UseSizeOvertime", true);
//        
//
//        
//        if (sizeOvertime->_separateAxes)
//        {
//            _sizeTexture = CurveRange::PackCurveRangeXYZ(SampleNum, sizeOvertime->_xSize, sizeOvertime->_ySize, sizeOvertime->_zSize, true);
//        }
//        else
//        {
//            _sizeTexture = CurveRange::PackCurveRangeN(SampleNum, sizeOvertime->_size, true);
//        }
//        
//        shader->setInt("size_mode", _sizeTexture->GetHeight());
//    }
//    else
//    {
//        shader->setBool("UseSizeOvertime", false);
//    }
//    
//    // color overtime
//    auto colorOvertime = std::dynamic_pointer_cast<PS3ColorOvertime>(_ps->_overtimeModules["colorOvertime"]);
//    enable = colorOvertime ? colorOvertime->_enable : false;
//    if (enable)
//    {
//        shader->setBool("UseColorOvertime", true);
//        
//        _colorTexture = GradientRange::PackGradientRange(SampleNum, colorOvertime->_color);
//
//        shader->setInt("color_mode", _colorTexture->GetHeight());
//    }
//    else
//    {
//        shader->setBool("UseColorOvertime", false);
//    }
//    
//    // rotation overtime
//    auto rotationOvertime = std::dynamic_pointer_cast<PS3RotationOvertime>(_ps->_overtimeModules["rotationOvertime"]);
//    enable = rotationOvertime ? rotationOvertime->_enable : false;
//    if (enable)
//    {
//        shader->setBool("UseRotationOvertime", true);
//        
//        if (rotationOvertime->_separateAxes)
//        {
//            _rotationTexture = CurveRange::PackCurveRangeXYZ(SampleNum, rotationOvertime->_xRotate, rotationOvertime->_yRotate, rotationOvertime->_zRotate, true);
//        }
//        else
//        {
//            _rotationTexture = CurveRange::PackCurveRangeZ(SampleNum, rotationOvertime->_zRotate, true);
//        }
//
//        shader->setInt("rotation_mode", _rotationTexture->GetHeight());
//    }
//    else
//    {
//        shader->setBool("UseRotationOvertime", false);
//    }
//    
//    // sampleInfo
//    vec2 sampleInfo = vec2(SampleNum, SampleInterval);
//    shader->setVec2("SampleInfo", sampleInfo);
//    
//    // 启用overtime 的texture
//    if (_velocityTexture)
//        _velocityTexture->BindToUniform("texture_velocity_tex0", _model->_renderer->_shader,_model->_renderer->GetTextureUnit("texture_velocity_tex0"));
//    if (_sizeTexture)
//        _sizeTexture->BindToUniform("texture_size_tex0", _model->_renderer->_shader,_model->_renderer->GetTextureUnit("texture_size_tex0"));
//    if (_colorTexture)
//        _colorTexture->BindToUniform("texture_color_tex0", _model->_renderer->_shader, _model->_renderer->GetTextureUnit("texture_color_tex0"));
//    if (_forceTexture)
//        _forceTexture->BindToUniform("texture_force_tex0",_model->_renderer->_shader, _model->_renderer->GetTextureUnit("texture_force_tex0"));
//    if (_rotationTexture)
//        _rotationTexture->BindToUniform("texture_rotation_tex0", _model->_renderer->_shader, _model->_renderer->GetTextureUnit("texture_rotation_tex0"));
//    if (_animationTexture)
//        _animationTexture->BindToUniform("texture_animation_tex0", _model->_renderer->_shader,_model->_renderer->GetTextureUnit("texture_animation_tex0"));
//    
//    // mainTexture
//    _ps->_texture->BindToUniform("MainTexture", _model->_renderer->_shader, _model->_renderer->GetTextureUnit("MainTexture"));
//    
//    // overtime info
//    
//}

void PS3RendererGPU::UpdateUniform()
{
    // InitUniform();
    
    if (!_ps->_isSubEmitter)
    {
        //_model->_renderer->SetWorldTransform(_ps->GetWorldTransform());
        _ps->_entity->SetWorldTransform(_ps->GetWorldTransform());
    }
    else
    {
        // 子发射器的粒子的世界矩阵，使用的是父发射器
        //_model->_renderer->SetWorldTransform(_ps->_mainEmitter->GetWorldTransform());
        _ps->_entity->SetWorldTransform(_ps->_mainEmitter->GetWorldTransform());
    }
    
    // time and delta
    vec2 timeDelta = vec2(_ps->_time, _ps->_dt);
    _ps->_entity->SetTimeAndDelta(timeDelta);
    //shader->setVec2("Time_Delta", timeDelta);
    
}

void PS3RendererGPU::Render()
{
    UpdateUniform();
    _ras->SyncOnChange(_rac);
    _rac->RenderUpdate(_model->_renderer->_shader);
    _model->RenderModel();
}

void PS3RendererGPU::SetNewParticle(PS3ParticlePtr p)
{
    // 把新粒子的数据传给model
    _model->AddParticleVertexData(p, _model->_particleCount, _ps->_time);
    ++ _model->_particleCount;
}

void PS3RendererGPU::SetNewParticles(std::vector<PS3ParticlePtr> &particles)
{
    for (auto &p : particles)
    {
        SetNewParticle(p);
    }
}
