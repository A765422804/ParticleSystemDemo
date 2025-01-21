//
//  ToolFunction.h
//  ParticleSystem
//
//  Created by evanbfeng on 2024/9/23.
//

#ifndef ToolFunction_h
#define ToolFunction_h

#include <iostream>
#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ToolFunction
{
public:
  static std::vector<vec2> calculatePointsOnCircle(double r, int n)
    {
      std::vector<vec2> points;
      double angleIncrement = 2 * M_PI / n; // 计算每个点之间的角度差

      for (int i = 0; i < n; ++i) 
      {
          double angle = i * angleIncrement;
          double x = r * cos(angle);
          double y = r * sin(angle);
          points.push_back(vec2(x, y));
      }

      return points;
  }
    
    static GLfloat randomInRange(GLfloat a, GLfloat b)
    {
        static std::random_device rd;  // 用于生成随机种子
        static std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
        std::uniform_real_distribution<float> dis(a, b); // 生成 [a, b] 范围内的浮点数

        return dis(gen);
    }
    
    static vec2 randomPointInAnnulus(float r1, float r2)
    {
        static std::random_device rd;  // 用于生成随机种子
        static std::mt19937 gen(rd()); // 使用 Mersenne Twister 作为随机数生成器
        std::uniform_real_distribution<float> radiusDist(r1, r2); // 半径分布
        std::uniform_real_distribution<float> angleDist(0.0f, 2 * M_PI); // 角度分布

        // 随机生成半径和角度
        float r = sqrt(radiusDist(gen)); // 确保均匀分布在圆环区域
        float theta = angleDist(gen);

        // 极坐标转换为笛卡尔坐标
        float x = r * cos(theta);
        float y = r * sin(theta);

        return vec2(x, y);
    }
};

#endif /* ToolFunction_h */
