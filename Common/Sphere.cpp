//
//  Sphere.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/17.
//

#include "Sphere.hpp"

Sphere::Sphere(float radius, int sectors, int stacks)
: _radius(radius), _sectors(sectors), _stacks(stacks)
{
    _sphereRenderer = std::make_shared<SphereRenderer>(radius, sectors, stacks);
}

void Sphere::Render()
{
    _sphereRenderer->SetWorldTransform(GetWorldTransform());
    
    _sphereRenderer->Render();
}

void Sphere::Move(float deltaTime)
{
    // 定义旋转速度
    float rotateSpeed = 30.0f; // 每秒旋转的角度

    // 计算旋转角度（弧度制）
    float rotateAngle = radians(rotateSpeed * deltaTime);

    // 计算球体的新位置（绕原点、半径为 2 的圆周运动）
    vec3 currentPosition = GetPosition3D();
    float radius = 2.0f; // 圆周运动的半径
    float newX = radius * cos(rotateAngle + atan2(currentPosition.z, currentPosition.x));
    float newZ = radius * sin(rotateAngle + atan2(currentPosition.z, currentPosition.x));

    // 更新球体的位置
    SetPosition3D(vec3(newX, currentPosition.y, newZ));

    // 创建绕 Y 轴旋转的四元数
    quat rotationDelta = angleAxis(rotateAngle, vec3(0.0f, 1.0f, 0.0f));

    // 更新 _rotation
    _rotation = normalize(rotationDelta * _rotation);
}
