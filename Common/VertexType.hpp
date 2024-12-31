//
//  VertexType.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/26.
//

#pragma once

#include "Head.h"

struct V3_C4_T2 // 每个顶点的数据结构
{
    glm::vec3 Vertices;
    glm::vec4 Colors;
    glm::vec2 TexCoords;
};

struct V3_C4_T2_Quad // 每个Quad的数据结构
{
    V3_C4_T2 tl;
    V3_C4_T2 bl;
    V3_C4_T2 tr;
    V3_C4_T2 br;
};
