//
//  Defines.h
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/9.
//

#pragma once

enum class SpaceMode
{
    WORLD,
    LOCAL,
    // TODO: CUSTOM
};

enum class ShapeType
{
    BOX, // 立方体
    CIRCLE, // 圆形
    CONE, // 圆锥
    SPHERE, // 球体
    HEMISPHERE, // 半球体
};

enum class EmitLocation
{
     BASE, // 基础位置发射（circle cone）
     EDGE, // 边框（box circle）
     SHELL, // 表面（all）
     VOLUME, // 内部（all）
};

enum class ArcMode
{
    RANDOM, // 随机
    LOOP, // 顺序循环
    PINGPONG, // 反弹循环
};

enum class RenderMode
{
    BILLBOARD, // 面向相机
    STRECTHED_BILLBOARD, // 面向相机但会根据参数拉伸
    HORIZONTAL_BILLBOARD, // 始终与xz平面平行
    VERTICAL_BILLBOARD, // 始终与y轴平行并面向相机
    MESH, // 保持模型本身状态
};
