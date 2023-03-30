//==============================================================================
//Definition objects
//TVTObjectPointer,
//TVTObjectDummy,
//==============================================================================
//unit UnitVTPointerObject.cpp;
//==============================================================================
#ifndef UnitVTPointerObject_h
  #define UnitVTPointerObject_h
  #include "UnitVTPointerObject.h"
#endif 


//==============================================================================
//==============================================================================
//type=27
boolean TVTObjectPointer::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t  err=0x00;
uint16_t oLevel=pVT_Net->level;
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
   getAID(); pVT_Net->level++;
    //paint
    //ref VTPointerReference
    getVTDrawListAddObj(pVT_Net,true,&vRect);
    //
    refIdx=getVTObjID(pVT_Net,VTValue); 
     if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       //runClassObj(pVT_Net,pViewRect,pVT_Net->VTObjType,pVT_Net->VTObjID,NULL);
       SetObjPaintObjToRef(&vRect,pVT_Net,VTValue);
     }
    getVTObjectFromList(pVT_Net,objIdx);
    pVT_Net->level=oLevel; 
  }//valid 
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTObjectPointer::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTObjectPointer::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTObjectPointer::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTValue=ww;break;
       }//switch i
    }//for i
};


//------------------------------------------------------------------------------
void TVTObjectPointer::setAID(){
 VTAttrAID[0].numAID=0;   VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;   VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTValue";  VTAttrAID[1].valueAID=String(VTValue);
 VT_AID_Nr=2;
};


//==============================================================================
//type=232
boolean TVTObjectDummy::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t  err=0x00;
uint16_t oLevel=pVT_Net->level;
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
   getAID(); pVT_Net->level++;
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   
   //paint
    refIdx=getVTObjID(pVT_Net,VTParentObjID); 
     if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       //SetObjPaintObjToRef(&vRect,pVT_Net,VTValue);
     }
     //   
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid 
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTObjectDummy::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTObjectDummy::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTObjectDummy::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTParentObjID=ww;break;
        case 2:VTParentObjType=ww;break;
       }//switch i
    }//for i
};



//------------------------------------------------------------------------------
void TVTObjectDummy::setAID(){
 VTAttrAID[0].numAID=0;   VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";      VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;   VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=2; VTAttrAID[1].nameAID="VTParentObjID";  VTAttrAID[1].valueAID=String(VTParentObjID);
 VTAttrAID[2].numAID=2;   VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTParentObjType";VTAttrAID[2].valueAID=String(VTParentObjType);
 VT_AID_Nr=3;
};


//==============================================================================
//==============================================================================
