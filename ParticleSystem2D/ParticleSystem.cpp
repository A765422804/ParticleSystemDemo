//
//  ParticleSystem.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/25.
//

#include "ParticleSystem.hpp"
#include "../Tool/RandomNumberGenerate.hpp"

ParticleSystem::ParticleSystem()
{
    // TODO: implement
}

void ParticleSystem::Init()
{
    _isActive = true;
    
    // 设置ParticleSystem的所有属性
    // 1. 初始化属性
    _elapsed = 0.0f;
    _isPaused = false;
    
    _timePerEmit = 0.0f;

    // 2. 超参数
//    _texture = std::make_shared<Texture2D>("/Users/evanbfeng/work/resource/textures/blue_background2.png");
    _texture = nullptr;
    
    _duration = -1;
    _maxParticleCount = 100;
    
    _emissionRate = 10.0f;
    _emitterMode = EmitterMode::GRAVITY;
    
    _sourcePosition = vec2(0.0f, 0.0f);
    _positionVar = vec2(3.0f, 3.0f); // 发射的位置
    _positionType = PositionType::FREE;
    
    _startSize = 1.0f;
    _startSizeVar = 0.0f;
    
    _endSize = 1.0f;
    _endSizeVar = 0.0f;
    
//    _colorRamp = std::make_shared<Gradient>(vec4(1, 0, 0, 1), vec4(1, 0, 0, 0));
//    _colorRamp->AddPoint(0, vec4(209.f/255, 107.f/255, 165.f/255, 1));
//    _colorRamp->AddPoint(0.5, vec4(134.f/255, 168.f/255, 231.f/255, 1));
//    _colorRamp->AddPoint(1.0, vec4(95.f/255, 251.f/255, 241.f/255, 0));
//    _colorRamp->SetInterpolationMode(Gradient::GRADIENT_INTERPOLATE_CUBIC);

    _startColor = vec4(0.0f, 0.0f ,0.0f ,1.0f);
    _startColorVar = vec4(1.f, 1.f, 1.f, 0.0f);
    
    _endColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    _endColorVar = vec4(0.0f);
    
    _startSpin = 0.0f;
    _startSpinVar = 0.0f;
    
    _endSpin = 0.0f;
    _endSpinVar = 0.0f;
    
    // 角度决定运动方向，和speed共同作用
    _angle = 0.0f;
    _angleVar = 180.0f;
    
    _lifeTime = 4.0f;
    _lifeTimeVar = 0.0f;
    
    // gravity
    _gravityAttribute.Gravity = vec2(0.0f, -5.0f);
    _gravityAttribute.Speed = 5.0f;
    _gravityAttribute.SpeedVar = 0.0f;
    
    _gravityAttribute.RadialAccel = 0.0f;
    _gravityAttribute.RadialAccelVar = 0.0f;
    
    _gravityAttribute.TangentialAccel = .0f;
    _gravityAttribute.TangentialAccelVal = 0.0f;
    
    // rotation
    _rotationAttribute.StartRadius = 5.0f;
    _rotationAttribute.StartRadiusVar = 0.0f;
    _rotationAttribute.EndRadius = 1.0f;
    _rotationAttribute.EndRadiusVar = 0.0f;
    
    _rotationAttribute.RotatePerSecond = 30.0f;
    _rotationAttribute.RotatePersecondVar = 0.0f;
    
    // pool
    _particles.clear();
    _particles.resize(_maxParticleCount);
}

