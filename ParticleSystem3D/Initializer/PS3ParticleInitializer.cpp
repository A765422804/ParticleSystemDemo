//
//  PS3ParticleInitializer.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3ParticleInitializer.hpp"
#include "PS3ParticleSystem.hpp"

PS3ParticleInitializer::PS3ParticleInitializer(PS3ParticleSystem * ps)
: _ps(ps)
{
    // startSize
    _startSizeX = CurveRange::CreateCurveByConstant(0.5);
    
    // startSpeed
    _startSpeed = CurveRange::CreateCurveByConstant(1.5);
    
    // startRotation
    _startRotationZ = CurveRange::CreateCurveByConstant(0);
    
    // startColor
//    ColorKey colorKey1 = {vec3(1.0f, 0.0f, 0.0f), 0.0f};
//    ColorKey colorKey2 = {vec3(0.0f, 1.0f, 0.0f), 1.0f};
//    AlphaKey alphaKey1 = {1.0f, 0.0f};
//    AlphaKey alphaKey2 = {1.0f, 1.0f};
//    std::vector<ColorKey> colorKeys = {colorKey1, colorKey2};
//    std::vector<AlphaKey> alphaKeys = {alphaKey1, alphaKey2};
//    GradientPtr gradient = Gradient::CreateByColorKeyAndAlphaKey(colorKeys, alphaKeys);
//    GradientRangePtr gradientRange = GradientRange::CreateByOneGradient(gradient);
//    _startColor = gradientRange;
    _startColor = GradientRange::CreateByOneColor(vec4(vec3(1.0f), 0.5f));
    
    _startLifeTime = CurveRange::CreateCurveByConstant(5);
}

void PS3ParticleInitializer::InitializeParticle(PS3ParticlePtr p, float dt)
{
    if (_ps->_isSubEmitter && _ps->_spaceMode == SpaceMode::LOCAL)
    {
        // 如果是子发射器且使用相对位置，更新位置为子发射器的位置
        p->_position = _ps->GetPosition3D();
        
        // 更新旋转方向为子发射器的方向
        auto rotationMat = toMat4(_ps->GetRotation());
        p->_velocity = vec3(rotationMat * vec4(p->_velocity, 1.0f));
    }
    
    // TODO: textureAnimationModule
    
    // velocity
    float curStartSpeed = _startSpeed->Evaluate(dt, Random01());
    p->_velocity = p->_velocity * curStartSpeed;
    
    if (_ps->_spaceMode == SpaceMode::WORLD)
    {
        // 根据坐标系更新位置
        mat4 worldMat;
        quat rotationQuat;
        if (_ps->_isSubEmitter)
        {
            worldMat = _ps->GetWorldTransform();
            auto mainRotationQuat = _ps->_mainEmitter->GetRotation();
            auto subRotationQuat = _ps->GetRotation();
            
            rotationQuat = subRotationQuat * mainRotationQuat;
        }
        else
        {
            worldMat = _ps->GetWorldTransform();
            rotationQuat = _ps->GetRotation();
        }
        p->_position = vec3(worldMat * vec4(p->_position, 1.0f));
        
        // 根据坐标系更新速度方向
        auto rotationMat = toMat4(rotationQuat);
        p->_velocity = vec3(rotationMat * vec4(p->_velocity, 1.0f));
        
    }
    p->_ultimateVelocity = p->_velocity;
    
    // 应用起始rotation
    if (_startRotation3D)
    {
        p->_startEuler = vec3(_startRotationX->Evaluate(dt, Random01()), _startRotationY->Evaluate(dt, Random01()), _startRotationZ->Evaluate(dt, Random01()));
    }
    else
    {
        p->_startEuler = vec3(0, 0, _startRotationZ->Evaluate(dt, Random01()));
    }
    p->_startQuat = quat(radians(p->_startEuler));
    p->_startMat = toMat4(p->_startQuat);
    p->_ultimateQuat = p->_startQuat;
    
    // 应用起始size
    if (_startSize3D)
    {
        p->_startSize = vec3(_startSizeX->Evaluate(dt, Random01()), _startSizeY->Evaluate(dt, Random01()), _startSizeZ->Evaluate(dt, Random01()));
    }
    else
    {
        p->_startSize = vec3(_startSizeX->Evaluate(dt, RandomM11()));
    }
    p->_size = p->_startSize;
    
    // 应用起始color
    p->_startColor = _startColor->Evaluate(dt, Random01());
    p->_color = p->_startColor;
    
    // 应用起始lifeTime
    p->_startLifeTime = _startLifeTime->Evaluate(dt, Random01()) + dt;
    p->_remainingLifeTime = p->_startLifeTime;
}
