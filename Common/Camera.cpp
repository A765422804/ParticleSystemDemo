//
//  Camera.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/27.
//

#include "Camera.hpp"

Camera::Camera(vec3 position, vec3 up, float fov, float nearClip, float farClip)
    : position(position)
    , front(vec3(0.0f, 0.0f, -1.0f))
    , up(up)
    , fov(fov)
    , nearClip(nearClip)
    , farClip(farClip)
    , worldUp(up)
    , movementSpeed(SPEED)
    , mouseSensitivity(SENSITIVITY)
    , yaw(-90.0f)
    , pitch(0.0f)
{
    UpdateCameraVectors();
}

mat4 Camera::GetViewTransform() const
{
    return lookAt(position, position + front, up);
}

mat4 Camera::GetProjTransform() const
{
    return perspective(radians(fov), ASPECT_RATIO, nearClip, farClip);
}

void Camera::ProcessKeyboardMovement(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool conststrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;
    
    yaw += xOffset;
    pitch += yOffset;
    
    if (conststrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
    fov -= yOffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    vec3 curFront;
    curFront.x = cos(radians(yaw)) * cos(radians(pitch));
    curFront.y = sin(radians(pitch));
    curFront.z = sin(radians(yaw)) * cos(radians(pitch));
    front = normalize(curFront);
    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));
}
