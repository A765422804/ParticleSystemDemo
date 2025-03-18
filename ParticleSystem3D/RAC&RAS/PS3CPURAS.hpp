//
//  PS3CPURAS.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/14.
//

#pragma once

#include "../PS3Common.h"
#include "PS3CPUProperty.hpp"
#include "PS3CPURAC.hpp"

class PS3CPURAS : public PropertyObserver
{
public:
    PS3CPURAS(PS3CPUPropertyPtr property)
    : _syncr(property)
    {
        _syncr->SetObserver(this);
    }
    
    ~PS3CPURAS() = default;
    
public:
    void OnValueChanged(const PropertyObservable& observable) override
    {
        if (const PS3CPUProperty* p = dynamic_cast<const PS3CPUProperty*>(&observable))
        {
            _syncr->SetWorldTransform(p->_worldTransform);
            _syncr->SetIsLocalSpace(p->_isLocalSpace);
            _syncr->SetFrameTile(p->_frameTile);
            _syncr->SetTexture(p->_texture);
        }
    }
    
    void SyncOnChange(PS3CPURACPtr rac)
    {
        rac->SetWorldTransform(_syncr->_worldTransform);
        rac->SetIsLocalSpace(_syncr->_isLocalSpace);
        rac->SetFrameTile(_syncr->_frameTile);
        rac->SetTexture(_syncr->_texture);
    }
    
public:
    PS3CPUPropertyPtr _syncr;
};

using PS3CPURASPtr = std::shared_ptr<PS3CPURAS>;
