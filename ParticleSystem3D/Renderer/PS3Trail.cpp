//
//  PS3Trail.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/2/12.
//

#include "PS3Trail.hpp"
#include "../PS3ParticleSystem.hpp"

const float DIRECTION_THRESHOLD = cos(radians(100.0f));
const int PRE_TRIANGLE_INDEX = 1;
const int NEXT_TRIANGLE_INDEX = 1 << 2;

PS3Trail::PS3Trail()
{
    _renderer = std::make_shared<TrailRenderer>();
    
    // 最小生成距离
    _minSquaredDistance = 0.001f;
    
    // trail life time
    _trailLifeTime = 1.0f;
    
    // color over trail
    _colorOverTrail = GradientRange::CreateByOneColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // color over time
    _colorOvertime = GradientRange::CreateByOneColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // width ratio
    _widthRatio = CurveRange::CreateCurveByConstant(0.1);
}

TrailSegment::TrailSegment(int maxTrailElementNum)
: _trailElements()
, _end(-1)
, _start(-1)
{
    while (maxTrailElementNum --)
    {
        _trailElements.push_back(std::make_shared<TrailElement>());
    }
    
}

TrailElementPtr TrailSegment::GetElement(int index)
{
    if (_start == -1) // no element
    {
        return nullptr;
    }
    if (index < 0 )
    {
        index = (index + _trailElements.size()) % int(_trailElements.size());
    }
    if (index >= _trailElements.size())
    {
        index %= _trailElements.size();
    }
    return _trailElements[index];
}

TrailElementPtr TrailSegment::AddElement()
{
    if (_trailElements.size() == 0)
        return nullptr;
    if (_start == -1) // 第一次取出元素
    {
        _start = 0;
        _end = 1;
        return _trailElements[0];
    }
    if (_start == _end)
    {
        auto insertPos = _trailElements.begin() + _end;
        _trailElements.insert(insertPos, std::make_shared<TrailElement>());
        
        _start = (_start + 1) % int(_trailElements.size());
    }
    auto newEleLoc = _end++;
    _end %= int(_trailElements.size());
    return _trailElements[newEleLoc];
}

int TrailSegment::Count()
{
    if (_start < _end)
        return _end - _start;
    else
        return int(_trailElements.size()) + _end - _start;
}

void TrailSegment::Clear()
{
    _start = -1;
    _end = -1;
}

void TrailSegment::IterateElements(std::shared_ptr<PS3Trail> targe, std::function<bool(std::shared_ptr<PS3Trail>, TrailElementPtr, PS3ParticlePtr, float)> func, PS3ParticlePtr p, float dt)
{
    auto end = _start >= _end ? _end + _trailElements.size() : _end;
    for (int i = _start ; i < end ; ++i)
    {
        if (func(targe, _trailElements[i % _trailElements.size()], p, dt))
        {
            // 说明粒子已经超过trailLifeTime，需要删除
            ++ _start;
            _start %= int(_trailElements.size());
        }
    }
    if (_start == _end)
    {
        _start = -1;
        _end = -1;
    }
}

