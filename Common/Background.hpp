//
//  Background.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/7.
//

#pragma once
#include "../Head.h"
#include "../Renderer/Renderer.hpp"
#include "Camera.hpp"

class Background
{
public:
    Background();
    ~Background() = default;
    
public:
    void Render();
    void SetCamera(CameraPtr camera);
    
private:
    RendererPtr _renderer;
    
    float _xMin;
    float _xMax;
    
    float _zMin;
    float _zMax;
    
    float _step;
};

using BackgroundPtr = std::shared_ptr<Background>;

