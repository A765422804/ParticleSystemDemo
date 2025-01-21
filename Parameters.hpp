//
//  Parameters.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/7/17.
//

#pragma once

#include "Head.h"
#include "Function/DecayFunction.hpp"
#include "Function/SpeedChange.hpp"

const unsigned int SCR_WIDTH = 800; // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度
const float ASPECT_RATIO = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT); //屏幕的宽高比

const float TARGET_FPS = 120.0f; // 目标帧率
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS; // 目标每帧时间间隔

const float POINT_LENGTH = 0.01; //点的长度
const int POINT_COUNT = 38; //点的数量
const float STEP = 0.05; // 控制下一个点和当前点的步长

const float ALPHA = 0.5; // catmull-rom曲线的参数，0.5最好
const int NUM_POINT = 100; //每两个点之间插值生成的子点数
const float LINE_LENGTH = 0.001; // 子点的长度

// 摄像机相关
const vec3 CAMERA_POS = vec3(0.0f); // 相机位置
const vec3 CAMERA_TARGET = vec3(0.0f, 0.0f, -1.0f); // 相机目标
const vec3 UP_VECTOR = vec3(0.0f, 1.0f, 0.0f); // 相机上方向
const float NEAR_PLANE = 0.1f; // 近平面
const float FAR_PLANE = 10.0f; // 远平面
const float FOV = 45.0f; //视场角

// ParticleSystem2D





