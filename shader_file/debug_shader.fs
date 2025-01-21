#version 330 core

out vec4 color; // 最终输出的颜色

uniform vec4 wireFrameColor;

void main()
{
    color = vec4(wireFrameColor);
}
