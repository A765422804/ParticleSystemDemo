#version 330 core

layout (location = 0) in vec3 aPos; // 位置属性

uniform mat4 model; // 坐标变换
uniform mat4 view;  // 视图矩阵
uniform mat4 projection; // 投影矩阵

void main()
{
    // 计算最终的顶点位置
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
