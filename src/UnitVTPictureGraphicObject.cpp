//==============================================================================
//Definition objects
//TVTGraphicObject,  TVTPictureGraphic
//==============================================================================
//unit UnitVTPictureGraphicObject.cpp;
//==============================================================================
//#include "BMP_functions.h" 


#ifndef UnitVTPictureGraphicObject_h
  #define UnitVTPictureGraphicObject_h
  #include "UnitVTPictureGraphicObject.h"
#endif 



//==============================================================================
//type=20
boolean TVTPictureGraphic::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean  valid=(getVTObjectListSize(pVT_Net)>0);
int16_t  x=pVT_Net->x, y=pVT_Net->y;
uint16_t ws=0,wa=0,ha=0,oLevel=pVT_Net->level;
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
 TVTPixelXY pXY;
 TVT_ViewRect vRect=getViewRect(pViewRect);
  if ((valid) && (objIdx>=0)) {
   getAID(); pVT_Net->level++;
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   //get pVT_Net->aIdx
   VTPicData=getVTObjectAttributeInt32("VTPicData",pVT_Net); 
   //paint
   ws=VTWidth;wa=VTActualWidth;ha=VTActualHeight;
    //
    if (pVT_Net->VTScaleType>=0) {
      return PaintPictureToDirect(pVT_Net,&pXY,NULL);
    }
    //
    if ((ws>0) && (wa>0) && (ha>0)) {
      float fkt=1.000*ws/wa;
      uint16_t w=round(1.000*fkt*wa); 
      uint16_t h=round(1.000*fkt*ha);
      //Set_resetViewport(pVT_Net);
      getViewport(pVT_Net,&vRect,x,y,w,h);
        //    
        if (PaintPictureToDirect(pVT_Net,&pXY,NULL)){
          //TEST
          //drawBmp("/parrot.bmp", x, y,pVT_Net);
        } else{
          Set_fillRect(pVT_Net,x,y,w,h,TFT_LIGHTGREY);
        }
    }//>0
    //
    getVTObjectFromList(pVT_Net,objIdx); 
    pVT_Net->level=oLevel;
  }//valid   
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTPictureGraphic::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};


//------------------------------------------------------------------------------
boolean TVTPictureGraphic::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(pStream->available());
uint16_t i=0,j=0;
uint8_t  bb=0;
uint32_t ww=0,nn=0;
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
        if (i==7) readPictureFromStream(pVT_Net,nn,pStream);
        if (i==8) getVTMacros(pVT_Net,nn, pStream);
    }//for i
    //
   //TEST
   //getHeapStatus(pVT_Net,160);
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};

//------------------------------------------------------------------------------
boolean TVTPictureGraphic::readPictureFromStream(TVT_Net *pVT_Net,uint32_t bCount,LoopbackStream *pStream){
boolean valid=(bCount<=pStream->available());
uint8_t* pbuff=pStream->getBuffer();
uint32_t pPos=pStream->getPos();
  if (valid){
    pVT_Net->streamStr.writeBytes((uint8_t*) pbuff,bCount,-1,pPos);
    pStream->setPos(pPos+bCount);
  }//valid
 return valid;
};



//------------------------------------------------------------------------------
void TVTPictureGraphic::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth          =ww;VTEvent+=2;break;
        case 2:VTActualWidth    =ww;VTEvent+=2;break;
        case 3:VTActualHeight   =ww;VTEvent+=2;break;
        case 4:VTFormat         =ww;VTEvent+=1;break;
        case 5:VTOptions        =ww;VTEvent+=1;break;
        case 6:VTTransparencyColour =ww;VTEvent+=1;break;
        //list
        case 7:VTPicData=ww;VTEvent+=4;VTEvent+=ww;break;
        case 8:VTMacros =ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};

  
  

//------------------------------------------------------------------------------
void TVTPictureGraphic::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";        VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";          VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=4;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=0; VTAttrAID[2].nameAID="VTActualWidth";    VTAttrAID[2].valueAID=String(VTActualWidth);
 VTAttrAID[3].numAID=5;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=0; VTAttrAID[3].nameAID="VTActualHeight";   VTAttrAID[3].valueAID=String(VTActualHeight);
 VTAttrAID[4].numAID=6;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=0; VTAttrAID[4].nameAID="VTFormat";         VTAttrAID[4].valueAID=String(VTFormat);
 VTAttrAID[5].numAID=2;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";        VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=3;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTTransparencyColour"; VTAttrAID[6].valueAID=String(VTTransparencyColour);
 //
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=4; VTAttrAID[7].typeAID=2; VTAttrAID[7].nameAID="VTPicData";        VTAttrAID[7].valueAID=VTPicData;
 VTAttrAID[8].numAID=8;  VTAttrAID[8].byteAID=1; VTAttrAID[8].typeAID=2; VTAttrAID[8].nameAID="VTMacros";         VTAttrAID[8].valueAID=VTMacros;
 VT_AID_Nr=9;
};


//==============================================================================
//==============================================================================
