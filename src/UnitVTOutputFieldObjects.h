//==============================================================================
//Definition objects TVTFieldObject,
//TVTOutputStringField, TVTOutputNumberField, TVTOutputListField
//==============================================================================
//unit UnitVTOutputFieldObjects.h
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif

//==============================================================================
//==============================================================================
//type
class   TVTFieldObject: public TVTObject{
  private:
  protected:
    //Events  ID
    uint8_t VTEventNr=4;
    uint8_t VTEventList[4]={VTOnChangeValue,VTOnChangeAttribute,
                            VTOnChangeSize,VTOnChangeBackgroundColour};
  public:
    uint16_t VTWidth=50;
    uint16_t VTFontAttributes=0xFFFF;
    uint16_t VTVariableReference=0xFFFF;
    boolean  VTZFontSynchron=false;
    String   VTZValue="";
    //
    uint8_t VTJustification=0x00;
    uint8_t VTJust_Horz=0x00;
    uint8_t VTJust_Vert=0x00;
    uint8_t VTBackgroundColour=0x01;
    //list
    String VTMacros="";
    //
    //procedure
    // by JG: return boolean value
    virtual boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net){return false;};
    virtual boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual void setAID(){};
    virtual void getAID(){};
};  


//------------------------------------------------------------------------------
//type
class   TVTOutputFieldObject: public TVTFieldObject{
  private:
  protected:

  public:
    uint16_t VTHeight=50;
    //procedure
     // by JG: return boolean value
    virtual boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net){return false;};
    virtual boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual void setAID(){};
    virtual void getAID(){};
};  


//------------------------------------------------------------------------------
//type 11
class   TVTOutputStringField: public TVTOutputFieldObject{
  private:
  protected:
  public:
    String   VTObjName="VTOutputStringField";
    //uint8_t  VTObjType=11;
    uint8_t  VTOptions=0x00;
    uint16_t VTLength=4;
    String   VTValue="TEXT";
    boolean  VTZValidLength=true;
    boolean  VTZValidCheck=true;
    boolean  VTZUniCode=false;
    //
    //procedure
    virtual boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    virtual boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    virtual boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};  



//------------------------------------------------------------------------------
//type 12
class   TVTOutputNumberField: public TVTOutputFieldObject{
  private:
  protected:
  public:
    String   VTObjName="VTOutputNumberField";
    //uint8_t  VTObjType=12;
    uint8_t  VTOptions=0x00;
    uint32_t VTValue=0;
    int32_t  VTOffset=0;
    int32_t  VTScale=1;
    boolean  VTZScaleCalc=true;
    //   
    uint8_t  VTNumOfDecimals=0;
    boolean  VTFormat=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};  





//------------------------------------------------------------------------------
//type 37
class   TVTOutputListField: public TVTOutputFieldObject{
  private:
  protected:
  public:
    String   VTObjName="VTOutputListField";
    //uint8_t  VTObjType=37;
    uint8_t ActiveItem=0;
    uint8_t VTZChangeIndex=0;
    //
    uint8_t VTValue=0;
    uint8_t VTBackgroundColour=0;
    boolean VTZSkipMode=false;
    //list
    String VTItems="";
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};  

//==============================================================================
//==============================================================================
