//
//  PS3CPURAC.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#include "PS3CPURAC.hpp"
#include "../PS3ParticleSystemCPU.hpp"

PS3CPURAC::PS3CPURAC(PS3ParticleSystemCPU* ps)
: _ps(ps)
, _worldTransform(mat4(0.0f))
, _isLocalSpace(false)
, _frameTile(vec2(0.0f))
{
    
}

void PS3CPURAC::RenderUpdate(ShaderPtr shader)
{
    shader->use();
    shader->setMat4("WorldTransform", _worldTransform);
    shader->setBool("IsLocalSpace", _isLocalSpace);
    shader->setVec2("FrameTile", _frameTile);
    
    if (_texture)
    {
        _texture->BindToUniform("MainTexture", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("MainTexture"));
    }
}

