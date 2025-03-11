//
//  PS3ParticleProcessorGPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/26.
//

#include "PS3ParticleProcessorGPU.hpp"
#include "../PS3ParticleSystem.hpp"
//#include "../Model/PS3ParticleBatchModel.hpp"

PS3ParticleProcessorGPU::PS3ParticleProcessorGPU(PS3ParticleSystem* ps)
: SuperType(ps)
{
    
}

void PS3ParticleProcessorGPU::UpdateParticles(std::vector<PS3ParticlePtr>& particles, float dt)
{
    //_ps->_renderer->_model->UpdateGPUParticles(_ps->_time, dt);
}

bool PS3ParticleProcessorGPU::UpdateParticle(PS3ParticlePtr p, float dt)
{
    // MARK: 这里直接在model里更新粒子的顶点数据了
    
    return false;
}
