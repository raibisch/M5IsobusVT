//==============================================================================
//Definition objects TVTAttributeObject,
//TVTFontAttribute, TVTLineAttribute, TVTFillAttribute, TVTInputAttribute,
//TVTExtendedInputAttribute
//==============================================================================
//unit UnitVTAttributeObjects.cpp
//==============================================================================

#ifndef UnitVTAttributeObjects_h
  #define UnitVTAttributeObjects_h
  #include "UnitVTAttributeObjects.h"
#endif 

//==============================================================================
//==============================================================================
//type=23
boolean TVTFontAttribute::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=String(VTMacros);
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
  if ((valid) && (objIdx>=0)) {
   getAID();
  }//valid
 //not paint
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTFontAttribute::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTFontAttribute::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==5) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTFontAttribute::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
  VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTFontColour=ww;VTEvent+=1;break;
        case 2:VTFontSize =ww;VTEvent+=1;break;
        //
        case 3:VTFontType =ww;VTEvent+=1;break;
        case 4:VTFontStyle=ww;VTEvent+=1;break;
        //list
        case 5:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTFontAttribute::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";   VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTFontColour";VTAttrAID[1].valueAID=String(VTFontColour);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTFontSize";  VTAttrAID[2].valueAID=String(VTFontSize);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTFontType";  VTAttrAID[3].valueAID=String(VTFontType);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTFontStyle"; VTAttrAID[4].valueAID=String(VTFontStyle);
 //
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTMacros";    VTAttrAID[5].valueAID=VTMacros;
 VT_AID_Nr=6;
};


//==============================================================================
//type=24
boolean TVTLineAttribute::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=String(VTMacros);
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
  if ((valid) && (objIdx>=0)) {
   getAID();
  }//valid
 //not paint
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTLineAttribute::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTLineAttribute::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==4) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
void TVTLineAttribute::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
  VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTLineColour=ww;VTEvent+=1;break;
        case 2:VTLineWidth =ww;VTEvent+=1;break;
        case 3:VTLineArt =ww;VTEvent+=2;break;
        //list
        case 4:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTLineAttribute::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";   VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTLineColour";VTAttrAID[1].valueAID=String(VTLineColour);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTLineWidth"; VTAttrAID[2].valueAID=String(VTLineWidth);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTLineArt";   VTAttrAID[3].valueAID=String(VTLineArt);
 //
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTMacros";    VTAttrAID[4].valueAID=VTMacros;
 VT_AID_Nr=5;
};


//==============================================================================
//type=25
boolean TVTFillAttribute::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=String(VTMacros);
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
  if ((valid) && (objIdx>=0)) {
   getAID();
  }//valid
 //not paint
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTFillAttribute::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTFillAttribute::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==4) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTFillAttribute::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTFillType=ww;VTEvent+=1;break;
        case 2:VTFillColour =ww;VTEvent+=1;break;
        case 3:VTFillPattern =ww;VTEvent+=2;break;
        //list
        case 4:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};

//------------------------------------------------------------------------------
void TVTFillAttribute::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";    VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTFillType";   VTAttrAID[1].valueAID=String(VTFillType);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTFillColour"; VTAttrAID[2].valueAID=String(VTFillColour);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTFillPattern";VTAttrAID[3].valueAID=String(VTFillPattern);
 // 
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=2; VTAttrAID[4].nameAID="VTMacros";     VTAttrAID[4].valueAID=VTMacros;
 VT_AID_Nr=5;
};


//==============================================================================
//type=26
boolean TVTInputAttribute::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t err=0x00;
int16_t objIdx=pVT_Net->objNr; 
 setAID(); setAID_Net(pVT_Net);
  if (pViewRect==NULL) {
      if (pVT_Net->nameAttr=="VTMacros"){
       getAID(); pVT_Net->newValueAttr=String(VTEvent);
       pVT_Net->nameAttr=String(VTMacros);
       return valid;   
      }
      if (pVT_Net->nameAttr!="") err=SetVTObjectAttributeDirect(pVT_Net->nameAttr, pVT_Net->newValueAttr,pVT_Net);
    pVT_Net->nameAttr="";
    valid=((valid) && (err==0x00));
    return valid; 
  }
  //
  if ((valid) && (objIdx>=0)) {
   getAID();
  }//valid
 //not paint
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTInputAttribute::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTInputAttribute::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
        if (i==2) getVTString(pVT_Net,nn,pStream);
    }//for i
    //
    for (i=3;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==3) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
void TVTInputAttribute::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTValidationType=ww;VTEvent+=1;break;
        case 2:VTLength =ww;VTEvent+=1;VTEvent+=ww;break;
        //list
        case 3:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTInputAttribute::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";       VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTValidationType";VTAttrAID[1].valueAID=String(VTObjType);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTLength";        VTAttrAID[2].valueAID=String(VTValidationString);
 //
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=2; VTAttrAID[3].nameAID="VTMacros";        VTAttrAID[3].valueAID=VTMacros;
 VT_AID_Nr=4;
};


//==============================================================================
//type=38
boolean TVTExtendedInputAttribute::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
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
  }//valid
 //not paint
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTExtendedInputAttribute::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTExtendedInputAttribute::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==2) getVTCodePlanes(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
void TVTExtendedInputAttribute::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTValidationType=ww;break;
        //list
        case 2:VTCodePlanes=ww;break; 
       }//switch i
    }//for i
};

//------------------------------------------------------------------------------
void TVTExtendedInputAttribute::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";       VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=0; VTAttrAID[1].nameAID="VTValidationType";VTAttrAID[1].valueAID=String(VTValidationType);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=2; VTAttrAID[2].nameAID="VTCodePlanes";    VTAttrAID[2].valueAID=VTCodePlanes;
 VT_AID_Nr=3;
};


//==============================================================================
//==============================================================================
