//
//  Sphere.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/17.
//

#pragma once

#include "../Head.h"
#include "Node.hpp"
#include "../Renderer/SphereRenderer.hpp"

class Sphere : public Node
{
public:
    Sphere(float radius, int section, int stacks);
    ~Sphere() = default;
    
public:
    void Render();
    void Move(float deltaTime);
    
public:
    float _radius;
    int _sectors;
    int _stacks;
    
    SphereRendererPtr _sphereRenderer;
};

using SpherePtr = std::shared_ptr<Sphere>;
