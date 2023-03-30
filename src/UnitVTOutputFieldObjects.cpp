//==============================================================================
//Definition objects TVTFieldObject,
//TVTOutputStringField, TVTOutputNumberField, TVTOutputListField
//==============================================================================
//unit UnitVTOutputFieldObjects.cpp
//==============================================================================
#ifndef UnitVTOutputFieldObjects_h
  #define UnitVTOutputFieldObjects_h
  #include "UnitVTOutputFieldObjects.h"
#endif 


//==============================================================================
//==============================================================================
//type 11
boolean TVTOutputStringField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x,y=pVT_Net->y,refIdx=-1;
uint16_t color=0,w=0,h=0,oLevel=pVT_Net->level;
uint8_t  opt=0;
String str="";
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
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
    getAID();pVT_Net->level++;
    VTValue=getVTObjectAttributeString("VTLength",pVT_Net);
    //paint
    w=VTWidth, h=VTHeight; opt=VTOptions;str=VTValue;
    //   
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,true,&vRect);
    //ref Variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
        getVTObjectFromList(pVT_Net,refIdx); 
        str=getVTObjectAttributeString("VTLength",pVT_Net);
        getVTObjectFromList(pVT_Net,objIdx); 
      }
     //
    //get ref Font  
    pVT_Net->w=w;  pVT_Net->h=h;pVT_Net->fntJust=VTJustification;
    setVTObjectText(str,&vRect,pVT_Net,VTFontAttributes);
    pVT_Net->SetLabel++;
    getVTObjectFromList(pVT_Net,objIdx); 
    pVT_Net->SetLabel--;
    pVT_Net->level=oLevel;  
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTOutputStringField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTOutputStringField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
        if (i==8) getVTString(pVT_Net,nn, pStream);
    }//for i
    //
    for (i=9;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==9) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTOutputStringField::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0,len=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0; len=VTAttrAID[i].valueAID.length();
       for (j=0;j<len;j++){
         ww+=char(VTAttrAID[i].valueAID[j])<<8*j; 
       }//for j
       switch (i) {
        case 1:VTWidth    =ww;VTEvent+=2;break;
        case 2:VTHeight   =ww;VTEvent+=2;break;
        case 3:VTBackgroundColour =ww;VTEvent+=1;break;
        case 4:VTFontAttributes =ww;VTEvent+=2;break;
        case 5:VTOptions=ww;VTEvent+=1;break;
        case 6:VTVariableReference=ww;VTEvent+=2;break;
        case 7:VTJustification=ww;VTEvent+=1;break;
        case 8:VTLength=ww; VTEvent+=2;VTEvent+=ww;break;
        //List
        case 9:VTMacros =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};

//------------------------------------------------------------------------------
void TVTOutputStringField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";          VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";            VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";           VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTBackgroundColour"; VTAttrAID[3].valueAID=String(VTBackgroundColour);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTFontAttributes";   VTAttrAID[4].valueAID=String(VTFontAttributes);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";          VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=2; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTVariableReference";VTAttrAID[6].valueAID=String(VTVariableReference);
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTJustification";    VTAttrAID[7].valueAID=String(VTJustification);
 VTAttrAID[8].numAID=8;  VTAttrAID[8].byteAID=2; VTAttrAID[8].typeAID=2; VTAttrAID[8].nameAID="VTLength";           VTAttrAID[8].valueAID=VTValue;
 //
 VTAttrAID[9].numAID=9;  VTAttrAID[9].byteAID=1; VTAttrAID[9].typeAID=2; VTAttrAID[9].nameAID="VTMacros";           VTAttrAID[9].valueAID=VTMacros;
 VT_AID_Nr=10;
};


