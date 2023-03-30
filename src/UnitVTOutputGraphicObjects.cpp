//==============================================================================
//Definition objects TVTOutputGraphicObject,
// TVTMeter, TVTLinearBargraph, TVTArchedBargraph
//==============================================================================
//unit UnitVTOutputGraphicObjects.cpp;
//==============================================================================
#ifndef UnitVTOutputGraphicObjects_h
  #define UnitVTOutputGraphicObjects_h
  #include "UnitVTOutputGraphicObjects.h"
#endif 


//==============================================================================
//==============================================================================
//type=17
boolean TVTMeter::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint32_t arcVal=0;
uint8_t  nTicks=0,optn=0;
uint16_t w=0,h=0,r=0,sw=0,ew=180,oLevel=pVT_Net->level;
int16_t  x=pVT_Net->x, y=pVT_Net->y,xx=x,yy=y,arcW=0,dVal=0,refIdx=-1;
int8_t   lw=1,bColor=0,tColor=0,nColor=0;
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
 TVT_StartEndPoint pp;
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //Paint
   w=VTWidth; h=w; r=w/2-1; pVT_Net->w=w;pVT_Net->h=h; 
   optn=VTOptions;
   bColor=VTBorderColour;
   tColor=VTArcAndTickColour;
   nColor=VTNeedleColour;
   sw=VTStartAngle;ew=VTEndAngle;nTicks=VTNumOfTicks;
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   //
   //setValue
   arcVal=VTValue;
    //ref variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       arcVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);
      }
    //
    //set min/max values
   dVal=VTMaxValue-VTMinValue;
     if (arcVal<VTMinValue)  arcVal=VTMinValue;
     if (arcVal>VTMaxValue)  arcVal=VTMaxValue;
     // 
     //set value/targ value
     if (dVal!=0){
       if (sw==ew){
        arcW=abs(180)*(arcVal-VTMinValue)/dVal;
       } else {
         if (sw>ew){
          arcW =abs(ew+180-sw)*(arcVal -VTMinValue)/dVal;
         }else{
          arcW =abs(ew-sw)*(arcVal -VTMinValue)/dVal;
         }
       }
     }
     //
    //paint arc
    pVT_Net->arcTicks=-1;
     if ((optn & 0x01)>0) pp=drawArcTicks(pVT_Net,sw,ew,lw,tColor);
     //paint ticks     
     if ((nTicks>0) && ((optn & 0x04)>0)) {
      pVT_Net->arcTicks=nTicks;pVT_Net->arcTickLength=8;
      pp=drawArcTicks(pVT_Net,sw,ew,lw,tColor);
      pVT_Net->arcTicks=-1;pVT_Net->arcTickLength=0;
     }
     //
      //paint border
      if ((optn & 0x02)>0) Set_drawRect(pVT_Net,x,y,w,h,bColor);
      //
      //paint value special not needle
      if ((optn & 0x10)==0) {
       pVT_Net->arcValue=true;pVT_Net->arcTicks=-1; pVT_Net->arcSeg=2;
        //clockwise
        if ((optn & 0x08)>0){
          pp=drawArcTicks(pVT_Net,(ew-arcW-2) % 180,(ew-arcW) % 180,lw,nColor);
        }else{
          pp=drawArcTicks(pVT_Net,(sw+arcW) % 180,(sw+arcW+2) % 180,lw,nColor);
        }
        //
        xx=pp.startXX; yy=pp.startYY;     
        //paint needle
        Set_drawLine(pVT_Net,x+w/2,y+w/2,xx,yy, nColor);
      }//not needle
      //
     //reset arc variable
     pVT_Net->arcSeg=6;
     pVT_Net->arcW=0; pVT_Net->arcValue=false;pVT_Net->arcTicks=-1;
     //
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid 
 return valid;  
}


