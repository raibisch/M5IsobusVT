//==============================================================================
//Definition objects TVTSoftkeyMask, TVTKey, TVTButton,
//TVTKeyGroupObject,TVTLabelReferenceListObject
//==============================================================================
//unit UnitVTKeyObjects.cpp;
//==============================================================================

#ifndef UnitVTKeyObjects_h
  #define UnitVTKeyObjects_h
  #include "UnitVTKeyObjects.h"
#endif 

//==============================================================================
//==============================================================================
boolean TVTSoftKeyMask::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_Height,oLevel=pVT_Net->level;
int16_t x=pVT_Net->x, y=pVT_Net->y;
uint8_t err=0x00;
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
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //Paint
   pViewRect->viewX = pVT_Net->TFT_Height;
   pViewRect->viewY = 0;
   pViewRect->viewW = pVT_Net->TFT_KeyWidth;
   pViewRect->viewH = pVT_Net->TFT_Height;
   //
   pVT_Net->x = pViewRect->viewX, pVT_Net->y = pViewRect->viewY;
   x=pVT_Net->x, y=pVT_Net->y;
   TVT_ViewRect vRect=getViewRect(pViewRect);
   //
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   //
   Set_fillRect(pVT_Net,x,y,w,h,VTBackgroundColour);
   //
   //VTItemObjects Keys Paint VTObjects 
   valid=getVTObjectsPaint(&vRect,pVT_Net); 
   pVT_Net->VTKeyIndex=0xFF;
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTSoftKeyMask::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTSoftKeyMask::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==2) getVTItems(pVT_Net,nn, pStream);
       if (i==3) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTSoftKeyMask::getAID(){
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
        //List
        case 2:VTItems    =ww;VTEvent++;VTEvent+=2*ww; break;
        case 3:VTMacros   =ww;VTEvent++;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0; 
};


//------------------------------------------------------------------------------
void TVTSoftKeyMask::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTBackgroundColour";VTAttrAID[1].valueAID=String(VTBackgroundColour);
 //
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTItemList"; VTAttrAID[2].valueAID=VTItems;
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTMacros";   VTAttrAID[3].valueAID=VTMacros;
 VT_AID_Nr=4;
};



//==============================================================================
//==============================================================================
boolean TVTKey::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_KeyHeight,oLevel=pVT_Net->level;
int16_t x=pVT_Net->x, y=pVT_Net->y;
uint8_t err=0x00;
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
TVT_ViewRect vvRect=vRect; //getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
      if (pVT_Net->VTKeyIndex==255) pVT_Net->VTKeyIndex=0;
   //Paint
   getViewport(pVT_Net,&vvRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vvRect);
   //   
   Set_fillRect(pVT_Net,x,y,w,h,VTBackgroundColour);
   //VTObjects 
   valid=getVTObjectsPaint(&vvRect,pVT_Net);
   //
   pVT_Net->SetLabel++;
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->SetLabel--;
   getViewport(pVT_Net,&vRect,x,y,w,h);
   Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
   //
   SetSelectState(pVT_Net,true,&vRect);
     //
   pVT_Net->VTKeyIndex++;
   pVT_Net->level=oLevel;
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTKey::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTKey::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTKey::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTBackgroundColour=ww; VTEvent++;break;
        case 2:VTKeyCode=ww;VTEvent++;break;
        //List
        case 3:VTObjects  =ww;VTEvent++; VTEvent+=6*ww;  break;
        case 4:VTMacros   =ww;VTEvent++;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTKey::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTBackgroundColour";VTAttrAID[1].valueAID=String(VTBackgroundColour);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTKeyCode";VTAttrAID[2].valueAID=String(VTKeyCode);
 //
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTObjects"; VTAttrAID[3].valueAID=VTObjects;
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTMacros";  VTAttrAID[4].valueAID=VTMacros;
 VT_AID_Nr=5;
};


