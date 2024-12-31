//
//  ParticleSystemQuad.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#include "ParticleSystemQuad.hpp"

ParticleSystemQuad::ParticleSystemQuad()
{
    // TODO: implement
}

ParticleSystemQuadPtr ParticleSystemQuad::Create()
{
    ParticleSystemQuadPtr ret = std::make_shared<ParticleSystemQuad>();
    
    // 初始化renderer并配置顶点描述
    ret->_renderer = Renderer::CreateRenderer();
    ret->_renderer->SetupVertexDesc();
    ret->_renderer->SetupShaderProgram("./shader_file/particle_shader.vs", "./shader_file/particle_shader.fs");
    
    ret->Init();
    
    return ret;
}

void ParticleSystemQuad::Init()
{
    SuperType::Init();
    AllocMemory();
    InitIndices();
}

void ParticleSystemQuad::AllocMemory()
{
    _quads.clear();
    _indices.clear();
    
    _quads.resize(_maxParticleCount);
    _indices.resize(_maxParticleCount * 6);
}

void ParticleSystemQuad::InitIndices()
{
    for (int i = 0; i < _maxParticleCount; ++i)
    {
        const unsigned int i6 = i * 6;
        const unsigned int i4 = i * 4;
        
        _indices[i6+0] = i4+0;
        _indices[i6+1] = i4+1;
        _indices[i6+2] = i4+2;

        _indices[i6+5] = i4+1;
        _indices[i6+4] = i4+2;
        _indices[i6+3] = i4+3;
    }
}

// main loop
void ParticleSystemQuad::UpdateParticleQuads()
{
    if (_particleCount <= 0)
        return;
    
    // position
    // TODO: use positionType to deal with 坐标系变换
    
    for (int i = 0 ; i < _particleCount; ++i)
    {
        UpdatePosWithParticle(i);
    }
    
    // color
    for (int i = 0 ; i < _particleCount; ++i)
    {
        float colorR = _particles[i]._colorR;
        float colorG = _particles[i]._colorG;
        float colorB = _particles[i]._colorB;
        float colorA = _particles[i]._colorA;
        
        _quads[i].bl.Colors = glm::vec4(colorR, colorG, colorB, colorA);
        _quads[i].br.Colors = glm::vec4(colorR, colorG, colorB, colorA);
        _quads[i].tl.Colors = glm::vec4(colorR, colorG, colorB, colorA);
        _quads[i].tr.Colors = glm::vec4(colorR, colorG, colorB, colorA);
    }
    
    // texCoord
    for (int i = 0; i < _particleCount; ++i)
    {
        _quads[i].bl.TexCoords = glm::vec2(0.0f, 0.0f);
        _quads[i].br.TexCoords = glm::vec2(1.0f, 0.0f);
        _quads[i].tl.TexCoords = glm::vec2(0.0f, 1.0f);
        _quads[i].tr.TexCoords = glm::vec2(1.0f, 1.0f);
    }
}

void ParticleSystemQuad::UpdatePosWithParticle(int quadIndex)
{
    glm::vec2 newPos = glm::vec2(_particles[quadIndex]._posX, _particles[quadIndex]._posY);
    float size = _particles[quadIndex]._size;
    float rotation = _particles[quadIndex]._rotation;
    
    float size_2 = size/ 2;
    float x1 = -size_2;
    float y1 = -size_2;
    float x2 = size_2;
    float y2 = size_2;
    float x = newPos.x;
    float y = newPos.y;
    
    float r = -glm::radians(rotation);
    float cr = glm::cos(r);
    float sr = glm::sin(r);
    
    float ax = x1 * cr - y1 * sr + x;
    float ay = x1 * sr + y1 * cr + y;
    float bx = x2 * cr - y1 * sr + x;
    float by = x2 * sr + y1 * cr + y;
    float cx = x2 * cr - y2 * sr + x;
    float cy = x2 * sr + y2 * cr + y;
    float dx = x1 * cr - y2 * sr + x;
    float dy = x1 * sr + y2 * cr + y;
    
    _quads[quadIndex].bl.Vertices.x = ax;
    _quads[quadIndex].bl.Vertices.y = ay;
    
    _quads[quadIndex].br.Vertices.x = bx;
    _quads[quadIndex].br.Vertices.y = by;
    
    _quads[quadIndex].tl.Vertices.x = dx;
    _quads[quadIndex].tl.Vertices.y = dy;
    
    _quads[quadIndex].tr.Vertices.x = cx;
    _quads[quadIndex].tr.Vertices.y = cy;
}

void ParticleSystemQuad::Draw()
{
    // 绑定纹理
    _texture->Bind();
    
    // 传递属性
    if (_particleCount > 0)
    {
        std::vector<V3_C4_T2> vertices;
        for (auto& i : _quads)
        {
            vertices.push_back(i.tl);
            vertices.push_back(i.bl);
            vertices.push_back(i.tr);
            vertices.push_back(i.br);
        }
        
        _renderer->SetVertexData(vertices);
        _renderer->SetIndexData(_indices);
        _renderer->Render();
    }
}
