//
//  Point.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/2.
//

#pragma once

#include "../Head.h"
#include "../Common/Node.hpp"
#include "../Renderer/Renderer.hpp"

class Point : public Node
{
public:
    Point();
    ~Point() = default;
    
public:
    static std::shared_ptr<Point> Create(float size, vec4 color);
    
public:
    void Draw();
    
private:
    float _size;
    vec4 _color;
    RendererPtr _renderer;
};

using PointPtr = std::shared_ptr<Point>;