//------------------------------------------------------------------------------
boolean TVTMeter::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTMeter::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTMeter::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth            =ww;VTEvent+=2;break;
        case 2:VTNeedleColour     =ww;VTEvent+=1;break;
        case 3:VTBorderColour     =ww;VTEvent+=1;break;
        case 4:VTArcAndTickColour =ww;VTEvent+=1;break;
        case 5:VTOptions          =ww;VTEvent+=1;break;
        case 6:VTNumOfTicks       =ww;VTEvent+=1;break;
        case 7:VTStartAngle       =ww;VTEvent+=1;break;
        case 8:VTEndAngle         =ww;VTEvent+=1;break;
        case 9:VTMinValue         =ww;VTEvent+=2;break;
        case 10:VTMaxValue         =ww;VTEvent+=2;break;
        case 11:VTVariableReference=ww;VTEvent+=2;break;
        case 12:VTValue            =ww;VTEvent+=2;break;
        //list
        case 13:VTMacros=ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTMeter::setAID(){
 VTAttrAID[0].numAID=0;   VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";     VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;   VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";       VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;   VTAttrAID[2].byteAID=1; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTNeedleColour";VTAttrAID[2].valueAID=String(VTNeedleColour);
 VTAttrAID[3].numAID=3;   VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTBorderColour";VTAttrAID[3].valueAID=String(VTBorderColour);
 VTAttrAID[4].numAID=4;   VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTArcAndTickColour";VTAttrAID[4].valueAID=String(VTArcAndTickColour);
 VTAttrAID[5].numAID=5;   VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";     VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=6;   VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTNumOfTicks";  VTAttrAID[6].valueAID=String(VTNumOfTicks);
 VTAttrAID[7].numAID=7;   VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTStartAngle";  VTAttrAID[7].valueAID=String(VTStartAngle);
 VTAttrAID[8].numAID=8;   VTAttrAID[8].byteAID=1; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTEndAngle";    VTAttrAID[8].valueAID=String(VTEndAngle);
 VTAttrAID[9].numAID=9;   VTAttrAID[9].byteAID=2; VTAttrAID[9].typeAID=1; VTAttrAID[9].nameAID="VTMinValue";    VTAttrAID[9].valueAID=String(VTMinValue);
 VTAttrAID[10].numAID=10; VTAttrAID[10].byteAID=2;VTAttrAID[10].typeAID=1;VTAttrAID[10].nameAID="VTMaxValue";   VTAttrAID[10].valueAID=String(VTMaxValue);
 VTAttrAID[11].numAID=11; VTAttrAID[11].byteAID=2;VTAttrAID[11].typeAID=1;VTAttrAID[11].nameAID="VTVariableReference";VTAttrAID[11].valueAID=String(VTVariableReference);
 VTAttrAID[12].numAID=12; VTAttrAID[12].byteAID=2;VTAttrAID[12].typeAID=0;VTAttrAID[12].nameAID="VTValue";      VTAttrAID[12].valueAID=String(VTValue);
 // 
 VTAttrAID[13].numAID=13; VTAttrAID[13].byteAID=1;VTAttrAID[13].typeAID=2;VTAttrAID[13].nameAID="VTMacros";     VTAttrAID[13].valueAID=VTMacros;
 VT_AID_Nr=14;
}



//==============================================================================
//type=18
boolean TVTLinearBarGraph::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t cIdx=0,nTicks=0,opt=0,col=0,col0=0,col1=0;
uint16_t w=0,h=0,ww=0,hh=0,minV=0,maxV=0,valV=0,tarV=0,diff=0,dVal=0,dd=0,oLevel=pVT_Net->level;
int16_t  x=pVT_Net->x,y=pVT_Net->y,xx=x,yy=y,refIdx=-1;
float    dTicks=0;
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
 TVTPixelXY pXY;
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //Paint
   w=VTWidth;   h=VTHeight;   minV=VTMinValue;   maxV=VTMaxValue;   
   valV=VTValue;  tarV=VTTargetValue;
   opt=VTOptions; nTicks=VTNumOfTicks;
   //
   col0=VTColour;
   col1=VTTargetLineColour;
    //ref variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       valV=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);
      }
    //ref VTTargetVariableReference
    refIdx=getVTObjID(pVT_Net,VTTargetVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       tarV=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);
      }
    //
   //paint
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
     //
     if ((opt & 0x01)>0) Set_drawRect(pVT_Net,x,y,w,h,col0);
   //
   valid=PaintObjToOptions(pVT_Net,&pXY,valV,opt,col0);  
     //set target color complement
     if (((opt &0x08)==0) && (col1==col0) && (tarV<=valV)) {
       col1=getColourMapInverse(pVT_Net,col1);
     }
     //draw target
     if ((opt & 0x02)>0) valid=PaintObjToOptions(pVT_Net,&pXY,tarV,((opt & 0xF0) | 0x08),col1);  
     //Ticks
     if (((opt & 0x04)>0) && (nTicks>1)) {
       for (int i=0;i<nTicks;i++) {
        col=col0;  
          //horizontal
          if ((opt & 0x10)>0){
             dTicks=i*w/(nTicks-1);
             xx=round(x+dTicks);
               if ((xx>=pXY.pixelX) && (xx<=pXY.pixelX+pXY.pixelW)){
                 if (col1==col) col1=getColourMapInverse(pVT_Net,col1); else col=col1;
               }
             Set_drawLine(pVT_Net,xx,y,xx,y+h/8,col);
             Set_drawLine(pVT_Net,xx,y+h-h/8,xx,y+h,col);
          }else {
             dTicks=i*h/(nTicks-1);
             yy=round(y+dTicks);
               if ((yy>=pXY.pixelY) && (yy<=pXY.pixelY+pXY.pixelH)){
                 if (col1==col) col1=getColourMapInverse(pVT_Net,col1); else col=col1;
               }
             Set_drawLine(pVT_Net,xx,yy,xx+w/8,yy,col);
             Set_drawLine(pVT_Net,xx+w-w/8,yy,xx+w,yy,col);
          }
         //
       }//for i
     }//Ticks 
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
 }//valid 
 return valid;  
}


