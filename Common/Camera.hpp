//
//  Camera.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/27.
//

#pragma once

#include "../Head.h"
#include "../Parameters.hpp"

enum Camera_Movement 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera {
public:
    // 构造函数，初始化相机位置、方向以及投影参数
    Camera(vec3 position = vec3(.0f, 10.0f, 15.0f),
           vec3 up = vec3(0.0f, 1.0f, 0.0f),
           float fov = 45.0f,
           float nearClip = 0.1f,
           float farClip = 100.0f);
    
    vec3 GetPosition() {return position;}

    // 返回视图矩阵
    mat4 GetViewTransform() const;

    // 返回投影矩阵
    mat4 GetProjTransform() const;
    
    // 处理键盘移动
    void ProcessKeyboardMovement(Camera_Movement direction, float deltaTime);
    
    // 处理鼠标移动
    void ProcessMouseMovement(float xOffset, float yOffset, bool conststrainPitch = true);
    
    // 处理鼠标滚轮
    void ProcessMouseScroll(float yOffset);

private:
    void UpdateCameraVectors(); // 内部函数，更新相机的方向向量
    
private:
    // 相机属性
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    
    // 欧拉角
    float yaw;
    float pitch;

    // 投影参数
    float fov;
    float nearClip;
    float farClip;
    
    // 相机运动参数
    float movementSpeed; // 移动灵敏
    float mouseSensitivity; // 鼠标灵敏
    
};

using CameraPtr = std::shared_ptr<Camera>;
