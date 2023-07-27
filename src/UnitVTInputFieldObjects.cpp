//==============================================================================
//Definition objects TVTInputObject,
//TVTInputBooleanField, TVTInputStringField, TVTInputNumberField, TVTInputListField
//==============================================================================
//unit UnitVTInputFieldObjects.cpp
//==============================================================================
#include "UnitVTInputFieldObjects.h"
#include "UnitVTAttributeObjects.h"

//==============================================================================
//==============================================================================
//type 7
boolean TVTInputBooleanField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
uint16_t color=0,w=0,h=0,oLevel=pVT_Net->level;
uint8_t oVal=0;
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
      if (pVT_Net->VTInputIndex==255) pVT_Net->VTInputIndex=0;
    //paint
    w=VTWidth; h=VTWidth;oVal=VTValue;
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,VTEnabled,&vRect);
    //ref variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       oVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);
      }
      //
    SetSelectState(pVT_Net,VTEnabled,&vRect);
    //get ref Font  
    pVT_Net->w=w;  pVT_Net->h=h; pVT_Net->fntJust=5;
    color=setVTObjectText(str,&vRect, pVT_Net,VTFontAttributes);
    //
    Set_drawRect(pVT_Net,x,y,w,h,color);
      if (oVal>0) {
       Set_drawLine(pVT_Net,x,y,x+w,y+h,color);
       Set_drawLine(pVT_Net,x,y+h,x+w,y,color);
      }
     //
    getVTObjectFromList(pVT_Net,objIdx); 
      if (VTEnabled) pVT_Net->VTInputIndex++;
    pVT_Net->level=oLevel;
  }//valid
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTInputBooleanField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTInputBooleanField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t objID=VTObjID, i=0,j=0,nn=0;
uint8_t  objType=VTObjType,bb=0;
uint32_t ww=0;
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
    for (i=7;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==7) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};  


//------------------------------------------------------------------------------
void TVTInputBooleanField::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTBackgroundColour   =ww;VTEvent+=1;break;
        case 2:VTWidth    =ww;VTEvent+=2;break;
        case 3:VTFontAttributes=ww;VTEvent+=2;break;
        case 4:VTVariableReference=ww;VTEvent+=2;break;
        case 5:VTValue    =ww;VTEvent+=1;break;
        case 6:VTEnabled  =ww;VTEvent+=1;break;
        //List
        case 7:VTMacros =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTInputBooleanField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";          VTAttrAID[0].valueAID=String(VTObjType); 
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=1; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTBackgroundColour"; VTAttrAID[1].valueAID=String(VTBackgroundColour);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTWidth";            VTAttrAID[2].valueAID=String(VTWidth);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTFontAttributes";   VTAttrAID[3].valueAID=String(VTFontAttributes);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTVariableReference";VTAttrAID[4].valueAID=String(VTVariableReference);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=0; VTAttrAID[5].nameAID="VTValue";            VTAttrAID[5].valueAID=String(VTValue);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=0; VTAttrAID[6].nameAID="VTEnabled";          VTAttrAID[6].valueAID=String(VTEnabled);
 //
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=2; VTAttrAID[7].nameAID="VTMacros";VTAttrAID[7].nameAID=VTMacros;
 VT_AID_Nr=8;
};



