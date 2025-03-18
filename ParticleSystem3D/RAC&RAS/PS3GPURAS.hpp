//
//  PS3GPURAS.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "PS3GPUProperty.hpp"
#include "PS3GPURAC.hpp"

class PS3GPURAS : public PropertyObserver
{
public:
    PS3GPURAS(PS3GPUPropertyPtr property)
    : _syncr(property)
    {
        _syncr->SetObserver(this);
    }
    
    ~PS3GPURAS() = default;
    
public:
    void OnValueChanged(const PropertyObservable& observable) override
    {
        if (const PS3GPUProperty* p = dynamic_cast<const PS3GPUProperty*>(&observable))
        {
            // Set boolean properties
            _syncr->SetUseAnimationOvertime(p->_useAnimationOvertime);
            _syncr->SetUseVelocityOvertime(p->_useVelocityOvertime);
            _syncr->SetUseForceOvertime(p->_useForceOvertime);
            _syncr->SetUseSizeOvertime(p->_useSizeOvertime);
            _syncr->SetUseColorOvertime(p->_useColorOvertime);
            _syncr->SetUseRotationOvertime(p->_useRotationOvertime);

            // Set mode properties
            _syncr->SetAnimationMode(p->_animationMode);
            _syncr->SetVelocityMode(p->_velocityMode);
            _syncr->SetForceMode(p->_forceMode);
            _syncr->SetSizeMode(p->_sizeMode);
            _syncr->SetColorMode(p->_colorMode);
            _syncr->SetRotationMode(p->_rotationMode);

            // Set texture properties
            _syncr->SetAnimationTexture(p->_animationTexture);
            _syncr->SetVelocityTexture(p->_velocityTexture);
            _syncr->SetForceTexture(p->_forceTexture);
            _syncr->SetSizeTexture(p->_sizeTexture);
            _syncr->SetColorTexture(p->_colorTexture);
            _syncr->SetRotationTexture(p->_rotationTexture);

            // Set vector and matrix properties
            _syncr->SetSampleInfo(p->_sampleInfo);
            _syncr->SetAnimInfo(p->_animInfo);
            _syncr->SetTimeAndDelta(p->_timeAndDelta);
            _syncr->SetFrameTile(p->_frameTile);
            _syncr->SetWorldTransform(p->worldTransform);

            // Set other properties
            _syncr->SetIsLocalSpace(p->_isLocalSpace);
            _syncr->SetTexture(p->_texture);
            
            _syncr->SetGravityTexture(p->_gravityTexture);
            _syncr->SetGravityMode(p->_gravityMode);
            
        }
    }
    
    void SyncOnChange(PS3GPURACPtr rac)
    {
        // Set boolean properties
        rac->SetUseAnimationOvertime(_syncr->_useAnimationOvertime);
        rac->SetUseVelocityOvertime(_syncr->_useVelocityOvertime);
        rac->SetUseForceOvertime(_syncr->_useForceOvertime);
        rac->SetUseSizeOvertime(_syncr->_useSizeOvertime);
        rac->SetUseColorOvertime(_syncr->_useColorOvertime);
        rac->SetUseRotationOvertime(_syncr->_useRotationOvertime);

        // Set mode properties
        rac->SetAnimationMode(_syncr->_animationMode);
        rac->SetVelocityMode(_syncr->_velocityMode);
        rac->SetForceMode(_syncr->_forceMode);
        rac->SetSizeMode(_syncr->_sizeMode);
        rac->SetColorMode(_syncr->_colorMode);
        rac->SetRotationMode(_syncr->_rotationMode);

        // Set texture properties
        rac->SetAnimationTexture(_syncr->_animationTexture);
        rac->SetVelocityTexture(_syncr->_velocityTexture);
        rac->SetForceTexture(_syncr->_forceTexture);
        rac->SetSizeTexture(_syncr->_sizeTexture);
        rac->SetColorTexture(_syncr->_colorTexture);
        rac->SetRotationTexture(_syncr->_rotationTexture);

        // Set vector and matrix properties
        rac->SetSampleInfo(_syncr->_sampleInfo);
        rac->SetAnimInfo(_syncr->_animInfo);
        rac->SetTimeAndDelta(_syncr->_timeAndDelta);
        rac->SetFrameTile(_syncr->_frameTile);
        rac->SetWorldTransform(_syncr->worldTransform);

        // Set other properties
        rac->SetIsLocalSpace(_syncr->_isLocalSpace);
        rac->SetTexture(_syncr->_texture);
        
        rac->SetGravityTexture(_syncr->_gravityTexture);
        rac->SetGravityMode(_syncr->_gravityMode);
    }
    
public:
    PS3GPUPropertyPtr _syncr;
};

using PS3GPURASPtr = std::shared_ptr<PS3GPURAS>;
