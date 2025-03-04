//
//  PS3ParticleBatchModel.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "PS3ParticleBatchModel.hpp"

PS3ParticleBatchModel::PS3ParticleBatchModel(int maxParticleCount, bool useGPU)
: _capacity(maxParticleCount)
{
    if (useGPU)
    {
        _vDataF.resize(4 * _vertAttrsFloatGPUCount * maxParticleCount);
    }
    else
    {
        _vDataF.resize(4 * _vertAttrsFloatCount * maxParticleCount);
    }

    _iDataI.resize(6 * maxParticleCount);
    
    _renderer = std::make_shared<ParticleRenderer>(useGPU);
    
    SetVertexAttributes();
    SetIndexData();
}

void PS3ParticleBatchModel::SetVertexAttributes()
{
    _renderer->SetupVertexDesc(); // 调用renderer的初始化顶点数据
}

void PS3ParticleBatchModel::SetIndexData()
{
    int dst = 0;
    for (int i = 0; i < _capacity; ++i)
    {
        float baseIdx = 4 * i;
        _iDataI[dst ++] = baseIdx;
        _iDataI[dst ++] = baseIdx + 1;
        _iDataI[dst ++] = baseIdx + 2;
        _iDataI[dst ++] = baseIdx + 3;
        _iDataI[dst ++] = baseIdx + 2;
        _iDataI[dst ++] = baseIdx + 1;
    }
}

void PS3ParticleBatchModel::AddParticleVertexData(int index, PVData pvdata)
{
    int offset = index * _vertAttrsFloatCount;
    _vDataF[offset ++] = pvdata.Position.x;
    _vDataF[offset ++] = pvdata.Position.y;
    _vDataF[offset ++] = pvdata.Position.z;
    _vDataF[offset ++] = pvdata.Texcoord.x;
    _vDataF[offset ++] = pvdata.Texcoord.y;
    _vDataF[offset ++] = pvdata.Texcoord.z;
    _vDataF[offset ++] = pvdata.Size.x;
    _vDataF[offset ++] = pvdata.Size.y;
    _vDataF[offset ++] = pvdata.Size.z;
    _vDataF[offset ++] = pvdata.Rotation.x;
    _vDataF[offset ++] = pvdata.Rotation.y;
    _vDataF[offset ++] = pvdata.Rotation.z;
    _vDataF[offset ++] = pvdata.Rotation.w;
    _vDataF[offset ++] = pvdata.Color.r;
    _vDataF[offset ++] = pvdata.Color.g;
    _vDataF[offset ++] = pvdata.Color.b;
    _vDataF[offset ++] = pvdata.Color.a;
}

void PS3ParticleBatchModel::UpdateGPUParticles(float time, float dt)
{
    int pSize = _vertAttrsFloatGPUCount * _vertCount;
    int pBaseIndex = 0;
    float startTime = 0.0;
    float lifeTime = 0.0;
    int lastBaseIndex = 0;
    float interval = 0.0;
    
    for (int i = 0 ; i < _particleCountGPU; ++i)
    {
        pBaseIndex = i * pSize;
        startTime = _vDataF[pBaseIndex + _startTimeOffset];
        lifeTime = _vDataF[pBaseIndex + _lifeTimeOffset];
        interval = time - startTime;
        if (lifeTime - interval < dt)
        {
            lastBaseIndex = --_particleCountGPU * pSize;
            std::copy(
                _vDataF.begin() + lastBaseIndex,           // 源起始位置
                _vDataF.begin() + lastBaseIndex + pSize,   // 源结束位置
                _vDataF.begin() + pBaseIndex               // 目标起始位置
            );
            --i;
        }
    }
}

void PS3ParticleBatchModel::RenderModelCPU(int count)
{
    if (count <= 0)
        return;
    
    // 已知顶点数据和索引数据，可以render了
    // 根据count得到vertex和index的数量
    
    int vertexCount = _vertAttrsFloatCount * 4 * count;
    int indexCount = 6 * count;
    std::vector<float> vertices(_vDataF.begin(), _vDataF.begin() + vertexCount);
    std::vector<unsigned int> indices(_iDataI.begin(), _iDataI.begin() + indexCount);
    
    _renderer->SetVertexData(vertices);
    _renderer->SetIndexData(indices);
    
    _renderer->Render();
}

