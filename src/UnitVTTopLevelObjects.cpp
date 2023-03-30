//==============================================================================
//Definition objects 
//TVTWorkingSet, TVTMask, TVTAlarmMask,
// TVTContainer, TVTWindowMaskObject
// TVTAnimationObject
//==============================================================================
//UnitVTTopLevelObjects.cpp
//==============================================================================
#ifndef UnitVTTopLevelObjects_h
  #define UnitVTTopLevelObjects_h
  #include "UnitVTTopLevelObjects.h"
#endif 



//==============================================================================
//==============================================================================
boolean TVTWorkingSet::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t  x=pVT_Net->x, y=pVT_Net->y;
uint16_t w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_KeyHeight,oLevel=pVT_Net->level;
uint8_t  err=0x00,color=0;
int16_t  objIdx=pVT_Net->objNr,refIdx=-1; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=VTMacros;
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
 pVT_Net->FlashList="";
  if ((valid) && (objIdx>=0)) {
    getAID();
    //paint
    color=VTBackgroundColour;
     //
     if (pVT_Net->optn==33){
        pVT_Net->level++;
       Set_fillRect(pVT_Net,x,y,w,h,color);
       //Paint VTObjects
       getVTDrawListAddObj(pVT_Net,true,&vRect);
       valid=getVTObjectsPaint(&vRect,pVT_Net); 
     }else{
        pVT_Net->level=0;
       getVTDrawListAddObj(pVT_Net,true,&vRect);
       Set_fillRect(pVT_Net,x,y,w,h,color);
       valid=PaintActiveMask(VTActiveMask,pViewRect,pVT_Net);
     }
    pVT_Net->level=oLevel;
    getVTObjectFromList(pVT_Net,objIdx); 
  }//valid;
   //
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTWorkingSet::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTWorkingSet::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,nn=0;
uint32_t ww=0;
uint8_t  bb=0;
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
       if (i==4) getVTObjects  (pVT_Net,nn, pStream);
       if (i==5) getVTMacros   (pVT_Net,nn, pStream);
       if (i==6) getVTLanguages(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTWorkingSet::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTBackgroundColour=ww;VTEvent+=1;break;
        case 2:VTSelectable=ww;VTEvent+=1;break;
        case 3:VTActiveMask=ww;VTEvent+=2;break;
        //List
        case 4:VTObjects  =ww;VTEvent+=1;VTEvent+=6*ww;break;
        case 5:VTMacros   =ww;VTEvent+=1;break;
        case 6:VTLanguages=ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};

//------------------------------------------------------------------------------
void TVTWorkingSet::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTBackgroundColour";VTAttrAID[1].valueAID=String(VTBackgroundColour);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=0; VTAttrAID[2].nameAID="VTSelectable";      VTAttrAID[2].valueAID=String(VTSelectable);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=0; VTAttrAID[3].nameAID="VTActiveMask";      VTAttrAID[3].valueAID=String(VTActiveMask);
 //
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTObjects";         VTAttrAID[4].valueAID=VTObjects;
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTMacros";          VTAttrAID[5].valueAID=VTMacros;
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=2; VTAttrAID[6].nameAID="VTLanguages";       VTAttrAID[6].valueAID=VTLanguages;
 VT_AID_Nr=7; 
};


