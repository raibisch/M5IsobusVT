//==============================================================================
//Definition objects TVTInputObject,
//TVTInputBooleanField, TVTInputStringField, TVTInputNumberField, TVTInputListField
//==============================================================================
//unit UnitVTInputFieldObjects.h
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


#ifndef UnitVTOutputFieldObjects_h
 #define UnitVTOutputFieldObjects_h
 #include "UnitVTOutputFieldObjects.h"
#endif


//==============================================================================
//type global
class TVTInputObject : public TVTFieldObject {
  private:
  protected:
     //InputStatus for InputObjects
     //      = (in0_NotInput,in1_ESC,in2_Entry,in3_EntryNew);  //Key Status

    //Events  ID
    uint8_t VTEventNr=7;
    uint8_t VTEventList[7]={VTOnEnable,VTOnDisable,
                            VTOnInputFieldSelection,VTOnInputFieldDeselection,
                            VTOnESC,VTOnEntryOfValue,VTOnEntryOfNewValue};
  public:
    uint8_t InputStatus=0;
    boolean ParentVisible=true;
    boolean VTEnabled=true;
    uint8_t VTEnabledOptions=0x00;
    boolean VTZ_EditMode=false;
    //
    //procedure
    // by JG return boolean
    virtual boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net){return false;};
    virtual boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual void setAID(){};
    virtual void getAID(){};
};




//------------------------------------------------------------------------------
//type 7
class TVTInputBooleanField  : public TVTInputObject {
  private:
  protected:
    //
  public:
    String   VTObjName="VTInputBooleanField";
    //uint8_t  VTObjType=7;
    boolean VTValue=true;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 8
class TVTInputStringField  : public TVTInputObject {
  private:
  protected:
    //
  public:
    String   VTObjName="VTInputStringField";
    //uint8_t  VTObjType=8;
    uint16_t VTHeight=50;
    uint16_t VTInputAttributes=0xFFFF;
    uint8_t  VTOptions=0x00;
    uint8_t  VTLength=4;
    String   VTValue="TEST";
    boolean  VTZValidCheck=false;
    boolean  VTZValidLength=false;
    boolean  VTZUniCode=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 9
class TVTInputNumberField  : public TVTInputObject {
  private:
  protected:
    //
  public:
    String   VTObjName="VTInputNumberField";
    //uint8_t  VTObjType=9;
    uint16_t VTHeight=50;
    uint16_t VTInputAttributes=0xFFFF;
    uint8_t  VTOptions=0x00;
    uint32_t VTValue=0;
    uint32_t VTMinValue=0;
    uint32_t VTMaxValue=0;
    uint32_t VTOffset=0;
    uint8_t  VTNumOfDecimals;
    boolean  VTFormat=false;

    uint32_t VTScale=1;
    boolean  VTZScaleCalc;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};



//------------------------------------------------------------------------------
//type 10
class TVTInputListField  : public TVTInputObject {
  private:
  protected:
  public:
    String   VTObjName="VTInputListField";
    //uint8_t  VTObjType=10;
    uint16_t VTHeight=50;
    uint8_t  VTValue=0;
    uint8_t  VTOptions=0x00;
    uint8_t  VTZChangeIndex;
    boolean  VTZSkipMode=false;
    uint8_t  VTZActiveItem=0;
    //list
    String  VTItems="";
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//==============================================================================
//==============================================================================
