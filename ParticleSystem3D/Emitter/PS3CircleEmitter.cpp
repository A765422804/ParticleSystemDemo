//
//  PS3CircleEmitter.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3CircleEmitter.hpp"
#include "../PS3ParticleSystem.hpp"

PS3CircleEmitter::PS3CircleEmitter()
{
    
}

std::shared_ptr<PS3CircleEmitter> PS3CircleEmitter::CreateCircleEmitter(ArcMode arcMode, float arcSpread, float arc, CurveRangePtr arcSpeed, float radius, float radiusThickness,float angleStep, PS3ParticleSystem* ps)
{
    PS3CircleEmitterPtr ret = std::make_shared<PS3CircleEmitter>();

    ret->_arcMode = arcMode;
    ret->_arcSpread = arcSpread;
    ret->_arc = arc;
    ret->_arcSpeed = arcSpeed;
    ret->_radius = radius;
    ret->_radiusThickness = radiusThickness;
    ret->_angleStep = angleStep;

    ret->_enable = true;
    ret->_ps = ps;

    return ret;
}

void PS3CircleEmitter::Emit(PS3ParticlePtr particle)
{
    float theta = SuperType::GenerateArcAngle();
    particle->_position = RandomPointBetweenCircleAtFixedAngle(_radius * (1 - _radiusThickness), _radius, theta);
    particle->_velocity = normalize(particle->_position);
}

void PS3CircleEmitter::RenderEmitter()
{
        std::vector<vec3> circleVertices;
        std::vector<unsigned int> circleIndices;
        unsigned int divideCount = 180;
        for (int i = 0; i < divideCount; ++i) // 圆顶点坐标
        {
            float theta = radians((360.0 / divideCount) * i);
            float x = _radius * cos(theta);
            float z = _radius * sin(theta);
            vec3 point = vec3(x, 0, z);
            circleVertices.push_back(point);
        }
        for (unsigned int i = 0 ; i < divideCount; ++i) // 构建索引
        {
            circleIndices.push_back(i);
            circleIndices.push_back((i + 1) % divideCount);
        }

        _emitterRenderer->SetVertexData(circleVertices);
        _emitterRenderer->SetIndexData(circleIndices);
        _emitterRenderer->SetWorldTransform(_ps->GetWorldTransform());
        _emitterRenderer->RenderLines();
}
