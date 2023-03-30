//==============================================================================
//Definition objects TVTWorkingSet, TVTMask, TVTAlarmMask,
// TVTContainer, TVTWindowMaskObject
// TVTAnimationObject
//==============================================================================
//unit UnitVTTopLevelObjects.h;
//==============================================================================
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif



//==============================================================================
//==============================================================================
//type 0
class TVTWorkingSet :public TVTObjectBgCol{
  private:
     
  protected:
    //Events ID
    uint8_t VTEventNr=5;
    uint8_t VTEventList[5]={VTOnActivate,VTOnDeactivate,VTOnChangeActiveMask,
                            VTOnChangeChildLocation,VTOnChangeChildPosition};
  public:
    String   VTObjName="VTWorkingSet";
    //uint8_t  VTObjType=0;
    //
    boolean  VTSelectable=1;
    uint16_t VTActiveMask=0xFFFF;
    uint16_t VTColourMap=0xFFFF ;
    uint16_t VTColourPalette=0xFFFF;
    //
    String VTObjects="";//TVTObjectPointerList;
    String VTMacros=""; //TVTMacroList;
    String VTLanguages="";
    //
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect, TVT_Net *pVT_Net);
    boolean  writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    //
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
    String getLanguages() {return VTLanguages;};
    //
    uint16_t getVTActiveMask(){return VTActiveMask;};
};




//------------------------------------------------------------------------------
//type global
class TVTMask :public TVTObjectBgCol {
  
  private:
  protected:
    String  VTObjName="VTMask";
    //Events  ID
    uint8_t VTEventNr=6;
    uint8_t VTEventList[6]={VTOnShow,VTOnHide,VTOnChangeSoftKeyMask,
                            VTOnChangeAttribute,VTOnChangeChildLocation,VTOnChangeChildPosition};
  public:
    uint16_t VTSoftKeyMask=0xFFFF;
    String VTObjects="";//TVTObjectPointerList;
    String VTMacros=""; //TVTMacroList;
    //procedure
    // by JG: return boolean
    virtual boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net){return false;};
    virtual boolean  readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    virtual boolean  writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream){return false;};
    //
    boolean getSoftKeyMask(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean PaintObjToMask(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    virtual void setAID(){};
    virtual void getAID(){};
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
    uint16_t getVTSoftKeyMask(){return VTSoftKeyMask;};
    
};


//------------------------------------------------------------------------------
//type 1
class TVTDataMask : public TVTMask {
  private:
  protected:
  public:
   String VTObjName="VTDataMask";
   //uint8_t VTObjType=1;
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};  



//------------------------------------------------------------------------------
//type 2
class  TVTAlarmMask : public TVTMask {
  private:
  protected:
    //Events ID
    uint8_t VTOnChangePriority=17;  //TVTOnChangePriority;  //17
  public:
    String VTObjName="VTAlarmMask";
    //uint8_t VTObjType=2;
    uint8_t VTPriority=1;
    uint8_t VTAcousticSignal=1;
    //procedure    
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
};

//------------------------------------------------------------------------------
//type 3
class  TVTContainer :public TVTObject {
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=5;
    uint8_t VTEventList[5]={VTOnShow,VTOnHide,VTOnChangeSize,
                            VTOnChangeChildLocation,VTOnChangeChildPosition};
  public:
    String  VTObjName="VTContainer";
    //uint8_t  VTObjType=3;
    uint16_t VTHeight=0;
    uint16_t VTWidth=0;;
    boolean  VTHidden;
    String VTObjects="";//TVTObjectPointerList;
    String VTMacros=""; //TVTMacroList;
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
};



//------------------------------------------------------------------------------
//ISOVersion >04
//------------------------------------------------------------------------------
//type 34
class  TVTWindowMaskObject :public TVTObjectBgCol {
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=5;
    uint8_t VTEventList[5]={VTOnShow,VTOnHide,VTOnChangeAttribute,
                            VTOnChangeChildLocation,VTOnChangeChildPosition};
  public:
    String  VTObjName="VTWindowMaskObject";
    //uint8_t VTObjType=34;
    uint8_t VTHeight=0;
    uint8_t VTWidth=0;;
    uint8_t VTWindowType=0; //TVTWindowType;
    uint8_t VTOptions=0;    //TVTKeyGroupOption;
    //
    uint16_t VTName       =0xFFFF;
    uint16_t VTWindowTitle=0xFFFF;
    uint16_t VTWindowIcon =0xFFFF;
    //
    String VTObjRefs="";//TVTObjectReferenceList;
    String VTObjects="";//TVTObjectPointerList;
    String VTMacros=""; //TVTMacroList;
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
};


//------------------------------------------------------------------------------
//ISOVersion >04
//------------------------------------------------------------------------------
//type 44
class  TVTAnimationObject :public TVTObject {
  private:
  protected:
    //Events ID
    uint8_t VTEventNr=5;
    uint8_t VTEventList[5]={VTOnEnable,VTOnDisable,VTOnChangeAttribute,
                            VTOnChangeValue,VTOnChangeSize};
  public:
    String  VTObjName="VTAnimationObject";
    //uint8_t  VTObjType=44;
    uint16_t VTHeight=0;
    uint16_t VTWidth=0;;
    uint16_t VTRefreshInterval;
    uint8_t  VTValue;
    //
    boolean  VTEnabled;
    uint8_t  VTFirstChildIndex;
    uint8_t  VTLastChildIndex;
    uint8_t  VTDefaultChildIndex;
    //
    uint8_t  VTAnimationOptions;
    //helper
    uint8_t  VTAnimationSequence;        //TVTAnimationSequence;         //=(a0_SingleShotMode,a1_LoopMode);
    uint8_t  VTAnimationDisabledBehavior;//TVTAnimationDisabledBehavior; // =(b0_PauseMode,b1_ResetMode,b2_DefaultObjectMode,b3_BlankMode);
    //
    //helper
    boolean  VTZ_VisibleMode;
    uint16_t VTZ_AnimateTime;
    uint8_t  VTZChangeIndex ;
    //list
    String VTObjects="";//TVTObjectPointerList;
    String VTMacros=""; //TVTMacroList;
    //procedure
    boolean  PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net);
    boolean  writeToStream  (TVT_Net *pVT_Net,LoopbackStream *pStream);
    boolean  readFromStream (TVT_Net *pVT_Net,LoopbackStream *pStream);
    void setAID();
    void getAID();
    //
    String getObjects()   {return VTObjects;};
    String getMacros()    {return VTMacros;};
};

//==============================================================================
//==============================================================================
