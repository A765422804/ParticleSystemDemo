#version 330 core

layout (location = 0) in vec3 aPos; // 位置属性
layout (location = 1) in vec4 aColor; // 颜色属性
layout (location = 2) in vec2 aTexCoord; // 纹理坐标属性

out vec4 FragColor; // 输出颜色到片段着色器
out vec2 TexCoord; // 输出纹理坐标到片段着色器

uniform mat4 view;  // 视图矩阵
uniform mat4 projection; // 投影矩阵

void main()
{
    // 计算最终的顶点位置
    gl_Position = projection * view * vec4(aPos, 1.0);
    
    // 将颜色和纹理坐标传递给片段着色器
    FragColor = aColor;
    TexCoord = aTexCoord;
}