//------------------------------------------------------------------------------
boolean TVTLinearBarGraph::PaintObjToOptions(TVT_Net *pVT_Net,TVTPixelXY *pXY, uint16_t valV, uint8_t opt,uint16_t color) {
uint16_t w=VTWidth,h=VTHeight,minV=VTMinValue,maxV=VTMaxValue,dVal=0,ww=0,hh=0;
int16_t  x=pVT_Net->x,y=pVT_Net->y,xx=x,yy=y,diff=maxV-minV;
boolean valid=(diff!=0);
    if (valid){
       if (valV>maxV) valV=maxV;
       if (valV<minV) valV=minV;
       //
       //set Value
       //horizontal
       if ((opt & 0x10)>0) {
        dVal=w*(valV-minV)/diff; xx=x+w-dVal;yy=y;ww=2;hh=h;   
          //direction
          if ((opt & 0x20)>0) {
            xx=x+dVal;
             //fill
             if ((opt & 0x08)==0) xx=x;
          }
          //fill value mode
          if ((opt & 0x08)==0) {
            ww=dVal;
              if (ww>w) ww=w;          
          }else{
            if (xx<=x)   {xx=xx+2;  ww=2;}
            if (xx>=x+w) {xx=x+w-2; ww=2;}
          }
       } else {
         dVal=h*(valV-minV)/diff; yy=y+dVal;xx=x; ww=w;hh=2;   
          //direction
          if ((opt & 0x20)>0) {
            yy=y+h-dVal;
          }else {
             //fill
             if ((opt & 0x08)==0) yy=y;
          }
          //fill value mode
          if ((opt & 0x08)==0){
            hh=dVal;
             if (hh>h) hh=h;
          }else {
            if (yy<=y)   {yy=y+2;   hh=2;}
            if (yy>=y+h) {yy=y+h-2; hh=2;}
          }
       }
       //fill
       if ((opt & 0x08)==0){
         pXY->pixelX=xx;pXY->pixelY=yy;pXY->pixelW=ww; pXY->pixelH=hh;
       }
      Set_fillRect(pVT_Net,xx,yy,ww,hh,color);
    }
  return valid;  
}


