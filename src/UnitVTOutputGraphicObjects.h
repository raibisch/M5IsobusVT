//==============================================================================
//Definition objects TVTOutputGraphicObject,
// TVTMeter, TVTLinearBargraph, TVTArchedBargraph
//==============================================================================
//unit UnitVTOutputGraphicObjects.h
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
//type global
class  TVTOutputGraphicObject: public TVTObject {
  private:
  protected:
    String   VTObjName="VTOutputGraphicObject";
    //Events  ID
    uint8_t VTEventNr=3;
    uint8_t VTEventList[3]={VTOnChangeValue,VTOnChangeAttribute,VTOnChangeSize};
  public:
    uint16_t VTWidth=50;
    uint16_t VTMinValue=0;
    uint16_t VTMaxValue=100;
    uint16_t VTValue=0;
    uint16_t VTVariableReference=0xFFFF;
    //list
    String VTMacros="";
    //
    //procedure
    virtual boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net){};
    virtual boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream){};
    virtual boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){};
    virtual void setAID(){};
    virtual void getAID(){};
};
  
//------------------------------------------------------------------------------
//type 17
class  TVTMeter: public TVTOutputGraphicObject {
  private:
  protected:
    //Events  ID
    uint8_t VTEventNr=3;
    uint8_t VTEventList[3]={VTOnChangeValue,VTOnChangeAttribute,VTOnChangeSize};
  public:
    String   VTObjName="VTMeter";
    //uint8_t VTObjType=17;
    uint8_t VTNeedleColour=0;
    uint8_t VTBorderColour=0;
    uint8_t VTArcAndTickColour=0;
    uint8_t VTOptions=0;
    uint8_t VTNumOfTicks=0;
    uint8_t VTStartAngle=0;
    uint8_t VTEndAngle=0;
    //
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};

  
//------------------------------------------------------------------------------
//type 18
class  TVTLinearBarGraph: public TVTOutputGraphicObject {
  private:
  protected:
    //Events  ID
    uint8_t VTEventNr=3;
    uint8_t VTEventList[3]={VTOnChangeValue,VTOnChangeAttribute,VTOnChangeSize};
  public:
    String   VTObjName="VTLinearBarGraph";
    //uint8_t  VTObjType=18;
    uint16_t VTHeight=0;
    uint8_t  VTColour=0;
    uint8_t  VTTargetLineColour=0;
    uint8_t  VTOptions=0;
    uint8_t  VTNumOfTicks=0;
    uint16_t VTTargetVariableReference=0xFFFF;
    uint8_t  VTTargetValue=0;
    //
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean PaintObjToOptions(TVT_Net *pVT_Net,TVTPixelXY *pXY,uint16_t valV, uint8_t opt,uint16_t color);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};

//------------------------------------------------------------------------------
//type 19
class  TVTArchedBarGraph: public TVTOutputGraphicObject {
  private:
  protected:
    //Events  ID
    uint8_t VTEventNr=3;
    uint8_t VTEventList[3]={VTOnChangeValue,VTOnChangeAttribute,VTOnChangeSize};
  public:
    String   VTObjName="VTArchedBarGraph";
    //uint8_t  VTObjType=19;
    uint16_t VTHeight=0;
    uint8_t  VTColour=0;
    uint8_t  VTTargetLineColour=0;
    uint8_t  VTOptions=0;
    uint8_t  VTStartAngle=0;
    uint8_t  VTEndAngle=0;
    uint16_t VTBarGraphWidth=0;
    uint16_t VTTargetVariableReference=0xFFFF;
    uint8_t  VTTargetValue=0;
    //
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    void    SetArchedValue(TVT_Net *pVT_Net,int16_t targW, uint8_t lw, uint8_t bw, uint16_t targColor);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();  
};

//==============================================================================
//==============================================================================
