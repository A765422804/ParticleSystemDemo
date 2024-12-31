#version 330 core

in vec4 FragColor; // 从顶点着色器接收的颜色
in vec2 TexCoord; // 从顶点着色器接收的纹理坐标
out vec4 color; // 最终输出的颜色

uniform sampler2D ourTexture; // 纹理采样器

void main()
{
    // 使用纹理颜色和顶点颜色进行混合
    //color = texture(ourTexture, TexCoord) * FragColor;
    color = FragColor;
}