void ParticleSystem::AddParticles(int count)
{
    if (_isPaused)
        return;
    
    int startIndex = _particleCount;
    _particleCount += count;
    
    //life
    for (int i = startIndex; i < _particleCount; ++ i)
    {
        float lifeTime = _lifeTime + _lifeTimeVar * TFRandomM11();
        _particles[i]._timeToLive = max(0.0f, lifeTime);
    }
    
    // position
    for (int i = startIndex; i < _particleCount; ++i)
    {
        _particles[i]._posX = _sourcePosition.x + _positionVar.x * TFRandomM11();
        _particles[i]._posY = _sourcePosition.y + _positionVar.y * TFRandomM11();
    }
    
    // color
//    for (int i = startIndex; i<_particleCount; ++i)
//    {
//        _particles[i]._colorR = std::clamp(_startColor.r + _startColorVar.r * TFRandomM11(), 0.0f ,1.0f);
//        _particles[i]._colorG = std::clamp(_startColor.g + _startColorVar.g * TFRandomM11(), 0.0f ,1.0f);
//        _particles[i]._colorB = std::clamp(_startColor.b + _startColorVar.b * TFRandomM11(), 0.0f ,1.0f);
//        _particles[i]._colorA = std::clamp(_startColor.a + _startColorVar.a * TFRandomM11(), 0.0f ,1.0f);
//    }
    
    // delta color
//    for (int i= startIndex; i < _particleCount; ++i)
//    {
//        _particles[i]._deltaColorR = (std::clamp(_endColor.r + _endColorVar.r * TFRandomM11(), 0.0f, 1.0f) - _particles[i]._colorR) / _particles[i]._timeToLive;
//        _particles[i]._deltaColorG = (std::clamp(_endColor.g + _endColorVar.g * TFRandomM11(), 0.0f, 1.0f) - _particles[i]._colorG) / _particles[i]._timeToLive;
//        _particles[i]._deltaColorB = (std::clamp(_endColor.b + _endColorVar.b * TFRandomM11(), 0.0f, 1.0f) - _particles[i]._colorB) / _particles[i]._timeToLive;
//        _particles[i]._deltaColorA = (std::clamp(_endColor.a + _endColorVar.a * TFRandomM11(), 0.0f, 1.0f) - _particles[i]._colorA) / _particles[i]._timeToLive;
//    }
    
    // size
    for (int i = startIndex; i < _particleCount; ++i)
    {
        _particles[i]._size = _startSize + _startSizeVar * TFRandomM11();
        _particles[i]._size = max(0.0f, _particles[i]._size);
    }
    
    // delta size
    for (int i = startIndex; i < _particleCount; ++i)
    {
        float endSize = _endSize + _endSizeVar * TFRandomM11();
        endSize = max(0.0f, endSize);
        _particles[i]._deltaSize = (endSize - _particles[i]._size) / _particles[i]._timeToLive;
    }
    
    // rotation
    for (int i = startIndex; i < _particleCount; ++i)
    {
        _particles[i]._rotation = _startSpin + _startSpinVar * TFRandomM11();
    }
    
    // rotation delta
    for (int i = startIndex; i < _particleCount; ++i)
    {
        float endRotation = _endSpin + _endSpinVar * TFRandomM11();
        _particles[i]._deltaRotation = (endRotation - _particles[i]._rotation) / _particles[i]._timeToLive;
    }
    
    vec2 startPos;
    if (_positionType == PositionType::FREE)
    {
        startPos = ConvertToWorldSpace(vec3(0.0f));
    }
    else if (_positionType == PositionType::RELATIVE)
    {
        startPos = _position;
    }
    else
    {
        startPos = vec2(0.0f);
    }
    for (int i = startIndex; i < _particleCount; ++i)
    {
        _particles[i]._startPosX = startPos.x;
        _particles[i]._startPosY = startPos.y;
    }
    
    // Mode Gravity
    if (_emitterMode == EmitterMode::GRAVITY)
    {
        // radial accel
        for (int i = startIndex ; i < _particleCount; ++i)
        {
            _particles[i]._gravityMode.RadialAccel = _gravityAttribute.RadialAccel + _gravityAttribute.RadialAccelVar * TFRandomM11();
        }
        
        // tangential accel
        for (int i = startIndex; i < _particleCount; ++i)
        {
            _particles[i]._gravityMode.TangentialAccel = _gravityAttribute.TangentialAccel + _gravityAttribute.TangentialAccelVal * TFRandomM11();
        }
        
        // dir(speed)
        for (int i = startIndex; i < _particleCount; ++i)
        {
            float a = radians(_angle + _angleVar * TFRandomM11());
            vec2 v(cos(a), sin(a));
            float s = _gravityAttribute.Speed + _gravityAttribute.SpeedVar * TFRandomM11();
            vec2 dir = v * s;
            _particles[i]._gravityMode.DirX = dir.x;
            _particles[i]._gravityMode.DirY = dir.y;
        }
    }
    // Mode Rotation
    else
    {
        // radius
        for (int i = startIndex; i < _particleCount; ++i)
        {
            _particles[i]._rotationMode.Radius = _rotationAttribute.StartRadius + _rotationAttribute.StartRadiusVar * TFRandomM11();
        }
        
        // delta radius
        for (int i = startIndex; i < _particleCount; ++i)
        {
            float endRadius = _rotationAttribute.EndRadius + _rotationAttribute.EndRadiusVar * TFRandomM11();
            _particles[i]._rotationMode.DeltaRadius = (endRadius - _particles[i]._rotationMode.Radius) / _particles[i]._timeToLive;
        }
        
        // 发射角度
        for (int i = startIndex; i < _particleCount; ++i)
        {
            _particles[i]._rotationMode.Angle = radians(_angle + _angleVar * TFRandomM11());
        }
        
        // 旋转角速度
        for (int i = startIndex; i < _particleCount; ++i)
        {
            _particles[i]._rotationMode.DeltaDegree = radians(_rotationAttribute.RotatePerSecond + _rotationAttribute.RotatePersecondVar * TFRandomM11());
        }
    }
}

