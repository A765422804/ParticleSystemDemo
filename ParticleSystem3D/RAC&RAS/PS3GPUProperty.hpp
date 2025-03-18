//
//  PS3GPUProperty.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "../../Function/Texture2D.hpp"

class PS3GPUProperty : public PropertyObservable
{
public:
    // Set methods
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

    // Get methods
    bool GetUseAnimationOvertime() const;
    bool GetUseVelocityOvertime() const;
    bool GetUseForceOvertime() const;
    bool GetUseSizeOvertime() const;
    bool GetUseColorOvertime() const;
    bool GetUseRotationOvertime() const;

    int GetAnimationMode() const;
    int GetVelocityMode() const;
    int GetForceMode() const;
    int GetSizeMode() const;
    int GetColorMode() const;
    int GetRotationMode() const;

    Texture2DPtr GetAnimationTexture() const;
    Texture2DPtr GetVelocityTexture() const;
    Texture2DPtr GetForceTexture() const;
    Texture2DPtr GetSizeTexture() const;
    Texture2DPtr GetColorTexture() const;
    Texture2DPtr GetRotationTexture() const;

    vec2 GetSampleInfo() const;
    vec3 GetAnimInfo() const;
    vec2 GetTimeAndDelta() const;
    vec2 GetFrameTile() const;

    mat4 GetWorldTransform() const;
    bool GetIsLocalSpace() const;
    Texture2DPtr GetTexture() const;
    
    Texture2DPtr GetGravityTexture() const;
    int GetGravityMode() const;
    
public:
    bool _useAnimationOvertime;
    bool _useVelocityOvertime;
    bool _useForceOvertime;
    bool _useSizeOvertime;
    bool _useColorOvertime;
    bool _useRotationOvertime;
    
    int _animationMode;
    int _velocityMode;
    int _forceMode;
    int _sizeMode;
    int _colorMode;
    int _rotationMode;
    
    Texture2DPtr _animationTexture;
    Texture2DPtr _velocityTexture;
    Texture2DPtr _forceTexture;
    Texture2DPtr _sizeTexture;
    Texture2DPtr _colorTexture;
    Texture2DPtr _rotationTexture;
    
    vec2 _sampleInfo;
    vec3 _animInfo;
    
    vec2 _timeAndDelta;
    vec2 _frameTile;
    
    mat4 worldTransform;
    bool _isLocalSpace;
    
    Texture2DPtr _texture;
    
    Texture2DPtr _gravityTexture;
    int _gravityMode;
};

using PS3GPUPropertyPtr = std::shared_ptr<PS3GPUProperty>;

inline void PS3GPUProperty::SetUseAnimationOvertime(bool use)
{
    _useAnimationOvertime = use;
}

inline void PS3GPUProperty::SetUseVelocityOvertime(bool use)
{
    _useVelocityOvertime = use;
}

inline void PS3GPUProperty::SetUseForceOvertime(bool use)
{
    _useForceOvertime = use;
}

inline void PS3GPUProperty::SetUseSizeOvertime(bool use)
{
    _useSizeOvertime = use;
}

inline void PS3GPUProperty::SetUseColorOvertime(bool use)
{
    _useColorOvertime = use;
}

inline void PS3GPUProperty::SetUseRotationOvertime(bool use)
{
    _useRotationOvertime = use;
}

inline bool PS3GPUProperty::GetUseAnimationOvertime() const
{
    return _useAnimationOvertime;
}

inline bool PS3GPUProperty::GetUseVelocityOvertime() const
{
    return _useVelocityOvertime;
}

inline bool PS3GPUProperty::GetUseForceOvertime() const
{
    return _useForceOvertime;
}

inline bool PS3GPUProperty::GetUseSizeOvertime() const
{
    return _useSizeOvertime;
}

inline bool PS3GPUProperty::GetUseColorOvertime() const
{
    return _useColorOvertime;
}

inline bool PS3GPUProperty::GetUseRotationOvertime() const
{
    return _useRotationOvertime;
}

inline void PS3GPUProperty::SetAnimationMode(int mode)
{
    _animationMode = mode;
}

inline void PS3GPUProperty::SetVelocityMode(int mode)
{
    _velocityMode = mode;
}

inline void PS3GPUProperty::SetForceMode(int mode)
{
    _forceMode = mode;
}

