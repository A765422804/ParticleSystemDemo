//
//  PS3RendererCPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "PS3RendererCPU.hpp"
#include "../PS3ParticleSystem.hpp"

PS3RendererCPU::PS3RendererCPU(PS3ParticleSystem* ps, int maxParticleCount)
: SuperType(ps, maxParticleCount, false)
{
    
}

void PS3RendererCPU::Clear()
{
    SuperType::Clear();
}

void PS3RendererCPU::UpdateRenderData(std::vector<PS3ParticlePtr> particles)
{
    int idx = 0;
    for (int i = 0; i < particles.size(); ++i)
    {
        auto p = particles[i];
        float fi = -1;
        auto textureAnimation = std::dynamic_pointer_cast<PS3TextureAnimationModule>(_ps->_overtimeModules["textureAnimationOvertime"]);
        if (textureAnimation && textureAnimation->_enable)
            fi = p->_frameIndex;
        idx = i * 4;
        FillMeshData(p, idx, fi);
    }
}

void PS3RendererCPU::FillMeshData(PS3ParticlePtr p, int idx, float fi)
{
    for (int j = 0; j < 4; ++ j)
    {
        _particleVertexData.Position = p->_position;
        vec3 tempAttribUV = vec3(uvs[2 * j], uvs[2 * j + 1], fi);
        _particleVertexData.Texcoord = tempAttribUV;
        _particleVertexData.Size = p->_size;
        p->_ultimateQuat = normalize(p->_ultimateQuat);
        _particleVertexData.Rotation = vec4(p->_ultimateQuat.x, p->_ultimateQuat.y, p->_ultimateQuat.z, p->_ultimateQuat.w);
        _particleVertexData.Color = p->_color;
        _model->AddParticleVertexData(idx ++, _particleVertexData);
    }
}

void PS3RendererCPU::SetNewParticle(PS3ParticlePtr particle)
{
    // do nothing
}

void PS3RendererCPU::InitUniform()
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
    
    // texture
    _ps->_texture->BindToUniform("MainTexture", _model->_renderer->_shader, _model->_renderer->GetTextureUnit("MainTexture"));
}

void PS3RendererCPU::UpdateUniform()
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
    
    // texture
    _ps->_texture->BindToUniform("MainTexture", _model->_renderer->_shader, _model->_renderer->GetTextureUnit("MainTexture"));
    
    if (!_ps->_isSubEmitter)
        _model->_renderer->SetWorldTransform(_ps->GetWorldTransform());
    else
    {
        // 子发射器的粒子的世界矩阵，使用的是父发射器
        _model->_renderer->SetWorldTransform(_ps->_mainEmitter->GetWorldTransform());
    }
}

void PS3RendererCPU::Render()
{
    UpdateUniform();
    _model->RenderModelCPU(_ps->GetParticleCount());
}
