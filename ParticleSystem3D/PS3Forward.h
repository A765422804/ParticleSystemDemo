//
//  Forward.h
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/9.
//

#pragma once

struct PVData // 粒子顶点的数据结构
{
    vec3 Position;
    vec3 Texcoord;
    vec3 Size;
    vec4 Rotation;
    vec4 Color; // 原版这个是写在了一个u32
    // 以上总计有17个float
};

struct PVDataGPU // 粒子顶点的数据结构
{
    vec4 Position_StartTime; // 位置和起始时间
    vec4 Size_Seed; // 大小
    vec4 Rotation; // 旋转
    vec4 Color; // 颜色
    vec4 Dir_Life; // 方向和生命
    vec2 Texcoord; // 纹理坐标
    // 以上共有22个float
};

struct TrailData // 纹理的数据结构
{
    vec3 Position;
    vec2 IndexAndSize;
    vec2 Texcoord;
    vec3 Velocity;
    vec4 Color;
    // 以上共有14个float
};

// 纹理的uv坐标
inline int uvs[8] =
{
    0, 0,
    1, 0,
    0, 1,
    1, 1,
};

namespace ModuleRandSeed {
    const int SIZE = 39825;
    const int TEXTURE = 90794;
    const int COLOR = 91041;
    const int FORCE = 212165;
    const int ROTATION = 125292;
    const int VELOCITY_X = 197866;
    const int VELOCITY_Y = 156497;
    const int VELOCITY_Z = 984136;
}

class PropertyObserver
{
public:
    virtual ~PropertyObserver() = default;
    virtual void OnValueChanged(const class PropertyObservable& observable) = 0;
};

class PropertyObservable
{
public:
    virtual ~PropertyObservable() = default; 
    void SetObserver(PropertyObserver* observer)
    {
        _observer = observer;
    }
    
protected:
    void NotifyObserver() const 
    {
        if (_observer)
        {
            _observer->OnValueChanged(*this);
        }
    }

private:
    PropertyObserver* _observer;
};
