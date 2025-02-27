//
//  PS3ConeEmitter.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3ConeEmitter.hpp"
#include "../PS3ParticleSystem.hpp"

PS3ConeEmitter::PS3ConeEmitter()
{
    
}

std::shared_ptr<PS3ConeEmitter> PS3ConeEmitter::CreateConeEmitter(EmitLocation emitLocation, ArcMode arcMode, float arcSpread, float arc, CurveRangePtr arcSpeed, float radius, float radiusThickness,float angleStep, float angle, float length, PS3ParticleSystem* ps)
{
    PS3ConeEmitterPtr ret = std::make_shared<PS3ConeEmitter>();

    ret->_emitLocation = emitLocation;
    ret->_arcMode = arcMode;
    ret->_arcSpread = arcSpread;
    ret->_arc = arc;
    ret->_arcSpeed = arcSpeed;
    ret->_radius = radius;
    ret->_radiusThickness = radiusThickness;
    ret->_angleStep = angleStep;
    ret->_angle = angle;
    ret->_length = length;

    ret->_enable = true;
    ret->_ps = ps;

    return ret;
}

void PS3ConeEmitter::Emit(PS3ParticlePtr particle)
{
    float theta = GenerateArcAngle();
    switch (_emitLocation)
    {
        case EmitLocation::BASE:
        {
            // pos
            particle->_position = RandomPointBetweenCircleAtFixedAngle(_radius * (1 - _radiusThickness), _radius, theta);

            // dir
            float r1 = _radius;
            float r2 = r1 + _length * tan(radians(_angle));
            float h = r1 * _length / (r2 - r1);
            vec3 p = vec3(0, -h, 0);
            particle->_velocity = normalize(particle->_position - p);

            return;
        }
        case EmitLocation::SHELL:
        {
            // pos
            particle->_position = RandomPointBetweenCircleAtFixedAngle(_radius, _radius, theta);

            // dir
            float r1 = _radius;
            float r2 = r1 + _length * tan(radians(_angle));
            float h = r1 * _length / (r2 - r1);
            vec3 p = vec3(0, -h, 0);
            particle->_velocity = normalize(particle->_position - p);

            return;
        }
        case EmitLocation::VOLUME:
        {
            // pos: 随机选一个高度，在这个高度的圆上随机取一个点
            float h = _length * Random01();
            float r1 = _radius;
            float r2 = r1 + _length * tan(radians(_angle));
            float r = r1 + h * (r2 - r1) / _length; // 该高度半径
            particle->_position = RandomPointBetweenCircleAtFixedAngle(r * (1- _radiusThickness), r, theta);
            particle->_position.y = h;

            // dir
            float ph = r1 * _length / (r2 - r1);
            vec3 p = vec3(0, -ph, 0);
            particle->_velocity = normalize(particle->_position - p);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3ConeEmitter::RenderEmitter()
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
    _emitterRenderer->SetWorldTransform(_ps->GetWorldTransform());
    _emitterRenderer->RenderLines();
    
}
