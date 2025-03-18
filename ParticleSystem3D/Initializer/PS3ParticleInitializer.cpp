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
    _startSizeX = CurveRange::CreateCurveByTwoConstant(0.7, 1.0);
    
    // startSpeed
    //_startSpeed = CurveRange::CreateCurveByTwoConstant();
    _startSpeed = CurveRange::CreateCurveByConstant(2.0);
    
    // startRotation
    _startRotationZ = CurveRange::CreateCurveByTwoConstant(80, 100);
    //_startRotationZ = CurveRange::CreateCurveByTwoConstant(0, 90);
    
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
    _startColor = GradientRange::CreateByOneColor(vec4(vec3(1.0f), 1.0f));
    
    _startLifeTime = CurveRange::CreateCurveByConstant(1.0);
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
    
    const float rand = PseudoRandom(RandomRangeInt(0, 233280));
    // std::cout<<"rand: "<<rand<<std::endl;
    
    // TODO: textureAnimationModule
    
    // velocity
    float curStartSpeed = _startSpeed->Evaluate(dt, rand);
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
        p->_startEuler = vec3(_startRotationX->Evaluate(dt, rand), _startRotationY->Evaluate(dt, rand), _startRotationZ->Evaluate(dt, rand));
    }
    else
    {
        p->_startEuler = vec3(0, 0, _startRotationZ->Evaluate(dt, rand));
    }
    p->_startQuat = quat(radians(p->_startEuler));
    p->_startMat = toMat4(p->_startQuat);
    p->_ultimateQuat = p->_startQuat;
    
    // 应用起始size
    if (_startSize3D)
    {
        p->_startSize = vec3(_startSizeX->Evaluate(dt, rand), _startSizeY->Evaluate(dt, rand), _startSizeZ->Evaluate(dt, rand));
    }
    else
    {
        p->_startSize = vec3(_startSizeX->Evaluate(dt, rand));
    }
    p->_size = p->_startSize;
    
    // 应用起始color
    p->_startColor = _startColor->Evaluate(dt, rand);
    p->_color = p->_startColor;
    
    // 应用起始lifeTime
    p->_startLifeTime = _startLifeTime->Evaluate(dt, rand);
    p->_remainingLifeTime = p->_startLifeTime;
    
    // 设置随机数种子
    p->_randomSeed = RandomRangeInt(0, 233280);
}
