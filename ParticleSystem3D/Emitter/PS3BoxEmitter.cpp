//
//  PS3BoxEmitter.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/24.
//

#include "PS3BoxEmitter.hpp"
#include "../PS3ParticleSystem.hpp"

inline const vec3 BOX = vec3(0.5f);

PS3BoxEmitter::PS3BoxEmitter()
{
    
}

std::shared_ptr<PS3BoxEmitter> PS3BoxEmitter::CreateBoxEmitter(EmitLocation emitLocation, PS3ParticleSystem* ps)
{
    PS3BoxEmitterPtr ret = std::make_shared<PS3BoxEmitter>();
    
    ret->_emitLocation = emitLocation;
    ret->_enable = true;
    ret->_ps = ps;
    
    return ret;
}

void PS3BoxEmitter::Emit(PS3ParticlePtr particle)
{
    std::vector<float> tmp(3, 0);
    particle->_velocity = vec3(0, 1, 0); // 统一沿着y轴正方向发射
    switch (SuperType::_emitLocation)
    {
        case EmitLocation::VOLUME:
        {
            particle->_position = RandomPointInCube(BOX);
            return;
        }
        case EmitLocation::SHELL:
        {
            tmp[0] = RandomRange(-0.5, 0.5);
            tmp[1] = RandomRange(-0.5, 0.5);
            tmp[2] = RandomSign() * 0.5;
            RandomSortArray(tmp);
            
            //ApplyBoxThickness(tmp); 暂时不考虑boxThickness
            particle->_position = vec3(tmp[0], tmp[1], tmp[2]);
            return;
        }
        case EmitLocation::EDGE:
        {
            tmp[0] = RandomRange(-0.5, 0.5);
            tmp[1] = RandomSign() * 0.5;
            tmp[2] = RandomSign() * 0.5;
            RandomSortArray(tmp);
            //ApplyBoxThickness(tmp);
            particle->_position = vec3(tmp[0], tmp[1], tmp[2]);
            return;
        }
        default:
            AssertShouldNotReachHere();
    }
}

void PS3BoxEmitter::RenderEmitter()
{
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
    
    SuperType::_emitterRenderer->SetVertexData(cubeVertices);
    SuperType::_emitterRenderer->SetIndexData(cubeIndices);
    SuperType::_emitterRenderer->SetWorldTransform(SuperType::_ps->GetWorldTransform());
    SuperType::_emitterRenderer->RenderLines();
}
