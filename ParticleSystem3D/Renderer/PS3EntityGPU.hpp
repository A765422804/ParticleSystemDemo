//
//  PS3EntityGPU.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "../RAC&RAS/PS3GPUProperty.hpp"
#include "../../Function/Texture2D.hpp"

class PS3EntityGPU
{
public:
    PS3EntityGPU()
    {
        _property = std::make_shared<PS3GPUProperty>();
    }
    
public:
    // set
    void SetUseAnimationOvertime(bool use);
    void SetUseVelocityOvertime(bool use);
    void SetUseForceOvertime(bool use);
    void SetUseSizeOvertime(bool use);
    void SetUseColorOvertime(bool use);
    void SetUseRotationOvertime(bool use);

    void SetAnimationMode(int mode);
    void SetVelocityMode(int mode);
    void SetForceMode(int mode);
    void SetSizeMode(int mode);
    void SetColorMode(int mode);
    void SetRotationMode(int mode);

    void SetAnimationTexture(Texture2DPtr texture);
    void SetVelocityTexture(Texture2DPtr texture);
    void SetForceTexture(Texture2DPtr texture);
    void SetSizeTexture(Texture2DPtr texture);
    void SetColorTexture(Texture2DPtr texture);
    void SetRotationTexture(Texture2DPtr texture);

    void SetSampleInfo(const vec2& sampleInfo);
    void SetAnimInfo(const vec3& animInfo);
    void SetTimeAndDelta(const vec2& timeAndDelta);
    void SetFrameTile(const vec2& frameTile);

    void SetWorldTransform(const mat4& transform);
    void SetIsLocalSpace(bool isLocalSpace);
    void SetTexture(Texture2DPtr texture);
    
    void SetGravityTexture(Texture2DPtr texture);
    void SetGravityMode(int mode);
    
public:
    PS3GPUPropertyPtr _property;
};

using PS3EntityGPUPtr = std::shared_ptr<PS3EntityGPU>;

inline void PS3EntityGPU::SetUseAnimationOvertime(bool use)
{
    _property->SetUseAnimationOvertime(use);
}

inline void PS3EntityGPU::SetUseVelocityOvertime(bool use)
{
    _property->SetUseVelocityOvertime(use);
}

inline void PS3EntityGPU::SetUseForceOvertime(bool use)
{
    _property->SetUseForceOvertime(use);
}

inline void PS3EntityGPU::SetUseSizeOvertime(bool use)
{
    _property->SetUseSizeOvertime(use);
}

inline void PS3EntityGPU::SetUseColorOvertime(bool use)
{
    _property->SetUseColorOvertime(use);
}

inline void PS3EntityGPU::SetUseRotationOvertime(bool use)
{
    _property->SetUseRotationOvertime(use);
}

inline void PS3EntityGPU::SetAnimationMode(int mode)
{
    _property->SetAnimationMode(mode);
}

inline void PS3EntityGPU::SetVelocityMode(int mode)
{
    _property->SetVelocityMode(mode);
}

inline void PS3EntityGPU::SetForceMode(int mode)
{
    _property->SetForceMode(mode);
}

inline void PS3EntityGPU::SetSizeMode(int mode)
{
    _property->SetSizeMode(mode);
}

inline void PS3EntityGPU::SetColorMode(int mode)
{
    _property->SetColorMode(mode);
}

inline void PS3EntityGPU::SetRotationMode(int mode)
{
    _property->SetRotationMode(mode);
}

inline void PS3EntityGPU::SetAnimationTexture(Texture2DPtr texture)
{
    _property->SetAnimationTexture(texture);
}

inline void PS3EntityGPU::SetVelocityTexture(Texture2DPtr texture)
{
    _property->SetVelocityTexture(texture);
}

inline void PS3EntityGPU::SetForceTexture(Texture2DPtr texture)
{
    _property->SetForceTexture(texture);
}

inline void PS3EntityGPU::SetSizeTexture(Texture2DPtr texture)
{
    _property->SetSizeTexture(texture);
}

inline void PS3EntityGPU::SetColorTexture(Texture2DPtr texture)
{
    _property->SetColorTexture(texture);
}

inline void PS3EntityGPU::SetRotationTexture(Texture2DPtr texture)
{
    _property->SetRotationTexture(texture);
}

inline void PS3EntityGPU::SetSampleInfo(const vec2& sampleInfo)
{
    _property->SetSampleInfo(sampleInfo);
}

inline void PS3EntityGPU::SetAnimInfo(const vec3& animInfo)
{
    _property->SetAnimInfo(animInfo);
}

inline void PS3EntityGPU::SetTimeAndDelta(const vec2& timeAndDelta)
{
    _property->SetTimeAndDelta(timeAndDelta);
}

inline void PS3EntityGPU::SetFrameTile(const vec2& frameTile)
{
    _property->SetFrameTile(frameTile);
}

inline void PS3EntityGPU::SetWorldTransform(const mat4& transform)
{
    _property->SetWorldTransform(transform);
}

inline void PS3EntityGPU::SetIsLocalSpace(bool isLocalSpace)
{
    _property->SetIsLocalSpace(isLocalSpace);
}

inline void PS3EntityGPU::SetTexture(Texture2DPtr texture)
{
    _property->SetTexture(texture);
}

inline void PS3EntityGPU::SetGravityTexture(Texture2DPtr texture)
{
    _property->SetGravityTexture(texture);
}

inline void PS3EntityGPU::SetGravityMode(int mode)
{
    _property->SetGravityMode(mode);
}
