//==============================================================================
//Definition objects TVTAuxiliary, TVTAuxiliaryFuncInp,
//TVTAuxiliaryFunction, TVTAuxiliaryInput,
//TVTAuxFunction2,TVTAuxInput2,
//TVTAuxObject, TVTAuxDesignPointer
//==============================================================================
//unit UnitVTAuxiliaryObjects.h
//==============================================================================

#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
//------------------------------------------------------------------------------
//type global
class TVTAuxiliaryFuncInp : public TVTObjectBgCol{
  private:
  protected:
    String   VTObjName="VTAuxiliaryFuncInp";
    //uint8_t  VTObjType=29,30,31,32;
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    uint8_t  VTFuncTyp=0;  //TVTFunctionTyp; //UnitVTObjDef.pas
    uint8_t  VTInputID=0;
    boolean  VTEnabled=true;
    uint16_t VTInputValue1=0;
    uint16_t VTInputValue2=0;
    //
    String VTAuxAssignment="";
    String VTAuxAssignName="";
    String VTObjects="";
    //
    uint16_t ModelIdentCode=0;
    uint16_t VTAuxAssignObjID=0xFFFF;
    uint16_t AuxPrefAssignObjID=0xFFFF;
    uint16_t AuxECU_Nr=0;
    boolean  AuxAssignFunction=false;
    boolean  AuxInpStatus=false;
    uint32_t AuxTimeStamp=0;
    //
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
};


//------------------------------------------------------------------------------
//type 29
class TVTAuxiliaryFunction : public TVTAuxiliaryFuncInp{
  private:
  protected:
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    String   VTObjName="VTAuxiliaryFuncion";
    //uint8_t  VTObjType=29;
  //procedure
};


//------------------------------------------------------------------------------
//type 30
class TVTAuxiliaryInput : public TVTAuxiliaryFuncInp{
  private:
  protected:
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    String   VTObjName="VTAuxiliaryInput";
    //uint8_t  VTObjType=30;
    //procedure
};


//------------------------------------------------------------------------------
//type 31
class TVTAuxiliaryFunction2 : public TVTAuxiliaryFuncInp{
  private:
  protected:
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    String   VTObjName="VTAuxiliaryFunction2";
    //uint8_t VTObjType=31;
    boolean VTCriticalControl=false; //Bit 5
    boolean VTLockAssign=false;      //Bit 6
    boolean VTSingleAssign=false;    //Bit 7
    //
    //procedure
    void getAID2();
};


//------------------------------------------------------------------------------
//type 32
class TVTAuxiliaryInput2 : public TVTAuxiliaryFuncInp{
  private:
  protected:
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    String   VTObjName="VTAuxiliaryInput2";
    //uint8_t VTObjType=32;
    boolean VTCriticalControl=false; //Bit 5
    boolean VTSingleAssign=false;    //Bit 7
    //
    //procedure
    void getAID2();
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//type global
class TVTAuxObject : public TVTObject{
  private:
  protected:
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    String   VTObjName="TVTAuxObject";
    uint8_t VTPointerTyp=0; //         :TVTAuxPointerTyp;
    uint16_t VTValue=0xFFFF;
    //
    uint16_t VTZExternalObjectRef=0xFFFF;
    //
    boolean VTEnabled=true;
    //fVTAuxItemList        :TVTObjectItemList;
    //fReferProp            :TVTObjectReferenceList;
    //
    //fVTFont               :TVTFont;
    //
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 33
class TVTAuxDesignPointer : public TVTAuxObject{
  private:
  protected:
    //Events
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnSoftKeyPress};
  public:
    String   VTObjName="TVTAuxObject";
    //uint8_t  VTObjType=33;
};


//==============================================================================
//==============================================================================
