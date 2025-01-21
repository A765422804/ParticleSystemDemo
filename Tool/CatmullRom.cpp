//
//  CatmullRom.cpp
//  openGL
//
//  Created by flowerdance on 2024/7/4.
//

#include "CatmullRom.hpp"

CatmullRom::CatmullRom(const std::vector<vec2> &points, const float &alpha, const int &num_point):alpha(alpha), num_point(num_point)
{
    vec2 P0(points[0][0], points[0][1]);
    vec2 P1(points[1][0], points[1][1]);
    vec2 Pstart(2 * P0.x - P1.x, 2 * P0.y - P1.y);
    
    vec2 P3(points[points.size() - 2][0], points[points.size() - 2][1]);
    vec2 P4(points[points.size() - 1][0], points[points.size() - 1][1]);
    vec2 Pend(2 * P4.x - P3.x, 2 * P4.y - P3.y);
    
    point_array.push_back(Pstart);
    for (auto &i: points)
    {
        point_array.push_back(vec2(i[0], i[1]));
    }
    point_array.push_back(Pend);
    
    calculateCurve();
}

std::vector<vec2> CatmullRom::getCurvePointArray() const
{
    return curve_point_array;
}

float CatmullRom::calculateT(const float &t, const vec2 & P1, const vec2 & P2)
{
    float a = pow((P1.x - P2.x), 2) + pow((P1.y - P2.y), 2);
    float b = pow(a, 0.5);
    float c = pow(b, alpha);
    
    return c + t;
}

vec2 CatmullRom::calculatePoint(const float &t, const vec2 &P0, const vec2 &P1, const vec2 &P2, const vec2 &P3, const std::vector<float> &ts)
{
    float t0 = 0;
    float t1 = ts[0];
    float t2 = ts[1];
    float t3 = ts[2];
    
    vec2 a1 = {(t1 - t) / (t1 - t0) * P0.x + (t - t0) / (t1 - t0) * P1.x,
                    (t1 - t) / (t1 - t0) * P0.y + (t - t0) / (t1 - t0) * P1.y};
    vec2 a2 = {(t2 - t) / (t2 - t1) * P1.x + (t - t1) / (t2 - t1) * P2.x,
                (t2 - t) / (t2 - t1) * P1.y + (t - t1) / (t2 - t1) * P2.y};
    vec2 a3 = {(t3 - t) / (t3 - t2) * P2.x + (t - t2) / (t3 - t2) * P3.x,
                (t3 - t) / (t3 - t2) * P2.y + (t - t2) / (t3 - t2) * P3.y};

    vec2 b1 = {(t2 - t) / (t2 - t0) * a1.x + (t - t0) / (t2 - t0) * a2.x,
                (t2 - t) / (t2 - t0) * a1.y + (t - t0) / (t2 - t0) * a2.y};
    vec2 b2 = {(t3 - t) / (t3 - t1) * a2.x + (t - t1) / (t3 - t1) * a3.x,
                (t3 - t) / (t3 - t1) * a2.y + (t - t1) / (t3 - t1) * a3.y};

    vec2 c = {(t2 - t) / (t2 - t1) * b1.x + (t - t1) / (t2 - t1) * b2.x,
               (t2 - t) / (t2 - t1) * b1.y + (t - t1) / (t2 - t1) * b2.y};
    
    return c;
}

void CatmullRom::calculateCurve()
{
    const int cost = 4;
    for (int i = 0 ; i < point_array.size() - (cost - 1) ; ++i)
    {
        
        float t0 = 0;
        float t1 = calculateT(t0, point_array[i], point_array[i + 1]);
        float t2 = calculateT(t1, point_array[i + 1], point_array[i + 2]);
        float t3 = calculateT(t2, point_array[i + 2], point_array[i + 3]);
        
        float step = (t2 - t1) / num_point;
        for (float j = t1; j <= t2 ; j += step)
        {
            vec2 c = calculatePoint(j, point_array[i], point_array[i + 1], point_array[i + 2], point_array[i + 3], std::vector<float>{t1, t2, t3});
            curve_point_array.push_back(c);
        }
    }
}
