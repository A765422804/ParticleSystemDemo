//
//  PS3ShapeModule.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#include "PS3ShapeModule.hpp"
#include "../PS3ParticleSystem.hpp"

void PS3ShapeModule::Emit(PS3ParticlePtr particle)
{
    switch(_shapeType)
    {
        case ShapeType::BOX:
            BoxEmit(particle->_position, particle->_velocity);
            break;
        case ShapeType::CIRCLE:
            CircleEmit(particle->_position, particle->_velocity);
            break;
        case ShapeType::CONE:
            ConeEmit(particle->_position, particle->_velocity);
            break;
        case ShapeType::SPHERE:
            SphereEmit(particle->_position, particle->_velocity);
            break;
        case ShapeType::HEMISPHERE:
            HemisphereEmit(particle->_position, particle->_velocity);
            break;
        default:
            AssertShouldNotReachHere();
    }
    
    if (_randomPositionAmount > 0)
    {
        particle->_position.x += RandomRange(-_randomPositionAmount, _randomPositionAmount);
        particle->_position.y += RandomRange(-_randomPositionAmount, _randomPositionAmount);
        particle->_position.z = RandomRange(-_randomPositionAmount, _randomPositionAmount);
    }
    // TODO: 应用四元数和mat
    // TODO: 应用球形速度调整
    _lastTime = _ps->_time;
    
}

void PS3ShapeModule::CircleEmit(vec3& pos, vec3& dir)
{
    // TODO: 确认是否正确
    float theta = GenerateArcAngle();
    pos += RandomPointBetweenCircleAtFixedAngle(_radius * (1 - _radiusThickness), _radius, theta);
    dir = normalize(pos);
}

float PS3ShapeModule::GenerateArcAngle()
{
    if (_arcMode == ArcMode::RANDOM)
        return RandomRange(0, _arc);
    
    float angle = _totalAngle + 2 * M_PI * _arcSpeed->Evaluate(_ps->_time, 1) * (_ps->_time - _lastTime);
    _totalAngle = angle;
    if (_arcSpread != 0)
    {
        angle = floor(angle / (_arc * _arcSpread)) * _arc * _arcSpread;
    }
    switch (_arcMode) 
    {
        case ArcMode::LOOP:
            return Repeat(angle, _arc);
        case ArcMode::PINGPONG:
            return PingPong(angle, _arc);
        default:
            AssertShouldNotReachHere();
    }
}

void PS3ShapeModule::ConeEmit(vec3& pos, vec3& dir)
{
    float theta = GenerateArcAngle();
    switch (_emitLocation) 
    {
        case EmitLocation::BASE:
        {
            pos += RandomPointBetweenCircleAtFixedAngle(_radius * (1 - _radiusThickness), _radius, theta);
            dir = pos * sin(_angle);
            dir.z = - cos(_angle) * _radius;
            dir = normalize(dir);
            pos.z = 0;
            return;
        }
        case EmitLocation::SHELL:
        {
            vec2 angleUnit = vec2(cos(theta), sin(theta));
            dir = pos * sin(_angle);
            dir.z = - cos(_angle) * _radius;
            dir = normalize(dir);
            pos = pos * _radius;
            pos.z = 0;
            return;
        }
        case EmitLocation::VOLUME:
        {
            pos += RandomPointBetweenCircleAtFixedAngle(_radius * (1 - _radiusThickness), _radius, theta);
            dir = pos * sin(_angle);
            dir.z = - cos(_angle) * _radius;
            dir = normalize(dir);
            pos.z = 0;
            pos += dir * (_length * Random01() / -dir.z);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3ShapeModule::SphereEmit(vec3& pos, vec3& dir)
{
    switch (_emitLocation) 
    {
        case EmitLocation::VOLUME:
        {
            pos = RandomPointBetweenSphere(_radius * (1 - _radiusThickness), _radius);
            dir = normalize(pos);
            return;
        }
        case EmitLocation::SHELL:
        {
            pos = RandomUnitVector();
            pos *= _radius;
            dir = normalize(pos);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3ShapeModule::HemisphereEmit(vec3 &pos, vec3 &dir)
{
    switch (_emitLocation) 
    {
        case EmitLocation::VOLUME:
        {
            pos = RandomPointBetweenSphere(_radius * (1 - _radiusThickness), _radius);
            if (pos.z > 0)
                pos.z *= -1;
            dir = normalize(pos);
            return;
        }
        case EmitLocation::SHELL:
        {
            pos = RandomUnitVector();
            pos *= _radius;
            if (pos.z > 0)
                pos.z *= -1;
            dir = normalize(pos);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3ShapeModule::BoxEmit(vec3& pos, vec3& dir)
{
    std::vector<float> tmp(3, 0);
    dir = vec3(0, 0, -1);
    switch (_emitLocation)
    {
        case EmitLocation::VOLUME:
        {
            pos = RandomPointInCube(vec3(0.5));
            return;
        }
        case EmitLocation::SHELL:
        {
            tmp[0] = RandomRange(-0.5, 0.5);
            tmp[1] = RandomRange(-0.5, 0.5);
            tmp[2] = RandomSign() * 0.5;
            RandomSortArray(tmp);
            ApplyBoxThickness(tmp);
            pos = vec3(tmp[0], tmp[1], tmp[2]);
            return;
        }
        case EmitLocation::EDGE:
        {
            tmp[0] = RandomRange(-0.5, 0.5);
            tmp[1] = RandomSign() * 0.5;
            tmp[2] = RandomSign() * 0.5;
            RandomSortArray(tmp);
            ApplyBoxThickness(tmp);
            pos = vec3(tmp[0], tmp[1], tmp[2]);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3ShapeModule::ApplyBoxThickness(std::vector<float>& pos)
{
    if (_boxThickness.x > 0) {
        pos[0] += 0.5 * RandomRange(-_boxThickness.x, _boxThickness.x);
        pos[0] = clamp(pos[0], -0.5f, 0.5f);
    }
    if (_boxThickness.y > 0) {
        pos[1] += 0.5 * RandomRange(-_boxThickness.y, _boxThickness.y);
        pos[1] = clamp(pos[1], -0.5f, 0.5f);
    }
    if (_boxThickness.z > 0) {
        pos[2] += 0.5 * RandomRange(-_boxThickness.z, _boxThickness.z);
        pos[2] = clamp(pos[2], -0.5f, 0.5f);
    }
}
