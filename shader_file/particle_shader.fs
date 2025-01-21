#version 330 core

in vec4 FragColor; // 从顶点着色器接收的颜色
in vec2 TexCoord; // 从顶点着色器接收的纹理坐标
out vec4 color; // 最终输出的颜色

uniform sampler2D ourTexture; // 纹理采样器
uniform bool useTexture;

void main()
{
    if (useTexture)
    {
        color = texture(ourTexture, TexCoord);
    }
    else
    {
        color = FragColor;
    }
}
