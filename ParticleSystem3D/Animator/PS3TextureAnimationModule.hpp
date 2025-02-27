//
//  PS3TextureAnimationModule.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#pragma once
#include "../PS3Common.h"
#include "../../Tool/CurveRange.hpp"
#include "../PS3Particle.hpp"
#include "PS3OvertimeModule.hpp"

class PS3TextureAnimationModule : public PS3OvertimeModule
{
public:
    PS3TextureAnimationModule(int numTilesX, int numTilesY, AnimationMode animationMode, CurveRangePtr frameOvertime, CurveRangePtr startFrame, float cycleCount, bool randomRow, int rowIndex);
    ~PS3TextureAnimationModule() = default;
    
public:
    void Animate(PS3ParticlePtr p, float dt) override;
    void SetUniform();
    
public:
    bool _isInitStartRow;
    // x和y方向图片数量，x方向朝右，y方向朝下
    int _numTilesX;
    int _numTilesY;
    
    // 动画播放方式
    AnimationMode _animationMode;
    
    // 一个周期内动画播放的帧与时间变化曲线
    CurveRangePtr _frameOvertime;
    
    // 起始从第几帧开始播放和一个生命周期内的循环次数
    CurveRangePtr _startFrame;
    float _cycleCount;
    
    // 播放方式为SingleRow时随机指定一行播放或指定某行播放
    bool _randomRow; // 随机播放，用到粒子的startRow属性（创建粒子时随机初始化一个行号）
    int _rowIndex; // 从0开始
};

using PS3TextureAnimationModulePtr = std::shared_ptr<PS3TextureAnimationModule>;

// TODO: 仅支持grid mode
