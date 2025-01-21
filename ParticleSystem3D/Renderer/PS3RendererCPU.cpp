//
//  PS3RendererCPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "PS3RendererCPU.hpp"
#include "../PS3ParticleSystem.hpp"

void PS3RendererCPU::UpdateRenderData()
{
    int idx = 0;
    for (int i = 0; i < _particles.size(); ++i)
    {
        auto p = _particles[i];
        int fi = 0;
        auto textureModule = _particleSystem->_textureAnimationModule;
        if (textureModule && textureModule->_enable)
            fi = p->_frameIndex;
        idx = i * 4;
        FillMeshData(p, idx, fi);
    }
}

void PS3RendererCPU::FillMeshData(PS3ParticlePtr p, int idx, int fi)
{
    for (int j = 0; j < 4; ++ j)
    {
        _particleVertexData.Position = p->_position;
        vec3 tempAttribUV = vec3(uvs[2 * j], uvs[2 * j + 1], fi);
        _particleVertexData.Texcoord = tempAttribUV;
        _particleVertexData.Size = p->_size;
        _particleVertexData.Rotation = p->_rotation;
        _particleVertexData.Color = p->_color;
        _model->AddParticleVertexData(idx ++, _particleVertexData);
    }
}

void PS3RendererCPU::Render()
{
    _model->RenderModel(_particles.size());
}
