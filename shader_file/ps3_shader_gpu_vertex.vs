// TODO: 省略了overtimeTexture的mode，也就是，只考虑height=1的情况

#version 330 core
#pragma optimize(off)

// in
layout(location = 0) in vec4 Position_StartTime; // 位置和起始时间
layout(location = 1) in vec3 Size;                 // 大小
layout(location = 2) in vec4 Rotation;             // 旋转
layout(location = 3) in vec4 Color;                // 颜色
layout(location = 4) in vec4 Dir_Life;             // 方向和生命
layout(location = 5) in vec2 Texcoord;             // 纹理坐标

// out
out vec4 FragColor;
out vec2 FragUV;

// uniform - overtime bool
uniform bool UseAnimationOvertime;
uniform bool UseVelocityOvertime;
uniform bool UseForceOvertime;
uniform bool UseSizeOvertime;
uniform bool UseColorOvertime;
uniform bool UseRotationOvertime;

// uniform - overtime texture
uniform sampler2D texture_animation_tex0;
uniform sampler2D texture_velocity_tex0;
uniform sampler2D texture_force_tex0;
uniform sampler2D texture_size_tex0;
uniform sampler2D texture_color_tex0;
uniform sampler2D texture_rotation_tex0;

// uniform overtime data
uniform vec2 SampleInfo; // overtime的纹理采样信息，分别是sampleNum和sampleInterval
uniform vec3 AnimInfo; // 动画信息，分别是高度、大小和循环次数

// uniform - data
uniform vec2 Time_Delta; // 粒子系统运行总时间和dt
uniform vec2 FrameTile; // 动画的x和y

// uniform - transform
uniform mat4 ViewInverse;
uniform bool IsLocalSpace;
uniform mat4 WorldTransform;
uniform mat4 VPTransform;

// function
vec3 UnpackCurveData(sampler2D tex, vec2 coord)
{
    vec4 a = texture(tex, coord);
    vec4 b = texture(tex, coord + SampleInfo.y);
    float c = fract(coord.x * SampleInfo.x);
    return mix(a.xyz, b.xyz, c);
}

vec3 UnpackCurveData(sampler2D tex, vec2 coord, out float w)
{
    vec4 a = texture(tex, coord);
    vec4 b = texture(tex, coord + SampleInfo.y);
    float c = fract(coord.x * SampleInfo.x);
    w = mix(a.w, b.w, c);
    return mix(a.xyz, b.xyz, c);
}

