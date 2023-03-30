//==============================================================================
//Definition objects TVTMacroCommandObject
//TVTESC                    , TVTHideShowObject       , TVTEnableDisableObject,TVTSelectInputObject,
//TVTControlAudioDevice     , TVTSetAudioVolume       ,
//TVTChangeChildLocation    , TVTChangeChildPosition  , TVTChangeSize
//TVTChangeBackgroundColour , TVTChangeNumericValue   , TVTChangeEndPoint,
//TVTChangeFontAttributes   , TVTChangeLineAttributes , TVTChangeFillAttributes
//TVTChangeActiveMask       , TVTChangeSoftKeyMask    , TVTChangeAttribute,
//TVTChangePriority         , TVTChangeListItem       , TVTDeleteObjectPool    ,TVTChangeStringValue
//
//ISOVersion 3
//TVTLockMaskObject         , TVTExecuteMacroObject   , TVTExtendedExecuteMacroObject
//TVTChangeObjectLabel      , TVTChangePolygonPoint   , TVTChangePolygonScale
//TVTGraphicContextCommand  ,
//TVTGetAttribute           ,TVTSelectColourMapObject
//TVTExtendedInputAttributeCommand
//TVTSelectActiveWorkingSet

//==============================================================================
//unit UnitVTMacroCommandObject.h
//==============================================================================
#ifndef UnitVTObjConsts_h
 #define UnitVTObjConsts_h
 #include "UnitVTObjConsts.h"
#endif

#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif


//==============================================================================
boolean MacroCANMsgPGN(TVT_Net *pVT_Net,CANMsg *pMsg,boolean macro);

//==============================================================================
//Commands and Messages
//------------------------------------------------------------------------------

//type  //146 $92
  class TVTESC :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0x92;
    uint8_t  VTEvent=VTOnESC;  
    
    uint16_t VTObjectReference=0xFFFF;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //160 $A0
  class TVTHideShowObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA0;
    uint8_t  VTEvent=VTOnShow;  
    //
    uint16_t VTObjectReference=0xFFFF;
    boolean  VTHidden=false;
    // 
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTHidden"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};
  

//------------------------------------------------------------------------------
//type   //161  $A1
  class TVTEnableDisableObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA1;
    uint8_t  VTEvent=VTOnEnable;  
    //
    uint16_t VTObjectReference=0xFFFF;
    boolean  VTEnabled=true;
    //
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTEnabled"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //162  $A2
  class TVTSelectInputObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA2;
    uint8_t  VTEvent=VTOnInputFieldSelection;  
    //
    uint16_t VTObjectReference=0xFFFF;
    boolean  VTSelect=false;
    //
    //
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTSelect"};
    
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};
  


//------------------------------------------------------------------------------
//type    //163     $A3
  class TVTControlAudioDevice :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA3;
    uint8_t  VTEvent=0; //  
    //
    uint8_t  VTRepetition     =0;
    uint16_t VTFrequency      =500;
    uint16_t VTOnTimeDuration =1000;
    uint16_t VTOffTimeDuration=1000;
    //
    uint8_t  VTAttrStrNr=4;
    String   VTAttrStr[4]={"VTRepetition","VTFrequency","VTOnTimeDuration","VTOffTimeDuration"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type   //164     $A4
  class TVTSetAudioVolume :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA4;
    uint8_t  VTEvent=0;   
    //
    uint8_t VTAudioVolume     =100;
    //
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTAudioVolume"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};

  
//------------------------------------------------------------------------------
//type  //165       $A5
  class TVTChangeChildLocation :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA5;
    uint8_t  VTEvent=VTOnChangeChildLocation;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTParentReference=0xFFFF;
    int8_t   VTxChange=0;
    int8_t   VTyChange=0;
    //
    uint8_t  VTAttrStrNr=4;
    String   VTAttrStr[4]={"VTObjectReference","VTParentReference","VTxChange","VTyChange"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};

  
//------------------------------------------------------------------------------
//type  //180  $B4
  class TVTChangeChildPosition :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB4;
    uint8_t  VTEvent=VTOnChangeChildPosition;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTParentReference=0xFFFF;
    int16_t  VTxPos=0;
    int16_t  VTyPos=0;
    //
    uint8_t  VTAttrStrNr=4;
    String   VTAttrStr[4]={"VTObjectReference","VTParentReference","VTxPos","VTyPos"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);
};
  



