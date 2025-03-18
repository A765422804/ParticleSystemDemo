//
//  PS3GPURAC.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "../../Function/Shader.hpp"
#include "../../Function/Texture2D.hpp"

class PS3ParticleSystemGPU;

class PS3GPURAC
{
public:
    PS3GPURAC(PS3ParticleSystemGPU* ps);
    ~PS3GPURAC() = default;
    
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
    
public:
    void RenderUpdate(ShaderPtr shader);
    
public:
    PS3ParticleSystemGPU* _ps;
    
    // shader data
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

using PS3GPURACPtr = std::shared_ptr<PS3GPURAC>;

inline void PS3GPURAC::SetUseAnimationOvertime(bool use) { _useAnimationOvertime = use; }
inline void PS3GPURAC::SetUseVelocityOvertime(bool use) { _useVelocityOvertime = use; }
inline void PS3GPURAC::SetUseForceOvertime(bool use) { _useForceOvertime = use; }
inline void PS3GPURAC::SetUseSizeOvertime(bool use) { _useSizeOvertime = use; }
inline void PS3GPURAC::SetUseColorOvertime(bool use) { _useColorOvertime = use; }
inline void PS3GPURAC::SetUseRotationOvertime(bool use) { _useRotationOvertime = use; }

inline void PS3GPURAC::SetAnimationMode(int mode) { _animationMode = mode; }
inline void PS3GPURAC::SetVelocityMode(int mode) { _velocityMode = mode; }
inline void PS3GPURAC::SetForceMode(int mode) { _forceMode = mode; }
inline void PS3GPURAC::SetSizeMode(int mode) { _sizeMode = mode; }
inline void PS3GPURAC::SetColorMode(int mode) { _colorMode = mode; }
inline void PS3GPURAC::SetRotationMode(int mode) { _rotationMode = mode; }

inline void PS3GPURAC::SetAnimationTexture(Texture2DPtr texture) { _animationTexture = texture; }
inline void PS3GPURAC::SetVelocityTexture(Texture2DPtr texture) { _velocityTexture = texture; }
inline void PS3GPURAC::SetForceTexture(Texture2DPtr texture) { _forceTexture = texture; }
inline void PS3GPURAC::SetSizeTexture(Texture2DPtr texture) { _sizeTexture = texture; }
inline void PS3GPURAC::SetColorTexture(Texture2DPtr texture) { _colorTexture = texture; }
inline void PS3GPURAC::SetRotationTexture(Texture2DPtr texture) { _rotationTexture = texture; }

inline void PS3GPURAC::SetSampleInfo(const vec2& sampleInfo) { _sampleInfo = sampleInfo; }
inline void PS3GPURAC::SetAnimInfo(const vec3& animInfo) { _animInfo = animInfo; }
inline void PS3GPURAC::SetTimeAndDelta(const vec2& timeAndDelta) { _timeAndDelta = timeAndDelta; }
inline void PS3GPURAC::SetFrameTile(const vec2& frameTile) { _frameTile = frameTile; }

inline void PS3GPURAC::SetWorldTransform(const mat4& transform) { worldTransform = transform; }
inline void PS3GPURAC::SetIsLocalSpace(bool isLocalSpace) { _isLocalSpace = isLocalSpace; }
inline void PS3GPURAC::SetTexture(Texture2DPtr texture) { _texture = texture; }

inline void PS3GPURAC::SetGravityTexture(Texture2DPtr texture) { _gravityTexture = texture; }
inline void PS3GPURAC::SetGravityMode(int mode) { _gravityMode = mode; }
