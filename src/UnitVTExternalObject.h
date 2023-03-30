//==============================================================================
//Definition objects
//TVTExternalObject, TVTExternalReferenceName,
//TVTExternalObjectDefinition, TVTExternalObjectPointer
//==============================================================================
//unit UnitVTExternalObject.h;
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
//type global
class  TVTExternalObject : public TVTObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeAttribute};
  public:
    String  VTObjName="VTExternalObject";
    boolean  VTOptions=false;
    uint32_t VTName0=0xFFFFFFFF;
    uint32_t VTName1=0xFFFFFFFF;
    String   VTItems="";
    //helper
    String   VTZWorkingSetName="";
    uint16_t VTZExternalObjectRef=0xFFFF;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};


//==============================================================================
//type 41
class  TVTExternalObjectDefinition : public TVTExternalObject{
  private:
  protected:
  public:
    String  VTObjName="VTExternalObjectDefinition";
    //uint8_t VTObjType=41;
    //helper
    uint8_t VTZChangeIndex=0;
    //procedure
    //boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
};


//==============================================================================
//type 42
class  TVTExternalReferenceName : public TVTExternalObject{
  private:
  protected:
  public:
    String  VTObjName="VTExternalReferenceName";
    //uint8_t VTObjType=42;
    //helper
    //
    //procedure
   // boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
};



//==============================================================================
//type 43
class  TVTExternalObjectPointer : public TVTObject{
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeAttribute};
  public:
    String  VTObjName="VTExternalObjectPointer";
    //uint8_t  VTObjType=43;
    uint16_t VTDefaultObjectRef =0xFFFF;
    uint16_t VTExternalNameRef  =0xFFFF;
    uint16_t VTExternalObjectRef=0xFFFF;
    String   VTValidationBitmask="";
    //helper
    boolean VTZ_ExternalMode=false;;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};



//==============================================================================
//==============================================================================
