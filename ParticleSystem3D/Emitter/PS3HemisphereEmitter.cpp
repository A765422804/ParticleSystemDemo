//
//  PS3HemisphereEmitter.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3HemisphereEmitter.hpp"
#include "../PS3ParticleSystem.hpp"

PS3HemisphereEmitter::PS3HemisphereEmitter()
{
    
}

std::shared_ptr<PS3HemisphereEmitter> PS3HemisphereEmitter::CreateHemisphereEmitter(EmitLocation emitLocation, float radius, float radiusThickness, PS3ParticleSystem* ps)
{
    PS3HemisphereEmitterPtr ret = std::make_shared<PS3HemisphereEmitter>();

    ret->_emitLocation = emitLocation;
    ret->_radius = radius;
    ret->_radiusThickness = radiusThickness;

    ret->_enable = true;
    ret->_ps = ps;

    return ret;
}

void PS3HemisphereEmitter::Emit(PS3ParticlePtr particle)
{
    switch (_emitLocation)
    {
        case EmitLocation::VOLUME:
        {
            particle->_position = RandomPointBetweenSphere(_radius * (1 - _radiusThickness), _radius);
            if (particle->_position.y < 0)
                particle->_position.y *= -1;
            particle->_velocity = normalize(particle->_position);
            return;
        }
        case EmitLocation::SHELL:
        {
            particle->_position = RandomUnitVector();
            particle->_position *= _radius;
            if (particle->_position.y > 0)
                particle->_position.y *= -1;
            particle->_velocity = normalize(particle->_position);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3HemisphereEmitter::RenderEmitter()
{
    std::vector<vec3> hemisphereVertices;
    std::vector<unsigned int> hemisphereIndices;
    unsigned int divideCount = 180;

    // 生成XOZ平面的整个圆
    unsigned int baseIndex = 0;
    for (unsigned int i = 0; i < divideCount; ++i) {
        float theta = radians((360.0f / divideCount) * i);
        float x = _radius * cos(theta);
        float z = _radius * sin(theta);
        hemisphereVertices.push_back(vec3(x, 0.0f, z));
        hemisphereIndices.push_back(baseIndex + i);
        hemisphereIndices.push_back(baseIndex + (i + 1) % divideCount);
    }

    // 生成XOY平面的半圆 (y > 0)
    baseIndex = unsigned(hemisphereVertices.size());
    for (unsigned int i = 0; i <= divideCount / 2; ++i) {
        float theta = radians((180.0f / (divideCount / 2)) * i);
        float x = _radius * cos(theta);
        float y = _radius * sin(theta);
        hemisphereVertices.push_back(vec3(x, y, 0.0f));
        if (i > 0) {
            hemisphereIndices.push_back(baseIndex + i - 1);
            hemisphereIndices.push_back(baseIndex + i);
        }
    }

    // 生成YOZ平面的半圆 (y > 0)
    baseIndex = unsigned(hemisphereVertices.size());
    for (unsigned int i = 0; i <= divideCount / 2; ++i) {
        float theta = radians((180.0f / (divideCount / 2)) * i);
        float z = _radius * cos(theta);
        float y = _radius * sin(theta);
        hemisphereVertices.push_back(vec3(0.0f, y, z));
        if (i > 0) {
            hemisphereIndices.push_back(baseIndex + i - 1);
            hemisphereIndices.push_back(baseIndex + i);
        }
    }

    _emitterRenderer->SetVertexData(hemisphereVertices);
    _emitterRenderer->SetIndexData(hemisphereIndices);
    _emitterRenderer->SetWorldTransform(_ps->GetWorldTransform());
    _emitterRenderer->RenderLines();
}
