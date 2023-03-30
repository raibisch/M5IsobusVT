//==============================================================================
//Definition objects TVTGraphicsContext
//==============================================================================
//unit UnitVTGraphicContextObject.h
//==============================================================================

#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


#ifndef UnitVTPictureGraphicObject_h
  #define UnitVTPictureGraphicObject_h
  #include "UnitVTPictureGraphicObject.h"
#endif 


//==============================================================================
//type 36
class TVTGraphicsContext : public TVTGraphicObject{
  private:
  protected:
    //Events ID
    //uint8_t VTEventNr=1;
    //uint8_t VTEventList[1]={VTOnChangeAttribute};  //9
  public:
    String  VTObjName="VTGraphicsContext";
    //uint8_t  VTObjType=36;
    uint16_t VTWidth=0 ;  //Canvas width
    uint16_t VTHeight=0;  //Canvas height
    //
    boolean VTViewportActive=false;
    uint16_t VTViewportWidth=0;
    uint16_t VTViewportHeight=0;
    String   ViewportSizeStr="";

    int16_t VTViewportX;
    int16_t VTViewportY;
    String  fViewportPosStr="";
    
    float VTViewportZoom=1.000;       //ShortInt -32.0 .. 32.0
    boolean VTViewportZoomMode=false; //Pan and Zoom
    boolean VTViewportVisible=true;
    //
    int16_t VTGraphicsCursorX=0;
    int16_t VTGraphicsCursorY=0;
    String  GraphicsCursorPosStr="";
    boolean VTGraphicsCursorVisible=false;
    boolean GraphicsCursorMove=false;
    //
    uint8_t VTForegroundColour=0;
    uint8_t VTBackgroundColour=1;
    //
    uint8_t  VTOptions; //TVTContextOption;
    uint16_t VTLineAttributes=0xFFFF;
    uint16_t VTFillAttributes=0xFFFF;
    uint16_t VTFontAttributes=0xFFFF;
    //
    String VTDrawText=""; //TVTWideString;
    //
    //helper
    boolean VTZUniCode=false;
    //
    uint16_t DrawVTObject=0xFFFF;
    uint16_t CopyCanvasToPicture=0xFFFF;
    uint16_t CopyViewPortToPicture=0xFFFF;
    //
    String DrawShape="";
    String CommandStr="";
    String SizeStr="";
    //
    boolean Undo=false;
    uint16_t DrawWidth=0;
    uint16_t DrawHeight=0;
    boolean DrawTextSize=false;
    //
    int16_t DrawOldX=0;
    int16_t DrawOldY=0;
    boolean DrawVisible=false;
    boolean DrawPoint=false;
    String  DrawPointText="";
    //
    boolean ViewPortPosXY=false;
    
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
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
