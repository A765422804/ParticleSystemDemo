//
//  Camera.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/27.
//

#include "Camera.hpp"

Camera::Camera(vec3 position, vec3 up, float fov, float nearClip, float farClip)
    : position(position), front(vec3(0.0f, 0.0f, -1.0f)), up(up),
      fov(fov), nearClip(nearClip), farClip(farClip)
{
    // 确保up向量是单位向量
    this->up = normalize(this->up);
}

mat4 Camera::GetViewTransform() const
{
    return lookAt(position, position + front, up);
}

mat4 Camera::GetProjTransform() const
{
    return perspective(radians(fov), ASPECT_RATIO, nearClip, farClip);
}
