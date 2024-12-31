//
//  Camera.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/27.
//

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float fov, float nearClip, float farClip)
    : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), up(up),
      fov(fov), nearClip(nearClip), farClip(farClip)
{
    // 确保up向量是单位向量
    this->up = glm::normalize(this->up);
}

glm::mat4 Camera::GetViewTransform() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjTransform() const
{
    return glm::perspective(glm::radians(fov), ASPECT_RATIO, nearClip, farClip);
}
