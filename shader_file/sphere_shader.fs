#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform vec3 lightPos;
//uniform vec3 lightColor;
//uniform vec3 objectColor;

void main()
{
//    // 环境光
//    float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * lightColor;
//
//    // 漫反射
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = diff * lightColor;

    // 最终颜色
    vec3 result = vec3(1.0, 1.0, 1.0);
    FragColor = vec4(result, 1.0);
}
