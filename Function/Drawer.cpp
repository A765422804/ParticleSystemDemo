//
//  Draw.cpp
//  openGL
//
//  Created by flowerdance on 2024/7/9.
//

#include "Drawer.hpp"

Drawer::Drawer(std::vector<std::vector<float>> point)
{
    int len = point.size();
    VAO.resize(len);
    VBO.resize(len);
    EBO.resize(len);
    
    glGenVertexArrays(len, VAO.data());
    glGenBuffers(len, VBO.data());
    glGenBuffers(len, EBO.data());
    
    for (int i = 0 ; i< len; ++i)
    {
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, point[i].size() * sizeof(float), point[i].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        // vertex attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);
        //texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Drawer::bindAndDraw()
{
    for (auto i : VAO)
    {
        glBindVertexArray(i);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