// main loop
void ParticleSystem::Update(float dt)
{
    if (_isActive && _emissionRate)
    {
        float rate = 1.0f / _emissionRate; // 发射一个粒子需要的时间
        if (_particleCount < _maxParticleCount)
        {
            _timePerEmit += dt;
            if (_timePerEmit < 0.0f)
                _timePerEmit = 0.0f;
        }
        
        int emitCount = min(_maxParticleCount - _particleCount, int(_timePerEmit / rate)); // 计算本次update发射的粒子数
        AddParticles(emitCount);
        _timePerEmit -= rate * emitCount;
        
        _elapsed += dt;
        if (_elapsed < 0.0f)
            _elapsed = 0.0f;
        if (_duration != -1 && _duration < _elapsed)
        {
            StopSystem();
        }
    }
    
    // life decay
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particles[i]._timeToLive -= dt;
    }
    
    // 回收消亡粒子
    for (int i = 0 ; i < _particleCount; ++i)
    {
        if (_particles[i]._timeToLive <= 0.0f)
        {
            int j = _particleCount - 1;
            while (j > 0 && _particles[j]._timeToLive <= 0)
            {
                -- _particleCount;
                -- j;
            }
            _particles[i] = _particles[_particleCount - 1];
            
            -- _particleCount;
            
            // TODO: 完成发射自动移除的逻辑
        }
    }
    
    // Gravity
    if (_emitterMode == EmitterMode::GRAVITY)
    {
        for (int i = 0; i < _particleCount; ++i)
        {
            vec2 tmp, radial, tangential;
            
            // radial acceleration
            if (_particles[i]._posX || _particles[i]._posY)
            {
                // 计算径向加速度方向
                radial = normalize(vec2(_particles[i]._posX, _particles[i]._posY));
            }
            tangential = radial;
            radial.x *= _particles[i]._gravityMode.RadialAccel;
            radial.y *= _particles[i]._gravityMode.RadialAccel;
            
            // tangential accleration
            std::swap(tangential.x, tangential.y);
            tangential.x *= - _particles[i]._gravityMode.TangentialAccel;
            tangential.y *= _particles[i]._gravityMode.TangentialAccel;
            
            // (gravity + radial + tangential) * dt 就是速度增量
            tmp.x = radial.x + tangential.x + _gravityAttribute.Gravity.x;
            tmp.y = radial.y + tangential.y + _gravityAttribute.Gravity.y;
            tmp.x *= dt;
            tmp.y *= dt;
            
            _particles[i]._gravityMode.DirX += tmp.x;
            _particles[i]._gravityMode.DirY += tmp.y;
            
            // 计算位移增量
            tmp.x = _particles[i]._gravityMode.DirX * dt;
            tmp.y = _particles[i]._gravityMode.DirY * dt;
            _particles[i]._posX += tmp.x;
            _particles[i]._posY += tmp.y;
        }
    }
    // Rotation
    else
    {
        // 角度变化
        for (int i = 0; i < _particleCount; ++ i)
        {
            _particles[i]._rotationMode.Angle += _particles[i]._rotationMode.DeltaDegree * dt;
        }
        
        // 半径变化
        for (int i = 0 ; i < _particleCount ; ++i)
        {
            _particles[i]._rotationMode.Radius += _particles[i]._rotationMode.DeltaRadius * dt;
        }
        
        // 位置更新
        for (int i = 0; i < _particleCount; ++i)
        {
            _particles[i]._posX = - cos(_particles[i]._rotationMode.Angle) * _particles[i]._rotationMode.Radius;
        }
        for (int i = 0; i < _particleCount; ++i)
        {
            _particles[i]._posY = - sin(_particles[i]._rotationMode.Angle) * _particles[i]._rotationMode.Radius;
        }
    }

    // color
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particles[i]._colorR += _particles[i]._deltaColorR * dt;
    }
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particles[i]._colorG += _particles[i]._deltaColorG * dt;
    }
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particles[i]._colorB += _particles[i]._deltaColorB * dt;
    }
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particles[i]._colorA += _particles[i]._deltaColorA * dt;
    }

//    for (int i = 0 ; i < _particleCount; ++i)
//    {
//        float weight = 1 - _particles[i]._timeToLive / _lifeTime;
//        vec4 color = _colorRamp->GetColorAtOffset(weight);
//        _particles[i]._colorR = color.r;
//        _particles[i]._colorG = color.g;
//        _particles[i]._colorB = color.b;
//        _particles[i]._colorA = color.a;
//    }
    
    // size
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particles[i]._size += (_particles[i]._deltaSize * dt);
    }
    
    // angle
    for (int i = 0 ; i < _particleCount ; ++i)
    {
        _particles[i]._rotation += _particles[i]._deltaRotation * dt;
    }
    
    UpdateParticleQuads();
}
    
void ParticleSystem::StopSystem()
{
    _isActive = false;
    _elapsed = _duration;
    _timePerEmit = 0.0f;
}
    
void ParticleSystem::CopyParticle(int p1, int p2)
{
    _particles[p1] = _particles[p2];
}