vec4 QuaternionFromAxis (vec3 xAxis,vec3 yAxis,vec3 zAxis){
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

vec4 EulerToQuat(vec3 euler) {
 vec3 er = euler * 0.5;
 float x = er.x, y = er.y, z = er.z;
 float sx = sin(x);
 float cx = cos(x);
 float sy = sin(y);
 float cy = cos(y);
 float sz = sin(z);
 float cz = cos(z);
 vec4 quat;
 quat.x = sx * cy * cz + cx * sy * sz;
 quat.y = cx * sy * cz + sx * cy * sz;
 quat.z = cx * cy * sz - sx * sy * cz;
 quat.w = cx * cy * cz - sx * sy * sz;
 return quat;
}

mat3 QuatToMat3(vec4 q) {
 vec3 m0 = vec3(
   1.0 - 2.0 * q.y * q.y - 2.0 * q.z * q.z,
   2.0 * q.x * q.y + 2.0 * q.w * q.z,
   2.0 * q.x * q.z - 2.0 * q.w * q.y);
    vec3 m1 = vec3(
   2.0 * q.x * q.y - 2.0 * q.w * q.z,
   1.0 - 2.0 * q.x * q.x - 2.0 * q.z * q.z,
   2.0 * q.y * q.z + 2.0 * q.w * q.x);
    vec3 m2 = vec3(
   2.0 * q.x * q.z + 2.0 * q.w * q.y,
   2.0 * q.y * q.z - 2.0 * q.w * q.x,
   1.0 - 2.0 * q.x * q.x - 2.0 * q.y * q.y);
 return mat3(m0, m1, m2);
}

vec4 Mat3ToQuat(mat3 mat) {
 float tr = mat[0][0] + mat[1][1] + mat[2][2];
    float qw, qx, qy, qz;
 if (tr > 0.0) {
   float S = sqrt(tr + 1.0) * 2.0;
      float invS = 1.0 / S;
      qw = 0.25 * S;
      qx = (mat[1][2] - mat[2][1]) * invS;
      qy = (mat[2][0] - mat[0][2]) * invS;
      qz = (mat[0][1] - mat[1][0]) * invS;
 } else if ((mat[0][0] > mat[1][1])&&(mat[0][0] > mat[2][2])) {
   float S = sqrt(1.0 + mat[0][0] - mat[1][1] - mat[2][2]) * 2.0;
      float invS = 1.0 / S;
      qw = (mat[1][2] - mat[2][1]) * invS;
      qx = 0.25 * S;
      qy = (mat[1][0] + mat[0][1]) * invS;
      qz = (mat[2][0] + mat[0][2]) * invS;
 } else if (mat[1][1] > mat[2][2]) {
      float S = sqrt(1.0 + mat[1][1] - mat[0][0] - mat[2][2]) * 2.0;
      float invS = 1.0 / S;
      qw = (mat[2][0] - mat[0][2]) * invS;
      qx = (mat[1][0] + mat[0][1]) * invS;
      qy = 0.25 * S;
      qz = (mat[2][1] + mat[1][2]) * invS;
 } else {
      float S = sqrt(1.0 + mat[2][2] - mat[0][0] - mat[1][1]) * 2.0;
      float invS = 1.0 / S;
      qw = (mat[0][1] - mat[1][0]) * invS;
      qx = (mat[2][0] + mat[0][2]) * invS;
      qy = (mat[2][1] + mat[1][2]) * invS;
      qz = 0.25 * S;
 }
 return vec4(qx, qy, qz, qw);
}

float Repeat (float t, float length) { // 取模函数
 return t - floor(t / length) * length;
}

vec4 QuatMultiply (vec4 a, vec4 b){
 vec4 quat;
 quat.x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
 quat.y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
 quat.z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
 quat.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
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

vec3 RotateInLocalSpace (vec3 pos, vec3 xAxis, vec3 yAxis, vec3 zAxis, vec4 q){
 vec4 viewQuat = QuaternionFromAxis(xAxis, yAxis, zAxis);
 vec4 rotQuat = QuatMultiply(viewQuat, q);
 RotateVecFromQuat(pos, rotQuat);
 return pos;
}

vec2 ComputeUV (float frameIndex, vec2 vertIndex, vec2 frameTile)
{
 vec2 aniUV = vec2(0, floor(frameIndex * frameTile.y));
 aniUV.x = floor(frameIndex * frameTile.x * frameTile.y - aniUV.y * frameTile.x);
 vertIndex.y = 1. - vertIndex.y;
 return (aniUV.xy + vertIndex) / vec2(frameTile.x, frameTile.y);
}

void ComputeVertPos (inout vec4 pos, vec2 vertOffset, vec4 q, vec3 s, mat4 viewInv) 
{
 vec3 viewSpaceVert = vec3(vertOffset.x * s.x, vertOffset.y * s.y, 0.);
 vec3 camX = normalize(vec3(viewInv[0][0], viewInv[1][0], viewInv[2][0]));
 vec3 camY = normalize(vec3(viewInv[0][1], viewInv[1][1], viewInv[2][1]));
 vec3 camZ = normalize(vec3(viewInv[0][2], viewInv[1][2], viewInv[2][2]));
 pos.xyz += RotateInLocalSpace(viewSpaceVert, camX, camY, camZ, q);
}

// main
void main()
{
    float activeTime = Time_Delta.x - Position_StartTime.w; // 粒子已存活时间
    float normalizedTime = clamp(activeTime / Dir_Life.w, 0.0, 1.0); // 粒子存活时间归一化
    vec2 timeCoord0 = vec2(normalizedTime, 0.0); // 粒子存活时间归一化后的坐标(height = 0)
    vec2 timeCoord1 = vec2(normalizedTime, 1.0); // 粒子存活时间归一化后的坐标(height = 1)
    
    vec2 vertIdx = Texcoord;
    vec4 velocity = vec4(Dir_Life.xyz, 0.0);
    vec4 pos = vec4(Position_StartTime.xyz, 1.0);
    vec3 size = Size;
    
    // overtime module
    // 1. size overtime
    if (UseSizeOvertime == true)
    {
       size *= UnpackCurveData(texture_size_tex0, timeCoord0);
    }
    // TODO: 根据size和scale计算真实scale，暂时只考虑了size
    vec3 compScale = size;
    
    // 2. force overtime
    if (UseForceOvertime == true)
    {
        vec3 forceAnim = vec3(0.0);
        forceAnim = UnpackCurveData(texture_force_tex0, timeCoord0);
        
        vec4 forceTrack = vec4(forceAnim, 0.0);
        // TODO: 根据force的space执行坐标变换
        
        velocity.xyz += forceTrack.xyz;
    }
    
    // 3. velocity overtime
    if (UseVelocityOvertime == true)
    {
        float speedModifier = 1.0f;
        vec3 velocityAnim = vec3(0.0);
        velocityAnim = UnpackCurveData(texture_velocity_tex0, timeCoord0, speedModifier);
        
        vec4 velocityTrack = vec4(velocityAnim, 0.0);
        // TODO: 根据velocity的space执行坐标变换
        velocity.xyz += velocityTrack.xyz;
        velocity.xyz *= speedModifier;
    }
    
    pos.xyz += velocity.xyz * normalizedTime * Dir_Life.w;
    
    // TODO: 对粒子执行坐标变换
    
    // 4. rotation overtime
    vec4 rot = Rotation;
    if (UseRotationOvertime == true)
    {
        vec3 euler = UnpackCurveData(texture_rotation_tex0, timeCoord0)  * normalizedTime * Dir_Life.w; // 得到总计旋转角度
        vec4 quat = EulerToQuat(euler); // 得到旋转四元数
        mat3 mLocal = QuatToMat3(quat); // 得到旋转矩阵
        mat3 mStart = QuatToMat3(rot);
        rot = Mat3ToQuat(mStart * mLocal);
    }
    
    // 5. color overtime
    if (UseColorOvertime == true)
    {
        FragColor = Color * texture(texture_color_tex0, timeCoord0);
    }
    else
    {
        FragColor = Color;
    }
    
    // 计算顶点位置
    vec2 cornerOffset = vec2(vertIdx - 0.5);
    ComputeVertPos(pos, cornerOffset, rot, compScale, ViewInverse);
    //pos.xy += cornerOffset * compScale.xy;
    pos = VPTransform * pos;
    
    // 6. texture overtime
    float frameIndex = 0.0;
    if (UseAnimationOvertime == true)
    {
        float startFrame = 0.0;
        vec3 frameInfo = vec3(0.0);
        // 高度信息，同样我们只考虑为0的情况
        frameInfo = UnpackCurveData(texture_animation_tex0, timeCoord0);
        startFrame = frameInfo.x / AnimInfo.y;
        float EPSILON = 1e-6;
        frameIndex = Repeat(AnimInfo.z * (frameInfo.y + startFrame), 1. + EPSILON);
    }
    
    //FragUV = ComputeUV(frameIndex, vertIdx, FrameTile.xy);
    FragUV = Texcoord;
    
    gl_Position = pos;
}


