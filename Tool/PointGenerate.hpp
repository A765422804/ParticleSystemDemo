//
//  PointGenerate.hpp
//  openGL
//
//  Created by flowerdance on 2024/7/4.
//

#ifndef PointGenerate_hpp
#define PointGenerate_hpp

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointGenerate
{
public:
    PointGenerate(std::vector<glm::vec2>, const float &); // 由中心点的集合创建所有的矩形的顶点集合
    PointGenerate(const int &num, const float &len, const float &step, int functionNo); //根据偏移量来创建中心点和顶点的集合
    std::vector<std::vector<float>> getVerticesArray() const; // 得到顶点
    std::vector<glm::vec2> getPointArray() const; // 得到中心点
private:
    float functionGetY(const float &) const; //根据x得到y的值
    std::vector<float> getRectangleVertices (const float &x, const float &y) const; //通过中心点得到矩形顶点
    
    int functionNo;
    int count; //点的数量
    float particle_length; //点的长度
    float step; //点的步长
    std::vector<glm::vec2> point_array; //中心点的集合
    std::vector<std::vector<float>> vertices_array; //矩形顶点的集合
};

#endif /* PointGenerate_hpp */
