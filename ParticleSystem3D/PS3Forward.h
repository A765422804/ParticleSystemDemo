//
//  Forward.h
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/9.
//

#pragma once

struct PVData // 粒子顶点的数据结构
{
    vec3 Position;
    vec3 Texcoord;
    vec3 Size;
    vec3 Rotation;
    vec3 Color; // 原版这个是写在了一个u32
    // 以上总计有15个float
};

// 纹理的uv坐标
int uvs[8] =
{
    0, 0,
    1, 0,
    0, 1,
    1, 1,
};
