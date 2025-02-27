//
//  PS3SphereEmitter.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3SphereEmitter.hpp"
#include "../PS3ParticleSystem.hpp"

PS3SphereEmitter::PS3SphereEmitter()
{
    
}

std::shared_ptr<PS3SphereEmitter> PS3SphereEmitter::CreateSphereEmitter(EmitLocation emitLocation, float radius, float radiusThickness, PS3ParticleSystem* ps)
{
    PS3SphereEmitterPtr ret = std::make_shared<PS3SphereEmitter>();

    ret->_emitLocation = emitLocation;
    ret->_radius = radius;
    ret->_radiusThickness = radiusThickness;

    ret->_enable = true;
    ret->_ps = ps;

    return ret;
}

void PS3SphereEmitter::Emit(PS3ParticlePtr particle)
{
    switch (_emitLocation)
    {
        case EmitLocation::VOLUME:
        {
            particle->_position = RandomPointBetweenSphere(_radius * (1 - _radiusThickness), _radius);
            particle->_velocity = normalize(particle->_position);
            return;
        }
        case EmitLocation::SHELL:
        {
            particle->_position = RandomUnitVector();
            particle->_position *= _radius;
            particle->_velocity = normalize(particle->_position);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3SphereEmitter::RenderEmitter()
{
        std::vector<vec3> sphereVertices;
        std::vector<unsigned int> sphereIndices;
        unsigned int divideCount = 180;

        // 生成XOY平面的圆
        unsigned int baseIndex = 0;
        for (unsigned int i = 0; i < divideCount; ++i) {
            float theta = radians((360.0f / divideCount) * i);
            float x = _radius * cos(theta);
            float y = _radius * sin(theta);
            sphereVertices.push_back(vec3(x, y, 0.0f));
            sphereIndices.push_back(baseIndex + i);
            sphereIndices.push_back(baseIndex + (i + 1) % divideCount);
        }

        // 生成YOZ平面的圆
        baseIndex = int(sphereIndices.size() / 2);
        for (unsigned int i = 0; i < divideCount; ++i) {
            float theta = radians((360.0f / divideCount) * i);
            float y = _radius * cos(theta);
            float z = _radius * sin(theta);
            sphereVertices.push_back(vec3(0.0f, y, z));
            sphereIndices.push_back(baseIndex + i);
            sphereIndices.push_back(baseIndex + (i + 1) % divideCount);
        }

        // 生成XOZ平面的圆
        baseIndex = int(sphereIndices.size() / 2);
        for (unsigned int i = 0; i < divideCount; ++i) {
            float theta = radians((360.0f / divideCount) * i);
            float x = _radius * cos(theta);
            float z = _radius * sin(theta);
            sphereVertices.push_back(vec3(x, 0.0f, z));
            sphereIndices.push_back(baseIndex + i);
            sphereIndices.push_back(baseIndex + (i + 1) % divideCount);
        }

        _emitterRenderer->SetVertexData(sphereVertices);
        _emitterRenderer->SetIndexData(sphereIndices);
        _emitterRenderer->SetWorldTransform(_ps->GetWorldTransform());
        _emitterRenderer->RenderLines();
}
