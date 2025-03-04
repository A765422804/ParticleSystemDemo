#version 330 core

// in
in vec4 FragColor;
in vec2 FragUV;

// out
out vec4 FinalColor;

// uniform
uniform sampler2D MainTexture;

// function
vec4 Add()
{
    // TODO: 源码还有一个tintColor作为底色，暂略
    vec4 color = 2.0 * FragColor * texture(MainTexture, FragUV);
    return color;
}

// main
void main()
{
    FinalColor = Add();
}
