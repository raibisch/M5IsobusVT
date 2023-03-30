//==============================================================================
//Definition objects
//TVTNumberVariable,TVTStringVariable,
//TVTColourMap,TVTColourPalette, 
//TVTGraphicData, TVTScaledGraphic, TVTWorkingSetControl
//==============================================================================
//unit UnitVTVariableObjects.cpp;
//==============================================================================
#ifndef UnitVTVariableObjects_h
  #define UnitVTVariableObjects_h
  #include "UnitVTVariableObjects.h"
#endif 


//==============================================================================
//type=21
boolean TVTNumberVariable::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
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
  if ((valid) && (objIdx>=0)) {
    getAID();
   //not paint
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTNumberVariable::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTNumberVariable::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint8_t  bb=0;
uint32_t ww=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTNumberVariable::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTValue =ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTNumberVariable::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=4; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTValue";  VTAttrAID[1].valueAID=String(VTValue);
 VT_AID_Nr=2;
};


//==============================================================================
//type=22
boolean TVTStringVariable::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
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
  if ((valid) && (objIdx>=0)) {
    getAID();
   //not paint
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTStringVariable::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTStringVariable::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint8_t  bb=0;
uint32_t ww=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
    //
    for (i=1;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==1) getVTString(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTStringVariable::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTLength =ww;break;
       }//switch i
    }//for i
};


//------------------------------------------------------------------------------
void TVTStringVariable::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=2; VTAttrAID[1].nameAID="VTLength"; VTAttrAID[1].valueAID=VTValue;
 VT_AID_Nr=2;
};


//==============================================================================
//type=39
boolean TVTColourMap::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
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
  if ((valid) && (objIdx>=0)) {
   getAID();
   //not paint set colorMap
    if (VTIndexNumber>0) setVTColourMap(pVT_Net);
  }//valid
 return valid;  
};



//------------------------------------------------------------------------------
void TVTColourMap::setVTColourMap(TVT_Net *pVT_Net) {
uint16_t i=0,nn=pVT_Net->streamStr.available();
uint8_t idx=0,aIdx=2;
uint8_t* buff=pVT_Net->streamStr.getBuffer();
//TEST
//getStreamStrInfo(pVT_Net);
  for (i=0;i<VT_AID_Nr;i++) aIdx+=VTAttrAID[i].byteAID;
  //
  for (i=0;i<VTIndexNumber;i++) {
    if (aIdx+i<nn){
      idx=buff[aIdx+i];
      pVT_Net->gISO_ColourMap[i]=pVT_Net->ggISO_ColourMap[idx];
    } 
  }//for i
};



//------------------------------------------------------------------------------
boolean TVTColourMap::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTColourMap::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint8_t  bb=0;
uint32_t ww=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);nn=ww;
    }//for i
    //
    for (i=1;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
        if (i==1) getVTColourItems(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTColourMap::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        //list
        case 1:VTIndexNumber =ww;break;
       }//switch i
    }//for i
};

  

//------------------------------------------------------------------------------
void TVTColourMap::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";      VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=2; VTAttrAID[1].nameAID="VTIndexNumber";  VTAttrAID[1].valueAID=String(VTIndexNumber);
 VT_AID_Nr=2;
};