//------------------------------------------------------------------------------
boolean TVTLinearBarGraph::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTLinearBarGraph::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
void TVTLinearBarGraph::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth            =ww;VTEvent+=2;break;
        case 2:VTHeight           =ww;VTEvent+=2;break;
        case 3:VTColour           =ww;VTEvent+=1;break;
        case 4:VTTargetLineColour =ww;VTEvent+=1;break;
        case 5:VTOptions          =ww;VTEvent+=1;break;
        case 6:VTNumOfTicks       =ww;VTEvent+=1;break;
        case 7:VTMinValue         =ww;VTEvent+=2;break;
        case 8:VTMaxValue         =ww;VTEvent+=2;break;
        case 9:VTVariableReference=ww;VTEvent+=2;break;
        case 10:VTValue            =ww;VTEvent+=2;break;
        case 11:VTTargetVariableReference=ww;VTEvent+=2;break;
        case 12:VTTargetValue      =ww;VTEvent+=2;break;
        //list
        case 13:VTMacros=ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTLinearBarGraph::setAID(){
 VTAttrAID[0].numAID=0;   VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;   VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";           VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;   VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";          VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;   VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTColour";          VTAttrAID[3].valueAID=String(VTColour);
 VTAttrAID[4].numAID=4;   VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTTargetLineColour";VTAttrAID[4].valueAID=String(VTTargetLineColour);
 VTAttrAID[5].numAID=5;   VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";         VTAttrAID[5].valueAID=String(VTOptions); 
 VTAttrAID[6].numAID=6;   VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTNumOfTicks";      VTAttrAID[6].valueAID=String(VTNumOfTicks);
 VTAttrAID[7].numAID=7;   VTAttrAID[7].byteAID=2; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTMinValue";        VTAttrAID[7].valueAID=String(VTMinValue);
 VTAttrAID[8].numAID=8;   VTAttrAID[8].byteAID=2; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTMaxValue";        VTAttrAID[8].valueAID=String(VTMaxValue);
 VTAttrAID[9].numAID=9;   VTAttrAID[9].byteAID=2; VTAttrAID[9].typeAID=1; VTAttrAID[9].nameAID="VTVariableReference";VTAttrAID[9].valueAID=String(VTVariableReference);
 VTAttrAID[10].numAID=12; VTAttrAID[10].byteAID=2;VTAttrAID[10].typeAID=0;VTAttrAID[10].nameAID="VTValue";          VTAttrAID[10].valueAID=String(VTValue); 
 VTAttrAID[11].numAID=10; VTAttrAID[11].byteAID=2;VTAttrAID[11].typeAID=1;VTAttrAID[11].nameAID="VTTargetVariableReference";VTAttrAID[11].valueAID=String(VTTargetVariableReference);
 VTAttrAID[12].numAID=11; VTAttrAID[12].byteAID=2;VTAttrAID[12].typeAID=1;VTAttrAID[12].nameAID="VTTargetValue";    VTAttrAID[12].valueAID=String(VTTargetValue);
 // 
 VTAttrAID[13].numAID=13; VTAttrAID[13].byteAID=1;VTAttrAID[13].typeAID=2;VTAttrAID[13].nameAID="VTMacros";         VTAttrAID[13].valueAID=VTMacros; 
 VT_AID_Nr=14;
}


