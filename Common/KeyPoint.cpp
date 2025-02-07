//
//  CenterPoint.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/7.
//

#include "KeyPoint.hpp"

KeyPoint::KeyPoint()
{
    _vertices.push_back(vec3(0.0f));
    _renderer = std::make_shared<Renderer>();
    _renderer->SetupVertexDescVec3();
    _renderer->SetupShaderProgram("./shader_file/point_shader.vs", "./shader_file/point_shader.fs");
}

void KeyPoint::Render()
{
    _renderer->SetVertexData(_vertices);
    
    _renderer->RenderPoints(10, _vertices.size());
}

void KeyPoint::SetCamera(CameraPtr camera)
{
    _renderer->SetCamera(camera);
}