//==============================================================================
//type 8
boolean TVTInputStringField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
uint16_t color=0,w=0,h=0,oLevel=pVT_Net->level;
uint8_t opt=0;
String str="";
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
TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
    getAID();pVT_Net->level++;
      if (pVT_Net->VTInputIndex==255) pVT_Net->VTInputIndex=0;
    VTValue=getVTObjectAttributeString("VTLength",pVT_Net);
    //paint
    w=VTWidth;h=VTHeight; opt=VTOptions;str=VTValue;
    //TEST
    //Serial.println("VTEnabled=" + String(VTEnabled));
    
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,VTEnabled,&vRect);
    //ref Variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
        getVTObjectFromList(pVT_Net,refIdx); 
        str=getVTObjectAttributeString("VTLength",pVT_Net);
        getVTObjectFromList(pVT_Net,objIdx);
      }
   //get ref Font  
   //Serial.println(str);
   pVT_Net->w=w;  pVT_Net->h=h; pVT_Net->fntJust=VTJustification;
   setVTObjectText(str,&vRect, pVT_Net,VTFontAttributes);
   getVTObjectFromList(pVT_Net,objIdx); 
     if (VTEnabled) pVT_Net->VTInputIndex++;
   pVT_Net->level=oLevel;
  }//valid 
  //
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTInputStringField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTInputStringField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
        if (i==9)  getVTString(pVT_Net,nn, pStream);
    }//for i
    //
    for (i=11;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==11) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTInputStringField::getAID(){
uint32_t ww=0,len=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0; len=VTAttrAID[i].valueAID.length();
       for (j=0;j<len;j++){
         ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       //
       switch (i) {
        case 1:VTWidth    =ww;VTEvent+=2;break;
        case 2:VTHeight   =ww;VTEvent+=2;break;
        case 3:VTBackgroundColour =ww;VTEvent+=1;break;
        case 4:VTFontAttributes =ww;VTEvent+=2;break;
        case 5:VTInputAttributes=ww;VTEvent+=2;break;
        case 6:VTOptions=ww;VTEvent+=1;break;
        case 7:VTVariableReference=ww;VTEvent+=2;break;
        case 8:VTJustification=ww;VTEvent+=1;break;
        case 9:VTLength=ww; VTEvent+=1;VTEvent+=ww;break;
       case 10:VTEnabled  =ww;VTEvent+=1;break;
        //List
        case 11:VTMacros =ww; VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTInputStringField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";           VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";          VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTBackgroundColour";VTAttrAID[3].valueAID=String(VTBackgroundColour);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTFontAttributes";  VTAttrAID[4].valueAID=String(VTFontAttributes);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=2; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTInputAttributes"; VTAttrAID[5].valueAID=String(VTInputAttributes);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTOptions";         VTAttrAID[6].valueAID=String(VTOptions);
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=2; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTVariableReference";VTAttrAID[7].valueAID=String(VTVariableReference);
 VTAttrAID[8].numAID=8;  VTAttrAID[8].byteAID=1; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTJustification";   VTAttrAID[8].valueAID=String(VTJustification);
 VTAttrAID[9].numAID=10; VTAttrAID[9].byteAID=1; VTAttrAID[9].typeAID=2; VTAttrAID[9].nameAID="VTLength";          VTAttrAID[9].valueAID=VTValue;
 VTAttrAID[10].numAID=9; VTAttrAID[10].byteAID=1;VTAttrAID[10].typeAID=0;VTAttrAID[10].nameAID="VTEnabled";        VTAttrAID[10].valueAID=String(VTEnabled);
 //
 VTAttrAID[11].numAID=11;VTAttrAID[11].byteAID=1; VTAttrAID[11].typeAID=2; VTAttrAID[11].nameAID="VTMacros";VTAttrAID[11].valueAID=VTMacros;
 VT_AID_Nr=12;
};


//==============================================================================
//type 9
boolean TVTInputNumberField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
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
      if (pVT_Net->VTInputIndex==255) pVT_Net->VTInputIndex=0;
    //paint
    w=VTWidth; h=VTHeight; oVal=VTValue;
   //   
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,VTEnabled,&vRect);
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
   pVT_Net->w=w;  pVT_Net->h=h; pVT_Net->fntJust=VTJustification;
   setVTObjectText(str,&vRect,pVT_Net,VTFontAttributes);
   getVTObjectFromList(pVT_Net,objIdx); 
     if (VTEnabled) pVT_Net->VTInputIndex++;
   pVT_Net->level=oLevel;
  }//valid 
  //
 return valid;  
};//PaintObjTo


//------------------------------------------------------------------------------
boolean TVTInputNumberField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTInputNumberField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=16;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==16) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};


//------------------------------------------------------------------------------
void TVTInputNumberField::getAID(){
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
        case 7:VTValue            =ww;VTEvent+=4;break;
        case 8:VTMinValue         =ww;VTEvent+=4;break;
        case 9:VTMaxValue         =ww;VTEvent+=4;break;//strtoul(VTAttrAID[9].valueAID.c_str(),NULL,0); 
        case 10:VTOffset          =ww;VTEvent+=4;break;//strtoul(VTAttrAID[10].valueAID.c_str(),NULL,0);
        case 11:VTScale           =ww;VTEvent+=4;break;//strtoul(VTAttrAID[11].valueAID.c_str(),NULL,0);
        //
        case 12:VTNumOfDecimals    =ww;VTEvent+=1;break;
        case 13:VTFormat           =ww;VTEvent+=1;break;
        // 
        case 14:VTJustification=ww;VTEvent+=1;break;
        case 15:VTEnabled=ww;VTEvent+=1;break;
        //list
        case 16:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEnabled=0;
};
  

