//==============================================================================
//Definition objects
//TVTExternalObject, TVTExternalReferenceName,
//TVTExternalObjectDefinition, TVTExternalObjectPointer
//==============================================================================
//unit UnitVTExternalObject.cpp;
//==============================================================================

#ifndef UnitVTExternalObject_h
  #define UnitVTExternalObject_h
  #include "UnitVTExternalObject.h"
#endif 


//==============================================================================
//==============================================================================
//type=41 and 42
boolean TVTExternalObject::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t  err=0x00;
uint16_t oLevel=pVT_Net->level;
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
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //
   pVT_Net->level=oLevel;
  
  }//valid
  //not paint
 return valid;  
}


//------------------------------------------------------------------------------
boolean TVTExternalObject::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTExternalObject::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    if (VT_AID_Nr>4) {
      for (i=4;i<VT_AID_Nr;i++) {
        nn=VTAttrAID[i].valueAID.toInt(); 
         if (i==4) getVTItems(pVT_Net,nn, pStream);
      }//for i
    }
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTExternalObject::getAID(){
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
        case 2:VTName0=ww;break;
        case 3:VTName1=ww;break;
        //list
        case 4:VTItems=ww;break; 
       }//switch i
    }//for i
};

  

//------------------------------------------------------------------------------
void TVTExternalObject::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType"; VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTOptions"; VTAttrAID[1].valueAID=String(VTOptions);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=4; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTName0";   VTAttrAID[2].valueAID=String(VTName0);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=4; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTName1";   VTAttrAID[3].valueAID=String(VTName1);
 VT_AID_Nr=4;
   // 
   if (VTObjType==41){
     VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTItems";   VTAttrAID[4].valueAID=VTItems;
     VT_AID_Nr=5;
   }  
}


//==============================================================================
//type=43
boolean TVTExternalObjectPointer::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
uint8_t  listNr=pVT_Net->listNr;
boolean  valid=(getVTObjectListSize(pVT_Net)>0);
uint16_t oLevel=pVT_Net->level;
uint8_t  err=0x00;
int16_t  objIdx=pVT_Net->objNr,refIdx=-1; 
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
   //
   getVTObjID(pVT_Net,VTExternalObjectRef);
   //if list=0 check list 1 or list=1 check list 0
   pVT_Net->listNr=(pVT_Net->listNr+1) % 2;
   refIdx=getVTObjID(pVT_Net,VTExternalObjectRef); 
     if (refIdx>=0) {
       SetObjPaintObjToRef(&vRect,pVT_Net,VTExternalObjectRef);
      //TODO  
      pVT_Net->listNr=listNr; 
      //
     }else {
       pVT_Net->listNr=listNr;
       //ref VTDefaultObjectRef
       //Set_fillRect(pVT_Net,pVT_Net->x,pVT_Net->y,20,20,TFT_BLUE);
       SetObjPaintObjToRef(&vRect,pVT_Net,VTDefaultObjectRef);
     }
     //   
   pVT_Net->listNr=listNr;
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid
  //
 return valid;  
}


//------------------------------------------------------------------------------
boolean TVTExternalObjectPointer::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTExternalObjectPointer::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTExternalObjectPointer::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTDefaultObjectRef =ww;break;
        case 2:VTExternalNameRef=ww;break;
        case 3:VTExternalObjectRef=ww;break;
        //list
        case 4:VTValidationBitmask=ww;break; 
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTExternalObjectPointer::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";          VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTDefaultObjectRef"; VTAttrAID[1].valueAID=String(VTDefaultObjectRef);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTExternalNameRef";  VTAttrAID[2].valueAID=String(VTExternalNameRef);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTExternalObjectRef";VTAttrAID[3].valueAID=String(VTExternalObjectRef);
 VT_AID_Nr=4;
}

//==============================================================================
//==============================================================================