void PS3Trail::Animate(PS3ParticlePtr p, float dt)
{
//    if (_trailSegments.size() == 0)
//        return;
    
    // TODO: 源码这里有基于loopCount的一个延迟机制
    
    auto trail = _particleTrail[p];
    if (trail == nullptr) // 还没有对应的拖尾，直接创建，然后返回
    {
        trail = std::make_shared<TrailSegment>();
        _particleTrail[p] = trail;
        _trailSegments.push_back(trail);
        return;
    }
    // 得到拖尾的头部元素
    auto lastSeg = trail->GetElement(trail->_end - 1);
    auto particlePos = p->_position; // TODO: 坐标变换
    
    
    if (lastSeg)
    {
        auto lambdaFunc = [this](PS3TrailPtr targe, TrailElementPtr element, PS3ParticlePtr p, float dt) {
            return this->UpdateTrailElement(element, p, dt);
        };
        trail->IterateElements(shared_from_this(), lambdaFunc, p, dt);
        float squaredDistance = length2(lastSeg->Position - particlePos);
        if (squaredDistance < _minSquaredDistance) {
            return;
        }
    }
    
    // 否则，创建新的元素
    lastSeg = trail->AddElement();
    if (!lastSeg)
    {
        return;
    }
    
    // 初始化新元素的属性
    // 位置
    lastSeg->Position = particlePos;
    lastSeg->LifeTime = 0;
    //宽度
    if (_widthFromParticle)
    {
        lastSeg->Width = p->_size.x * _widthRatio->Evaluate(0, Random01());
    }
    else
    {
        lastSeg->Width = _widthRatio->Evaluate(0, Random01());
    }
    // 颜色
    if (_colorFromParticle)
    {
        lastSeg->Color = p->_color;
    }
    else
    {
        lastSeg->Color = _colorOvertime->Evaluate(0, Random01());
    }
    // velocity，即方向向量
    auto trailNum = trail->Count();
    if (trailNum == 2) // 只有两个元素
    {
        auto lastSecondSeg = trail->GetElement(trail->_end - 2); // 获取第二个元素
        lastSecondSeg->Velocity = lastSeg->Position - lastSecondSeg->Position; // 指向第一个元素 TODO: 是否要进行归一化
    }
    else if (trailNum > 2)
    {
        auto lastSecondSeg = trail->GetElement(trail->_end - 2); // 获取第二个元素
        auto lastThirdSeg = trail->GetElement(trail->_end - 3); // 获取第三个元素
        
        auto tempVec1 = lastThirdSeg->Position - lastSecondSeg->Position;
        auto tempVec2 = lastSeg->Position - lastSecondSeg->Position;
        lastSecondSeg->Velocity = tempVec2 - tempVec1;
        if (lastSecondSeg->Velocity == zero<vec3>())
            lastSecondSeg->Velocity = tempVec1;
        lastSecondSeg->Velocity = normalize(lastSecondSeg->Velocity);
        CheckDirectionReverse(lastSecondSeg, lastThirdSeg);
    }
}

bool PS3Trail::UpdateTrailElement(TrailElementPtr trailElement, PS3ParticlePtr p, float dt)
{
    trailElement->LifeTime += dt;
    
    // 颜色
    if (_colorFromParticle)
    {
        trailElement->Color = p->_color; // 从粒子中获得颜色
        trailElement->Color *= _colorOvertime->Evaluate(1 - p->_remainingLifeTime / p->_startLifeTime, Random01());
    }
    else
    {
        trailElement->Color = _colorOvertime->Evaluate(1 - p->_remainingLifeTime / p->_startLifeTime, Random01());
    }
    
    // 宽度
    if (_widthFromParticle)
    {
        trailElement->Width = p->_size.x * _widthRatio->Evaluate(trailElement->LifeTime / _trailLifeTime, Random01());
    }
    else
    {
        trailElement->Width = _widthRatio->Evaluate(trailElement->LifeTime / _trailLifeTime, Random01());
    }
    
    return trailElement->LifeTime > _trailLifeTime; // element是否已经过期
}

void PS3Trail::CheckDirectionReverse(TrailElementPtr currElement, TrailElementPtr prevElement)
{
    if (dot(currElement->Velocity, prevElement->Velocity) < DIRECTION_THRESHOLD) // 判断角度是否达到阈值
    {
        currElement->Direction = 1 - prevElement->Direction;
    }
    else
    {
        currElement->Direction = prevElement->Direction;
    }
}

void PS3Trail::RemoveParticle(PS3ParticlePtr p)
{
    auto trail = _particleTrail[p];
    if (trail && _trailSegments.size() != 0)
    {
        trail->Clear();
        _particleTrail.erase(p);
        for (auto it = _trailSegments.begin(); it != _trailSegments.end(); ++it)
        {
            if (*it == trail)
            {
                _trailSegments.erase(it);
                break;
            }
        }
    }
}

void PS3Trail::UpdateRenderData()
{
    // 每次清空上次的buffer的内容
    _vBuffer.clear();
    _iBuffer.clear();
    auto indexOffset = 0;
    
    for (auto& [p, t] : _particleTrail) // 遍历所有的粒子和粒子的trail
    {
        if (t->_start == -1)
            continue;
        // TODO: 计算indexOffset

        auto end = t->_start >= t->_end ? t->_end + t->_trailElements.size() : t->_end;
        auto trailNum = end - t->_start;
        auto textCoordSeg = 1.0f / trailNum;
        auto startSegEle = t->_trailElements[t->_start];
        FillVertexBuffer(startSegEle, _colorOverTrail->Evaluate(1, Random01()), indexOffset, 1, 0, NEXT_TRIANGLE_INDEX);
        
        for (auto i = t->_start + 1; i < end ; ++i)
        {
            auto segEle = t->_trailElements[i % t->_trailElements.size()];
            auto j = i - t->_start;
            if (i == end - 1)
                FillVertexBuffer(segEle, _colorOverTrail->Evaluate(1 - j / trailNum, Random01()), indexOffset, 1 - j * textCoordSeg, j, PRE_TRIANGLE_INDEX);
            else
                FillVertexBuffer(segEle, _colorOverTrail->Evaluate(1 - j / trailNum, Random01()), indexOffset, 1 - j * textCoordSeg, j, PRE_TRIANGLE_INDEX | NEXT_TRIANGLE_INDEX);
        }
        indexOffset += 2 * trailNum;
        
        // TODO: 接下来是把粒子的位置作为一个临时的元素的情况
    }
}

