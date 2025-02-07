#version 330 core

// in
layout(location = 0) in vec3 Position;      // 位置
layout(location = 1) in vec3 TexCoord;     // 纹理坐标
layout(location = 2) in vec3 Size;         // 大小
layout(location = 3) in vec3 Rotation;     // 旋转
layout(location = 4) in vec4 Color;        // 颜色

// out
out vec4 FragColor;
out vec2 FragUV;

// uniform
uniform mat4 WorldTransform;
uniform mat4 VPTransform;
uniform mat4 ViewInverse;

// function

vec4 QuaternionFromAxis (vec3 xAxis,vec3 yAxis,vec3 zAxis)
{
    mat3 m = mat3(xAxis,yAxis,zAxis);
    float trace = m[0][0] + m[1][1] + m[2][2];
    vec4 quat;
    if (trace > 0.) {
    float s = 0.5 / sqrt(trace + 1.0);
    quat.w = 0.25 / s;
    quat.x = (m[2][1] - m[1][2]) * s;
    quat.y = (m[0][2] - m[2][0]) * s;
    quat.z = (m[1][0] - m[0][1]) * s;
    } else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) {
    float s = 2.0 * sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]);
    quat.w = (m[2][1] - m[1][2]) / s;
    quat.x = 0.25 * s;
    quat.y = (m[0][1] + m[1][0]) / s;
    quat.z = (m[0][2] + m[2][0]) / s;
    } else if (m[1][1] > m[2][2]) {
    float s = 2.0 * sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]);
    quat.w = (m[0][2] - m[2][0]) / s;
    quat.x = (m[0][1] + m[1][0]) / s;
    quat.y = 0.25 * s;
    quat.z = (m[1][2] + m[2][1]) / s;
    } else {
    float s = 2.0 * sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]);
    quat.w = (m[1][0] - m[0][1]) / s;
    quat.x = (m[0][2] + m[2][0]) / s;
    quat.y = (m[1][2] + m[2][1]) / s;
    quat.z = 0.25 * s;
    }
    float len = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
    if (len > 0.) {
    len = 1. / sqrt(len);
    quat.x = quat.x * len;
    quat.y = quat.y * len;
    quat.z = quat.z * len;
    quat.w = quat.w * len;
    }
    return quat;
}

vec4 QuatMultiply (vec4 a, vec4 b){
    vec4 quat;
    quat.x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
    quat.y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
    quat.z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
    quat.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return quat;
}

vec4 QuaternionFromEuler (vec3 angle) // 欧拉角转四元数
{
    float x = angle.x / 2.;
    float y = angle.y / 2.;
    float z = angle.z / 2.;
    float sx = sin(x);
    float cx = cos(x);
    float sy = sin(y);
    float cy = cos(y);
    float sz = sin(z);
    float cz = cos(z);
    vec4 quat = vec4(0);
    quat.x = sx * cy * cz + cx * sy * sz;
    quat.y = cx * sy * cz + sx * cy * sz;
    quat.z = cx * cy * sz - sx * sy * cz;
    quat.w = cx * cy * cz - sx * sy * sz;
    return quat;
}

void RotateVecFromQuat (inout vec3 v, vec4 q){
    float ix = q.w * v.x + q.y * v.z - q.z * v.y;
    float iy = q.w * v.y + q.z * v.x - q.x * v.z;
    float iz = q.w * v.z + q.x * v.y - q.y * v.x;
    float iw = -q.x * v.x - q.y * v.y - q.z * v.z;
    v.x = ix * q.w + iw * -q.x + iy * -q.z - iz * -q.y;
    v.y = iy * q.w + iw * -q.y + iz * -q.x - ix * -q.z;
    v.z = iz * q.w + iw * -q.z + ix * -q.y - iy * -q.x;
}

vec3 RotateInLocalSpace (vec3 pos, vec3 xAxis, vec3 yAxis, vec3 zAxis, vec4 q)
{
    vec4 viewQuat = QuaternionFromAxis(xAxis, yAxis, zAxis);
    vec4 rotQuat = QuatMultiply(viewQuat, q);
    RotateVecFromQuat(pos, rotQuat);
    return pos;
}

void ComputeVertPos(inout vec4 pos, vec2 vertOffset, vec4 q, vec3 s
, mat4 viewInv) // 根据旋转和缩放计算实际位置
{
    vec3 viewSpaceVert = vec3(vertOffset.x * s.x, vertOffset.y * s.y, 0.);
    vec3 camX = normalize(vec3(viewInv[0][0], viewInv[1][0], viewInv[2][0]));
    vec3 camY = normalize(vec3(viewInv[0][1], viewInv[1][1], viewInv[2][1]));
    vec3 camZ = normalize(vec3(viewInv[0][2], viewInv[1][2], viewInv[2][2]));
    
    pos.xyz += RotateInLocalSpace(viewSpaceVert, camX, camY, camZ, q);
}

vec2 ComputeUV() // 计算实际纹理坐标
{
    // TODO: impl
    // 暂时不考虑sprite，因此纹理只是单张图
    return TexCoord.xy;
}

// main
void main()
{
    // TODO: 根据size和scale计算真实scale，暂时只考虑了size
    vec3 compScale = Size;
    
    vec4 pos = vec4(Position.xyz, 1.0);
    
    // TODO: 得到世界坐标，即绝对位置
    pos = WorldTransform * pos;
    
    // TODO: 计算rotate，还要考虑rotate随着时间变化的情况
    vec4 rot = QuaternionFromEuler(Rotation);
    
    // 计算顶点在四边形中相对中心点的偏移
    vec2 cornerOffset = vec2(TexCoord.xy - 0.5);
    
    // 计算顶点实际位置
    ComputeVertPos(pos, cornerOffset, rot, compScale, ViewInverse);
    //pos.xy += cornerOffset * compScale.xy; // 不face camera
    
    // 设置颜色
    FragColor = Color;
    
    // 设置uv
    FragUV = ComputeUV();
    
    // 使用VP矩阵
    pos = VPTransform * pos;
    
    gl_Position = pos;
}