//==============================================================================
//==============================================================================
boolean TVTButton::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t  x=pVT_Net->x,y=pVT_Net->y;
uint16_t oLevel=pVT_Net->level;
uint8_t  dd=4,err=0x00;
int16_t  sel=-1;
String attrStr="";
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=VTMacros;
       return valid;   
      }
      if (pVT_Net->nameAttr!="") {
         //check button set disabled VTOptions
         if  (pVT_Net->nameAttr=="VTEnabled") {
          getAID();
          pVT_Net->nameAttr="VTOptions";
          bitWrite8(VTOptions,4,!char(pVT_Net->newValueAttr[0])); //set bit 4
          pVT_Net->newValueAttr=char(VTOptions);
         }//VTEnabled
        err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
      }
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
 TVT_ViewRect vvRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
      if (pVT_Net->VTButtonIndex==255) pVT_Net->VTButtonIndex=0;
   //Paint
   uint16_t w=VTWidth,h=VTHeight,ww=w,hh=h;
   int16_t  xx=x,yy=y;
   //   
   getViewport(pVT_Net,&vRect,x,y,w,h);
     //
     if ((VTObjID==pVT_Net->TFT_ButtonSelectObjID) && (pVT_Net->TFT_ButtonSelect<255)){
       //check latchable
       if ((VTOptions & 0x01)==0x01) {
          if (pVT_Net->TFT_ButtonPressed==1) VTOptions=(VTOptions | 0x02);
          if (pVT_Net->TFT_ButtonPressed==0) VTOptions=(VTOptions & 0xFD);
         //TEST
         //Serial.println("VTOptions=" + String(VTOptions));
         if (pVT_Net->TFT_ButtonPressed>=0){
          pVT_Net->nameAttr="VTOptions";  pVT_Net->newValueAttr=char(VTOptions);
          SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
          pVT_Net->nameAttr="";pVT_Net->newValueAttr=""; 
         }
       }
      pVT_Net->TFT_ButtonPressed=-1;
     }
     //
   //Latchable/LatchState
   uint8_t la=(VTOptions>>0) & 0x01; //latchable
   uint8_t ls=(VTOptions>>1) & 0x01; //latch state
   uint8_t su=(VTOptions>>2) & 0x01; //supress Border
   uint8_t tr=(VTOptions>>3) & 0x01; //transparent
   uint8_t ds=(VTOptions>>4) & 0x01; //disabled
   uint8_t nb=(VTOptions>>5) & 0x01; //noBorder
   getVTDrawListAddObj(pVT_Net,!ds,&vRect);
    //
    if (nb==0) {
     ww=w-2*dd;hh=h-2*dd;
      if ((la==1) && (ls==1)){
        if (su==0){
          Set_fillRect(pVT_Net,x,y,w,dd,VTBorderColour);
          Set_fillRect(pVT_Net,x+w-dd,y,dd,h,VTBorderColour);
          Set_fillRect(pVT_Net,x,y+h-dd,w,dd,VTBorderColour);
          Set_fillRect(pVT_Net,x,y,dd,h,VTBorderColour);
        }
       xx=x+dd; yy=y+dd;    
      } else {
        if (su==0){
          Set_fillRect(pVT_Net,x+w-2*dd,y+dd,2*dd,h-dd,VTBorderColour);
          Set_fillRect(pVT_Net,x+dd,y+h-2*dd,w-dd,2*dd,VTBorderColour);
        }
      }
    }
    //
    if (tr==0) Set_fillRect(pVT_Net,xx,yy,ww,hh,VTBackgroundColour);
   Set_drawRect(pVT_Net,xx,yy,ww,hh,0);
    if ((nb==1) && (la==1) && (ls==1)) Set_drawRect(pVT_Net,xx+1,yy+1,ww-2,hh-2,0); 
    //
   getViewport(pVT_Net,&vRect,x,y,w,h);
   SetSelectState(pVT_Net,(ds==0),&vRect);
   //
   getViewport(pVT_Net,&vvRect,xx,yy,ww,hh);
   //VTObjects
   valid=getVTObjectsPaint(&vvRect,pVT_Net); 
   getVTObjectFromList(pVT_Net,objIdx); 
     //
     //check enabled
     if (!ds) pVT_Net->VTButtonIndex++;
   pVT_Net->level=oLevel;
 }//valid
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTButton::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTButton::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=7;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==7) getVTObjects(pVT_Net,nn, pStream);
       if (i==8) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTButton::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth=ww;VTEvent+=2;break;
        case 2:VTHeight=ww;VTEvent+=2;break;
        case 3:VTBackgroundColour=ww;VTEvent+=1;break;
        case 4:VTBorderColour=ww;VTEvent+=1;break;
        case 5:VTKeyCode=ww;VTEvent+=1;break;
        case 6:VTOptions=ww;VTEvent+=1;break;
        //List
        case 7:VTObjects  =ww;VTEvent++; VTEvent+=6*ww;break;
        case 8:VTMacros   =ww;VTEvent++;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTButton::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";  VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight"; VTAttrAID[2].valueAID=String(VTHeight); 
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTBackgroundColour";VTAttrAID[3].valueAID=String(VTBackgroundColour);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTBorderColour";VTAttrAID[4].valueAID=String(VTBorderColour);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTKeyCode";VTAttrAID[5].valueAID=String(VTKeyCode);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTOptions";VTAttrAID[6].valueAID=String(VTOptions);
 //
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=2; VTAttrAID[7].nameAID="VTObjects"; VTAttrAID[7].valueAID=VTObjects;
 VTAttrAID[8].numAID=8;  VTAttrAID[8].byteAID=1; VTAttrAID[8].typeAID=2; VTAttrAID[8].nameAID="VTMacros";  VTAttrAID[8].valueAID=VTMacros;
 VT_AID_Nr=9;
};



