//==============================================================================
//Definition objects TVTAuxiliary, TVTAuxiliaryFuncInp,
//TVTAuxiliaryFunction, TVTAuxiliaryInput,
//TVTAuxFunction2,TVTAuxInput2,
//TVTAuxObject, TVTAuxDesignPointer
//==============================================================================
//unit UnitVTAuxiliaryObjects.cpp
//==============================================================================
#ifndef UnitVTAuxiliaryObjects_h
  #define UnitVTAuxiliaryObjects_h
  #include "UnitVTAuxiliaryObjects.h"
#endif 


//==============================================================================
//==============================================================================
boolean TVTAuxiliaryFuncInp::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_KeyHeight,oLevel=pVT_Net->level;
int16_t x=pVT_Net->x, y=pVT_Net->y;
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
 TVT_ViewRect vvRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //Paint
   getViewport(pVT_Net,&vvRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vvRect);

   Set_fillRect(pVT_Net,x,y,w,h,VTBackgroundColour);
   //VTObjects 
   valid=getVTObjectsPaint(&vvRect,pVT_Net);
   //
   getViewport(pVT_Net,&vRect,x,y,w,h);
   SetSelectState(pVT_Net,true,&vRect);
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTAuxiliaryFuncInp::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTAuxiliaryFuncInp::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
   getVTObjects(pVT_Net,nn, pStream);
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
void TVTAuxiliaryFuncInp::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTBackgroundColour =ww;break;
        case 2:VTFuncTyp=ww;break;
        case 3: if (VTObjType==29) VTObjects=ww;
                if (VTObjType==30) VTInputID=ww;
                if (VTObjType==31) {
                  //VTCriticalControl=((VTFuncTyp>>5) & 0x01)>0); //Bit 5
                  //VTLockAssign     =((VTFuncTyp>>6) & 0x01)>0); //Bit 6
                  //VTSingleAssign   =((VTFuncTyp>>7) & 0x01)>0);//Bit 7
                  //List VTObjects
                  VTObjects=ww;
                }
               break;
        //list
        case 4: if (VTObjType==30) VTObjects=ww;
               break; 
       }//switch i
    }//for i
};

  

//------------------------------------------------------------------------------
void TVTAuxiliaryFuncInp::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=2; VTAttrAID[1].nameAID="VTBackgroundColour";VTAttrAID[1].valueAID=String(VTBackgroundColour);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTFuncTyp";VTAttrAID[2].valueAID=String(VTFuncTyp);
   //AuxFunction 1
   if (VTObjType==29){
     VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTObjects"; VTAttrAID[3].valueAID=VTObjects;
     VT_AID_Nr=4;
   }
   //AuxInput 1
   if (VTObjType==30){
     VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTInputID";VTAttrAID[3].valueAID=String(VTInputID);
     VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTObjects"; VTAttrAID[4].valueAID=VTObjects;
     VT_AID_Nr=5;
   }
   //AuxFunction 2
   if (VTObjType==31){
     VTAttrAID[1].typeAID=1; VTAttrAID[2].typeAID=0;
     VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTObjects"; VTAttrAID[3].valueAID=VTObjects;
     VT_AID_Nr=4;
   }
   //AuxInput 2
   if (VTObjType==32){
     VTAttrAID[1].typeAID=1;   VTAttrAID[2].typeAID=0;
     VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTObjects"; VTAttrAID[3].valueAID=VTObjects;
     VT_AID_Nr=4;
   }
 //
};


//==============================================================================
void TVTAuxiliaryFunction2::getAID2(){
  getAID();
  VTCriticalControl=(((VTFuncTyp>>5) & 0x01)>0); //Bit 5
  VTLockAssign     =(((VTFuncTyp>>6) & 0x01)>0); //Bit 6
  VTSingleAssign   =(((VTFuncTyp>>7) & 0x01)>0); //Bit 7
};

//==============================================================================
void TVTAuxiliaryInput2::getAID2(){
  getAID();
  VTCriticalControl=(((VTFuncTyp>>5) & 0x01)>0); //Bit 5
  //VTLockAssign     =(((VTFuncTyp>>6) & 0x01)>0); //Bit 6 not for auxinput
  VTSingleAssign   =(((VTFuncTyp>>7) & 0x01)>0); //Bit 7
};


//==============================================================================
boolean TVTAuxObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean  valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_KeyHeight,oLevel=pVT_Net->level;
int16_t  x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
uint8_t  err=0x00;
int16_t  objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
 TVT_ViewRect vRect=getViewRect(pViewRect);
 TVT_ViewRect vvRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
    getAID();pVT_Net->level++;
    //Paint
    getViewport(pVT_Net,&vvRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,true,&vvRect);
     //
     if ((VTPointerTyp==1) || (VTPointerTyp==3)){
       //Serial.println(VTValue);
       //Serial.println(VTPointerTyp);
       //ref VTVariableReference external TODO
       //SetObjPaintObjToRef(&vvRect,pVT_Net,VTValue);
       Set_fillRect(pVT_Net,x+1,y+1,10,10,TFT_BLUE);    
     }
     if (VTPointerTyp==0){
      SetObjPaintObjToRef(&vvRect,pVT_Net,VTValue);
     }
     if (VTPointerTyp==2){
      pVT_Net->optn=33;
      SetObjPaintObjToRef(&vvRect,pVT_Net,0,true);
     }
     //  
    pVT_Net->optn=0;
    getViewport(pVT_Net,&vRect,x,y,w,h);
    SetSelectState(pVT_Net,true,&vRect);
    getVTObjectFromList(pVT_Net,objIdx); 
    pVT_Net->level=oLevel;
  }//valid 
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTAuxObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTAuxObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTAuxObject::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTPointerTyp =ww;break;
        case 2:VTValue=ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTAuxObject::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTPointerTyp";VTAttrAID[1].valueAID=String(VTPointerTyp);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTValue";VTAttrAID[2].valueAID=String(VTValue);
 VT_AID_Nr=3;
};


//==============================================================================
//==============================================================================
