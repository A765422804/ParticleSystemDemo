#version 410 core
// #pragma optimize(off)

// in
layout(location = 0) in vec4 Position_StartTime; // 位置和起始时间
layout(location = 1) in vec4 Size_Seed;                 // 大小和种子
layout(location = 2) in vec4 Rotation;             // 旋转
layout(location = 3) in vec4 Color;                // 颜色
layout(location = 4) in vec4 Dir_Life;             // 方向和生命
layout(location = 5) in vec2 Texcoord;             // 纹理坐标

// out
out vec4 FragColor;
out vec2 FragUV;

// Transform Feedback 输出
out vec3 DeadParticlePosition; // 死亡粒子的位置

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

// uniform - overtime mode
uniform int animation_mode;
uniform int velocity_mode;
uniform int force_mode;
uniform int size_mode;
uniform int color_mode;
uniform int rotation_mode;

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

// uniform gravity
uniform sampler2D texture_gravity;
uniform int gravity_mode;

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

// 从 mat4 中提取旋转四元数
vec4 ExtractRotationQuat(mat4 worldTransform) {
    // 提取 3x3 旋转矩阵
    mat3 rotationMatrix = mat3(worldTransform);

    // 计算四元数
    vec4 q;
    float trace = rotationMatrix[0][0] + rotationMatrix[1][1] + rotationMatrix[2][2];

    if (trace > 0.0) {
        float s = 0.5 / sqrt(trace + 1.0);
        q.w = 0.25 / s;
        q.xyz = vec3(
            rotationMatrix[2][1] - rotationMatrix[1][2],
            rotationMatrix[0][2] - rotationMatrix[2][0],
            rotationMatrix[1][0] - rotationMatrix[0][1]
        ) * s;
    } else if (rotationMatrix[0][0] > rotationMatrix[1][1] && rotationMatrix[0][0] > rotationMatrix[2][2]) {
        float s = 2.0 * sqrt(1.0 + rotationMatrix[0][0] - rotationMatrix[1][1] - rotationMatrix[2][2]);
        q.w = (rotationMatrix[2][1] - rotationMatrix[1][2]) / s;
        q.xyz = vec3(
            0.25 * s,
            (rotationMatrix[0][1] + rotationMatrix[1][0]) / s,
            (rotationMatrix[0][2] + rotationMatrix[2][0]) / s
        );
    } else if (rotationMatrix[1][1] > rotationMatrix[2][2]) {
        float s = 2.0 * sqrt(1.0 + rotationMatrix[1][1] - rotationMatrix[0][0] - rotationMatrix[2][2]);
        q.w = (rotationMatrix[0][2] - rotationMatrix[2][0]) / s;
        q.xyz = vec3(
            (rotationMatrix[0][1] + rotationMatrix[1][0]) / s,
            0.25 * s,
            (rotationMatrix[1][2] + rotationMatrix[2][1]) / s
        );
    } else {
        float s = 2.0 * sqrt(1.0 + rotationMatrix[2][2] - rotationMatrix[0][0] - rotationMatrix[1][1]);
        q.w = (rotationMatrix[1][0] - rotationMatrix[0][1]) / s;
        q.xyz = vec3(
            (rotationMatrix[0][2] + rotationMatrix[2][0]) / s,
            (rotationMatrix[1][2] + rotationMatrix[2][1]) / s,
            0.25 * s
        );
    }

    return q;
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

mat4 QuatToMat4(vec4 q) {
    // 提取四元数的分量
    float w = q.w;
    float x = q.x;
    float y = q.y;
    float z = q.z;

    // 计算旋转矩阵的元素
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    // 构造 4x4 旋转矩阵
    mat4 m;
    m[0][0] = 1.0 - 2.0 * (yy + zz);
    m[0][1] = 2.0 * (xy - wz);
    m[0][2] = 2.0 * (xz + wy);
    m[0][3] = 0.0;

    m[1][0] = 2.0 * (xy + wz);
    m[1][1] = 1.0 - 2.0 * (xx + zz);
    m[1][2] = 2.0 * (yz - wx);
    m[1][3] = 0.0;

    m[2][0] = 2.0 * (xz - wy);
    m[2][1] = 2.0 * (yz + wx);
    m[2][2] = 1.0 - 2.0 * (xx + yy);
    m[2][3] = 0.0;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;

    return m;
}

vec2 ComputeUV (float frameIndex, vec2 vertIndex, vec2 frameTile)
{
    // 计算当前图片的索引
    float totalFrames = frameTile.x * frameTile.y;
    float currentFrame = floor(frameIndex * totalFrames);
    
    // 计算当前图片在行列索引中的位置
    float colIndex = mod(currentFrame, frameTile.x);
    float rowIndex = floor(currentFrame/ frameTile.x);
    
    // 修改原点在左下角
    rowIndex = frameTile.y - rowIndex - 1;
    
    // 计算每个图片在UV空间的大小
    vec2 tileSize = 1.0 / frameTile;
    
    // 计算当前图片的uv偏移
    vec2 uvOffset = vec2(colIndex, rowIndex) * tileSize;
    
    // 调整原始UV坐标到当前图片的UV空间
    vec2 adjustedUV = uvOffset + vertIndex * tileSize;
    
    return adjustedUV;
}

void ComputeVertPos (inout vec4 pos, vec2 vertOffset, vec4 q, vec3 s, mat4 viewInv) 
{
 vec3 viewSpaceVert = vec3(vertOffset.x * s.x, vertOffset.y * s.y, 0.);
 vec3 camX = normalize(vec3(viewInv[0][0], viewInv[1][0], viewInv[2][0]));
 vec3 camY = normalize(vec3(viewInv[0][1], viewInv[1][1], viewInv[2][1]));
 vec3 camZ = normalize(vec3(viewInv[0][2], viewInv[1][2], viewInv[2][2]));
 pos.xyz += RotateInLocalSpace(viewSpaceVert, camX, camY, camZ, q);
}

float PseudoRandom(float x) {
 float seed = mod(x, 233280.);
 float q = (seed * 9301. + 49297.) / 233280.;
 return fract(q);
}

// 定义无穷大值
const float EPSILON = 1.0 / 1000000.0;

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
    vec3 size = Size_Seed.xyz;
    float seed = Size_Seed.w;
    
    // overtime module
    // 1. size overtime
    if (UseSizeOvertime == true)
    {
        if (size_mode == 1)
        {
            size *= UnpackCurveData(texture_size_tex0, timeCoord0);
        }
        else
        {
            vec3 size_0 = UnpackCurveData(texture_size_tex0, timeCoord0);
            vec3 size_1 = UnpackCurveData(texture_size_tex0, timeCoord1);
            float factor = PseudoRandom(seed + 39825.0);
            size *= mix(size_0, size_1, factor);
        }
    }
    // TODO: 根据size和scale计算真实scale，暂时只考虑了size
    vec3 compScale = size;
    
    // 2. force overtime
    if (UseForceOvertime == true)
    {
        vec3 forceAnim = vec3(0.0);
        if (force_mode == 1)
        {
            forceAnim = UnpackCurveData(texture_force_tex0, timeCoord0);
        }
        else
        {
            vec3 forceAnim_0 = UnpackCurveData(texture_force_tex0, timeCoord0);
            vec3 forceAnim_1 = UnpackCurveData(texture_force_tex0, timeCoord1);
            float factor = PseudoRandom(seed + 212165.0);
            forceAnim = mix(forceAnim_0, forceAnim_1, factor);
        }
        vec4 forceTrack = vec4(forceAnim, 0.0);
        // TODO: 根据force的space执行坐标变换
        
        velocity.xyz += forceTrack.xyz * normalizedTime * Dir_Life.w;
    }
    
    // TODO: 2.5 gravity:
    vec3 gravity;
    if (gravity_mode == 1)
    {
        gravity = UnpackCurveData(texture_gravity, timeCoord0);
    }
    else
    {
        vec3 gravity_0 = UnpackCurveData(texture_gravity, timeCoord0);
        vec3 gravity_1 = UnpackCurveData(texture_gravity, timeCoord1);
        float factor = PseudoRandom(seed + 197866.); // 这个偏移量我随便写的
        gravity = mix(gravity_0, gravity_1, factor);
    }
    float gravityEffect = -gravity.x * 9.8 * normalizedTime * Dir_Life.w;
    if (IsLocalSpace)
    {
        vec4 gravityTrack = vec4(0, gravityEffect,0,  1);
        gravityTrack = transpose(QuatToMat4(ExtractRotationQuat(WorldTransform))) * gravityTrack;
        velocity.xyz += gravityTrack.xyz;
    }
    else
    {
        velocity.y += gravityEffect;
    }
    
    // 3. velocity overtime
    if (UseVelocityOvertime == true)
    {
        vec3 velocityAnim = vec3(0.0);
        if (velocity_mode == 1)
        {
            velocityAnim = UnpackCurveData(texture_velocity_tex0, timeCoord0);
        }
        else
        {
            vec3 velocityAnim_0 = UnpackCurveData(texture_velocity_tex0, timeCoord0);
            vec3 velocityAnim_1 = UnpackCurveData(texture_velocity_tex0, timeCoord1);
            float factor = PseudoRandom(seed + 197866.);
            velocityAnim = mix(velocityAnim_0, velocityAnim_1, factor);
        }
        vec4 velocityTrack = vec4(velocityAnim, 0.0);
        // TODO: 根据velocity的space执行坐标变换
        velocity.xyz += velocityTrack.xyz;
        //velocity.xyz = velocity.xyz * speedModifier;
    }
    
    pos.xyz += velocity.xyz * normalizedTime * Dir_Life.w;
    
    // 检测粒子是否死亡
    if (normalizedTime >= 1.0 - Time_Delta.y && Texcoord == vec2(0.0, 0.0)) // 每个粒子四个顶点，确保只发射一次子粒子
    {
        DeadParticlePosition = pos.xyz;
    }
    else
    {
        DeadParticlePosition = vec3(1.0 / 0.0);
    }
    
    if (IsLocalSpace)
    {
        pos = WorldTransform * pos;
    }
    
    // TODO: 对粒子执行坐标变换
    
    // 4. rotation overtime
    vec4 rot = Rotation;
    if (UseRotationOvertime == true)
    {
        vec3 euler;
        if (rotation_mode == 1)
        {
            euler = UnpackCurveData(texture_rotation_tex0, timeCoord0)  * normalizedTime * Dir_Life.w; // 得到总计旋转角度
        }
        else
        {
            vec3 rotation_0 = UnpackCurveData(texture_rotation_tex0, timeCoord0);
            vec3 rotation_1 = UnpackCurveData(texture_rotation_tex0, timeCoord1);
            float factor = PseudoRandom(seed + 125292.);
            euler = mix(rotation_0, rotation_1, factor) * normalizedTime * Dir_Life.w;
        }

        vec4 quat = EulerToQuat(euler); // 得到旋转四元数
        mat3 mLocal = QuatToMat3(quat); // 得到旋转矩阵
        mat3 mStart = QuatToMat3(rot);
        rot = Mat3ToQuat(mStart * mLocal);
    }
    
    // 5. color overtime
    if (UseColorOvertime == true)
    {
        vec4 color;
        if (color_mode == 1)
        {
            color = Color * texture(texture_color_tex0, timeCoord0);
        }
        else
        {
            vec4 color_0 = texture(texture_color_tex0, timeCoord0);
            vec4 color_1 = texture(texture_color_tex0, timeCoord1);
            float factor = PseudoRandom(seed + 91041.);
            color = Color * mix(color_0, color_1, factor);
        }
        FragColor = color;
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
        if (animation_mode == 1)
        {
            frameInfo = UnpackCurveData(texture_animation_tex0, timeCoord0);
        }
        else
        {
            vec3 frameInfo_0 = UnpackCurveData(texture_animation_tex0, timeCoord0);
            vec3 frameInfo_1 = UnpackCurveData(texture_animation_tex0, timeCoord1);
            float factor = PseudoRandom(seed + 90794.);
            frameInfo = mix(frameInfo_0, frameInfo_1, factor);
        }
        startFrame = frameInfo.x / AnimInfo.y;
        float EPSILON = 1e-6;
        frameIndex = Repeat(AnimInfo.z * (frameInfo.y + startFrame), 1. + EPSILON);
    }
    
    if (UseAnimationOvertime == true)
    {
        FragUV = ComputeUV(frameIndex, vertIdx, FrameTile);
    }
    else
    {
        FragUV = Texcoord;
    }
    
    gl_Position = pos;
}
