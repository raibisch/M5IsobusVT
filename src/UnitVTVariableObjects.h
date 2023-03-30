//==============================================================================
//Definition objects TVTNumberVariable,TVTStringVariable,
//TVTColourMap,
//TVTColourPalette, TVTGraphicData, TVTScaledGraphic, TVTWorkingSetControl
//==============================================================================
//unit UnitVTVariableObjects.h
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif



//==============================================================================
//type 21
class  TVTNumberVariable : public TVTObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeValue};
  public:
    String  VTObjName="VTNumberVariable";
    //uint8_t VTObjType=21;
    uint32_t VTValue=0;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 22
class TVTStringVariable :public TVTObject{
  private:
  protected:
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeValue};

  public:
    String  VTObjName="VTStringVariable";
    //uint8_t VTObjType=22;
    uint16_t VTLength=4;
    String   VTValue="TEST";
    boolean  VTZValidLength=true;
    boolean  VTZValidCheck=true;
    boolean  VTZUniCode=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 39
class TVTColourMap :public TVTObject{
  private:
  protected:
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnRefresh};
  public:
    String  VTObjName="VTColourMap";
    //uint8_t VTObjType=39;
    uint16_t VTIndexNumber=2; //TVTColourMapTyp;
    //helper
    uint8_t VTZIndexItem=0;
    boolean VTZIndexChange=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    void    setVTColourMap(TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};
  
//------------------------------------------------------------------------------
//type 45
class TVTColourPalette :public TVTObject{
  private:
  protected:
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeAttribute};
  public:
    String  VTObjName="VTColourPalette";
    //uint8_t VTObjType=45;
    uint16_t VTOptions=0;
    uint16_t VTColourItems=0; 
    //helper
    uint8_t VTZIndexItem=0;
    boolean VTZIndexChange=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    void    setVTColourPalette(TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 46
class TVTGraphicData :public TVTObject{
  private:
  protected:
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnRefresh};
  public:
    String  VTObjName="VTGraphicData";
    //uint8_t VTObjType=46;
    uint8_t  VTFormat=0;
    String   VTPicData=""; //TMemoryStream;
    //uint32_t VTRawCount=0;
    //helper
    String   VTZPictureFileName="";
    uint8_t  Scale=1;
    uint16_t ScaleWidth=0;
    uint16_t ScaleHeight=0;
    uint8_t  ScaleType=0;
    uint8_t  ScaleOptions=0;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};

//------------------------------------------------------------------------------
//type 48
class TVTScaledGraphic :public TVTObject{
  private:
  protected:
    uint8_t VTEventNr=2;
    uint8_t VTEventList[2]={VTOnChangeAttribute,VTOnChangeValue};
  public:
    String  VTObjName="VTScaledGraphic";
    //uint8_t VTObjType=48;
    uint16_t VTWidth=0;
    uint16_t VTHeight=0;
    //
    uint8_t  VTScaleType=0;
    //
    uint8_t  VTScale_Mode=0;//TVTScaleMode;
    uint8_t  VTScale_Hor=0; //TVTScaleHorizontal;
    uint8_t  VTScale_Ver=0; //TVTScaleVertical;
    //
    uint8_t  VTScaleOptions=0;//TVTScaleOption;
    uint16_t VTValue=0xFFFF;
    //
    String VTMacros="";
    //helper
    boolean VTZClipping=false;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};


//------------------------------------------------------------------------------
//type 47
class TVTWorkingSetControl :public TVTObject{
  private:
  protected:
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnRefresh};
  public:
    String   VTObjName="VTWorkingSetControl";
    //uint8_t  VTObjType=47;
    uint8_t  VTByteNumber=0;
    uint16_t VTColourMap=0xFFFF;
    uint16_t VTColourPalette=0xFFFF;
    String   VTLanguages="";
    //
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};

//==============================================================================
//==============================================================================