void PS3Trail::FillVertexBuffer(TrailElementPtr trailSeg, vec4 ColorModifer, int indexOffset, float xTexCoord, int trailElementIdx, int indexSet)
{
    // 一个顶点
    _vBuffer.push_back(trailSeg->Position.x);
    _vBuffer.push_back(trailSeg->Position.y);
    _vBuffer.push_back(trailSeg->Position.z);
    _vBuffer.push_back(trailSeg->Direction);
    _vBuffer.push_back(trailSeg->Width);
    _vBuffer.push_back(xTexCoord);
    _vBuffer.push_back(0);
//    _vBuffer.push_back(trailSeg->Velocity.x);
//    _vBuffer.push_back(trailSeg->Velocity.y);
//    _vBuffer.push_back(trailSeg->Velocity.z);
    _vBuffer.push_back(0);
    _vBuffer.push_back(1);
    _vBuffer.push_back(0);
    auto color = ColorModifer * trailSeg->Color;
    _vBuffer.push_back(color.r);
    _vBuffer.push_back(color.g);
    _vBuffer.push_back(color.b);
    _vBuffer.push_back(color.a);
    
    // 另一个顶点
    _vBuffer.push_back(trailSeg->Position.x);
    _vBuffer.push_back(trailSeg->Position.y);
    _vBuffer.push_back(trailSeg->Position.z);
    _vBuffer.push_back(1 - trailSeg->Direction);
    _vBuffer.push_back(trailSeg->Width);
    _vBuffer.push_back(xTexCoord);
    _vBuffer.push_back(1);
//    _vBuffer.push_back(trailSeg->Velocity.x);
//    _vBuffer.push_back(trailSeg->Velocity.y);
//    _vBuffer.push_back(trailSeg->Velocity.z);
    _vBuffer.push_back(0);
    _vBuffer.push_back(1);
    _vBuffer.push_back(0);
    _vBuffer.push_back(color.r);
    _vBuffer.push_back(color.g);
    _vBuffer.push_back(color.b);
    _vBuffer.push_back(color.a);
    
    // 索引数据
    if (indexSet & PRE_TRIANGLE_INDEX) { // 与前一个构建索引
        _iBuffer.push_back(indexOffset + 2 * trailElementIdx);
        _iBuffer.push_back(indexOffset + 2 * trailElementIdx - 1);
        _iBuffer.push_back(indexOffset + 2 * trailElementIdx + 1);
    }
    if (indexSet & NEXT_TRIANGLE_INDEX) { // 与后一个构建索引
        _iBuffer.push_back(indexOffset + 2 * trailElementIdx);
        _iBuffer.push_back(indexOffset + 2 * trailElementIdx + 1);
        _iBuffer.push_back(indexOffset + 2 * trailElementIdx + 2);
    }
}

void PS3Trail::Render()
{
    _renderer->SetVertexData(_vBuffer);
    if (_iBuffer.size() <= 3) // hack的写法，避免只有一个顶点的时候进行渲染，当完成在粒子位置形成temp的顶点时可以删掉。
        return;
    if (_iBuffer.size())
    {
        std::vector<unsigned> indice(_iBuffer.begin(), _iBuffer.begin() + _iBuffer.size());
        _renderer->SetIndexData(indice);
    }
    else
        _renderer->SetIndexData(_iBuffer);
    
    _renderer->_shader->use();
    _renderer->_shader->setBool("IsLocalSpace", _ps->_spaceMode == SpaceMode::LOCAL);
    
    if (_ps->_isSubEmitter)
    {
        _renderer->SetWorldTransform(_ps->_mainEmitter->GetWorldTransform());
    }
    else
    {
        _renderer->SetWorldTransform(_ps->GetWorldTransform());
    }
    _renderer->Render();
}

void PS3Trail::Clear()
{
    // TODO: 我只是简单的clear vector
    _trailSegments.clear();
    _particleTrail.clear();
}
