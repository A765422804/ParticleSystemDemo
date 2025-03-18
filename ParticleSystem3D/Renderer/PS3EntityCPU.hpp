//
//  PS3EntityCPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "../RAC&RAS/PS3CPUProperty.hpp"
#include "../../Function/Texture2D.hpp"

class PS3EntityCPU
{
public:
    PS3EntityCPU()
    {
        _property = std::make_shared<PS3CPUProperty>();
    }
    
public:
    // set
    void SetWorldTransform(mat4 worldTransform);
    void SetIsLocalSpace(bool isLocalSpace);
    void SetFrameTile(vec2 frameTile);
    void SetTexture(Texture2DPtr texture);
    
public:
    PS3CPUPropertyPtr _property;
};

using PS3EntityCPUPtr = std::shared_ptr<PS3EntityCPU>;

inline void PS3EntityCPU::SetWorldTransform(mat4 worldTransform)
{
    _property->SetWorldTransform(worldTransform);
}

inline void PS3EntityCPU::SetIsLocalSpace(bool isLocalSpace)
{
    _property->SetIsLocalSpace(isLocalSpace);
}

inline void PS3EntityCPU::SetFrameTile(vec2 frameTile)
{
    _property->SetFrameTile(frameTile);
}

inline void PS3EntityCPU::SetTexture(Texture2DPtr texture)
{
    _property->SetTexture(texture);
}
