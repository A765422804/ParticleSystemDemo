//
//  PS3CPURAC.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "../../Function/Shader.hpp"
#include "../../Function/Texture2D.hpp"

class PS3ParticleSystemCPU;

class PS3CPURAC
{
public:
    PS3CPURAC(PS3ParticleSystemCPU* ps);
    ~PS3CPURAC() = default;

public:
    void SetWorldTransform(const mat4 &worldTransform);
    void SetIsLocalSpace(bool isLocalSpace);
    void SetFrameTile(const vec2 &frameTile);
    void SetTexture(Texture2DPtr texture);
    
    void RenderUpdate(ShaderPtr shader);
    
public:
    PS3ParticleSystemCPU* _ps;
    
    // shader data
    mat4 _worldTransform;
    bool _isLocalSpace;
    vec2 _frameTile;
    Texture2DPtr _texture;
};

using PS3CPURACPtr = std::shared_ptr<PS3CPURAC>;

inline void PS3CPURAC::SetWorldTransform(const mat4 &worldTransform)
{
    _worldTransform = worldTransform;
}

inline void PS3CPURAC::SetIsLocalSpace(bool isLocalSpace)
{
    _isLocalSpace = isLocalSpace;
}

inline void PS3CPURAC::SetFrameTile(const vec2 &frameTile)
{
    _frameTile = frameTile;
}

inline void PS3CPURAC::SetTexture(Texture2DPtr texture)
{
    _texture = texture;
}
