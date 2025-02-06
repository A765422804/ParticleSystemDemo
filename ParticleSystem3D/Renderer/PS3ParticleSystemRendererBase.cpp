//
//  PS3ParticleSystemRendererBase.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#include "PS3ParticleSystemRendererBase.hpp"

void PS3ParticleSystemRendererBase::Clear()
{
    if (_model)
        _model->_enabled = false;
}
