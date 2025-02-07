//
//  PS3ShapeModule.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/13.
//

#include "PS3ShapeModule.hpp"
#include "../PS3ParticleSystem.hpp"

inline const vec3 BOX = vec3(0.5f);

std::vector<vec3> cubeVertices = {
    vec3(-0.5f, -0.5f, -0.5f),  // 0
    vec3(0.5f, -0.5f, -0.5f),   // 1
    vec3(0.5f, 0.5f, -0.5f),    // 2
    vec3(-0.5f, 0.5f, -0.5f),   // 3
    vec3(-0.5f, -0.5f, 0.5f),   // 4
    vec3(0.5f, -0.5f, 0.5f),    // 5
    vec3(0.5f, 0.5f, 0.5f),     // 6
    vec3(-0.5f, 0.5f, 0.5f)     // 7
};
std::vector<unsigned int> cubeIndices = {
    0, 1, 1, 2, 2, 3, 3, 0, // 底面
    4, 5, 5, 6, 6, 7, 7, 4, // 顶面
    0, 4, 1, 5, 2, 6, 3, 7  // 侧面连接
};

PS3ShapeModule::PS3ShapeModule()
{
    _emitterRenderer = std::make_shared<Renderer>();
    _emitterRenderer->SetupVertexDescVec3();
    _emitterRenderer->SetupShaderProgram("./shader_file/debug_shader.vs", "./shader_file/debug_shader.fs");
    
}

std::shared_ptr<PS3ShapeModule> PS3ShapeModule::CreateBoxEmitter(EmitLocation emitLocation, vec3 boxThickness, PS3ParticleSystem* ps)
{
    PS3ShapeModulePtr ret = std::make_shared<PS3ShapeModule>();
    
    ret->_shapeType = ShapeType::BOX;
    ret->_emitLocation = emitLocation;
    ret->_boxThickness = boxThickness;
    
    ret->_enable = true;
    ret->_ps = ps;
    
    return ret;
}

std::shared_ptr<PS3ShapeModule> PS3ShapeModule::CreateConeEmitter(EmitLocation emitLocation, ArcMode arcMode, float arcSpread, float arc, CurveRangePtr arcSpeed, float radius, float radiusThickness, float angle, float length, PS3ParticleSystem* ps)
{
    PS3ShapeModulePtr ret = std::make_shared<PS3ShapeModule>();
    
    ret->_shapeType = ShapeType::CONE;
    ret->_emitLocation = emitLocation;
    ret->_arcMode = arcMode;
    ret->_arcSpeed = arcSpeed;
    ret->_arc = arc;
    ret->_arcSpeed = arcSpeed;
    ret->_radius = radius;
    ret->_radiusThickness = radiusThickness;
    ret->_angle = angle;
    ret->_length = length;
    
    ret->_enable = true;
    ret->_ps = ps;
    
    return ret;
}

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
            // pos
            pos = RandomPointBetweenCircleAtFixedAngle(_radius * (1 - _radiusThickness), _radius, theta);

            // dir
            float r1 = _radius;
            float r2 = r1 + _length * tan(radians(_angle));
            float h = r1 * _length / (r2 - r1);
            vec3 p = vec3(0, -h, 0);
            dir = normalize(pos - p);
            
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
    dir = vec3(0, 1, 0);
    switch (_emitLocation)
    {
        case EmitLocation::VOLUME:
        {
            pos = RandomPointInCube(BOX);
            return;
        }
        case EmitLocation::SHELL:
        {
            tmp[0] = RandomRange(-0.5, 0.5);
            tmp[1] = RandomRange(-0.5, 0.5);
            tmp[2] = RandomSign() * 0.5;
            RandomSortArray(tmp);
            
            //ApplyBoxThickness(tmp); 暂时不考虑boxThickness
            pos = vec3(tmp[0], tmp[1], tmp[2]);
            return;
        }
        case EmitLocation::EDGE:
        {
            tmp[0] = RandomRange(-0.5, 0.5);
            tmp[1] = RandomSign() * 0.5;
            tmp[2] = RandomSign() * 0.5;
            RandomSortArray(tmp);
            //ApplyBoxThickness(tmp);
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

void PS3ShapeModule::RenderEmitter()
{
    switch (_shapeType) {
        case ShapeType::BOX:
        {
            _emitterRenderer->SetVertexData(cubeVertices);
            _emitterRenderer->SetIndexData(cubeIndices);
            _emitterRenderer->RenderLines();
            
            break;
        }
        case ShapeType::CONE:
        {
            float r1 = _radius;
            float r2 = _radius + _length * tan(radians(_angle));
            
            std::vector<vec3> coneVertices;
            std::vector<unsigned int> coneIndices;
            unsigned int divideCount = 180;
            
            // 填充顶点坐标
            for (int i = 0; i < divideCount; ++i) // 底面圆
            {
                float theta = radians((360.0 / divideCount) * i);
                float x = r1 * cos(theta);
                float z = r1 * sin(theta);
                vec3 point = vec3(x, 0, z);
                coneVertices.push_back(point);
            }
            for (int i = 0 ; i < divideCount ; ++i) // 顶面圆
            {
                float theta = radians((360.0 / divideCount) * i);
                float x = r2 * cos(theta);
                float z = r2 * sin(theta);
                vec3 point = vec3(x, _length, z);
                coneVertices.push_back(point);
            }
            
            // 构建索引
            for (unsigned int i = 0 ; i < divideCount; ++i) // 底面圆
            {
                coneIndices.push_back(i);
                coneIndices.push_back((i + 1) % divideCount);
            }
            for (unsigned int i = 0 ; i < divideCount; ++i) // 顶面圆
            {
                coneIndices.push_back(i + divideCount);
                coneIndices.push_back((i + 1) % divideCount + divideCount);
            }
            int lineCount = 8;
            for (int i = 0 ; i < lineCount; ++i) // 母线索引
            {
                int step = divideCount / lineCount * i;
                coneIndices.push_back(step);
                coneIndices.push_back(step + divideCount);
            }
            
            _emitterRenderer->SetVertexData(coneVertices);
            _emitterRenderer->SetIndexData(coneIndices);
            _emitterRenderer->RenderLines();
            
            break;
        }
        default:
            break;
    }
}