//==============================================================================
//type=45
boolean TVTColourPalette::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
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
  if ((valid) && (objIdx>=0)) {
   getAID();
   //not paint set colorPalette
   setVTColourPalette(pVT_Net);
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
void TVTColourPalette::setVTColourPalette(TVT_Net *pVT_Net) {
uint16_t i=0,nn=pVT_Net->streamStr.available();
uint8_t  b=0,g=0,r=0,a=0;
uint16_t aIdx=2;
uint32_t cc=0;
uint8_t*buff=pVT_Net->streamStr.getBuffer();
//TEST
//getStreamStrInfo(pVT_Net);
  for (i=0;i<VT_AID_Nr;i++) aIdx+=VTAttrAID[i].byteAID;
  //
  for (i=0;i<VTColourItems;i++) {
    if (aIdx+4*i<nn){
      b=buff[aIdx+4*i+0];
      g=buff[aIdx+4*i+1];
      r=buff[aIdx+4*i+2];
      a=buff[aIdx+4*i+3];
      cc=(a<<24) + (r<<16) + (g<<8) + (b<<0);
      pVT_Net->gISO_ColourMap[i]=cc;
    } 
  }//for i
};


//------------------------------------------------------------------------------
boolean TVTColourPalette::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTColourPalette::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint8_t  bb=0;
uint32_t ww=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
    //
    for (i=2;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt();
       if (i==2) getVTColourItems(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   //TEST
   //getStreamStrInfo(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTColourPalette::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTOptions =ww;break;
        //list
        case 2:VTColourItems =ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTColourPalette::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";    VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTOptions";    VTAttrAID[1].valueAID=String(VTOptions);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTColourItems";VTAttrAID[2].valueAID=String(VTColourItems);
 VT_AID_Nr=3;
};


//==============================================================================
//type=46
boolean TVTGraphicData::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
uint16_t nn=8;
int16_t  objIdx=pVT_Net->objNr; 
TVTPixelXY pXY;
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
  if ((valid) && (objIdx>=0)) {
   getAID();
   valid=GraphicDataPaintObjTo(pVT_Net,&pXY, nn);
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTGraphicData::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTGraphicData::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0;
uint8_t  bb=0;
uint32_t ww=0,j=0,nn=0;
String attrStr="";
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
    //
    for (i=2;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==2) {
          if (nn>0){         
            for (j=0;j<nn;j++) {
             bb=pStream->read();pVT_Net->streamStr.write(bb);
            }//for j
          }
       }//i==2
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTGraphicData::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTFormat =ww;break;
        //list
        case 2:VTPicData =ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTGraphicData::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType"; VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTFormat";  VTAttrAID[1].valueAID=VTFormat;
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=4; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTPicData"; VTAttrAID[2].valueAID=VTPicData;
 VT_AID_Nr=3;
}


//==============================================================================
//type=48
boolean TVTScaledGraphic::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
uint16_t w=pVT_Net->w,h=pVT_Net->h,oLevel=pVT_Net->level;
int16_t  x=pVT_Net->x,y=pVT_Net->y, objIdx=pVT_Net->objNr,refIdx=-1; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
 TVT_ViewRect vRect=getViewRect(pViewRect);
  //
  if ((valid) && (objIdx>=0)) {
   getAID(); pVT_Net->level++;
   w=VTWidth;  h=VTHeight;
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
    //paint
    //ref VTPointerReference
    refIdx=getVTObjID(pVT_Net,VTValue); 
     if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       //runClassObj(pVT_Net,pViewRect,pVT_Net->VTObjType,pVT_Net->VTObjID,NULL);
       pVT_Net->VTScaleOptions=VTScaleOptions;
       pVT_Net->VTScaleType=VTScaleType;
       pVT_Net->w=w;pVT_Net->h=h;
       SetObjPaintObjToRef(&vRect,pVT_Net,VTValue);
       pVT_Net->VTScaleOptions=-1;
       pVT_Net->VTScaleType=-1;
     }
    getVTObjectFromList(pVT_Net,objIdx); 
    pVT_Net->level=oLevel;
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTScaledGraphic::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTScaledGraphic::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint8_t  bb=0;
uint32_t ww=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
    //
    for (i=6;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==6) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTScaledGraphic::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth  =ww;break;
        case 2:VTHeight =ww;break;
        case 3:VTScaleType =ww;break;
        case 4:VTScaleOptions =ww;break;
        case 5:VTValue  =ww;break;
        //list
        case 6:VTMacros =ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTScaledGraphic::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";  VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";    VTAttrAID[1].valueAID=VTWidth;
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";   VTAttrAID[2].valueAID=VTHeight;
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTScaleType";VTAttrAID[3].valueAID=VTScaleType;
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTScaleOptions";  VTAttrAID[4].valueAID=VTScaleOptions;
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=2; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTValue";    VTAttrAID[5].valueAID=VTValue;
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=2; VTAttrAID[6].nameAID="VTMacros";   VTAttrAID[6].valueAID=VTMacros;
 VT_AID_Nr=7;
}



//==============================================================================
//type=48
boolean TVTWorkingSetControl::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
  //
  if (pViewRect==NULL) {
    setAID(); setAID_Net(pVT_Net);
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 int16_t objIdx=getVTObjID(pVT_Net,VTObjID); 
  if ((valid) && (objIdx>=0)) {
   getVTObjectFromList(pVT_Net,objIdx); 
   setAID(); setAID_Net(pVT_Net);getAID();
   //not paint set ColorMap
   resetColorMap(pVT_Net);
   SetObjPaintObjToRef(pViewRect,pVT_Net,VTColourMap);
   //
   SetObjPaintObjToRef(pViewRect,pVT_Net,VTColourPalette);
  }//valid 
 return valid;  
}


//------------------------------------------------------------------------------
boolean TVTWorkingSetControl::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTWorkingSetControl::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint8_t  bb=0;
uint32_t ww=0;
 setAID();
 pVT_Net->streamStr.clear();
  if (valid) {
   bb=(VTObjID>>0   & 0xFF); pVT_Net->streamStr.write(bb);
   bb=((VTObjID>>8) & 0xFF); pVT_Net->streamStr.write(bb);
   bb=(VTObjType>>0 & 0xFF); pVT_Net->streamStr.write(bb); 
    //  
    for (i=1;i<VT_AID_Nr;i++) {
      VTAttrAID[i].valueAID="";ww=0; 
        for (j=0;j<VTAttrAID[i].byteAID;j++){
         bb=pStream->read(); pVT_Net->streamStr.write(bb);
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
    //
    for (i=4;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==4) getVTLanguages(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTWorkingSetControl::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTByteNumber  =ww;break;
        case 2:VTColourMap =ww;break;
        case 3:VTColourPalette =ww;break;
        //list
        case 4:VTLanguages =ww;break;
       }//switch i
    }//for i
};

  

//------------------------------------------------------------------------------
void TVTWorkingSetControl::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";      VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTByteNumber";   VTAttrAID[1].valueAID=VTByteNumber;
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=0; VTAttrAID[2].nameAID="VTColourMap";    VTAttrAID[2].valueAID=VTColourMap;
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=0; VTAttrAID[3].nameAID="VTColourPalette";VTAttrAID[3].valueAID=VTColourPalette;
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTLanguages";    VTAttrAID[4].valueAID=VTLanguages;
 VT_AID_Nr=5;
}


//==============================================================================
//==============================================================================
