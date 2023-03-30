//==============================================================================
//Definition objects
//TVTGraphicObject,  TVTPictureGraphic
//==============================================================================
//unit UnitVTPictureGraphicObject.h;
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif



//==============================================================================
//==============================================================================
//type global
class TVTGraphicObject : public TVTObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=2;
    uint8_t VTEventList[2]={VTOnChangeAttribute,VTOnChangeBackgroundColour};  //9
  public:
    String  VTObjName="VTGraphicObject";
    uint16_t VTWidth=8;
    uint16_t VTActualWidth=8;
    uint16_t VTActualHeight=8;
    uint8_t  VTFormat=0;
    uint8_t  VTOptions=0;
    uint8_t  VTTransparencyColour=0;
    uint32_t VTPicData=0;
    uint8_t  VTMacros=0;
};


//==============================================================================
//type 20
class TVTPictureGraphic : public TVTGraphicObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeAttribute};  //9
  public:
    String  VTObjName="VTPictureGraphic";
    //uint8_t VTObjType=20;
    //procedure
    boolean  PaintObjTo    (TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    //
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readPictureFromStream(TVT_Net *pVT_Net,uint32_t bCount,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
};



//==============================================================================
//==============================================================================