//==============================================================================
//==============================================================================
boolean TVTMask::getSoftKeyMask(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=true;
uint16_t  w= pVT_Net->TFT_KeyWidth, h= pVT_Net->TFT_Height, x = h,y=0;
TVT_ViewRect vRect=getViewRect(pViewRect);
  //
  Set_setViewport(pVT_Net,pViewRect->viewX,pViewRect->viewY,pViewRect->viewW,pViewRect->viewH);
  //
  pVT_Net->VT_ActiveSRC = pVT_Net->VT_DST[pVT_Net->listNr];
  pVT_Net->VT_ActiveMask = VTObjID;
  //
  //Set new softkey select
  pVT_Net->VTChangeSelect=(pVT_Net->VT_ActiveSoftKeyMask!=VTSoftKeyMask);
  //
  pVT_Net->VT_ActiveSoftKeyMask = VTSoftKeyMask;
  //
  PaintActiveSoftKeyMask(VTSoftKeyMask,pViewRect,pVT_Net);
  //
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTMask::PaintObjToMask(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t w=pVT_Net->TFT_Height,h=w, oLevel=pVT_Net->level;
int16_t  x=pVT_Net->x, y=pVT_Net->y;
uint8_t  err=0x00,color=0;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=VTMacros;
       return valid;   
      }
      //
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
  //
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   pVT_Net->VTKeyIndex=255;
   pVT_Net->VTButtonIndex=255;
   pVT_Net->VTInputIndex=255;
   //
   //paint
   pVT_Net->w=w, pVT_Net->h=h;
   vRect.viewW=w;vRect.viewH=h;
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   //
   color=VTBackgroundColour;
   Set_fillRect(pVT_Net,x,y,w,h,color);
   //
   //Paint VTObjects
   valid=getVTObjectsPaint(&vRect,pVT_Net); 
   getSoftKeyMask(pViewRect,pVT_Net);  
   pVT_Net->SetLabel++;
   pVT_Net->x=x;pVT_Net->y=y;
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->SetLabel--;
   pVT_Net->level=oLevel;
  } else {
   pVT_Net->VT_ActiveMask = 0xFFFF;
   pVT_Net->VT_ActiveSoftKeyMask = 0xFFFF;
  }
  //
 return valid;  
};


//==============================================================================
//==============================================================================
boolean TVTDataMask::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
  return PaintObjToMask(pViewRect,pVT_Net);
};

//------------------------------------------------------------------------------
boolean TVTDataMask::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTDataMask::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0,ww=0,nn=0;
uint8_t  bb=0;
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
    for (i=3;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==3) getVTObjects(pVT_Net,nn, pStream);
       if (i==4) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};



//------------------------------------------------------------------------------
void TVTDataMask::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0; 
  VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTBackgroundColour=ww;VTEvent++;break;
        case 2:VTSoftKeyMask=ww;VTEvent+=2;break;
        //List
        case 3:VTObjects  =ww;VTEvent++; VTEvent+=6*ww; break;
        case 4:VTMacros   =ww;VTEvent++;break;
       }//switch i
       //
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTDataMask::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);  
 VTAttrAID[1].numAID=1;VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1;VTAttrAID[1].nameAID="VTBackgroundColour";VTAttrAID[1].valueAID=String(VTBackgroundColour); 
 VTAttrAID[2].numAID=2;VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1;VTAttrAID[2].nameAID="VTSoftKeyMask";     VTAttrAID[2].valueAID=String(VTSoftKeyMask);
 //
 VTAttrAID[3].numAID=3; VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTObjects"; VTAttrAID[3].valueAID=VTObjects;
 VTAttrAID[4].numAID=4; VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTMacros";  VTAttrAID[4].valueAID=VTMacros;
 VT_AID_Nr=5;
};


//==============================================================================
//==============================================================================
boolean TVTAlarmMask::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
 setAID();
 return PaintObjToMask(pViewRect,pVT_Net);
};

//------------------------------------------------------------------------------
boolean TVTAlarmMask::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTAlarmMask::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=5;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==5) getVTObjects(pVT_Net,nn, pStream);
       if (i==6) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
