//
//  Background.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/7.
//

#include "Background.hpp"

Background::Background()
: _xMin(-50.0f)
, _xMax(50.0f)
, _zMin(-50.0f)
, _zMax(50.0f)
, _step(1.0f)
{
    _renderer = std::make_shared<Renderer>();
    _renderer->SetupVertexDescVec3();
    _renderer->SetupShaderProgram("./shader_file/background_vertex.vs", "./shader_file/background_fragment.fs");
}

void Background::Render()
{
    std::vector<vec3> vertices;
    std::vector<unsigned int> indices;
    
    // 固定z，x从min到max
    for (float i = _xMin; i <= _xMax; i += _step)
    {
        vertices.push_back(vec3(i, 0.0f, _zMin));
        vertices.push_back(vec3(i, 0.0f, _zMax));
    }
    for (float i = _zMin; i <= _zMax; i += _step)
    {
        vertices.push_back(vec3(_xMin, 0.0f , i));
        vertices.push_back(vec3(_xMax, 0.0f , i));
    }
    
    // 构建索引
    for (int i = 0 ; i < vertices.size(); ++i)
    {
        indices.push_back(i);
    }
    
    _renderer->SetVertexData(vertices);
    _renderer->SetIndexData(indices);
    _renderer->RenderLines();
}

void Background::SetCamera(CameraPtr camera)
{
    _renderer->SetCamera(camera);
}