//------------------------------------------------------------------------------
//type   //166      $A6
class TVTChangeSize :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA6;
    uint8_t  VTEvent=VTOnChangeSize;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTWidth=0;
    uint16_t VTHeight=0;
    //
    uint8_t  VTAttrStrNr=2;
    String   VTAttrStr[2]={"VTWidth","VTHeight"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //167       $A7
class TVTChangeBackgroundColour :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA7;
    uint8_t  VTEvent=VTOnChangeBackgroundColour;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTBackgroundColour=0;
    //
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTBackgroundColour"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //168       $A8
class TVTChangeNumericValue :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA8;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint32_t VTValue=0;
    //
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTValue"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //169      $A9
class TVTChangeEndPoint :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xA9;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTWidth=50;
    uint16_t VTHeight=50;
    boolean  VTLineDirection=false;
    //
    uint8_t  VTAttrStrNr=1;
    String   VTAttrStr[1]={"VTLineDirection"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //170      $AA
class TVTChangeFontAttributes :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xAA;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTFontColour=0;
    uint8_t  VTFontSize=0; 
    uint8_t  VTFontType=0; 
    uint8_t  VTFontStyle=0;
    //
    uint8_t  VTAttrStrNr=4;
    String   VTAttrStr[4]={"VTFontColour","VTFontSize","VTFontType","VTFontStyle"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type   //171      $AB
class TVTChangeLineAttributes :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xAB;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTLineColour=0;
    uint8_t  VTLineWidth=1; 
    uint16_t VTLineArt=0xFFFF; 
    //
    uint8_t  VTAttrStrNr=3;
    String   VTAttrStr[3]={"VTLineColour","VTLineWidth","VTLineArt"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //172      $AC
class TVTChangeFillAttributes :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xAC;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTFillColour=0;
    uint8_t  VTFillType=0; 
    uint16_t VTFillPattern=0xFFFF; 
    //
    uint8_t  VTAttrStrNr=3;
    String   VTAttrStr[3]={"VTFillColour","VTFillType","VTFillPattern"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //173     $AD
class TVTChangeActiveMask :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xAD;
    uint8_t  VTEvent=VTOnChangeActiveMask;  
    //
    uint16_t VTWorkingSet=0xFFFF;
    uint16_t VTNewActiveMask=0xFFFF;
    //
    uint8_t  VTAttrStrNr=2;
    String   VTAttrStr[2]={"VTWorkingSet","VTNewActiveMask"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //174    $AE
class TVTChangeSoftKeyMask :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xAE;
    uint8_t  VTEvent=VTOnChangeSoftKeyMask;  
    //
    uint8_t  VTDataMaskType=0xFF;
    uint16_t VTMaskID=0xFFFF;
    uint16_t VTNewMaskID=0xFFFF;
    //
    uint8_t  VTAttrStrNr=3;
    String   VTAttrStr[3]={"VTDataMaskType","VTMaskID","VTNewMaskID"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //175    $AF
class TVTChangeAttribute :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xAF;
    uint8_t  VTEvent=VTOnChangeSoftKeyMask;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTAttributeID=0xFF;
    uint32_t VTValue=0;
    uint8_t  VTAttrStrNr=3;
    String   VTAttrStr[3]={"VTObjectReference","VTAttributeID","VTValue"};
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //176    $B0
class TVTChangePriority :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB0;
    uint8_t  VTEvent=VTOnChangePriority;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTNewPriority=0;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //177    $B1
class TVTChangeListItem :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB1;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTNewListIndex=0;
    uint16_t VTNewObjectID=0xFFFF;
    //
    boolean setMsgToAttr (CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //179    $B3
class TVTChangeStringValue :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB3;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTLength=0;
    String   VTNewValue="";
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);
};



//------------------------------------------------------------------------------
//type  //182 $B6
class TVTChangePolygonPoint :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB6;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t VTPointIndex=0;
    int16_t VTxPos=0;
    int16_t VTyPos=0;
    String  VTPoints="";
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type   //183     $B7
class TVTChangePolygonScale :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB7;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTNewWidth=0;
    uint16_t VTNewHeight=0;
    String  VTPoints="";
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//Begin Messages ISO Version 3
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//type    //189  $BD
class TVTLockMaskObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xBD;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    boolean VTLock=false;
    uint16_t VTObjectReference=0xFFFF;
    uint16_t VTTimeOut        =0xFFFF;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type    //190  $BE
class TVTExecuteMacroObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xBE;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint8_t VTObjectReference     =0xFF;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type    //188 $BC
class TVTExtendedExecuteMacroObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xBC;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference     =0xFFFF;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type    //181  $B5
class TVTChangeObjectLabel :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB5;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference     =0xFFFF;
    uint16_t VTVariableReference   =0xFFFF;
    uint8_t  VTFontType            =0x00;
    uint16_t VTGraphicReference    =0xFFFF;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type   //184   $B8
class TVTGraphicContextCommand :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xB8;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    uint8_t  VTSubCommandID=0;
    String VTCommand="";
    boolean NewCommand=false;
    boolean AttrCommand=false;
    String AttrCommandStr="";
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};


//------------------------------------------------------------------------------
//type    //186  $BA
class TVTSelectColourMapObject :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0xBA;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint16_t VTObjectReference=0xFFFF;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};




//------------------------------------------------------------------------------
//type   //0 .. 15 , $00   $0F, CodePlanes
class TVTExtendedInputAttributeCommand :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0x0F;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    uint8_t VTRangeNumber=0;
    String  VTCommand="";
    boolean NewCommand =false;
    boolean AttrCommand=false;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net);
};



//------------------------------------------------------------------------------
//type  //144  $90
class TVTSelectActiveWorkingSet :public TVTObject { 
  private:
    //
  protected:
  //
  public:
    uint8_t  VT0PCommFunction=0x90;
    uint8_t  VTEvent=VTOnChangeValue;  
    //
    String   VTWorkingSetName;
    //
    boolean setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream);
};





//==============================================================================
//==============================================================================