//==============================================================================
//VTObjType=35
//==============================================================================
boolean TVTKeyGroupObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
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
   //Paint
   //w=VTWidth;  h=VTHeight;
   Set_resetViewport(pVT_Net);
   //bounds test
   //Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
   //
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   
        //ref VTVariableReference
       if (SetObjPaintObjToRef(&vRect,pVT_Net,VTName)){
        //arcVal=pVT_Net->VTAttrAID[1].valueAID.toInt();
       }
        //ref VTVariableReference
       if (SetObjPaintObjToRef(&vRect,pVT_Net,VTKeyGroupIcon)){
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
}

//------------------------------------------------------------------------------
boolean TVTKeyGroupObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTKeyGroupObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==4) getVTItems(pVT_Net,nn, pStream);
       if (i==5) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
}


//------------------------------------------------------------------------------
void TVTKeyGroupObject::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTOptions=ww;VTEvent++;break;
        case 2:VTName=ww;VTEvent+=2;VTEvent+=ww;break;
        case 3:VTKeyGroupIcon=ww;VTEvent+=2;break;
        //List
        case 4:VTItems    =ww;VTEvent++; VTEvent+=2*ww;break;
        case 5:VTMacros   =ww;VTEvent++;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTKeyGroupObject::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";     VTAttrAID[0].valueAID=String(VTObjType);  
 VTAttrAID[1].numAID=1;VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1;VTAttrAID[1].nameAID="VTOptions";     VTAttrAID[1].valueAID=String(VTOptions);
 VTAttrAID[2].numAID=2;VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1;VTAttrAID[2].nameAID="VTName";        VTAttrAID[2].valueAID=String(VTName);
 VTAttrAID[3].numAID=3;VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=2;VTAttrAID[3].nameAID="VTKeyGroupIcon";VTAttrAID[3].valueAID=String(VTKeyGroupIcon);
 //
 VTAttrAID[4].numAID=4;VTAttrAID[4].byteAID=1;VTAttrAID[4].typeAID=2;VTAttrAID[4].nameAID="VTItemList";VTAttrAID[4].valueAID=VTItems;
 VTAttrAID[5].numAID=5;VTAttrAID[5].byteAID=1;VTAttrAID[5].typeAID=2;VTAttrAID[5].nameAID="VTMacros";  VTAttrAID[5].valueAID=VTMacros;
 VT_AID_Nr=6; 
}


//==============================================================================
//VTObjType=40
//==============================================================================
boolean TVTLabelReferenceListObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t cIdx=0,err=0x00;
uint16_t colTransp=TFT_WHITE,ww0=30,vW,vH,ww,hh,w,h,oLevel=pVT_Net->level;
int16_t vX0,vY0,vX1,vY1,x0,y0,x1,y1, x=pVT_Net->x, y=pVT_Net->y;
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
   getAID();pVT_Net->level++;
   //Paint
  /*
  //
  if (valid){
   setAID();
   //Paint
    //Paint VTObjects
      //w=VTWidth;  h=VTHeight;
      Set_resetViewport(pVT_Net);
      //bounds test
      //Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
      //
      getViewport(pVT_Net,&vRect,x,y,w,h);
      //VTObjects
      valid=getVTObjectsPaint(&vRect,pVT_Net); 
      Set_resetViewport(pVT_Net);
   */
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  } //valid
 return valid;  
}

//------------------------------------------------------------------------------
boolean TVTLabelReferenceListObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTLabelReferenceListObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
         bb=pStream->read(); 
         pVT_Net->streamStr.write(bb);
         
         ww+=(bb<<(j*8)); 
        }//for j
       VTAttrAID[i].valueAID=String(ww);
    }//for i
    //
    for (i=1;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==1) getVTLabels(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
}


//------------------------------------------------------------------------------
void TVTLabelReferenceListObject::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTLabels   =ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTLabelReferenceListObject::setAID(){
 VTAttrAID[0].numAID=0;VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0;VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);  
 //
 VTAttrAID[1].numAID=1;VTAttrAID[1].byteAID=2;VTAttrAID[1].typeAID=0;VTAttrAID[1].nameAID="VTLabels";VTAttrAID[1].valueAID=VTLabels;
 VT_AID_Nr=2; 
}

//==============================================================================
//==============================================================================
