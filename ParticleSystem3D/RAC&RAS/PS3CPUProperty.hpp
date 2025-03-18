//
//  PS3CPUProperty.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "../../Function/Texture2D.hpp"

class PS3CPUProperty : public PropertyObservable
{
public:
    // set
    void SetWorldTransform(const mat4 &worldTransform);
    void SetIsLocalSpace(bool isLocalSpace);
    void SetFrameTile(const vec2 &frameTile);
    void SetTexture(Texture2DPtr texture);
    
public:
    mat4 _worldTransform;
    bool _isLocalSpace;
    vec2 _frameTile;
    Texture2DPtr _texture;
};

using PS3CPUPropertyPtr = std::shared_ptr<PS3CPUProperty>;

inline void PS3CPUProperty::SetWorldTransform(const mat4 &worldTransform)
{
    _worldTransform = worldTransform;
}

inline void PS3CPUProperty::SetIsLocalSpace(bool isLocalSpace)
{
    _isLocalSpace = isLocalSpace;
}

inline void PS3CPUProperty::SetFrameTile(const vec2 &frameTile)
{
    _frameTile = frameTile;
}

inline void PS3CPUProperty::SetTexture(Texture2DPtr texture)
{
    _texture = texture;
}
