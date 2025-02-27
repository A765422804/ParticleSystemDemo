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
