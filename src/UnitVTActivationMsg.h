//==============================================================================
//Definition objects VTActivation Messages
//TVTMaintenanceMessage   , TVT_VTStatusMessage,
//TVT_VTSoftKeyActivation , TVT_VTButtonActivation  , TVT_VTPointingEvent
//TVT_VTSelectInput       , TVT_VTESC               , TVT_VTChangeNumericValue
//TVT_VTChangeActiveMask  , TVT_VTChangeSoftKeyMask , TVT_VTInputStringValue
//TVT_VTDisplayActivation , TVT_VTControlAudioDeviceTermination
//TVTUnsupportedFunction
//==============================================================================
//unit UnitVTActivationMsg.h;
//==============================================================================

#ifndef UnitVTObjConsts_h
 #define UnitVTObjConsts_h
 #include "UnitVTObjConsts.h"
#endif

#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif

//==============================================================================
//Commands and Messages global VT/ECU
//==============================================================================

const String KeyErrorStr[6]=
                   {"no Error",
                    "not valid object type or active data mask",
                    "pressed without released",
                    "released/stillpressed without pressed",
                    "not valid abort",
                    "any other error, mask error"};



//==============================================================================
//type  $FF
  class TVTMaintenanceMessage :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0xFF;
    uint8_t  VTEvent=0;  
    //
    uint8_t VTBusyCode=0x00;
    uint8_t VTVersionNumber=0x00;
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type  $FE
  class TVT_VTStatusMessage :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0xFE;
    uint8_t  VTEvent=0;  
    //
    uint8_t  VTSourceAddress=0x26;
    uint16_t VTParentReference=0xFFFF;
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTBusyCode=0xFF;
    uint8_t  VTFunction=0xFE;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};

//------------------------------------------------------------------------------
//type  $00
  class TVT_VTSoftKeyActivation :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x00;
    uint8_t  VTEvent=0;  
    //
    uint8_t  VTKeyActivationCode=0x00;
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTParentReference=0xFFFF;
    uint8_t  VTKeyNumber=0x00;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type   //$01
  class TVT_VTButtonActivation :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x01;
    uint8_t  VTEvent=0;  
    //
    uint8_t  VTKeyActivationCode=0x00;
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTParentReference=0xFFFF;
    uint8_t  VTKeyNumber=0x00;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   // $02
  class TVT_VTPointingEvent :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x02;
    uint8_t  VTEvent=0;  
    //
    int16_t VTxPos=0;
    int16_t VTyPos=0;
    uint8_t VTActivationCode=0x00;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type    //$03
  class TVT_VTSelectInput :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x03;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTSelect=0x00;
    uint8_t  VTEdit=0x00;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//type  //$04
  class TVT_VTESC :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x04;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTObjectReference=0xFFFF;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //$05
  class TVT_VTChangeNumericValue :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x05;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint32_t VTValue=0;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};

//------------------------------------------------------------------------------
//type   //$06
  class TVT_VTChangeActiveMask :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x06;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTActiveMask=0xFFFF;
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTParentReference=0xFFFF;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type    //$07
  class TVT_VTChangeSoftKeyMask :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x07;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTActiveMask=0xFFFF;
    uint16_t VTObjectReference=0xFFFF;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //$08
  class TVT_VTInputStringValue :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x08;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTParentReference=0xFFFF;
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTLength=4;
    String   VTNewValue="TEST";
    boolean  VTZUniCode=false;
    boolean  UniCode=false;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type    //$09
  class TVT_VTDisplayActivation :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x09;
    uint8_t  VTEvent=0;  
    //
    uint16_t VTMask0  =0xFFFF;
    uint8_t  VTStatus0=0x00;
    uint16_t VTMask1  =0xFFFF;
    uint8_t  VTStatus1=0x00;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type  //$0A
  class TVT_VTControlAudioDeviceTermination :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0x0A;
    uint8_t  VTEvent=0;  
    //
    uint8_t VTTerminationCause=0x00;
    boolean  setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type  //$FD 253
  class TVTUnsupportedFunction :public TVTObject{
  private:
  protected:
  public:
    uint8_t  VT0PCommFunction=0xFD;
    uint8_t  VTEvent=0;  
    //
    uint8_t VTUnsupported=0xFF;
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//==============================================================================
//==============================================================================
