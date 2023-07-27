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
boolean  valid=(getVTObjectListSize(pVT_Net)>0),assign=false,TEST=false;
uint16_t w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_KeyHeight,oLevel=pVT_Net->level,objID=0xFFFF;
int16_t  objIdx=pVT_Net->objNr, x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
uint8_t  err=0x00,oList=pVT_Net->listNr;
int8_t   WS_auxFunc_listNr=-1,WS_auxInp_listNr=-1;
String str="",WS_auxFunc="",WS_auxInp="",auxFunc="",auxInp="",auxValue="";
 //
 setAID(); setAID_Net(pVT_Net);
  //
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
      if (pVT_Net->VTInputIndex==255) pVT_Net->VTInputIndex=0;
    getVTDrawListAddObj(pVT_Net,VTEnabled,&vvRect);
     //
     if ((VTPointerTyp==1) || (VTPointerTyp==3)){
       if (TEST){ 
        Serial.println("VTValue=" + String(VTValue));
        Serial.println("VTPointerTyp=" + String(VTPointerTyp));
        Serial.println("pVT_Net->listNr=" + String(pVT_Net->listNr));
        Serial.println(delm0);
       }
     }
     //
     if (VTPointerTyp==0){
       if (SetObjPaintObjToRef(&vvRect,pVT_Net,VTValue)) refIdx=0;
     }
     if (VTPointerTyp==2){
      pVT_Net->optn=33;
       if (SetObjPaintObjToRef(&vvRect,pVT_Net,0,true)) refIdx=0;
     }
     //
     //get Assignment auxObj or auxWS
     if ((pVT_Net->VTAuxAssignList.length()>40) && ((VTPointerTyp==1) || (VTPointerTyp==3))){
     //if (VTPointerTyp==1){
      refIdx=getVTObjID(pVT_Net,VTValue,false,true);
      //Serial.println("refID=" + String(refIdx));
        //        
        if (refIdx>=0){
          //
          if (HasInArray(pVT_Net->VTObjType,auxAllObjSet2)){
           auxValue=getStringHEX(VTValue,4);
           str=pVT_Net->VTAuxAssignList;
            //
            if (TEST){
             Serial.println("refID=" + String(refIdx));
             Serial.println("refAux=" + String(pVT_Net->VTObjType));
             getStreamStrInfo(pVT_Net);            
             Serial.println(str);
             Serial.println(auxValue);
            }
            //
            while (str.length()>=40){
             WS_auxFunc=str.substring(0,16); 
             auxFunc=str.substring(16,20); 
             WS_auxFunc_listNr=getWSlistNrFromName(pVT_Net,WS_auxFunc);
             //
             WS_auxInp=str.substring(20,36); 
             auxInp=str.substring(36,40); 
             WS_auxInp_listNr=getWSlistNrFromName(pVT_Net,WS_auxInp);
               //
               if (TEST){
                Serial.println("WS_auxFunc=" + WS_auxFunc);
                Serial.println("auxFunc="    + auxFunc);
                Serial.println("WS_auxFunc_listNr=" + String(WS_auxFunc_listNr));
                //
                Serial.println("WS_auxInp="  + WS_auxInp);
                Serial.println("auxInp="     + auxInp);
                Serial.println("WS_auxInp_listNr=" + String(WS_auxInp_listNr));
               }
               //
               //FOUND AUX_ASSIGN
               //found auxInp assignment for auxFunc
               if ((pVT_Net->VTObjType==gAuxFuncType) && (auxFunc==auxValue)) {
                Serial.println("FOUND auxFunc=" + auxFunc);
                objID=hexCharacterToObjID(auxInp);
                Serial.println("WS_auxInp_listNr=" + String(WS_auxInp_listNr));
                Serial.println("auxInp_objID=" + String(objID));
                  //
                  if ((WS_auxInp_listNr<2) && (oList!=WS_auxInp_listNr)) {
                    pVT_Net->listNr=(pVT_Net->listNr+1) % 2;
                  }
                  //
                  if (VTPointerTyp==3){
                   objID=0xFFFF;
                    if (getVTObjID(pVT_Net,0,true,true)>=0) {
                      pVT_Net->optn=33;
                      objID=pVT_Net->VTObjID; 
                      Serial.println("auxInp_WS_objID=" + String(objID));
                    }
                  }
                  // 
                Serial.println("pVT_Net->listNr=" + String(pVT_Net->listNr));
                assign=SetObjPaintObjToRef(&vvRect,pVT_Net,objID);
                break; //while
               }//gAuxFuncType
               //
               //found auxFunc assignment for auxInp
               if ((pVT_Net->VTObjType==gAuxInpType) && (auxInp==auxValue)) {
                Serial.println("FOUND auxInp=" + auxInp);
                objID=hexCharacterToObjID(auxFunc);
                Serial.println("WS_auxFunc_listNr=" + String(WS_auxFunc_listNr));
                Serial.println("auxFunc_objID=" + String(objID));
                  //
                  if ((WS_auxFunc_listNr<2) && (oList!=WS_auxFunc_listNr)) {
                    pVT_Net->listNr=(pVT_Net->listNr+1) % 2;
                  }
                  //
                  if (VTPointerTyp==3){
                   objID=0xFFFF;
                    if (getVTObjID(pVT_Net,0,true,true)>=0) {
                      pVT_Net->optn=33;
                      objID=pVT_Net->VTObjID; 
                      Serial.println("auxFunc_WS_objID=" + String(objID));
                    }
                  }
                  // 
                Serial.println("pVT_Net->listNr=" + String(pVT_Net->listNr));
                assign=SetObjPaintObjToRef(&vvRect,pVT_Net,objID);
                break; //while
               }//gAuxInpType
               //
               //
             str.remove(0,41);  
            }//while
            //
           pVT_Net->optn=0;
           pVT_Net->listNr=oList;
           Serial.println(delm0);
          }//refAux
          //         
          if (!assign) refIdx=-1;
        }//>40
     }//VTPointerTyp==1 or VTPointerTyp==3 
     //
     //
     if (refIdx>=0){
      //Serial.println(delm1);
     } else {
      Set_fillRect(pVT_Net,x+1,y+1,10,10,TFT_BLUE);    
     }
     //  
    //Serial.println(delm1);
      
    pVT_Net->optn=0;
    getViewport(pVT_Net,&vRect,x,y,w,h);
    SetSelectState(pVT_Net,true,&vRect);
    getVTObjectFromList(pVT_Net,objIdx); 
      if (VTEnabled) pVT_Net->VTInputIndex++;
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
