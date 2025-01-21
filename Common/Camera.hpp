//
//  Camera.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/27.
//

#pragma once

#include "../Head.h"
#include "../Parameters.hpp"

class Camera {
public:
    // 构造函数，初始化相机位置、方向以及投影参数
    Camera(vec3 position = vec3(0.0f, 0.0f, 30.0f),
           vec3 up = vec3(0.0f, 1.0f, 0.0f),
           float fov = 45.0f,
           float nearClip = 0.1f,
           float farClip = 100.0f);

    // 返回视图矩阵
    mat4 GetViewTransform() const;

    // 返回投影矩阵
    mat4 GetProjTransform() const;

private:
    // 相机属性
    vec3 position;
    vec3 front;
    vec3 up;

    // 投影参数
    float fov;
    float nearClip;
    float farClip;
};

using CameraPtr = std::shared_ptr<Camera>;
