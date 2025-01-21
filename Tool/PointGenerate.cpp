//
//  PointGenerate.cpp
//  openGL
//
//  Created by flowerdance on 2024/7/4.
//

#include "PointGenerate.hpp"

PointGenerate::PointGenerate(std::vector<vec2> array , const float &len):particle_length(len) 
{
    for (auto &i : array)
    {
        std::vector<float> vertices = getRectangleVertices(i.x, i.y);
        vertices_array.push_back(vertices);
    }
}

PointGenerate::PointGenerate(const int &num, const float &len, const float &step, int functionNo):count(num), particle_length(len), step(step), functionNo(functionNo)
{
    float x = -0.9f;
    
    for (int i = 0 ; i < count; ++i)
    {
        float y = functionGetY(x);
        point_array.push_back(vec2(x, y));
        vertices_array.push_back(getRectangleVertices(x, y));
        x += this->step;
    }
}

std::vector<std::vector<float>> PointGenerate::getVerticesArray() const
{
    return vertices_array;
}

std::vector<vec2> PointGenerate::getPointArray() const
{
    return point_array;
}

float PointGenerate::functionGetY(const float &x) const
{
    if (functionNo == 1)
    // acos(-1) == pi
        return 0.75 * sin(acos(-1) * x);
    
    else
        return 0.75 * cos(acos(-1) * (x - acos(-1)/2));
}

std::vector<float> PointGenerate::getRectangleVertices(const float &x, const float &y) const
{
    std::vector<float> res = {
        x + particle_length / 2, y + particle_length / 2 , 0.0, 1., 1., 1.,1.0f, 1.0f, // top right
        x + particle_length / 2, y - particle_length / 2 , 0.0, 1., 1., 1., 1.0f, 0.0f, // bottom right
        x - particle_length / 2, y - particle_length / 2 , 0.0, 1., 1., 1., 0.0f ,0.0f,// bottom left
        x - particle_length / 2, y + particle_length / 2 , 0.0, 1., 1., 1., 0.0f ,1.0f// top left
    };

    return res;
}
