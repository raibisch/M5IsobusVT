//==============================================================================
//Definition objects TVTGraphicsContext,
//==============================================================================
//unit UnitVTGraphicContextObject.cpp
//==============================================================================


#ifndef UnitVTGraphicContextObject_h
  #define UnitVTGraphicContextObject_h
  #include "UnitVTGraphicContextObject.h"
#endif 



//==============================================================================
//type=36
boolean TVTGraphicsContext::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean  valid=(getVTObjectListSize(pVT_Net)>0);
int16_t  x=pVT_Net->x, y=pVT_Net->y;
uint16_t ws=VTWidth,wa=VTActualWidth,ha=VTActualHeight,oLevel=pVT_Net->level;
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
TVTPixelXY pXY;

  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //
   VTPicData=getVTObjectAttributeInt32("VTPicData",pVT_Net); 
   //VTMacros=getVTObjectAttributeInt32("VTMacros",pVT_Net); 
   //
   //paint
   ws=VTWidth;wa=VTActualWidth;ha=VTActualHeight;
    //
    if ((ws>0) && (wa>0) && (ha>0)) {
      float fkt=1.000*ws/wa;
      uint16_t w=round(1.000*fkt*wa); 
      uint16_t h=round(1.000*fkt*ha);
      //Set_resetViewport(pVT_Net);
      getViewport(pVT_Net,&vRect,x,y,w,h);
      getVTDrawListAddObj(pVT_Net,true,&vRect);
      //TEST
      Set_fillRect(pVT_Net,x,y,w,h,TFT_LIGHTGREY);
        if (PaintPictureToDirect(pVT_Net,&pXY,NULL)){
          //TEST
          //drawBmp("/parrot.bmp", x, y,pVT_Net);
        } else{
          Set_fillRect(pVT_Net,x,y,w,h,TFT_LIGHTGREY);
        }
    }//>0
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid   
 return valid;  
};




//------------------------------------------------------------------------------
boolean TVTGraphicsContext::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTGraphicsContext::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid;
};

//------------------------------------------------------------------------------
boolean TVTGraphicsContext::readPictureFromStream(TVT_Net *pVT_Net,uint32_t bCount,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint32_t j=0;
uint8_t  bb=0;  
  if ((valid) && (bCount>0)){
     for (j=0;j<bCount;j++) {
      if (pStream->available()) {
       bb=pStream->read();pVT_Net->streamStr.write(bb);
      }else break;
     }//for j
  }
 return valid;
};


//------------------------------------------------------------------------------
void TVTGraphicsContext::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTViewportWidth     =ww;break;
        case 2:VTViewportHeight    =ww;break;
        case 3:VTViewportX         =ww;break;
        case 4:VTViewportY         =ww;break;
        case 5:VTWidth             =ww;break;
        case 6:VTHeight            =ww;break;
        //
        case 7:VTViewportZoom      =ww;break;
        case 8:VTGraphicsCursorX   =ww;break;
        case 9:VTGraphicsCursorY   =ww;break;
        case 10:VTForegroundColour  =ww;break;
        case 11:VTBackgroundColour  =ww;break;
        //
        case 12:VTFontAttributes    =ww;break;
        case 13:VTLineAttributes    =ww;break;
        case 14:VTFillAttributes    =ww;break;

        case 15:VTFormat            =ww;break;
        case 16:VTOptions           =ww;break;
        case 17:VTTransparencyColour=ww;break;
       }//switch i
    }//for i
};

  

//------------------------------------------------------------------------------
void TVTGraphicsContext::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";       VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTViewportWidth"; VTAttrAID[1].valueAID=String(VTViewportWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTViewportHeight";VTAttrAID[2].valueAID=String(VTViewportHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTViewportX";     VTAttrAID[3].valueAID=String(VTViewportX);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTViewportY";     VTAttrAID[4].valueAID=String(VTViewportY);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=2; VTAttrAID[5].typeAID=0; VTAttrAID[5].nameAID="VTWidth";   VTAttrAID[5].valueAID=String(VTWidth);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=2; VTAttrAID[6].typeAID=0; VTAttrAID[6].nameAID="VTHeight";  VTAttrAID[6].valueAID=String(VTHeight);
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=4; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTViewportZoom";  VTAttrAID[7].valueAID=String(VTViewportZoom);
 VTAttrAID[8].numAID=8;  VTAttrAID[8].byteAID=2; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTGraphicsCursorX";  VTAttrAID[8].valueAID=String(VTGraphicsCursorX);
 VTAttrAID[9].numAID=9;  VTAttrAID[9].byteAID=2; VTAttrAID[9].typeAID=1; VTAttrAID[9].nameAID="VTGraphicsCursorY";  VTAttrAID[9].valueAID=String(VTGraphicsCursorY);
 VTAttrAID[10].numAID=10;  VTAttrAID[10].byteAID=1; VTAttrAID[10].typeAID=1; VTAttrAID[10].nameAID="VTForegroundColour";  VTAttrAID[10].valueAID=String(VTForegroundColour);
 VTAttrAID[11].numAID=11;  VTAttrAID[11].byteAID=1; VTAttrAID[11].typeAID=1; VTAttrAID[11].nameAID="VTBackgroundColour";  VTAttrAID[11].valueAID=String(VTBackgroundColour);
 VTAttrAID[12].numAID=12;  VTAttrAID[12].byteAID=2; VTAttrAID[12].typeAID=1; VTAttrAID[12].nameAID="VTFontAttributes";  VTAttrAID[12].valueAID=String(VTFontAttributes);
 VTAttrAID[13].numAID=13;  VTAttrAID[13].byteAID=2; VTAttrAID[13].typeAID=1; VTAttrAID[13].nameAID="VTLineAttributes";  VTAttrAID[13].valueAID=String(VTLineAttributes);
 VTAttrAID[14].numAID=14;  VTAttrAID[14].byteAID=2; VTAttrAID[14].typeAID=1; VTAttrAID[14].nameAID="VTFillAttributes";  VTAttrAID[14].valueAID=String(VTFillAttributes);
 VTAttrAID[15].numAID=15;  VTAttrAID[15].byteAID=1; VTAttrAID[15].typeAID=1; VTAttrAID[15].nameAID="VTFormat";          VTAttrAID[15].valueAID=String(VTFormat);
 VTAttrAID[16].numAID=16;  VTAttrAID[16].byteAID=1; VTAttrAID[16].typeAID=1; VTAttrAID[16].nameAID="VTOptions";         VTAttrAID[16].valueAID=String(VTOptions);
 VTAttrAID[17].numAID=17;  VTAttrAID[17].byteAID=1; VTAttrAID[17].typeAID=1; VTAttrAID[17].nameAID="VTTransparencyColour";  VTAttrAID[17].valueAID=String(VTTransparencyColour);
 VT_AID_Nr=18;
};

//==============================================================================
//==============================================================================