void PS3ParticleBatchModel::RenderModelGPU()
{
    if (_particleCountGPU <= 0)
        return;
    
    std::cout<<"particle count:"<<_particleCountGPU<<std::endl;
    
    // 已知顶点数据和索引数据，可以render了
    // 根据count得到vertex和index的数量
    
    int vertexCount = _vertAttrsFloatGPUCount * 4 * _particleCountGPU;
    int indexCount = 6 * _particleCountGPU;
    std::vector<float> vertices(_vDataF.begin(), _vDataF.begin() + vertexCount);
    std::vector<unsigned int> indices(_iDataI.begin(), _iDataI.begin() + indexCount);
    
    _renderer->SetVertexData(vertices);
    _renderer->SetIndexData(indices);
    
    _renderer->Render();
}

void PS3ParticleBatchModel::AddGPUParticleVertexData(PS3ParticlePtr p, int num, float time)
{
    int offset = num * _vertAttrsFloatGPUCount * _vertCount;
    
    for (int i = 0 ; i < _vertCount; ++i)
    {
        _vDataF[offset ++] = p->_position.x;
        _vDataF[offset ++] = p->_position.y;
        _vDataF[offset ++] = p->_position.z;
        _vDataF[offset ++] = time;
        
        _vDataF[offset ++] = p->_size.x;
        _vDataF[offset ++] = p->_size.y;
        _vDataF[offset ++] = p->_size.z;
        
        _vDataF[offset ++] = p->_ultimateQuat.x;
        _vDataF[offset ++] = p->_ultimateQuat.y;
        _vDataF[offset ++] = p->_ultimateQuat.z;
        _vDataF[offset ++] = p->_ultimateQuat.w;
        
        _vDataF[offset ++] = p->_color.r;
        _vDataF[offset ++] = p->_color.g;
        _vDataF[offset ++] = p->_color.b;
        _vDataF[offset ++] = p->_color.a;
        
        _vDataF[offset ++] = p->_velocity.x;
        _vDataF[offset ++] = p->_velocity.y;
        _vDataF[offset ++] = p->_velocity.z;
        _vDataF[offset ++] = p->_startLifeTime;
        
        _vDataF[offset ++] = uvs[2 * i];
        _vDataF[offset ++] = uvs[2 * i + 1];
    }
}

