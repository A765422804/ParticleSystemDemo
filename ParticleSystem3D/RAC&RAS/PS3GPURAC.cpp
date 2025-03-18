//
//  PS3GPURAC.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#include "PS3GPURAC.hpp"
#include "../PS3ParticleSystemGPU.hpp"

PS3GPURAC::PS3GPURAC(PS3ParticleSystemGPU* ps)
    : _ps(ps), // 初始化 _ps
      _useAnimationOvertime(false), // bool 类型初始化为 false
      _useVelocityOvertime(false),
      _useForceOvertime(false),
      _useSizeOvertime(false),
      _useColorOvertime(false),
      _useRotationOvertime(false),
      _animationMode(0), // int 类型初始化为 0
      _velocityMode(0),
      _forceMode(0),
      _sizeMode(0),
      _colorMode(0),
      _rotationMode(0),
      _animationTexture(nullptr), // Texture2DPtr 初始化为 nullptr
      _velocityTexture(nullptr),
      _forceTexture(nullptr),
      _sizeTexture(nullptr),
      _colorTexture(nullptr),
      _rotationTexture(nullptr),
      _sampleInfo(0.0f, 0.0f), // vec2 初始化为 (0.0, 0.0)
      _animInfo(0.0f, 0.0f, 0.0f), // vec3 初始化为 (0.0, 0.0, 0.0)
      _timeAndDelta(0.0f, 0.0f), // vec2 初始化为 (0.0, 0.0)
      _frameTile(0.0f, 0.0f), // vec2 初始化为 (0.0, 0.0)
      worldTransform(mat4(0.0)), // mat4 初始化为全 0 矩阵
      _isLocalSpace(false), // bool 类型初始化为 false
      _texture(nullptr) // Texture2DPtr 初始化为 nullptr
{
    // 构造函数体为空，所有成员变量已在初始化列表中完成初始化
}

void PS3GPURAC::RenderUpdate(ShaderPtr shader)
{
    // 使用着色器
    shader->use();

    // 设置矩阵和布尔值
    shader->setMat4("WorldTransform", worldTransform);
    shader->setBool("IsLocalSpace", _isLocalSpace);

    // 设置向量
    shader->setVec2("SampleInfo", _sampleInfo);
    shader->setVec3("AnimInfo", _animInfo);
    shader->setVec2("Time_Delta", _timeAndDelta);
    shader->setVec2("FrameTile", _frameTile);

    // 设置布尔标志
    shader->setBool("UseAnimationOvertime", _useAnimationOvertime);
    shader->setBool("UseVelocityOvertime", _useVelocityOvertime);
    shader->setBool("UseForceOvertime", _useForceOvertime);
    shader->setBool("UseSizeOvertime", _useSizeOvertime);
    shader->setBool("UseColorOvertime", _useColorOvertime);
    shader->setBool("UseRotationOvertime", _useRotationOvertime);

    // 设置模式值
    shader->setInt("animation_mode", _animationMode);
    shader->setInt("velocity_mode", _velocityMode);
    shader->setInt("force_mode", _forceMode);
    shader->setInt("size_mode", _sizeMode);
    shader->setInt("color_mode", _colorMode);
    shader->setInt("rotation_mode", _rotationMode);
    shader->setInt("gravity_mode", _gravityMode);

    // 绑定纹理
    if (_texture)
    {
        _texture->BindToUniform("MainTexture", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("MainTexture"));
    }

    if (_animationTexture)
    {
        _animationTexture->BindToUniform("texture_animation_tex0", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_animation_tex0"));
    }

    if (_velocityTexture)
    {
        _velocityTexture->BindToUniform("texture_velocity_tex0", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_velocity_tex0"));
    }

    if (_forceTexture)
    {
        _forceTexture->BindToUniform("texture_force_tex0", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_force_tex0"));
    }

    if (_sizeTexture)
    {
        _sizeTexture->BindToUniform("texture_size_tex0", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_size_tex0"));
    }

    if (_colorTexture)
    {
        _colorTexture->BindToUniform("texture_color_tex0", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_color_tex0"));
    }

    if (_rotationTexture)
    {
        _rotationTexture->BindToUniform("texture_rotation_tex0", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_rotation_tex0"));
    }
    
    if (_gravityTexture)
    {
        _gravityTexture->BindToUniform("texture_gravity", shader, _ps->_renderer->_model->_renderer->GetTextureUnit("texture_gravity"));
    }
}
