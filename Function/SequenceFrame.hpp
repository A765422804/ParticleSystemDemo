////
////  SequenceFrame.hpp
////  ParticleSystem
////
////  Created by evanbfeng on 2024/7/23.
////
//
//#ifndef SequenceFrame_hpp
//#define SequenceFrame_hpp
//
//#include "Texture2D.hpp"
//#include <vector>
//#include <string>
//
//class SequenceFrame
//{
//public:
//    SequenceFrame(std::string TexturePath, GLenum start_unit, GLuint format, unsigned int count, float duration):count(count), duration(duration)
//    {
//        // 装载count个的贴图，注意贴图的名字规定为x.png，其中x属于1-count
//        for (int i = 0 ; i < count; ++i)
//        {
//            std::string s = TexturePath + "/" + std::to_string(i + 1) + ".png";
//            textures.push_back(Texture2D(s.c_str(), start_unit + i, format));
//        }
//        
//        time_per_frame = duration / count;
//        elapsed_time = 0.0;
//    }
//    
//    int getCurrentFrame()
//    {
//        int current_frame = static_cast<int>(elapsed_time / time_per_frame) % count;
//        
//        return current_frame;
//    }
//    
//    GLenum getCurrentFrameUnit()
//    {
//        return textures[getCurrentFrame()].texture_unit;
//    }
//    
//    void bindTexture()
//    {
//        textures[getCurrentFrame()].bindTexture();
//    }
//    
//    void update(const float &dt)
//    {
//        elapsed_time = (elapsed_time + dt);
//        if (elapsed_time >= count)
//            elapsed_time = 0;
//    }
//    
//    std::vector<Texture2D> textures; // 序列帧的所有纹理
//    unsigned int count; //序列帧纹理的个数
//    float duration; // 播放完所有序列帧的时间
//    float time_per_frame; //每张图的时间
//    float elapsed_time; //记录已经经过的时间，初始化为0
//};
//#endif /* SequenceFrame_hpp */