// vert
/*
 precision highp float;
 vec4 quaternionFromAxis (vec3 xAxis,vec3 yAxis,vec3 zAxis){
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
 vec4 quaternionFromEuler (vec3 angle){
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
 mat4 matrixFromRT (vec4 q, vec3 p){
  float x2 = q.x + q.x;
  float y2 = q.y + q.y;
  float z2 = q.z + q.z;
  float xx = q.x * x2;
  float xy = q.x * y2;
  float xz = q.x * z2;
  float yy = q.y * y2;
  float yz = q.y * z2;
  float zz = q.z * z2;
  float wx = q.w * x2;
  float wy = q.w * y2;
  float wz = q.w * z2;
  return mat4(
    1. - (yy + zz), xy + wz, xz - wy, 0,
    xy - wz, 1. - (xx + zz), yz + wx, 0,
    xz + wy, yz - wx, 1. - (xx + yy), 0,
    p.x, p.y, p.z, 1
  );
 }
 mat4 matFromRTS (vec4 q, vec3 t, vec3 s){
  float x = q.x, y = q.y, z = q.z, w = q.w;
  float x2 = x + x;
  float y2 = y + y;
  float z2 = z + z;
  float xx = x * x2;
  float xy = x * y2;
  float xz = x * z2;
  float yy = y * y2;
  float yz = y * z2;
  float zz = z * z2;
  float wx = w * x2;
  float wy = w * y2;
  float wz = w * z2;
  float sx = s.x;
  float sy = s.y;
  float sz = s.z;
  return mat4((1. - (yy + zz)) * sx, (xy + wz) * sx, (xz - wy) * sx, 0,
    (xy - wz) * sy, (1. - (xx + zz)) * sy, (yz + wx) * sy, 0,
    (xz + wy) * sz, (yz - wx) * sz, (1. - (xx + yy)) * sz, 0,
    t.x, t.y, t.z, 1);
 }
 vec4 quatMultiply (vec4 a, vec4 b){
  vec4 quat;
  quat.x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
  quat.y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
  quat.z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
  quat.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
  return quat;
 }
 void rotateVecFromQuat (inout vec3 v, vec4 q){
  float ix = q.w * v.x + q.y * v.z - q.z * v.y;
  float iy = q.w * v.y + q.z * v.x - q.x * v.z;
  float iz = q.w * v.z + q.x * v.y - q.y * v.x;
  float iw = -q.x * v.x - q.y * v.y - q.z * v.z;
  v.x = ix * q.w + iw * -q.x + iy * -q.z - iz * -q.y;
  v.y = iy * q.w + iw * -q.y + iz * -q.x - ix * -q.z;
  v.z = iz * q.w + iw * -q.z + ix * -q.y - iy * -q.x;
 }
 vec3 rotateInLocalSpace (vec3 pos, vec3 xAxis, vec3 yAxis, vec3 zAxis, vec4 q){
  vec4 viewQuat = quaternionFromAxis(xAxis, yAxis, zAxis);
  vec4 rotQuat = quatMultiply(viewQuat, q);
  rotateVecFromQuat(pos, rotQuat);
  return pos;
 }
 mat3 quatToMat3(vec4 q) {
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
 vec4 mat3ToQuat(mat3 mat) {
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
 layout(std140) uniform Constants {
  vec4 mainTiling_Offset;
  vec4 frameTile_velLenScale;
  vec4 scale;
  vec4 nodeRotation;
 };
 layout(std140) uniform CCGlobal {
  highp   vec4 cc_time;
  mediump vec4 cc_screenSize;
  mediump vec4 cc_nativeSize;
  mediump vec4 cc_probeInfo;
  mediump vec4 cc_debug_view_mode;
 };
 layout(std140) uniform CCCamera {
  highp   mat4 cc_matView;
  highp   mat4 cc_matViewInv;
  highp   mat4 cc_matProj;
  highp   mat4 cc_matProjInv;
  highp   mat4 cc_matViewProj;
  highp   mat4 cc_matViewProjInv;
  highp   vec4 cc_cameraPos;
  mediump vec4 cc_surfaceTransform;
  mediump vec4 cc_screenScale;
  mediump vec4 cc_exposure;
  mediump vec4 cc_mainLitDir;
  mediump vec4 cc_mainLitColor;
  mediump vec4 cc_ambientSky;
  mediump vec4 cc_ambientGround;
  mediump vec4 cc_fogColor;
  mediump vec4 cc_fogBase;
  mediump vec4 cc_fogAdd;
  mediump vec4 cc_nearFar;
  mediump vec4 cc_viewPort;
 };
 layout(std140) uniform CCLocal {
  highp mat4 cc_matWorld;
  highp mat4 cc_matWorldIT;
  highp vec4 cc_lightingMapUVParam;
  highp vec4 cc_localShadowBias;
  highp vec4 cc_reflectionProbeData1;
  highp vec4 cc_reflectionProbeData2;
  highp vec4 cc_reflectionProbeBlendData1;
  highp vec4 cc_reflectionProbeBlendData2;
 };
 
 out mediump vec2 uv;
 out mediump vec4 color;
 void computeVertPos (inout vec4 pos, vec2 vertOffset, vec4 q, vec3 s
  , mat4 viewInv
 ) {
  vec3 viewSpaceVert = vec3(vertOffset.x * s.x, vertOffset.y * s.y, 0.);
  vec3 camX = normalize(vec3(viewInv[0][0], viewInv[1][0], viewInv[2][0]));
  vec3 camY = normalize(vec3(viewInv[0][1], viewInv[1][1], viewInv[2][1]));
  vec3 camZ = normalize(vec3(viewInv[0][2], viewInv[1][2], viewInv[2][2]));
  pos.xyz += rotateInLocalSpace(viewSpaceVert, camX, camY, camZ, q);
 }
 
 vec2 computeUV (float frameIndex, vec2 vertIndex, vec2 frameTile){
  vec2 aniUV = vec2(0, floor(frameIndex * frameTile.y));
  aniUV.x = floor(frameIndex * frameTile.x * frameTile.y - aniUV.y * frameTile.x);
  vertIndex.y = 1. - vertIndex.y;
  return (aniUV.xy + vertIndex) / vec2(frameTile.x, frameTile.y);
 }
 
 in vec3 a_texCoord1; // size
 in vec3 a_texCoord2; // rotation
 in vec4 a_color; // 颜色
 in vec3 a_texCoord; // uv, frame idx
 in vec3 a_position; // position
 
 vec4 lpvs_main () {
  vec3 compScale = scale.xyz * a_texCoord1;
  vec4 pos = vec4(a_position.xyz, 1);

  #if !CC_USE_WORLD_SPACE // 非世界坐标
    pos = cc_matWorld * pos; // 得到绝对位置
  #endif
 
 // 旋转
  #if ROTATION_OVER_TIME_MODULE_ENABLE
    vec3 rotTmp = a_texCoord2;
    float mulFactor = 1.0;
    if (rotTmp.x > 10.0 * 0.5) {
        rotTmp.x -= 10.0;
        mulFactor = -1.0;
    }
    vec4 rot = vec4(rotTmp, 0.0);
    rot.w = mulFactor * sqrt(abs(1.0 - rot.x * rot.x - rot.y * rot.y - rot.z * rot.z));
  #endif
  #if !ROTATION_OVER_TIME_MODULE_ENABLE
        vec3 rotEuler = a_texCoord2; // 设置旋转
      vec4 rot = quaternionFromEuler(rotEuler);
  #endif
    vec2 cornerOffset = vec2((a_texCoord.xy - 0.5)); // 得到当前顶点在四边形的位置
      computeVertPos(pos, cornerOffset, rot, compScale, cc_matViewInv); // 计算顶点实际位置
    color = a_color; // 设置颜色
    uv = computeUV(a_texCoord.z, a_texCoord.xy, frameTile_velLenScale.xy) * mainTiling_Offset.xy + mainTiling_Offset.zw; // 设置uv
  pos = cc_matViewProj * pos; // 最终位置就是VP * 位置
  return pos;
 }
 void main() { gl_Position = lpvs_main(); }
*/

