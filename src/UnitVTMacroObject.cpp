//==============================================================================
//Definition objects TVTMacroObject
//==============================================================================
//unit UnitVTMacroObject.cpp;
//==============================================================================
#ifndef UnitVTMacroObject_h
  #define UnitVTMacroObject_h
  #include "UnitVTMacroObject.h"
#endif 

//==============================================================================
//==============================================================================
//type=13
boolean TVTMacroObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t color=TFT_BLACK,w=0,h=0;
uint8_t err=0x00;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID();
   //not Paint
   //getViewport(pTFT,&vRect,x,y,w,h);
  }//valid 
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTMacroObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTMacroObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint8_t  bb=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
   getVTCommands(pVT_Net,pStream);
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
void TVTMacroObject::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTCommands =ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTMacroObject::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType"; VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=2; VTAttrAID[1].nameAID="VTCommands";VTAttrAID[1].valueAID=String(VTCommands);
 VT_AID_Nr=2;
};
  


//==============================================================================
//==============================================================================
