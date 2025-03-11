//
//  PS3ParticleBatchModelGPU.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/10.
//

#include "PS3ParticleBatchModelGPU.hpp"
#include "../PS3ParticleSystemGPU.hpp"

PS3ParticleBatchModelGPU::PS3ParticleBatchModelGPU(int maxParticleCount, PS3ParticleSystem* ps)
: _capacity(maxParticleCount)
, _ps(ps)
, _enabled(true)
{
    _vDataF.resize(4 * _vertAttrsFloatCount * maxParticleCount);
    _iDataI.resize(6 * maxParticleCount);
    
    _renderer = std::make_shared<ParticleRenderer>(true);
    
    SetVertexAttributes();
    SetIndexData();
}

void PS3ParticleBatchModelGPU::SetVertexAttributes()
{
    _renderer->SetupVertexDesc(); // 调用renderer的初始化顶点数据
}

void PS3ParticleBatchModelGPU::SetIndexData()
{
    int dst = 0;
    for (int i = 0; i < _capacity; ++i)
    {
        float baseIdx = 4 * i;
        _iDataI[dst ++] = baseIdx;
        _iDataI[dst ++] = baseIdx + 1;
        _iDataI[dst ++] = baseIdx + 2;
        _iDataI[dst ++] = baseIdx + 3;
        _iDataI[dst ++] = baseIdx + 2;
        _iDataI[dst ++] = baseIdx + 1;
    }
}

void PS3ParticleBatchModelGPU::AddParticleVertexData(PS3ParticlePtr p, int num, float time)
{
    int offset = num * _vertAttrsFloatCount * _vertCount;
    
    for (int i = 0 ; i < _vertCount; ++i)
    {
        _vDataF[offset ++] = p->_position.x;
        _vDataF[offset ++] = p->_position.y;
        _vDataF[offset ++] = p->_position.z;
        _vDataF[offset ++] = time;
        
        _vDataF[offset ++] = p->_size.x;
        _vDataF[offset ++] = p->_size.y;
        _vDataF[offset ++] = p->_size.z;
        
        _vDataF[offset ++] = p->_ultimateQuat.x;
        _vDataF[offset ++] = p->_ultimateQuat.y;
        _vDataF[offset ++] = p->_ultimateQuat.z;
        _vDataF[offset ++] = p->_ultimateQuat.w;
        
        _vDataF[offset ++] = p->_color.r;
        _vDataF[offset ++] = p->_color.g;
        _vDataF[offset ++] = p->_color.b;
        _vDataF[offset ++] = p->_color.a;
        
        _vDataF[offset ++] = p->_velocity.x;
        _vDataF[offset ++] = p->_velocity.y;
        _vDataF[offset ++] = p->_velocity.z;
        _vDataF[offset ++] = p->_startLifeTime;
        
        _vDataF[offset ++] = uvs[2 * i];
        _vDataF[offset ++] = uvs[2 * i + 1];
    }
}

void PS3ParticleBatchModelGPU::ClearParticles()
{
    _particleCount = 0;
    _vDataF.clear();
}

void PS3ParticleBatchModelGPU::RenderModel()
{
    if (_particleCount <= 0)
        return;
    
    std::cout<<"particle count:"<<_particleCount<<std::endl;
    
    // 已知顶点数据和索引数据，可以render了
    // 根据count得到vertex和index的数量
    
    int vertexCount = _vertAttrsFloatCount * 4 * _particleCount;
    int indexCount = 6 * _particleCount;
    std::vector<float> vertices(_vDataF.begin(), _vDataF.begin() + vertexCount);
    std::vector<unsigned int> indices(_iDataI.begin(), _iDataI.begin() + indexCount);
    
    _renderer->SetVertexData(vertices);
    _renderer->SetIndexData(indices);
    
    _renderer->Render();
}

void PS3ParticleBatchModelGPU::UpdateParticles(float time, float dt)
{
    int pSize = _vertAttrsFloatCount * _vertCount;
    int pBaseIndex = 0;
    float startTime = 0.0;
    float lifeTime = 0.0;
    int lastBaseIndex = 0;
    float interval = 0.0;
    
    for (int i = 0 ; i < _particleCount; ++i)
    {
        pBaseIndex = i * pSize;
        startTime = _vDataF[pBaseIndex + _startTimeOffset];
        lifeTime = _vDataF[pBaseIndex + _lifeTimeOffset];
        interval = time - startTime;
        if (lifeTime - interval < dt)
        {
            lastBaseIndex = --_particleCount * pSize;
            std::copy(
                _vDataF.begin() + lastBaseIndex,           // 源起始位置
                _vDataF.begin() + lastBaseIndex + pSize,   // 源结束位置
                _vDataF.begin() + pBaseIndex               // 目标起始位置
            );
            
            // 通知子发射器死亡事件
            vec3 deathPos = CalculateDeathPos(pBaseIndex, interval);
            dynamic_cast<PS3ParticleSystemGPU*>(_ps)->NotifySubEmitters(deathPos, EventType::DEATH);
            --i;
        }
    }
}

vec3 PS3ParticleBatchModelGPU::CalculateDeathPos(int index, float interval)
{
    float lifeTime = _vDataF[index + _lifeTimeOffset];
    vec3 position = vec3(_vDataF[index + _positionOffset + 0], _vDataF[index + _positionOffset + 1], _vDataF[index + _positionOffset + 2]);
    float normalizedTime = clamp(interval / lifeTime, 0.0f, 1.0f);
    
    auto texture = (std::dynamic_pointer_cast<PS3RendererGPU>(   (  dynamic_cast<PS3ParticleSystemGPU*>(_ps)  )  ->_renderer)   )       ->_velocityTexture;
    // 计算纹理坐标
    float u = normalizedTime; // 时间t作为纹理坐标的x分量
    float v = 0.0f; // 假设高度为1，v分量为0}
    
    // 将纹理坐标映射到像素索引
    float x = u * (texture->width - 1);
    int x0 = static_cast<int>(x);
    int x1 = x0 + 1;
    float frac = x - x0;
    
    // 确保坐标在纹理范围内
    x0 = glm::clamp(x0, 0, texture->width - 1);
    x1 = glm::clamp(x1, 0, texture->width - 1);

    // 计算像素索引
    int index0 = (0 * texture->width + x0) * 4; // 第一个像素
    int index1 = (0 * texture->width + x1) * 4; // 第二个像素

    // 提取像素值并进行线性插值
    glm::vec3 a(texture->dataFloat[index0], texture->dataFloat[index0 + 1], texture->dataFloat[index0 + 2]);
    glm::vec3 b(texture->dataFloat[index1], texture->dataFloat[index1 + 1], texture->dataFloat[index1 + 2]);

    vec3 vel = glm::mix(a, b, frac);
    
    position += (vel * (normalizedTime * lifeTime));
    
    return position;
}
