//
//  RandomNumberGenerate.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/25.
//

#pragma once

// 工具类，命名规范TF - ToolFunction

#include <iostream>
#include <random>

// M11: [-1, 1]
inline float TFRandomM11()
{
    static std::random_device rd;  // 用于生成随机种子
    static std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
    std::uniform_real_distribution<float> dis(-1, 1); // 生成 [a, b] 范围内的浮点数

    return dis(gen);
}
