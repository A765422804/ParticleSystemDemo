//
//  CenterPoint.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/7.
//

#pragma once
#include "../Head.h"
#include "../Renderer/Renderer.hpp"

class KeyPoint
{
public:
    KeyPoint();
    ~KeyPoint() = default;
    
public:
    void Render();
    
public:
    void SetCamera(CameraPtr camera);
    
public:
    RendererPtr _renderer;
    std::vector<vec3> _vertices;
};

using KeyPointPtr = std::shared_ptr<KeyPoint>;
