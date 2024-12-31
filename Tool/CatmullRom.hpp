//
//  CatmullRom.hpp
//  openGL
//
//  Created by flowerdance on 2024/7/4.
//

#ifndef CatmullRom_hpp
#define CatmullRom_hpp

#include "../Common/Head.h"

class CatmullRom
{
public:
    CatmullRom(const std::vector<glm::vec2> &, const float &alpha, const int &num_point);
    std::vector<glm::vec2> getCurvePointArray() const;
private:
    float calculateT(const float &t, const glm::vec2 &, const glm::vec2 &); //计算t的值
    glm::vec2 calculatePoint(const float &t, const glm::vec2 &P0, const glm::vec2 &P1, const glm::vec2 &P2, const glm::vec2 &P3, const std::vector<float> &); //计算点的坐标
    void calculateCurve(); //计算曲线
    
    std::vector<glm::vec2> point_array; //原始点的数组
    std::vector<glm::vec2> curve_point_array; // 得到最终的结果
    
    float alpha;
    int num_point; //每两个原始点之间插入点的数目
};

#endif /* CatmullRom_hpp */