// frag
/*
 precision mediump float;
 layout(std140) uniform CCGlobal {
 highp   vec4 cc_time;
 mediump vec4 cc_screenSize;
 mediump vec4 cc_nativeSize;
 mediump vec4 cc_probeInfo;
 mediump vec4 cc_debug_view_mode;
 };
 layout(std140) uniform CCCamera {
 highp   mat4 cc_matView;
 highp   mat4 cc_matViewInv;
 highp   mat4 cc_matProj;
 highp   mat4 cc_matProjInv;
 highp   mat4 cc_matViewProj;
 highp   mat4 cc_matViewProjInv;
 highp   vec4 cc_cameraPos;
 mediump vec4 cc_surfaceTransform;
 mediump vec4 cc_screenScale;
 mediump vec4 cc_exposure;
 mediump vec4 cc_mainLitDir;
 mediump vec4 cc_mainLitColor;
 mediump vec4 cc_ambientSky;
 mediump vec4 cc_ambientGround;
 mediump vec4 cc_fogColor;
 mediump vec4 cc_fogBase;
 mediump vec4 cc_fogAdd;
 mediump vec4 cc_nearFar;
 mediump vec4 cc_viewPort;
 };
 vec4 CCFragOutput (vec4 color) {
 return color;
 }
 in vec2 uv;
 in vec4 color;
 uniform sampler2D mainTexture;
 layout(std140) uniform FragConstants {
 vec4 tintColor;
 };
 vec4 add () {
 vec4 col = 2.0 * color * tintColor * texture(mainTexture, uv);
 return CCFragOutput(col);
 }
 layout(location = 0) out vec4 cc_FragColor;
 void main() { cc_FragColor = add(); }
 */
