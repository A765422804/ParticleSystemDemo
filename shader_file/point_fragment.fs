#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    // 计算到点中心的距离（归一化）
    float dist = length(gl_PointCoord - vec2(0.5));

    // 根据距离计算新的透明度值
    float newAlpha = 1.0 - smoothstep(0.5, 0.5, dist);

    // 使用新的透明度值
    FragColor = vec4(vertexColor.rgb, vertexColor.a * newAlpha);
}
