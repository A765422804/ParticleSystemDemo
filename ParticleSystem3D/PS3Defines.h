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
    EVEN, // 均匀，这是我自己加的，希望发射多个粒子时，围绕圆环均匀分布
};

enum class RenderMode
{
    BILLBOARD, // 面向相机
    STRECTHED_BILLBOARD, // 面向相机但会根据参数拉伸
    HORIZONTAL_BILLBOARD, // 始终与xz平面平行
    VERTICAL_BILLBOARD, // 始终与y轴平行并面向相机
    MESH, // 保持模型本身状态
};

enum class AnimationMode
{
    WHOLE_SHEET, // 所有帧
    SINGLE_ROW, // 其中一行
};

enum class EventType
{
    SPAWN, // 出生
    DEATH, // 死亡
    COLLISION, // 碰撞
};
