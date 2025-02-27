#version 330 core

// in
layout(location = 0) in vec3 Position;      // 位置
layout(location = 1) in vec2 IndexAndSize; // 索引和大小
layout(location = 2) in vec2 TexCoord;     // 纹理坐标
layout(location = 3) in vec3 Velocity;     // 速度(方向)
layout(location = 4) in vec4 Color;        // 颜色

// out
out vec4 FragColor;
out vec2 FragUV;

// uniform
uniform mat4 WorldTransform;
uniform mat4 VPTransform;
uniform vec3 CameraPos;
uniform bool IsLocalSpace;

void main()
{
    vec4 pos = vec4(Position, 1.0);
    vec4 velocity = vec4(Velocity, 0);
    
    if (IsLocalSpace)
    {
        pos = WorldTransform * pos;
        velocity = WorldTransform * velocity;
    }
        
    float vertOffset = (IndexAndSize.x - 0.5) * IndexAndSize.y; // 得到一个原始点的两个分点
    vec3 camUp = normalize(cross(pos.xyz - CameraPos.xyz, velocity.xyz));
    pos.xyz += camUp * vertOffset;
    pos = VPTransform * pos;

    FragUV = TexCoord;
    FragColor = Color;
    
    gl_Position = pos;
}