//------------------------------------------------------------------------------
void TVTInputNumberField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";           VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";          VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTBackgroundColour";VTAttrAID[3].valueAID=String(VTBackgroundColour);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTFontAttributes";  VTAttrAID[4].valueAID=String(VTFontAttributes);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";         VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=2; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTVariableReference";VTAttrAID[6].valueAID=String(VTVariableReference);
 VTAttrAID[7].numAID=14; VTAttrAID[7].byteAID=4; VTAttrAID[7].typeAID=0; VTAttrAID[7].nameAID="VTValue";           VTAttrAID[7].valueAID=String(VTValue);
 VTAttrAID[8].numAID=7;  VTAttrAID[8].byteAID=4; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTMinValue";        VTAttrAID[8].valueAID=String(VTMinValue);
 VTAttrAID[9].numAID=8;  VTAttrAID[9].byteAID=4; VTAttrAID[9].typeAID=1; VTAttrAID[9].nameAID="VTMaxValue";        VTAttrAID[9].valueAID=String(VTMaxValue);
 VTAttrAID[10].numAID=9; VTAttrAID[10].byteAID=4;VTAttrAID[10].typeAID=1;VTAttrAID[10].nameAID="VTOffset";         VTAttrAID[10].valueAID=String(VTOffset);
 VTAttrAID[11].numAID=10;VTAttrAID[11].byteAID=4;VTAttrAID[11].typeAID=1;VTAttrAID[11].nameAID="VTScale";          VTAttrAID[11].valueAID=String(VTScale);
 VTAttrAID[12].numAID=11;VTAttrAID[12].byteAID=1;VTAttrAID[12].typeAID=1;VTAttrAID[12].nameAID="VTNumOfDecimals";  VTAttrAID[12].valueAID=String(VTNumOfDecimals);
 VTAttrAID[13].numAID=12;VTAttrAID[13].byteAID=1;VTAttrAID[13].typeAID=1;VTAttrAID[13].nameAID="VTFormat";         VTAttrAID[13].valueAID=String(VTFormat);
 VTAttrAID[14].numAID=13;VTAttrAID[14].byteAID=1;VTAttrAID[14].typeAID=1;VTAttrAID[14].nameAID="VTJustification";  VTAttrAID[14].valueAID=String(VTJustification);
 VTAttrAID[15].numAID=15;VTAttrAID[15].byteAID=1;VTAttrAID[15].typeAID=0;VTAttrAID[15].nameAID="VTEnabled";        VTAttrAID[15].valueAID=String(VTEnabled);
 //
 VTAttrAID[16].numAID=16;VTAttrAID[16].byteAID=1; VTAttrAID[16].typeAID=2; VTAttrAID[16].nameAID="VTMacros";VTAttrAID[16].valueAID=VTMacros;
 VT_AID_Nr=17;
};


//==============================================================================
//type 10
boolean TVTInputListField::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
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
 TVT_ViewRect vvRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
    getAID();pVT_Net->level++;
      if (pVT_Net->VTInputIndex==255) pVT_Net->VTInputIndex=0;
    //paint
    w=VTWidth; h=VTHeight;oVal=VTValue;
    getViewport(pVT_Net,&vvRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,VTEnabled,&vvRect);
    //ref Value
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
     if (refIdx>=0){
      getVTObjectFromList(pVT_Net,refIdx);
      oVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
      getVTObjectFromList(pVT_Net,objIdx); 
     }
     //
    objID=getVTItemsNumber(oVal,"VTItems",pVT_Net);
    SetObjPaintObjToRef(&vvRect,pVT_Net,objID);
    //
    getViewport(pVT_Net,&vRect,x,y,w,h);
    
    SetSelectState(pVT_Net,VTEnabled,&vRect);
    getVTObjectFromList(pVT_Net,objIdx); 
      if (VTEnabled) pVT_Net->VTInputIndex++;
    pVT_Net->level=oLevel;
  }//valid
  //
 return valid;  
};

//------------------------------------------------------------------------------
boolean TVTInputListField::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTInputListField::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
       if (i==7) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};  


//------------------------------------------------------------------------------
void TVTInputListField::getAID(){
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
        case 3:VTVariableReference=ww;VTEvent+=2;break;
        //
        case 4:VTValue   =ww;VTEvent+=1;break;
        case 5:VTItems   =ww;VTEvent+=1;VTEvent+=2*ww;break;
        case 6:VTOptions =ww;VTEnabled=(ww & 0x01); VTEvent+=1;break;
        //list
        case 7:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEnabled=0;
};

//------------------------------------------------------------------------------
void TVTInputListField::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";          VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";            VTAttrAID[1].valueAID=String(VTWidth); 
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";           VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTVariableReference";VTAttrAID[3].valueAID=String(VTVariableReference);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=0; VTAttrAID[4].nameAID="VTValue";            VTAttrAID[4].valueAID=String(VTValue);
 //
 VTAttrAID[5].numAID=6;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTItems";            VTAttrAID[5].valueAID=VTItems;
 //
 VTAttrAID[6].numAID=5;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=0; VTAttrAID[6].nameAID="VTOptions";          VTAttrAID[6].valueAID=String(VTOptions);
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=2; VTAttrAID[7].nameAID="VTMacros";           VTAttrAID[7].valueAID=VTMacros;
 VT_AID_Nr=8;
};

//==============================================================================
//==============================================================================
