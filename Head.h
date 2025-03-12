//
//  Head.h
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/31.
//

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>
#include <stdlib.h>
#include <signal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace glm;

#define AssertFatal(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "Fatal assertion failed: %s, file %s, line %d\n", #expr, __FILE__, __LINE__); \
            raise(SIGTRAP); /* 发送 SIGTRAP 信号以中断程序 */ \
            exit(EXIT_FAILURE); /* 确保程序退出 */ \
        } \
    } while (0)

#define AssertShouldNotReachHere() \
    do { \
            fprintf(stderr, "Fatal assertion failed: file %s, line %d\n", __FILE__, __LINE__); \
            raise(SIGTRAP); /* 发送 SIGTRAP 信号以中断程序 */ \
            exit(EXIT_FAILURE); /* 确保程序退出 */ \
    } while (0)

template <typename T>
inline T LinearInterpolate(T start, T end, float t)
{
    return start * (1 - t)+end * t;
}

template <typename T>
inline T CubicInterpolate(T from, T to, T pre, T post, float t) {
    return 0.5f *
           ((from * 2.0f) +
            (-pre + to) * t +
            (2.0f * pre - 5.0f * from +
             4.0f * to - post) * (t * t) +
            (-pre + 3.0f * from -
             3.0f * to + post) * (t * t * t));
}

template <typename T>
inline T BezierInterpolate(T p0, T p1, T p2, T p3, float t)
{
    float u = 1 - t;
    float coeff0 = u * u * u;
    float coeff1 = 3 * u * u * t;
    float coeff2 = 3 * u * t * t;
    float coeff3 = t * t * t;
    return coeff0 * p0 + coeff1 * p1 + coeff2 * p2 + coeff3 * p3;
}

// 返回t / length的浮点余数
inline float Repeat(float t, float length)
{
    return t - floor(t / length) * length;
}

inline float PingPong(float t, float length)
{
    t = Repeat(t, length * 2);
    t = length - abs(t - length);
    return t;
}

// 返回从-1到1的随机数
inline float RandomM11()
{
    static std::random_device rd;  // 用于生成随机种子
    static std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
    std::uniform_real_distribution<float> dis(-1, 1); // 生成 [a, b] 范围内的浮点数

    return dis(gen);
}

// 从0到1的随机数
inline float Random01()
{
    static std::random_device rd;  // 用于生成随机种子
    static std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
    std::uniform_real_distribution<float> dis(0, 1); // 生成 [a, b] 范围内的浮点数

    return dis(gen);
}

inline float RandomRange(float left, float right)
{
    static std::random_device rd;  // 用于生成随机种子
    static std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
    std::uniform_real_distribution<float> dis(left, right); // 生成 [a, b] 范围内的浮点数

    return dis(gen);
}

// 生成圆上给定角度的随机点
inline vec3 RandomPointBetweenCircleAtFixedAngle(float minRadius, float maxRadius, float theta)
{
    vec2 dir = vec2(cos(theta), sin(theta));
    vec3 res = vec3(dir.x, 0, dir.y);
    res.x *= minRadius + (maxRadius - minRadius) * Random01();
    res.z *= minRadius + (maxRadius - minRadius) * Random01();
    return res;
}

// 生成随机方向的单位向量
inline vec3 RandomUnitVector()
{
    float z = RandomM11();
    float a = RandomRange(0, 2 * M_PI);
    float r = sqrt(1 - z * z);
    float x = r * cos(a);
    float y = r * sin(a);
    return vec3(x, y, z);
    
}

// 生成给定半径的球体内部随机点
inline vec3 RandomPointBetweenSphere(float minRadius, float maxRadius)
{
    vec3 res = RandomUnitVector();
    res *= minRadius + (maxRadius - minRadius) * Random01();
    return res;
}

// 生成给定长宽高的立方体内部随机点
inline vec3 RandomPointInCube(vec3 extents)
{
    vec3 res = vec3(RandomRange(-extents.x, extents.x),
                    RandomRange(-extents.y, extents.y),
                    RandomRange(-extents.z, extents.z));
    return res;
}

// 随机生成-1或1
inline float RandomSign()
{
    float f = RandomM11();
    if (f >= 0) return 1.0f;
    else return -1.0f;
}

// 随机打乱Array
inline void RandomSortArray(std::vector<float>& vec) {
    // 使用随机设备生成种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    
    // 使用 std::shuffle 打乱 vector
    std::shuffle(vec.begin(), vec.end(), rng);
}

// 重载 << 运算符以输出 glm::mat4
inline std::ostream& operator<<(std::ostream& os, const glm::mat4& mat) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            os << mat[j][i] << " ";
        }
        os << std::endl;
    }
    return os;
}

// 重载 << 运算符以输出 glm::vec3
inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

// 使用 Hull-Dobell 算法的线性同余生成器构造伪随机数
inline float PseudoRandom(unsigned int seed)
{
    seed = (seed * 9301 + 49297) % 233280;
    return seed / (233280.0f);
}

inline unsigned int RandomRangeInt(unsigned int min, unsigned int max)
{
    return floor(RandomRange(min, max));
}