//==============================================================================
//type=19
boolean TVTArchedBarGraph::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int32_t arcVal=0,targVal=0,dVal=0;
int16_t arcW=0,targW=0,refIdx=-1;
uint16_t w=0,h=0,bw=0,oLevel=pVT_Net->level;
int16_t x=pVT_Net->x, y=pVT_Net->y,sw=0,ew=180,x0=0,y0=0,x1=0,y1=0,x2=0,y2=0,x3=0,y3=0;
uint8_t lw=1,optn=0,color=0,targColor=0;
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
 TVT_StartEndPoint pp;
  if ((valid) && (objIdx>=0)) {
   getAID();pVT_Net->level++;
   //Paint
   w=VTWidth;   h=VTHeight; pVT_Net->w=w; pVT_Net->h=h; 
   bw=VTBarGraphWidth;
     if (2*bw>w) bw=w/2;
     if (2*bw>h) bw=h/2;
   color=VTColour;
   targColor=VTTargetLineColour;
   sw=VTStartAngle; ew=VTEndAngle;optn=VTOptions;
   sw=sw % 180; ew=ew % 180;
   //
   getViewport(pVT_Net,&vRect,x,y,w+1,h+1);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   
   //TEST
   //drawArcTicks(pVT_Net,sw % 180,ew % 180,bw,color);
   //TODO
   //return valid;
   //
   
   
   //   
   //setValue
   arcVal=VTValue;targVal=VTTargetValue;
    //ref variable
    refIdx=getVTObjID(pVT_Net,VTVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       arcVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);
      }
    //ref VTTargetVariableReference
    refIdx=getVTObjID(pVT_Net,VTTargetVariableReference);
      if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx); 
       targVal=getVTObjectAttributeInt32("VTValue",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);
      }
    //set min/max values
   dVal=VTMaxValue-VTMinValue;
     if (arcVal<VTMinValue)  arcVal=VTMinValue;
     if (arcVal>VTMaxValue)  arcVal=VTMaxValue;
     if (targVal<VTMinValue) targVal=VTMinValue;
     if (targVal>VTMaxValue) targVal=VTMaxValue;
    
     //set value/targ value
     if (dVal!=0){
       if (sw==ew){
        arcW=abs(180)*(arcVal-VTMinValue)/dVal; targW=abs(180)*(targVal-VTMinValue)/dVal;
       } else {
         if (sw>ew){
          arcW =abs(ew+180-sw)*(arcVal -VTMinValue)/dVal;
          targW=abs(ew+180-sw)*(targVal-VTMinValue)/dVal;
         }else{
          arcW =abs(ew-sw)*(arcVal -VTMinValue)/dVal;
          targW=abs(ew-sw)*(targVal-VTMinValue)/dVal;
         }
       }
     }
     //
     //no fill value
     if ((optn & 0x08)>0){
      pVT_Net->arcSeg=1;
       //direction
       if ((optn & 0x10)>0){
        SetArchedValue(pVT_Net,ew-arcW,lw,bw,color);
       }else{
        SetArchedValue(pVT_Net,sw+arcW,lw,bw,color);
       }
      pVT_Net->arcSeg=6;
     }else{
       //set target color complement for fill=true
       if ((color==targColor) && (targVal<=arcVal)) {
         targColor=getColourMapInverse(pVT_Net,targColor);
       }
       //
       //direction
       if ((optn & 0x10)>0){
        drawArcTicks(pVT_Net,(ew-arcW) % 180,(ew+1) % 180,bw,color);
       }else{
        drawArcTicks(pVT_Net,sw % 180,(sw+arcW) % 180,bw,color);
       }
     }
     //Set target value
     if ((optn &0x02)>0){ 
      pVT_Net->arcSeg=1;
       //direction
       if ((optn & 0x10)>0){
        SetArchedValue(pVT_Net,ew-targW, lw,bw,targColor);
       }else{
        SetArchedValue(pVT_Net,sw+targW, lw,bw,targColor);
       }
      pVT_Net->arcSeg=6;
     }
     //
     //set body
     if ((optn & 0x01)>0){
      pVT_Net->arcSeg=4;
      pp=drawArcTicks(pVT_Net,sw,ew,lw,color);
      x0=pp.startXX;y0=pp.startYY;x1=pp.endXX;y1=pp.endYY;
      x2=x+w/2; x3=x2; y2=y+h/2; y3=y2;
      //
      pVT_Net->x=x+bw;pVT_Net->y=y+bw; pVT_Net->w=w-2*bw;pVT_Net->h=h-2*bw; 
       if ((pVT_Net->w>0) && (pVT_Net->h>0)) {
         pp=drawArcTicks(pVT_Net,sw,ew,lw,color);
         x2=pp.startXX;y2=pp.startYY;x3=pp.endXX;y3=pp.endYY;
       }
      pVT_Net->x=x,pVT_Net->y=y; pVT_Net->w=w;pVT_Net->h=h; 
      //end lines     
      Set_drawLine(pVT_Net,x0,y0,x2,y2,color);
      Set_drawLine(pVT_Net,x1,y1,x3,y3,color);
      pVT_Net->arcSeg=6;
     }
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid 
 return valid;  
};



//------------------------------------------------------------------------------
void TVTArchedBarGraph::SetArchedValue(TVT_Net *pVT_Net,int16_t targW, uint8_t lw, uint8_t bw, uint16_t targColor){
uint16_t w=pVT_Net->w, h=pVT_Net->h;
int16_t x=pVT_Net->x, y=pVT_Net->y,x0=0,y0=0,x1=0,y1=0;
TVT_StartEndPoint pp;
  pp=drawArcTicks(pVT_Net,targW % 180,(targW+1) % 180,lw,targColor);
  x1=pp.startXX;y1=pp.startYY;
    //
    if ((bw>=w/2) || (bw>=h/2)){
      x0=x+w/2; y0=y+h/2;
    } else {
      pp=drawArcTicks(pVT_Net,(targW) % 180,(targW+1) % 180,bw,targColor);
      x0=pp.startXX;y0=pp.startYY; 
    }
    //
  Set_drawLine(pVT_Net,x0,y0,x1,y1,targColor);
}        


