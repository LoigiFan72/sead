#pragma once

#include <controller/seadController.h>
#include <nn/hid.h>

namespace sead{

class CtrController : public Controller{
    SEAD_RTTI_OVERRIDE(CtrController, Controller);
public:
    CtrController();
    virtual ~CtrController(){ }
    virtual void calcImpl_();
    
    void setStickClampMode(nn::hid::CTR::AnalogStickClamper::ClampMode stickMode);
};

}