inline void PS3GPUProperty::SetSizeMode(int mode)
{
    _sizeMode = mode;
}

inline void PS3GPUProperty::SetColorMode(int mode)
{
    _colorMode = mode;
}

inline void PS3GPUProperty::SetRotationMode(int mode)
{
    _rotationMode = mode;
}

inline int PS3GPUProperty::GetAnimationMode() const
{
    return _animationMode;
}

inline int PS3GPUProperty::GetVelocityMode() const
{
    return _velocityMode;
}

inline int PS3GPUProperty::GetForceMode() const
{
    return _forceMode;
}

inline int PS3GPUProperty::GetSizeMode() const
{
    return _sizeMode;
}

inline int PS3GPUProperty::GetColorMode() const
{
    return _colorMode;
}

inline int PS3GPUProperty::GetRotationMode() const
{
    return _rotationMode;
}

inline void PS3GPUProperty::SetAnimationTexture(Texture2DPtr texture)
{
    _animationTexture = texture;
}

inline void PS3GPUProperty::SetVelocityTexture(Texture2DPtr texture)
{
    _velocityTexture = texture;
}

inline void PS3GPUProperty::SetForceTexture(Texture2DPtr texture)
{
    _forceTexture = texture;
}

inline void PS3GPUProperty::SetSizeTexture(Texture2DPtr texture)
{
    _sizeTexture = texture;
}

inline void PS3GPUProperty::SetColorTexture(Texture2DPtr texture)
{
    _colorTexture = texture;
}

inline void PS3GPUProperty::SetRotationTexture(Texture2DPtr texture)
{
    _rotationTexture = texture;
}

inline Texture2DPtr PS3GPUProperty::GetAnimationTexture() const
{
    return _animationTexture;
}

inline Texture2DPtr PS3GPUProperty::GetVelocityTexture() const
{
    return _velocityTexture;
}

inline Texture2DPtr PS3GPUProperty::GetForceTexture() const
{
    return _forceTexture;
}

inline Texture2DPtr PS3GPUProperty::GetSizeTexture() const
{
    return _sizeTexture;
}

inline Texture2DPtr PS3GPUProperty::GetColorTexture() const
{
    return _colorTexture;
}

inline Texture2DPtr PS3GPUProperty::GetRotationTexture() const
{
    return _rotationTexture;
}

inline void PS3GPUProperty::SetSampleInfo(const vec2& sampleInfo)
{
    _sampleInfo = sampleInfo;
}

inline void PS3GPUProperty::SetAnimInfo(const vec3& animInfo)
{
    _animInfo = animInfo;
}

inline void PS3GPUProperty::SetTimeAndDelta(const vec2& timeAndDelta)
{
    _timeAndDelta = timeAndDelta;
}

inline void PS3GPUProperty::SetFrameTile(const vec2& frameTile)
{
    _frameTile = frameTile;
}

inline vec2 PS3GPUProperty::GetSampleInfo() const
{
    return _sampleInfo;
}

inline vec3 PS3GPUProperty::GetAnimInfo() const
{
    return _animInfo;
}

inline vec2 PS3GPUProperty::GetTimeAndDelta() const
{
    return _timeAndDelta;
}

inline vec2 PS3GPUProperty::GetFrameTile() const
{
    return _frameTile;
}

inline void PS3GPUProperty::SetWorldTransform(const mat4& transform)
{
    worldTransform = transform;
}

inline void PS3GPUProperty::SetIsLocalSpace(bool isLocalSpace)
{
    _isLocalSpace = isLocalSpace;
}

inline mat4 PS3GPUProperty::GetWorldTransform() const
{
    return worldTransform;
}

inline bool PS3GPUProperty::GetIsLocalSpace() const
{
    return _isLocalSpace;
}

inline void PS3GPUProperty::SetTexture(Texture2DPtr texture)
{
    _texture = texture;
}

inline Texture2DPtr PS3GPUProperty::GetTexture() const
{
    return _texture;
}

inline void PS3GPUProperty::SetGravityTexture(Texture2DPtr texture)
{
    _gravityTexture = texture;
}

inline Texture2DPtr PS3GPUProperty::GetGravityTexture() const
{
    return _gravityTexture;
}

inline void PS3GPUProperty::SetGravityMode(int mode)
{
    _gravityMode = mode;
}

inline int PS3GPUProperty::GetGravityMode() const
{
    return _gravityMode;
}