//------------------------------------------------------------------------------
boolean TVTArchedBarGraph::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTArchedBarGraph::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=15;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==15) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTArchedBarGraph::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
  VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth            =ww;VTEvent+=2;break;
        case 2:VTHeight           =ww;VTEvent+=2;break;
        case 3:VTColour           =ww;VTEvent+=1;break;
        case 4:VTTargetLineColour =ww;VTEvent+=1;break;
        case 5:VTOptions          =ww;VTEvent+=1;break;
  
        case 6:VTStartAngle       =ww;VTEvent+=1;break;
        case 7:VTEndAngle         =ww;VTEvent+=1;break;
        case 8:VTBarGraphWidth    =ww;VTEvent+=2;break;
  
        case 9:VTMinValue         =ww;VTEvent+=2;break;
        case 10:VTMaxValue        =ww;VTEvent+=2;break;
  
        case 11:VTVariableReference=ww;VTEvent+=2;break;
        case 12:VTValue            =ww;VTEvent+=2;break;
        case 13:VTTargetVariableReference=ww;VTEvent+=2;break;
        case 14:VTTargetValue      =ww;VTEvent+=2;break;
        //list
        case 15:VTMacros=ww;VTEvent+=1;break;
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTArchedBarGraph::setAID(){
 VTAttrAID[0].numAID=0;   VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";         VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;   VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";           VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;   VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";          VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;   VTAttrAID[3].byteAID=1; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTColour";          VTAttrAID[3].valueAID=String(VTColour);
 VTAttrAID[4].numAID=4;   VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTTargetLineColour";VTAttrAID[4].valueAID=String(VTTargetLineColour);
 VTAttrAID[5].numAID=5;   VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTOptions";         VTAttrAID[5].valueAID=String(VTOptions);
 VTAttrAID[6].numAID=6;   VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTStartAngle";      VTAttrAID[6].valueAID=String(VTStartAngle);
 VTAttrAID[7].numAID=7;   VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTEndAngle";        VTAttrAID[7].valueAID=String(VTEndAngle); 
 VTAttrAID[8].numAID=8;   VTAttrAID[8].byteAID=2; VTAttrAID[8].typeAID=1; VTAttrAID[8].nameAID="VTBarGraphWidth";   VTAttrAID[8].valueAID=String(VTBarGraphWidth);
 VTAttrAID[9].numAID=9;   VTAttrAID[9].byteAID=2; VTAttrAID[9].typeAID=1; VTAttrAID[9].nameAID="VTMinValue";        VTAttrAID[9].valueAID=String(VTMinValue);
 VTAttrAID[10].numAID=10; VTAttrAID[10].byteAID=2;VTAttrAID[10].typeAID=1;VTAttrAID[10].nameAID="VTMaxValue";       VTAttrAID[10].valueAID=String(VTMaxValue);
 VTAttrAID[11].numAID=11; VTAttrAID[11].byteAID=2;VTAttrAID[11].typeAID=1;VTAttrAID[11].nameAID="VTVariableReference";VTAttrAID[11].valueAID=String(VTVariableReference);
 VTAttrAID[12].numAID=14; VTAttrAID[12].byteAID=2;VTAttrAID[12].typeAID=0;VTAttrAID[12].nameAID="VTValue";          VTAttrAID[12].valueAID=String(VTValue);
 VTAttrAID[13].numAID=12; VTAttrAID[13].byteAID=2;VTAttrAID[13].typeAID=1;VTAttrAID[13].nameAID="VTTargetVariableReference";VTAttrAID[13].valueAID=String(VTTargetVariableReference);
 VTAttrAID[14].numAID=13; VTAttrAID[14].byteAID=2;VTAttrAID[14].typeAID=1;VTAttrAID[14].nameAID="VTTargetValue";VTAttrAID[14].valueAID=String(VTTargetValue);
 // 
 VTAttrAID[15].numAID=15; VTAttrAID[15].byteAID=1;VTAttrAID[15].typeAID=2;VTAttrAID[15].nameAID="VTMacros";VTAttrAID[15].valueAID=VTMacros;
 VT_AID_Nr=16;
};

//==============================================================================
//==============================================================================
