//
//  Draw.hpp
//  openGL
//
//  Created by flowerdance on 2024/7/9.
//

#ifndef Draw_hpp
#define Draw_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

// 用来画图的
class Drawer
{
public:
    Drawer(std::vector<std::vector<float>> point);
    
    void bindAndDraw();
private:
    std::vector<unsigned int> VAO;
    std::vector<unsigned int> VBO;
    std::vector<unsigned int> EBO;
    
    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3,
    };
};

#endif /* Draw_hpp */
