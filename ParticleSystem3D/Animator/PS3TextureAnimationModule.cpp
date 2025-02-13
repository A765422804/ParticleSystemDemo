//
//  PS3TextureAnimationModule.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/1/17.
//

#include "PS3TextureAnimationModule.hpp"

PS3TextureAnimationModule::PS3TextureAnimationModule(int numTilesX, int numTilesY, AnimationMode animationMode, CurveRangePtr frameOvertime, CurveRangePtr startFrame, float cycleCount, bool randomRow, int rowIndex)
    : _enable(true)
    , // 可以根据实际情况初始化 _enable 的值，这里假设默认启用
    _numTilesX(numTilesX)
    , _numTilesY(numTilesY)
    , _animationMode(animationMode)
    , _frameOvertime(frameOvertime)
    , _startFrame(startFrame)
    , _cycleCount(cycleCount)
    , _randomRow(randomRow)
    , _rowIndex(rowIndex)
{
}

void PS3TextureAnimationModule::Animate(PS3ParticlePtr p, float dt)
{
    float normalizedTime = 1 - p->_remainingLifeTime / p->_startLifeTime;
    float startFrame = _startFrame->Evaluate(normalizedTime, Random01()) / (_numTilesX * _numTilesY);
    float frameOvertime = _frameOvertime->Evaluate(normalizedTime, Random01());
    
    if (_animationMode == AnimationMode::WHOLE_SHEET)
    {
        p->_frameIndex = Repeat(_cycleCount * (frameOvertime + startFrame), 1);
    }
    else
    {
        float rowLength = 1.0 / _numTilesY; // 行高
        if (_randomRow) // 随机行
        {
            float f = Repeat(_cycleCount * (frameOvertime + startFrame), 1);
            float from = p->_startRow * rowLength;
            float to = from + rowLength;
            p->_frameIndex = LinearInterpolate(from, to, f);
        }
        else // 确定行
        {
            float from = _rowIndex * rowLength;
            float to = from + rowLength;
            p->_frameIndex = LinearInterpolate(from, to, Repeat(_cycleCount * (frameOvertime + startFrame), 1));
        }
    }
}

//precision mediump float;
//layout(std140) uniform Constants {
// vec4 mainTiling_Offset;
// vec4 frameTile_velLenScale;
// vec4 scale;
// vec4 nodeRotation;
//};
//layout(std140) uniform CCGlobal {
// highp   vec4 cc_time;
// mediump vec4 cc_screenSize;
// mediump vec4 cc_nativeSize;
// mediump vec4 cc_probeInfo;
// mediump vec4 cc_debug_view_mode;
//};
//layout(std140) uniform CCCamera {
// highp   mat4 cc_matView;
// highp   mat4 cc_matViewInv;
// highp   mat4 cc_matProj;
// highp   mat4 cc_matProjInv;
// highp   mat4 cc_matViewProj;
// highp   mat4 cc_matViewProjInv;
// highp   vec4 cc_cameraPos;
// mediump vec4 cc_surfaceTransform;
// mediump vec4 cc_screenScale;
// mediump vec4 cc_exposure;
// mediump vec4 cc_mainLitDir;
// mediump vec4 cc_mainLitColor;
// mediump vec4 cc_ambientSky;
// mediump vec4 cc_ambientGround;
// mediump vec4 cc_fogColor;
// mediump vec4 cc_fogBase;
// mediump vec4 cc_fogAdd;
// mediump vec4 cc_nearFar;
// mediump vec4 cc_viewPort;
//};
//layout(std140) uniform CCLocal {
// highp mat4 cc_matWorld;
// highp mat4 cc_matWorldIT;
// highp vec4 cc_lightingMapUVParam;
// highp vec4 cc_localShadowBias;
// highp vec4 cc_reflectionProbeData1;
// highp vec4 cc_reflectionProbeData2;
// highp vec4 cc_reflectionProbeBlendData1;
// highp vec4 cc_reflectionProbeBlendData2;
//};
//out mediump vec2 uv;
//out mediump vec4 color;
//in vec3 a_position; // 位置
//in vec4 a_texCoord; // index(一个原始点的左右两个点), size, texcoord
//in vec3 a_texCoord1; // 速度
//in vec3 a_texCoord2; // debug
//in vec4 a_color; // 颜色
//vec4 vs_main() {
// highp vec4 pos = vec4(a_position, 1); // 位置
// vec4 velocity = vec4(a_texCoord1.xyz, 0); // 速度
// #if !CC_USE_WORLD_SPACE
//   pos = cc_matWorld * pos; // 世界坐标系的位置
//   velocity = cc_matWorld * velocity; // 世界坐标系的速度
// #endif
// float vertOffset = (a_texCoord.x - 0.5) * a_texCoord.y; // 一个原始点的两个端点的位置偏移！！！
// vec3 camUp = normalize(cross(pos.xyz - cc_cameraPos.xyz, velocity.xyz)); // 得到顶点的位置方向
// pos.xyz += camUp * vertOffset; // 得到实际的顶点位置
// pos = cc_matViewProj * pos; // 计算VP矩阵
// uv = a_texCoord.zw * mainTiling_Offset.xy + mainTiling_Offset.zw;; //纹理坐标
// color = a_color; // 颜色
// return pos;
//}
//void main() { gl_Position = vs_main(); }

//precision mediump float;
//layout(std140) uniform CCGlobal {
//  highp   vec4 cc_time;
//  mediump vec4 cc_screenSize;
//  mediump vec4 cc_nativeSize;
//  mediump vec4 cc_probeInfo;
//  mediump vec4 cc_debug_view_mode;
//};
//layout(std140) uniform CCCamera {
//  highp   mat4 cc_matView;
//  highp   mat4 cc_matViewInv;
//  highp   mat4 cc_matProj;
//  highp   mat4 cc_matProjInv;
//  highp   mat4 cc_matViewProj;
//  highp   mat4 cc_matViewProjInv;
//  highp   vec4 cc_cameraPos;
//  mediump vec4 cc_surfaceTransform;
//  mediump vec4 cc_screenScale;
//  mediump vec4 cc_exposure;
//  mediump vec4 cc_mainLitDir;
//  mediump vec4 cc_mainLitColor;
//  mediump vec4 cc_ambientSky;
//  mediump vec4 cc_ambientGround;
//  mediump vec4 cc_fogColor;
//  mediump vec4 cc_fogBase;
//  mediump vec4 cc_fogAdd;
//  mediump vec4 cc_nearFar;
//  mediump vec4 cc_viewPort;
//};
//vec4 CCFragOutput (vec4 color) {
//  return color;
//}
//in vec2 uv;
//in vec4 color;

//uniform sampler2D mainTexture; // 纹理
//layout(std140) uniform FragConstants {
//  vec4 tintColor;
//};
//vec4 add () {
//  vec4 col = 2.0 * color * tintColor * texture(mainTexture, uv); // 计算颜色
//  return CCFragOutput(col);
//}
//layout(location = 0) out vec4 cc_FragColor;
//void main() { cc_FragColor = add(); }
