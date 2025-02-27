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
    ret->_renderer = std::make_shared<Renderer>();
    ret->_renderer->SetupVertexDescV3_C4_T2();
    ret->_renderer->SetupShaderProgram("./shader_file/particle_shader.vs", "./shader_file/particle_shader.fs");
    
#ifdef DEBUG_MODE
    ret->_emitterRenderer = std::make_shared<Renderer>();
    ret->_emitterRenderer->SetupVertexDescV3();
    ret->_emitterRenderer->SetupShaderProgram("./shader_file/debug_shader.vs", "./shader_file/debug_shader.fs");
#endif
    
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
    
    if (_positionType == PositionType::FREE)
    {
        vec3 newPos;
        for (int i = 0 ; i < _particleCount; ++i)
        {
            newPos = vec3(_particles[i]._startPosX + _particles[i]._posX, _particles[i]._startPosY + _particles[i]._posY, 0.0f);
            UpdatePosWithParticle(newPos, i);
        }
    }
    else if (_positionType == PositionType::RELATIVE)
    {
        vec3 currentPosition = GetParent()->ConvertToWorldSpace(vec3(0.0f));
        vec3 newPos;
        for (int i = 0 ; i < _particleCount; ++i)
        {
            newPos = vec3(currentPosition.x + _particles[i]._startPosX + _particles[i]._posX, currentPosition.y + _particles[i]._startPosY + _particles[i]._posY, 0.0f);
            UpdatePosWithParticle(newPos, i);
        }
    }
    else
    {
        vec3 currentPosition = ConvertToWorldSpace(vec3(0.0f));
        vec3 newPos;
        for (int i = 0; i < _particleCount; ++i)
        {
            newPos = vec3(currentPosition.x + _particles[i]._posX, currentPosition.y + _particles[i]._posY, 0.0f);
            UpdatePosWithParticle(newPos, i);
        }
    }
    
    // color
    for (int i = 0 ; i < _particleCount; ++i)
    {
        float colorR = _particles[i]._colorR;
        float colorG = _particles[i]._colorG;
        float colorB = _particles[i]._colorB;
        float colorA = _particles[i]._colorA;
        
        _quads[i].bl.Colors = vec4(colorR, colorG, colorB, colorA);
        _quads[i].br.Colors = vec4(colorR, colorG, colorB, colorA);
        _quads[i].tl.Colors = vec4(colorR, colorG, colorB, colorA);
        _quads[i].tr.Colors = vec4(colorR, colorG, colorB, colorA);
    }
    
    // texCoord
    for (int i = 0; i < _particleCount; ++i)
    {
        _quads[i].bl.TexCoords = vec2(0.0f, 0.0f);
        _quads[i].br.TexCoords = vec2(1.0f, 0.0f);
        _quads[i].tl.TexCoords = vec2(0.0f, 1.0f);
        _quads[i].tr.TexCoords = vec2(1.0f, 1.0f);
    }
}

void ParticleSystemQuad::UpdatePosWithParticle(vec3 newPos, int quadIndex)
{
    float size = _particles[quadIndex]._size;
    float rotation = _particles[quadIndex]._rotation;
    
    float size_2 = size/ 2;
    float x1 = -size_2;
    float y1 = -size_2;
    float x2 = size_2;
    float y2 = size_2;
    float x = newPos.x;
    float y = newPos.y;
    
    float r = -radians(rotation);
    float cr = cos(r);
    float sr = sin(r);
    
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
    if (_texture != nullptr)
    {
        _renderer->SetUseTexture(true);
        //_texture->Bind();
    }
    else
    {
        _renderer->SetUseTexture(false);
    }
    
    // 传递属性
    if (_particleCount > 0)
    {
        std::vector<V3_C4_T2> vertices;
        for (int i = 0 ; i < _particleCount; ++ i)
        {
            vertices.push_back(_quads[i].tl);
            vertices.push_back(_quads[i].bl);
            vertices.push_back(_quads[i].tr);
            vertices.push_back(_quads[i].br);
        }
        std::vector<unsigned int> indices(_indices.begin(), _indices.begin() + std::min(_particleCount * 6, static_cast<int>(_indices.size())));
        
        _renderer->SetVertexData(vertices);
        _renderer->SetIndexData(indices);
        _renderer->Render();
    }
    
#ifdef DEBUG_MODE
    if (_emitterMode == EmitterMode::GRAVITY)
    {
        std::vector<V3> emitterPosition;
        emitterPosition.resize(4);
        emitterPosition[0].Position = vec3(GetWorldPosition().x - _positionVar.x, GetWorldPosition().y + _positionVar.y, 0.0f);
        emitterPosition[1].Position = vec3(GetWorldPosition().x - _positionVar.x, GetWorldPosition().y - _positionVar.y, 0.0f);
        emitterPosition[2].Position = vec3(GetWorldPosition().x + _positionVar.x, GetWorldPosition().y + _positionVar.y, 0.0f);
        emitterPosition[3].Position = vec3(GetWorldPosition().x + _positionVar.x, GetWorldPosition().y - _positionVar.y, 0.0f);
        std::vector<unsigned int> emitterIndices = {
            0, 1, 2,
            1, 2, 3
        };
        _emitterRenderer->SetVertexData(emitterPosition);
        _emitterRenderer->SetIndexData(emitterIndices);
        _emitterRenderer->SetIsWireframeEnable(true);
        _emitterRenderer->SetWireFrameColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        _emitterRenderer->Render();
    }
    else
    {
        std::vector<V3> emitterPosition;
        V3 v3;
        unsigned int segmentCount = 1000;
        for (int i = 0; i <= segmentCount; ++i)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(segmentCount);
            float x = _rotationAttribute.StartRadius * cos(theta) + GetWorldPosition().x;
            float y = _rotationAttribute.StartRadius * sin(theta) + GetWorldPosition().y;
            v3.Position = vec3(x, y, 0);
            emitterPosition.push_back(v3);
        }
        
        _emitterRenderer->SetVertexData(emitterPosition);
        _emitterRenderer->SetWireFrameColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        _emitterRenderer->RenderCircle(segmentCount);
        
        emitterPosition.clear();
        for (int i = 0; i <= segmentCount; ++i)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(segmentCount);
            float x = _rotationAttribute.EndRadius * cos(theta) + GetWorldPosition().x;
            float y = _rotationAttribute.EndRadius * sin(theta) + GetWorldPosition().y;
            v3.Position = vec3(x, y, 0);
            emitterPosition.push_back(v3);
        }
        
        _emitterRenderer->SetVertexData(emitterPosition);
        _emitterRenderer->SetWireFrameColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        _emitterRenderer->RenderCircle(segmentCount);
    }
#endif
}
