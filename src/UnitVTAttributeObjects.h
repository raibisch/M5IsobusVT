//==============================================================================
//Definition objects TVTAttributeObject,
//TVTFontAttribute, TVTLineAttribute, TVTFillAttribute, TVTInputAttribute,
//TVTExtendedInputAttribute
//==============================================================================
//unit UnitVTAttributeObjects.h
//==============================================================================

#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
//type global
class TVTAttributeObject : public TVTObject {
  private:
  protected:
  public:
    String VTMacros="";
    //procedure
    virtual boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net){};
    virtual boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream){};
    virtual boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){};
    virtual void setAID(){};
    virtual void getAID(){};
};

//------------------------------------------------------------------------------
//type 23
class TVTFontAttribute : public TVTAttributeObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=2;
    uint8_t VTEventList[2]={VTOnChangeFontAttributes,VTOnChangeAttribute};
  public:
    String  VTObjName="VTFontAttribute";
    //uint8_t VTObjType=23;
    uint8_t VTFontColour=0;
    uint8_t VTFontSize=0;
    uint8_t VTFontType=0;
    uint8_t VTFontStyle=0;
    boolean VTZFontHSFlash=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 24
class TVTLineAttribute : public TVTAttributeObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=2;
    uint8_t VTEventList[2]={VTOnChangeLineAttributes,VTOnChangeAttribute};
  public:
    String  VTObjName="VTLineAttribute";
    //uint8_t  VTObjType=24;
    uint8_t  VTLineColour=0;
    uint8_t  VTLineWidth=1;
    uint16_t VTLineArt=0xFFFF;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 25
class TVTFillAttribute : public TVTAttributeObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=2;
    uint8_t VTEventList[2]={VTOnChangeFillAttributes,VTOnChangeAttribute};
  public:
    String  VTObjName="VTFillAttribute";
    //uint8_t   VTObjType=25;
    uint8_t   VTFillType=0;
    uint8_t   VTFillColour=0;
    uint16_t  VTFillPattern=0xFFFF;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 26
class TVTInputAttribute : public TVTAttributeObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeValue};
  public:
    String  VTObjName="VTInputAttribute";
    //uint8_t VTObjType=26;
    boolean VTValidationType=false;
    uint8_t VTLength        =4;
    String  VTValidationString="TEST";
    boolean VTZValidLength=false;
    boolean VTZValidCheck =false;
    boolean VTZUniCode    =false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 38
class TVTExtendedInputAttribute : public TVTAttributeObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeValue};
  public:
    String  VTObjName="VTExtendedInputAttribute";
    //uint8_t VTObjType=38;
    boolean VTValidationType=false;
    String  VTCodePlanes="";
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};

//==============================================================================
//==============================================================================