void TVTAlarmMask::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
  VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTBackgroundColour=ww;VTEvent+=1;break;
        case 2:VTSoftKeyMask=ww;VTEvent+=2;break;
        case 3:VTPriority=ww;VTEvent+=1;break;
        case 4:VTAcousticSignal=ww;VTEvent+=1;break;
        //List
        case 5:VTObjects  =ww;VTEvent+=1;VTEvent+=6*ww;break;
        case 6:VTMacros   =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTAlarmMask::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1;VTAttrAID[1].nameAID="VTBackgroundColour";VTAttrAID[1].valueAID=String(VTBackgroundColour); 
 VTAttrAID[2].numAID=2;VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1;VTAttrAID[2].nameAID="VTSoftKeyMask";     VTAttrAID[2].valueAID=String(VTSoftKeyMask);
 VTAttrAID[3].numAID=3;VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1;VTAttrAID[3].nameAID="VTPriority";        VTAttrAID[3].valueAID=String(VTPriority);
 VTAttrAID[4].numAID=4;VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1;VTAttrAID[4].nameAID="VTAcousticSignal";  VTAttrAID[4].valueAID=String(VTAcousticSignal);
 //
 VTAttrAID[5].numAID=5;VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTObjects";        VTAttrAID[5].valueAID=VTObjects;
 VTAttrAID[6].numAID=6;VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=2; VTAttrAID[6].nameAID="VTMacros";         VTAttrAID[6].valueAID=VTMacros;
 VT_AID_Nr=7; 
};


//==============================================================================
//==============================================================================
boolean TVTContainer::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
uint16_t w=0,h=0,oLevel=pVT_Net->level;
int16_t  x=pVT_Net->x, y=pVT_Net->y;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=VTMacros;
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID(); pVT_Net->level++;
    //paint
    if (VTHidden==0) {
      w=VTWidth;  h=VTHeight;
      getViewport(pVT_Net,&vRect,x,y,w,h);
      pVT_Net->w=w;pVT_Net->h=h;
      //
      //bounds test1      //
      //Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
      //VTObjects
      getVTDrawListAddObj(pVT_Net,true,&vRect);
      valid=getVTObjectsPaint(&vRect,pVT_Net); 
    }
   pVT_Net->level=oLevel;
   //getVTObjectFromList(pVT_Net,objIdx); 
  }//valid 
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTContainer::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTContainer::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==4) getVTObjects(pVT_Net,nn, pStream);
       if (i==5) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTContainer::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth =ww;VTEvent+=2;break;
        case 2:VTHeight=ww;VTEvent+=2;break;
        case 3:VTHidden=ww;VTEvent+=1;break;
        //List
        case 4:VTObjects=ww;VTEvent+=1;VTEvent+=6*ww;break;
        case 5:VTMacros =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTContainer::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);  
 VTAttrAID[1].numAID=1;VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=0;VTAttrAID[1].nameAID="VTWidth";  VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=0;VTAttrAID[2].nameAID="VTHeight"; VTAttrAID[2].valueAID=String(VTHeight); 
 VTAttrAID[3].numAID=3;VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=0;VTAttrAID[3].nameAID="VTHidden"; VTAttrAID[3].valueAID=String(VTHidden); 
 //
 VTAttrAID[4].numAID=4;VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTObjects";VTAttrAID[4].valueAID=VTObjects;
 VTAttrAID[5].numAID=5;VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTMacros"; VTAttrAID[5].valueAID=VTMacros;
 VT_AID_Nr=6; 
};


