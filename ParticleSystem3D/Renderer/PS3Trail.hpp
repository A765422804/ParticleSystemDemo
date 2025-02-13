//
//  PS3Trail.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/12.
//

#pragma once
#include "../PS3Common.h"
#include "../PS3Particle.hpp"
#include "../../Tool/GradientRange.hpp"
#include "../../Tool/CurveRange.hpp"

struct TrailElement
{
    vec3 Position;
    float LifeTime;
    float Width;
    vec3 Velocity; // 方向
    int Direction; // 由一个原始点得到的左右两个顶点的方向
    vec4 Color;
};

using TrailElementPtr = std::shared_ptr<TrailElement>;

class PS3Trail;

// the valid element is in [start,end) range.if start equals -1,it represents the array is empty.
class TrailSegment
{
public:
    TrailSegment(int maxTrailElementNum = 100);
    ~TrailSegment() = default;
    
public:
    TrailElementPtr GetElement(int index); // 返回指定的元素
    TrailElementPtr AddElement(); // 从trailElements取出一个新元素并返回
    
public:
    int Count(); // 返回trailElements中元素的个数
    void Clear(); // 清空trailElements

public:
    void IterateElements(std::shared_ptr<PS3Trail> targe, std::function<bool(std::shared_ptr<PS3Trail>, TrailElementPtr, PS3ParticlePtr, float)> func, PS3ParticlePtr p, float dt);
    
public:
    int _start;
    int _end;
    std::vector<TrailElementPtr> _trailElements;
};

using TrailSegmentPtr = std::shared_ptr<TrailSegment>;

class PS3Trail : public std::enable_shared_from_this<PS3Trail>
{
public:
    PS3Trail();
    ~PS3Trail() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt);
    
public:
    bool UpdateTrailElement(TrailElementPtr trailElement, PS3ParticlePtr p, float dt); // 更新某个element
    
    void CheckDirectionReverse(TrailElementPtr currElement, TrailElementPtr prevElement); // 判断是否反向
    
    void RemoveParticle(PS3ParticlePtr p); // 粒子消亡时对于particle的处理
    
public:
    void UpdateRenderData(); // 更新渲染数据
    void FillVertexBuffer(); // 填充顶点数据
    void Render(); // 把渲染数据传递给Renderer告诉Renderer进行渲染
    
public:
    float _minSquaredDistance; // 两个原始点间的最小距离
    
    bool _colorFromParticle; // 颜色随着粒子
    GradientRangePtr _colorOvertime; // 随时间变化的颜色
    
    bool _widthFromParticle;
    CurveRangePtr _widthRatio; // 宽度比例
    
    float _trailLifeTime; // 拖尾的生命周期
    
    std::unordered_map<PS3ParticlePtr, TrailSegmentPtr> _particleTrail; // 每个粒子对应的轨迹
    std::vector<TrailSegmentPtr> _trailSegments; // 所有的轨迹
    
public: // buffer
    std::vector<float> _vBuffer;
    std::vector<unsigned int> _iBuffer;
    
    int _vBufferOffset;
    int _iBufferOffset;
};

using PS3TrailPtr = std::shared_ptr<PS3Trail>;
