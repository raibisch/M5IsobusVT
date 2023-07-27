//==============================================================================
//Definition objects
//TVTAuxiliaryAssignment
//TVTAuxiliaryPrefAssignment, TVTAuxiliaryInputStatus,
//TVTAuxiliaryInputMaintenance, TVTAuxiliaryInputStatusEnable
//==============================================================================

//unit UnitVTAuxAssignment.h
//==============================================================================

//==============================================================================
#ifndef UnitVTObjConsts_h
 #define UnitVTObjConsts_h
 #include "UnitVTObjConsts.h"
#endif


//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
//CLASSES
//==============================================================================
//type  //All Assignment global 0xFF
class TVTAuxiliaryAssign {
  private:
  //
  protected:
  //  
  public:
    //uint8_t  VT0PCommFunction=0xFF;
    uint8_t  VTError=0xFF;
    uint16_t VTInputReference=0xFFFF;
    uint16_t VTFunctionReference=0xFFFF;
    String   VTWSetName="FFFFFFFFFFFFFFFF";
    //
    virtual boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){};
};


//------------------------------------------------------------------------------
//type  //32 $20
class TVTAuxiliaryAssignment :public TVTAuxiliaryAssign {
  private:
    //
  protected:
    //
  public:
    uint8_t VT0PCommFunction=0x20;
    uint8_t VTAuxFunction=0xFF;
    uint8_t VTSrcAddress=0xFF;
    uint8_t VTInputNumber=0xFF;
    //

};


//------------------------------------------------------------------------------
//type  //36 $24
class  TVTAuxiliaryAssignment3 :public TVTAuxiliaryAssignment {
  private:
    //
  protected:
    //
  public:
    uint8_t  VT0PCommFunction=0x24;
};



//------------------------------------------------------------------------------
//type  //34 $22
class TVTAuxiliaryPrefAssignment :public TVTAuxiliaryAssign{
  private:
    //
  protected:
    //
  public:
    uint8_t  VT0PCommFunction=0x22;
    //list 1
    uint8_t  VTInputUnitNumber=0xFF;
    uint16_t VTModelIdentCode=0xFFFF;

    //list 2
    uint8_t VTPrefFuncNumber=0xFF;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);
    uint8_t setVTAuxAssignList(TVT_Net *pVT_Net,uint8_t dst);    
};


//------------------------------------------------------------------------------
//type  //33 $21
class TVTAuxiliaryInputStatus {
  private:
    //
  protected:
    //
  public:
    uint8_t  VT0PCommFunction=0x21;
    //
    uint16_t VTObjectReference=0xFFFF;
    uint32_t VTInputValue1=0;
    uint32_t VTInputValue2=0;
    uint8_t VTFuncTyp=0xFF;
    uint8_t VTInputID=0xFF;
    uint8_t VTInputStatus=0xFF;
    //gInpObj :TVTObject;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);

};


//------------------------------------------------------------------------------
//type  //38 $26
class TVTAuxiliaryInputStatus3 :public TVTAuxiliaryInputStatus{
  private:
    //
  protected:
    //
  public:
    uint8_t  VT0PCommFunction=0x26;

};


//------------------------------------------------------------------------------
//type  //35 $23
class  TVTAuxiliaryInputMaintenance {
  private:
    //
  protected:
    //
  public:
    uint8_t  VT0PCommFunction=0x23;
    uint16_t VTModelIdentCode=0xFFFF;
    uint8_t  VTReadyMode=0xFF;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);
};


//------------------------------------------------------------------------------
//type   //35 $25
class TVTAuxiliaryInputStatusEnable {
  private:
    //
  protected:
    //
  public:
    uint8_t  VT0PCommFunction=0x25;
    uint16_t VTObjectReference=0xFFFF;
    boolean  VTEnabled=false;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);
};

//==============================================================================
//==============================================================================