//==============================================================================
//VTObjType=34
//==============================================================================
boolean TVTWindowMaskObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t cIdx=0,err=0x00;
uint16_t colTransp=TFT_WHITE,ww0=30,vW,vH,ww,hh,w,h,oLevel=pVT_Net->level;
int16_t vX0,vY0,vX1,vY1,x0,y0,x1,y1, x=pVT_Net->x, y=pVT_Net->y;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=VTMacros;
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID(); pVT_Net->level++;
   //paint
      w=VTWidth;  h=VTHeight;
      Set_resetViewport(pVT_Net);
      //bounds test
      //Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
      //
      getViewport(pVT_Net,&vRect,x,y,w,h);
      getVTDrawListAddObj(pVT_Net,true,&vRect);
       //       
       //ref VTVariableReference
       if (SetObjPaintObjToRef(&vRect,pVT_Net,VTName)){
        //arcVal=pVT_Net->VTAttrAID[1].valueAID.toInt();
       }
        //ref VTVariableReference
       if (SetObjPaintObjToRef(&vRect,pVT_Net,VTWindowTitle)){
        //arcVal=pVT_Net->VTAttrAID[1].valueAID.toInt();
       }
        //ref VTVariableReference
       if (SetObjPaintObjToRef(&vRect,pVT_Net,VTWindowIcon)){
        //arcVal=pVT_Net->VTAttrAID[1].valueAID.toInt();
       }
      //VTObjects
      valid=getVTObjectsPaint(&vRect,pVT_Net); 
      Set_resetViewport(pVT_Net);
      getVTObjectFromList(pVT_Net,objIdx); 
      pVT_Net->level=oLevel;
  }//valid 
  //
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTWindowMaskObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTWindowMaskObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=9;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==9)  getVTItems(pVT_Net,nn, pStream);
       if (i==10) getVTObjects(pVT_Net,nn, pStream);
       if (i==11) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTWindowMaskObject::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:  VTWidth   =ww;VTEvent+=2;break;
        case 2:VTHeight    =ww;VTEvent+=2;break;
        case 3:VTWindowType=ww;VTEvent+=1;break;
        case 4:VTBackgroundColour=ww;VTEvent+=1;break;
        case 5:VTOptions    =ww;VTEvent+=1;break;
        case 6:VTName       =ww;VTEvent+=2;break;
        case 7:VTWindowTitle=ww;VTEvent+=2;break;
        case 8:VTWindowIcon =ww;VTEvent+=2;break;
        //List
        case 9:VTObjRefs=ww;VTEvent+=2;break;
        case 10:VTObjects=ww;VTEvent+=2;VTEvent+=6*ww;break;
        case 11:VTMacros =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTWindowMaskObject::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);  
 VTAttrAID[1].numAID=4;VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=2;VTAttrAID[1].nameAID="VTWidth";  VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=5;VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=2;VTAttrAID[2].nameAID="VTHeight"; VTAttrAID[2].valueAID=String(VTHeight); 
 VTAttrAID[3].numAID=3;VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2;VTAttrAID[3].nameAID="VTWindowType"; VTAttrAID[3].valueAID=String(VTWindowType); 
 VTAttrAID[4].numAID=1;VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1;VTAttrAID[4].nameAID="VTBackgroundColour"; VTAttrAID[4].valueAID=String(VTBackgroundColour); 
 VTAttrAID[5].numAID=2;VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1;VTAttrAID[5].nameAID="VTOptions"; VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=3;VTAttrAID[6].byteAID=2; VTAttrAID[6].typeAID=1;VTAttrAID[6].nameAID="VTName";    VTAttrAID[6].valueAID=String(VTName);
 VTAttrAID[7].numAID=7;VTAttrAID[7].byteAID=2; VTAttrAID[7].typeAID=2;VTAttrAID[7].nameAID="VTWindowTitle";VTAttrAID[7].valueAID=String(VTWindowTitle);
 VTAttrAID[8].numAID=8;VTAttrAID[8].byteAID=2; VTAttrAID[8].typeAID=2;VTAttrAID[8].nameAID="VTWindowIcon";VTAttrAID[8].valueAID=String(VTWindowIcon);
 //
 VTAttrAID[9].numAID=9;  VTAttrAID[9].byteAID=1; VTAttrAID[9].typeAID=2; VTAttrAID[9].nameAID="VTObjRefs";VTAttrAID[9].valueAID=VTObjRefs;
 VTAttrAID[10].numAID=10;VTAttrAID[10].byteAID=1;VTAttrAID[10].typeAID=2;VTAttrAID[10].nameAID="VTObjects";VTAttrAID[10].valueAID=VTObjects;
 VTAttrAID[11].numAID=10;VTAttrAID[11].byteAID=1;VTAttrAID[11].typeAID=2;VTAttrAID[11].nameAID="VTMacros";VTAttrAID[11].valueAID=VTMacros;
 VT_AID_Nr=12; 
};


