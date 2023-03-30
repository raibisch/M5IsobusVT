//==============================================================================
//Definition objects TVTSoftkeyMask, TVTKey, TVTButton,
//TVTKeyGroupObject,TVTLabelReferenceListObject
//==============================================================================
//unit UnitVTKeyObjects.h;
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
//==============================================================================
//type 4
class  TVTSoftKeyMask :public TVTObjectBgCol  {
  private:
  protected:
    //Events
    uint8_t VTEventNr=4;
    uint8_t VTEventList[4]={VTOnShow,VTOnHide,VTOnChangeSoftKeyMask,
                            VTOnChangeAttribute};
  public:
    String   VTObjName="VTSoftKeyMask";
    //uint8_t VTObjType=4;
    String  VTItems=""; //TVTObjectItemList;
    String  VTMacros="";  //TVTMacroList;
    //procedure
    String  getVTObjName(){return VTObjName;}; 
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getItems()     {return VTItems;};
    String getMacros()    {return VTMacros;};
};


//------------------------------------------------------------------------------
//type 5
class TVTKey : public TVTObjectBgCol{
  private:
  protected:
    //Events
    uint8_t VTEventNr=7;
    uint8_t VTEventList[7]={VTOnSoftKeyPress,VTOnSoftKeyRelease,
                            VTOnChangeAttribute,VTOnChangeChildLocation,VTOnChangeChildPosition,
                            VTOnInputFieldSelection,VTOnInputFieldDeselection};
  public:
    String   VTObjName="VTKey";
    //uint8_t  VTObjType=5;
    //uint16_t VTInputStatus=0;
    uint8_t  VTKeyCode=0;
    String  VTObjects ="";//TVTObjectList;
    String  VTMacros="";  //TVTMacroList;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
};

//------------------------------------------------------------------------------
//type 6
class TVTButton : public TVTObject {
  private:
  protected:
    //Events
    uint8_t VTEventNr=11;
    uint8_t VTEventList[11]={VTOnSoftKeyPress,VTOnSoftKeyRelease,VTOnChangeSize,
                            VTOnChangeAttribute,VTOnChangeBackgroundColour,
                            VTOnChangeChildLocation,VTOnChangeChildPosition,
                            VTOnEnable,VTOnDisable,
                            VTOnInputFieldSelection,VTOnInputFieldDeselection};
  public:
    String   VTObjName="VTButton";
    //uint8_t VTObjType=6;
    int16_t VTInputStatus=0;
    int16_t VTWidth=0;
    int16_t VTHeight=0;
    //
    uint8_t VTBackgroundColour=0;
    uint8_t VTBorderColour=0;
    uint8_t VTKeyCode=0;
    uint8_t VTOptions=0;
    String  VTObjects ="";//TVTObjectList;
    String  VTMacros="";  //TVTMacroList;
    //procedure
    boolean PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
};



//------------------------------------------------------------------------------
//Version 4
//------------------------------------------------------------------------------
//type 35
class  TVTKeyGroupObject :public TVTObject {
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnChangeAttribute};
  public:
    String  VTObjName="TVTKeyGroupObject";
    //uint8_t VTObjType=35;
    uint8_t VTOptions=0;    //TVTKeyGroupOption;
    //
    uint16_t VTName        =0xFFFF;
    uint16_t VTKeyGroupIcon=0xFFFF;
    //
    String VTItems=""; //TVTObjectItemList;
    String VTMacros=""; //TVTMacroList;
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getItems()     {return VTItems;};
    String getMacros()    {return VTMacros;};
};


//------------------------------------------------------------------------------
//type 40
class  TVTLabelReferenceListObject :public TVTObject {
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=1;
    uint8_t VTEventList[1]={VTOnRefresh};
  public:
    String  VTObjName="VTLabelReferenceListObject";
    //uint8_t VTObjType=40;
    //
    String VTLabels=""; //TVTObjectLabelList;
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getLabels()     {return VTLabels;};
};


//==============================================================================
//==============================================================================