//==============================================================================
//type 12
boolean TVTOutputNumberField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x,y=pVT_Net->y,refIdx=-1;
uint16_t color=0,w=0,h=0,oLevel=pVT_Net->level;
uint32_t oVal=0;
String str="";
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
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
    getAID();pVT_Net->level++;
    //paint
    w=VTWidth; h=VTHeight;oVal=VTValue;
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,true,&vRect);
    //ref variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       oVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx); 
      }
      //
    //set result value
    str=getNumericResult(oVal,VTOffset,VTScale,VTOptions,VTNumOfDecimals,VTFormat);
    //get ref Font 
    pVT_Net->w=w;  pVT_Net->h=h;pVT_Net->fntJust=VTJustification;
    setVTObjectText(str,&vRect,pVT_Net,VTFontAttributes);
    //
    getVTObjectFromList(pVT_Net,objIdx); 
    pVT_Net->level=oLevel;
  }//valid
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTOutputNumberField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTOutputNumberField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=13;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==13) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTOutputNumberField::getAID(){
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
        //
        case 3:VTBackgroundColour =ww;VTEvent+=1;break;
        case 4:VTFontAttributes   =ww;VTEvent+=2;break;
        case 5:VTOptions          =ww;VTEvent+=1;break;
        case 6:VTVariableReference=ww;VTEvent+=2;break;
        //
        case 7:VTValue           =ww;VTEvent+=4;break;
        case 8:VTOffset          =ww;VTEvent+=4;break;//strtoul(VTAttrAID[10].valueAID.c_str(),NULL,0);
        case 9:VTScale           =ww;VTEvent+=4;break;//strtoul(VTAttrAID[11].valueAID.c_str(),NULL,0);
        //
        case 10:VTNumOfDecimals  =ww;VTEvent+=1;break;
        case 11:VTFormat         =ww;VTEvent+=1;break;
        // 
        case 12:VTJustification  =ww;VTEvent+=1;break;
        //list
        case 13:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTOutputNumberField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";          VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";            VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";           VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTBackgroundColour"; VTAttrAID[3].valueAID=String(VTBackgroundColour);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTFontAttributes";   VTAttrAID[4].valueAID=String(VTFontAttributes); 
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";          VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=2; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTVariableReference";VTAttrAID[6].valueAID=String(VTVariableReference);
 VTAttrAID[7].numAID=12; VTAttrAID[7].byteAID=4; VTAttrAID[7].typeAID=0; VTAttrAID[7].nameAID="VTValue";            VTAttrAID[7].valueAID=String(VTValue);
 VTAttrAID[8].numAID=7;  VTAttrAID[8].byteAID=4; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTOffset";           VTAttrAID[8].valueAID=String(VTOffset);
 VTAttrAID[9].numAID=8;  VTAttrAID[9].byteAID=4; VTAttrAID[9].typeAID=1; VTAttrAID[9].nameAID="VTScale";            VTAttrAID[9].valueAID=String(VTScale);
 VTAttrAID[10].numAID=9; VTAttrAID[10].byteAID=1;VTAttrAID[10].typeAID=1;VTAttrAID[10].nameAID="VTNumOfDecimals";   VTAttrAID[10].valueAID=String(VTNumOfDecimals);
 VTAttrAID[11].numAID=10;VTAttrAID[11].byteAID=1;VTAttrAID[11].typeAID=1;VTAttrAID[11].nameAID="VTFormat";          VTAttrAID[11].valueAID=String(VTFormat);
 VTAttrAID[12].numAID=11;VTAttrAID[12].byteAID=1;VTAttrAID[12].typeAID=1;VTAttrAID[12].nameAID="VTJustification";   VTAttrAID[12].valueAID=String(VTJustification); 
 //
 VTAttrAID[13].numAID=11;VTAttrAID[13].byteAID=1;VTAttrAID[13].typeAID=1;VTAttrAID[13].nameAID="VTMacros";VTAttrAID[13].valueAID=VTMacros;
VT_AID_Nr=14;
};


//==============================================================================
//type 37
boolean TVTOutputListField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x,y=pVT_Net->y,refIdx=-1;
uint16_t color=0,w=0,h=0,objID=0xFFFF,oLevel=pVT_Net->level;
uint8_t oVal=0;
String valueAttr="";
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
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
    getAID();pVT_Net->level++;
    //paint
    w=VTWidth; h=VTHeight;oVal=VTValue;
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,true,&vRect);
    //ref Value
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
     if (refIdx>=0){
      getVTObjectFromList(pVT_Net,refIdx);
      oVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
      getVTObjectFromList(pVT_Net,objIdx); 
     }
     //
    objID=getVTItemsNumber(oVal,"VTItemList",pVT_Net);
    SetObjPaintObjToRef(&vRect,pVT_Net,objID);
    //
    getVTObjectFromList(pVT_Net,objIdx); 
    pVT_Net->level=oLevel;
  }//valid
  //
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTOutputListField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};



//------------------------------------------------------------------------------
boolean TVTOutputListField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==5) getVTItems(pVT_Net,nn, pStream);
       if (i==6) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};  


//------------------------------------------------------------------------------
void TVTOutputListField::getAID(){
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
        //
        case 3:VTVariableReference=ww;VTEvent+=2;break;
        case 4:VTValue            =ww;VTEvent+=1;break;
        //list
        case 5:VTItems =ww;VTEvent+=1;VTEvent+=2*ww;break; 
        case 6:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTOutputListField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType"; VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";   VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";  VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTVariableReference";VTAttrAID[3].valueAID=String(VTVariableReference);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=0; VTAttrAID[4].nameAID="VTValue";   VTAttrAID[4].valueAID=String(VTValue);
 //
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTItemList";VTAttrAID[5].valueAID=VTItems;
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=2; VTAttrAID[6].nameAID="VTMacros";  VTAttrAID[6].valueAID=VTMacros;
 VT_AID_Nr=7;
};

//=============================================================================
//=============================================================================
