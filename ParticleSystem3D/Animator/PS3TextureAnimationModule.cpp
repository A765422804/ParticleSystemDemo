//
//  PS3TextureAnimationModule.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "PS3TextureAnimationModule.hpp"

PS3TextureAnimationModule::PS3TextureAnimationModule(int numTilesX, int numTilesY, AnimationMode animationMode, CurveRangePtr frameOvertime, CurveRangePtr startFrame, float cycleCount, bool randomRow, int rowIndex)
    : _enable(true)
    , // 可以根据实际情况初始化 _enable 的值，这里假设默认启用
    _numTilesX(numTilesX)
    , _numTilesY(numTilesY)
    , _animationMode(animationMode)
    , _frameOvertime(frameOvertime)
    , _startFrame(startFrame)
    , _cycleCount(cycleCount)
    , _randomRow(randomRow)
    , _rowIndex(rowIndex)
{
}

void PS3TextureAnimationModule::Animate(PS3ParticlePtr p, float dt)
{
    float normalizedTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    float startFrame = _startFrame->Evaluate(normalizedTime, Random01()) / (_numTilesX * _numTilesY);
    float frameOvertime = _frameOvertime->Evaluate(normalizedTime, Random01());
    
    if (_animationMode == AnimationMode::WHOLE_SHEET)
    {
        p->_frameIndex = Repeat(_cycleCount * (frameOvertime + startFrame), 1);
    }
    else
    {
        float rowLength = 1.0 / _numTilesY; // 行高
        if (_randomRow) // 随机行
        {
            float f = Repeat(_cycleCount * (frameOvertime + startFrame), 1);
            float from = p->_startRow * rowLength;
            float to = from + rowLength;
            p->_frameIndex = LinearInterpolate(from, to, f);
        }
        else // 确定行
        {
            float from = _rowIndex * rowLength;
            float to = from + rowLength;
            p->_frameIndex = LinearInterpolate(from, to, Repeat(_cycleCount * (frameOvertime + startFrame), 1));
        }
    }
}