//==============================================================================
//VTObjType=44
//==============================================================================
boolean TVTAnimationObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t cIdx=0,err=0x00;
uint16_t colTransp=TFT_WHITE,ww0=30,vW,vH,ww,hh,w,h,oLevel=pVT_Net->level;
int16_t vX0,vY0,vX1,vY1,x0,y0,x1,y1, x=pVT_Net->x, y=pVT_Net->y;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=VTMacros;
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //paint
   w=VTWidth;  h=VTHeight;
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   //bounds test
   //Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
   //VTObjects
   pVT_Net->animationIdx=VTValue;
   valid=getVTObjectsPaint(&vRect,pVT_Net); 
   pVT_Net->animationIdx=-1;
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid 
  //
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTAnimationObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTAnimationObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=10;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==10) getVTObjects(pVT_Net,nn, pStream);
       if (i==11) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTAnimationObject::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth     =ww;VTEvent+=2;break;
        case 2:VTHeight    =ww;VTEvent+=2;break;
        case 3:VTRefreshInterval=ww;VTEvent+=2;break;
        case 4:VTValue=ww;VTEvent+=1;break;
        case 5:VTEnabled    =ww;VTEvent+=1;break;
        case 6:VTFirstChildIndex       =ww;VTEvent+=1;break;
        case 7:VTLastChildIndex=ww;VTEvent+=1;break;
        case 8:VTDefaultChildIndex =ww;VTEvent+=1;break;
        case 9:VTAnimationOptions =ww;VTEvent+=1;break;
        //List
        case 10:VTObjects=ww;VTEvent+=1;VTEvent+=6*ww;break;
        case 11:VTMacros =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};

//------------------------------------------------------------------------------
void TVTAnimationObject::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);  
 VTAttrAID[1].numAID=1;VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1;VTAttrAID[1].nameAID="VTWidth";  VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1;VTAttrAID[2].nameAID="VTHeight"; VTAttrAID[2].valueAID=String(VTHeight); 
 VTAttrAID[3].numAID=3;VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1;VTAttrAID[3].nameAID="VTRefreshInterval"; VTAttrAID[3].valueAID=String(VTRefreshInterval); 
 VTAttrAID[4].numAID=4;VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1;VTAttrAID[4].nameAID="VTValue";   VTAttrAID[4].valueAID=String(VTValue); 
 VTAttrAID[5].numAID=5;VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1;VTAttrAID[5].nameAID="VTEnabled"; VTAttrAID[5].valueAID=String(VTEnabled);
 VTAttrAID[6].numAID=6;VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1;VTAttrAID[6].nameAID="VTFirstChildIndex"; VTAttrAID[6].valueAID=String(VTFirstChildIndex);
 VTAttrAID[7].numAID=7;VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=1;VTAttrAID[7].nameAID="VTLastChildIndex";  VTAttrAID[7].valueAID=String(VTLastChildIndex);
 VTAttrAID[8].numAID=8;VTAttrAID[8].byteAID=1; VTAttrAID[8].typeAID=1;VTAttrAID[8].nameAID="VTDefaultChildIndex";VTAttrAID[8].valueAID=String(VTDefaultChildIndex);
 VTAttrAID[9].numAID=9;VTAttrAID[9].byteAID=1; VTAttrAID[9].typeAID=1;VTAttrAID[9].nameAID="VTAnimationOptions";VTAttrAID[9].valueAID=String(VTAnimationOptions);
 //
 VTAttrAID[10].numAID=10;VTAttrAID[10].byteAID=1;VTAttrAID[10].typeAID=2;VTAttrAID[10].nameAID="VTObjects";VTAttrAID[10].valueAID=VTObjects;
 VTAttrAID[11].numAID=11;VTAttrAID[11].byteAID=1;VTAttrAID[11].typeAID=2;VTAttrAID[11].nameAID="VTMacros";VTAttrAID[11].valueAID=VTMacros;
 VT_AID_Nr=12;
};

//==============================================================================
//==============================================================================
