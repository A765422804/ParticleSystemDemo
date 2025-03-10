//
//  PS3ParticleSystemRendererBase.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#include "PS3ParticleSystemRendererBase.hpp"

PS3ParticleSystemRendererBase::PS3ParticleSystemRendererBase(PS3ParticleSystem* ps, int maxParticleCount, bool useGPU)
: _ps(ps)
, _model(std::make_shared<PS3ParticleBatchModel>(maxParticleCount, useGPU, ps))
{
    
}

void PS3ParticleSystemRendererBase::Clear()
{
    if (_model)
        _model->_enabled = false;
    // 清空粒子
    _model->_iDataI.clear();
    _model->_vDataF.clear();
}
