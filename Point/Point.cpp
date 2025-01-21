//
//  Point.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/2.
//

#include "Point.hpp"

Point::Point()
: _size(0.0f)
, _renderer(nullptr)
{
}

std::shared_ptr<Point> Point::Create(float size, vec4 color)
{
    PointPtr ret = std::make_shared<Point>();
    ret->_size = size;
    ret->_color = color;
    
    ret->_renderer = std::make_shared<Renderer>();
    ret->_renderer->SetupVertexDescV3();
    ret->_renderer->SetupShaderProgram("./shader_file/point_shader.vs", "./shader_file/point_shader.fs");
    
    return ret;
}

void Point::Draw()
{
    vec3 position = GetWorldPosition();
    std::vector<V3> vertices;
    V3 v3;
    v3.Position = position;
    vertices.push_back(v3);
    
    _renderer->SetVertexData(vertices);
    _renderer->RenderPoint(_size, _color);
}
