//
//  Camera.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/27.
//

#pragma once

#include "Head.h"
#include "../Parameters.hpp"

class Camera {
public:
    // 构造函数，初始化相机位置、方向以及投影参数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float fov = 45.0f,
           float nearClip = 0.1f,
           float farClip = 100.0f);

    // 返回视图矩阵
    glm::mat4 GetViewTransform() const;

    // 返回投影矩阵
    glm::mat4 GetProjTransform() const;

private:
    // 相机属性
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    // 投影参数
    float fov;
    float nearClip;
    float farClip;
};

using CameraPtr = std::shared_ptr<Camera>;
