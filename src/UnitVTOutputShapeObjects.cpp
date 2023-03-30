//==============================================================================
//Definition objects TVTOutputShapeObject,
//TVTLine, TVTRectangle,TVTEllipse, TVTPolygon
//==============================================================================
//unit UnitVTOutputShapeObjects.cpp
//==============================================================================
#ifndef UnitVTOutputShapeObjects_h
  #define UnitVTOutputShapeObjects_h
  #include "UnitVTOutputShapeObjects.h"
#endif 

//------------------------------------------------------------------------------
#ifndef UnitVTPictureGraphicObject_h
  #define UnitVTPictureGraphicObject_h
  #include "UnitVTPictureGraphicObject.h"
#endif 


//==============================================================================
//==============================================================================
//line and fill of shape
boolean TVTOutputShapeObject::getLineAndFill(uint16_t lineID, uint16_t fillID, TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0),setFill=false;
uint8_t cIdx=0;
int16_t objIdx=pVT_Net->objNr, refIdx=-1;
  if (valid){
     if (lineID<0xFFFF) refIdx=getVTObjID(pVT_Net,lineID); else refIdx=-1;
     if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx);
       lineColor=getVTObjectAttributeInt32("VTLineColour",pVT_Net);
       lineWidth=getVTObjectAttributeInt32("VTLineWidth",pVT_Net);
       lineArt = getVTObjectAttributeInt32("VTLineArt",pVT_Net);
       getVTObjectFromList(pVT_Net,objIdx);  
     }
    //get ref VTFillAttributes
     if (fillID<0xFFFF) refIdx=getVTObjID(pVT_Net,fillID);else refIdx=-1;
     if (refIdx>=0){
       getVTObjectFromList(pVT_Net,refIdx);
       fillType =getVTObjectAttributeInt32("VTFillType",pVT_Net);
       fillColor=getVTObjectAttributeInt32("VTFillColour",pVT_Net);
       fillAttr =getVTObjectAttributeInt32("VTFillPattern",pVT_Net);
       setFill=fillType>0;
          //
          if (fillType==1) fillColor=lineColor;
          if (fillType==3) {
            setFill=false;
            refIdx=getVTObjID(pVT_Net,fillAttr);
              if (refIdx>=0){
                getVTObjectFromList(pVT_Net,refIdx);
                aw=getVTObjectAttributeInt32("VTActualWidth" ,pVT_Net);
                ah=getVTObjectAttributeInt32("VTActualHeight",pVT_Net);
                /*
                //TEST
                Serial.println(fillAttr);   
                Serial.println(aw);   
                Serial.println(ah);   
                */
                pVT_Net->objNr=refIdx;
                fillColor=cl_transp;   //pVT_Net->patternTransp;
                setFill=((aw>0) && (ah>0));  
              } 
          }
       getVTObjectFromList(pVT_Net,objIdx);
     }//VTFillAttribute
  }//valid
 return setFill; 
}  

//------------------------------------------------------------------------------
//fillPattern, fill with bitmap
boolean TVTOutputShapeObject::PaintObjToFillBitmap(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net,TVTPoints points[],uint16_t pNr) {
boolean valid=(getVTObjectListSize(pVT_Net)>0);
int16_t  x=pVT_Net->x,y=pVT_Net->y;
uint16_t w=pVT_Net->w,h=pVT_Net->h;
  if ((valid) && (points!=NULL) && (pNr>=3)) {
         if (fillType==3) pVT_Net->VTFillAttribute=fillAttr;
       Set_fillPolygon(pVT_Net,points,pNr,fillColor);
       pVT_Net->x=x;pVT_Net->y=y;pVT_Net->w=w;pVT_Net->h=h;
       pVT_Net->VTFillAttribute=0xFFFF; pVT_Net->VTFillMode=false;pVT_Net->VTEllipseType=0; pVT_Net->lineWidth=1;
  }    
  //
 return valid;  
}

//==============================================================================
//==============================================================================
//type=13
boolean TVTLine::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean  valid=(getVTObjectListSize(pVT_Net)>0),setFill=false;
uint8_t  cIdx=0;
int16_t  x=pVT_Net->x, y=pVT_Net->y,refIdx=-1;
uint16_t w=0,h=0,oLevel=pVT_Net->level;
float    ax,ay,bx,by;
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
    //Paint
    w=VTWidth;h=VTHeight;
    getViewport(pVT_Net,&vRect,x,y,w,h);
    getVTDrawListAddObj(pVT_Net,true,&vRect);
    //
    //get ref VTLineAttributes and VTFillAttributes
    getLineAndFill(VTLineAttributes,0xFFFF,pVT_Net);
    ax=x;ay=y, bx=x+w-1,by=y+h-1;
     //
     if (VTLineDirection) {
      ay=y+h-1,by=y;
     }
     //
     if ((lineWidth>0) && (lineArt>0)) {
       pVT_Net->lineArt=lineArt;
       Set_drawWideLine(pVT_Net,ax,ay,bx,by,lineWidth,lineColor);
       pVT_Net->lineArt=0xFFFF;
     }
   getVTObjectFromList(pVT_Net,objIdx);
   pVT_Net->level=oLevel; 
  }//valid
 return valid;  
}


//------------------------------------------------------------------------------
boolean TVTLine::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTLine::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};

//------------------------------------------------------------------------------
void TVTLine::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTLineAttributes =ww;VTEvent+=2;break;
        case 2:VTWidth =ww;VTEvent+=2;break;
        case 3:VTHeight=ww;VTEvent+=2;break;
        case 4:VTLineDirection   =ww;VTEvent+=1;break;
        //list
        case 5:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};

//------------------------------------------------------------------------------
void TVTLine::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";       VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTLineAttributes";VTAttrAID[1].valueAID=String(VTLineAttributes);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTWidth";         VTAttrAID[2].valueAID=String(VTWidth);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTHeight";        VTAttrAID[3].valueAID=String(VTHeight); 
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTLineDirection"; VTAttrAID[4].valueAID=String(VTLineDirection);
 //
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=2; VTAttrAID[5].nameAID="VTMacros";        VTAttrAID[5].valueAID=VTMacros;
 VT_AID_Nr=6;
};
  

//==============================================================================
//type=14
boolean TVTRectangle::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0),setFill=false;
uint8_t lineSup=0;
int16_t x=pVT_Net->x, y=pVT_Net->y,xx=x, yy=y,ox=x,oy=y;
uint16_t w=0,h=0,ww=w,hh=h,oLevel=pVT_Net->level;
float ax,ay,bx,by,wd;
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
    //Paint
    w=VTWidth;h=VTHeight;ww=w;hh=h;lineSup=VTLineSuppression;
    getVTDrawListAddObj(pVT_Net,true,&vRect);
    //TEST
    //getStreamStrInfo(pVT_Net);
    //Set_drawRect(pVT_Net,x,y,w,h,TFT_BLUE);
    
    //get ref VTLineAttributes and VTFillAttributes
    setFill=getLineAndFill(VTLineAttributes,VTFillAttributes,pVT_Net);
     //
     if (lineWidth>hh/2) lineWidth=hh/2;
     if (lineWidth>ww/2) lineWidth=ww/2;
     //
     if (setFill) {
      yy=yy+lineWidth; hh=hh-2*lineWidth; 
      xx=xx+lineWidth;ww=ww-2*lineWidth;
      getViewport(pVT_Net,&vvRect,xx,yy,ww,hh);
        //
        if ((ww>0) and (hh>0)) {
          //fillPattern, fill with bitmap
          if (fillType==3) {
            TVTPoints points[4];
            points[0].pointX=lineWidth;     points[0].pointY=lineWidth;
            points[1].pointX=lineWidth+ww;  points[1].pointY=lineWidth;
            points[2].pointX=lineWidth+ww;  points[2].pointY=lineWidth+hh;
            points[3].pointX=lineWidth;     points[3].pointY=lineWidth+hh;
            PaintObjToFillBitmap(&vvRect,pVT_Net,points,4);
          }else{
            Set_fillRect(pVT_Net,xx,yy,ww,hh,fillColor); 
          }
        }
        //fillPattern
      pVT_Net->x=x;pVT_Net->y=y;pVT_Net->w=w;pVT_Net->h=h;
     }//setFill
     //   
     //VTLineAttributes
     if ((lineWidth>0) && (lineArt>0)){
       vRect=getViewRect(pViewRect);
       xx=x;yy=y;ww=w;hh=h;
        //VTLineSuppression
        if (lineArt==0) lineSup=0x0F; 
        //
        if ((lineSup & 0x0F)>0) {
          if ((lineSup & 0x01)>0) {yy=yy+lineWidth; hh=hh-lineWidth;} //Top 
          if ((lineSup & 0x04)>0) hh=hh-lineWidth;                    //Bottom
          if ((lineSup & 0x02)>0) ww=ww-lineWidth;                    //Right
          if ((lineSup & 0x08)>0) {xx=xx+lineWidth;ww=ww-lineWidth;}  //Left
        }
        //
       getViewport(pVT_Net,&vRect,xx,yy,ww,hh);
        //
        if (lineArt<0xFFFF) {
         uint8_t k=15;
          for (int i=0;i<w;i+=lineWidth) {
            //
            if (((lineArt>>k) & 0x01)>0) {
              Set_fillRect(pVT_Net,x+i,y,lineWidth,lineWidth,lineColor);
              Set_fillRect(pVT_Net,x+i,y+h-lineWidth,lineWidth,lineWidth,lineColor);
            }
            // 
            if (k==0) k=16;           
           k--;          
          }//for i
         k=15;
          for (int i=0;i<h;i+=lineWidth) {
            //
            if (((lineArt>>k) & 0x01)>0) {
              Set_fillRect(pVT_Net,x+w-lineWidth,y+i,lineWidth,lineWidth,lineColor);
              Set_fillRect(pVT_Net,x,y+i,lineWidth,lineWidth,lineColor);
            }
            // 
            if (k==0) k=16;           
           k--;          
          }

        }else {
          Set_fillRect(pVT_Net,x+lineWidth,y,w-lineWidth,lineWidth,lineColor);
          Set_fillRect(pVT_Net,x+w-lineWidth,y+lineWidth,lineWidth,h-lineWidth,lineColor);
          Set_fillRect(pVT_Net,x,y+h-lineWidth,w-lineWidth,lineWidth,lineColor);
          Set_fillRect(pVT_Net,x,y,lineWidth,h-lineWidth,lineColor);
        }
     }
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid   
 return valid;  
};


//------------------------------------------------------------------------------
boolean TVTRectangle::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTRectangle::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=6;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==6) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
  //
 return valid; 
};


//------------------------------------------------------------------------------
void TVTRectangle::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTLineAttributes =ww;VTEvent+=2;break;
        case 2:VTWidth =ww;VTEvent+=2;break;
        case 3:VTHeight=ww;VTEvent+=2;break;
        case 4:VTLineSuppression   =ww;VTEvent+=1;break;
        case 5:VTFillAttributes   =ww;VTEvent+=2;break;
        //list
        case 6:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTRectangle::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";        VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTLineAttributes"; VTAttrAID[1].valueAID=String(VTLineAttributes);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTWidth";          VTAttrAID[2].valueAID=String(VTWidth);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTHeight";         VTAttrAID[3].valueAID=String(VTHeight);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTLineSuppression";VTAttrAID[4].valueAID=String(VTLineSuppression);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=2; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTFillAttributes"; VTAttrAID[5].valueAID=String(VTFillAttributes);
 //
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=2; VTAttrAID[6].nameAID="VTMacros";         VTAttrAID[6].valueAID=VTMacros;
 VT_AID_Nr=7;
};


//==============================================================================
//type=15
boolean TVTEllipse::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0),setFill=false,setPattern=false;
int16_t x=pVT_Net->x, y=pVT_Net->y,x0=x,y0=y,x1=x,y1=y,sw=0,ew=180,xx=x,yy=y,ox=x,oy=y;
uint16_t w=0,h=0,ww=w,hh=h,oLevel=pVT_Net->level;
TVT_StartEndPoint pp;
float ax=0,ay=0;
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
   //paint
   w=VTWidth;h=VTHeight;pVT_Net->w=w; pVT_Net->h=h;ww=w;hh=h;
     //Paint
     if (VTEllipseType>0) {
      sw=VTStartAngle; ew=VTEndAngle;
     }
     //   
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);
   //
   //get ref VTLineAttributes and VTFillAttributes
   setFill=getLineAndFill(VTLineAttributes,VTFillAttributes,pVT_Net);
     //
     if (lineWidth>w/2) lineWidth=w/2; 
     if (lineWidth>h/2) lineWidth=h/2;
     //
    setFill=(setFill) && (VTEllipseType!=1);
     //   
     if (setFill) {
      pVT_Net->x=x+lineWidth  ;pVT_Net->y=y+lineWidth;
      pVT_Net->w=w-2*lineWidth;pVT_Net->h=h-2*lineWidth;
      pVT_Net->VTEllipseType=VTEllipseType;
      pVT_Net->VTFillMode=true;
      
        //fillPattern, fill with bitmap
        /*
        if (fillType==3) {
          pVT_Net->lineWidth=lineWidth;
          PaintObjToFillBitmap(&vvRect,pVT_Net,NULL,0);
        }else {
          pp=drawArcTicks(pVT_Net,sw,ew,lineWidth,fillColor);//fill Ellipse
        }
        */
        
          if (fillType==3) pVT_Net->VTFillAttribute=fillAttr;
        pp=drawArcTicks(pVT_Net,sw,ew,lineWidth,fillColor);//fill Ellipse
        
        //fillPattern
     }//setFill
     //
   pVT_Net->x=x;pVT_Net->y=y;pVT_Net->w=w;pVT_Net->h=h;
   pVT_Net->VTFillAttribute=0xFFFF; pVT_Net->VTEllipseType=0;pVT_Net->VTFillMode=false;
     //
     if ((lineWidth>0) && (lineArt>0)) {
      pVT_Net->lineArt=lineArt;
      pp=drawArcTicks(pVT_Net,sw,ew,lineWidth,lineColor);
      x=x+w/2;y=y+h/2;
      x0=pp.startX[0]+(pp.startX[1]-pp.startX[0])/2;
      y0=pp.startY[0]+(pp.startY[1]-pp.startY[0])/2;
      x1=pp.endX[2]+(pp.endX[3]-pp.endX[2])/2;
      y1=pp.endY[2]+(pp.endY[3]-pp.endY[2])/2;
       // 
       //Segment close
       if (VTEllipseType==2) {
         Set_drawWideLine(pVT_Net,x,y,x0,y0,lineWidth,lineColor);
         Set_drawWideLine(pVT_Net,x,y,x1,y1,lineWidth,lineColor);
       }
       //Section close
       if (VTEllipseType==3) {
         Set_drawWideLine(pVT_Net,x0,y0,x1,y1,lineWidth,lineColor);
       }
      pVT_Net->lineArt=0xFFFF; 
     }//lineWidth and lineArt  
         
   //TEST
   //Set_resetViewport(pVT_Net);
   //pVT_Net->ImgTFT.drawEllipse(x,y,w/2,h/2,TFT_BLUE); 
   // 
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
 } //valid    
 return valid;  
};



//------------------------------------------------------------------------------
boolean TVTEllipse::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTEllipse::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=8;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==8) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};


//------------------------------------------------------------------------------
void TVTEllipse::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTLineAttributes =ww;VTEvent+=2;break;
        case 2:VTWidth      =ww;VTEvent+=2;break;
        case 3:VTHeight     =ww;VTEvent+=2;break;
        case 4:VTEllipseType=ww;VTEvent+=1;break;
        case 5:VTStartAngle=ww;VTEvent+=1;break;
        case 6:VTEndAngle  =ww;VTEvent+=1;break;
        case 7:VTFillAttributes=ww;VTEvent+=2;break;
        //list
        case 8:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};


//------------------------------------------------------------------------------
void TVTEllipse::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";       VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTLineAttributes";VTAttrAID[1].valueAID=String(VTLineAttributes);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTWidth";         VTAttrAID[2].valueAID=String(VTWidth);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTHeight";        VTAttrAID[3].valueAID=String(VTHeight);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=1; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTEllipseType";   VTAttrAID[4].valueAID=String(VTEllipseType);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTStartAngle";    VTAttrAID[5].valueAID=String(VTStartAngle);
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=1; VTAttrAID[6].nameAID="VTEndAngle";      VTAttrAID[6].valueAID=String(VTEndAngle);
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=2; VTAttrAID[7].typeAID=1; VTAttrAID[7].nameAID="VTFillAttributes";VTAttrAID[7].valueAID=String(VTFillAttributes);
 //
 VTAttrAID[8].numAID=8;  VTAttrAID[8].byteAID=1; VTAttrAID[8].typeAID=2; VTAttrAID[8].nameAID="VTMacros";         VTAttrAID[8].valueAID=VTMacros;
 VT_AID_Nr=9;
};


//==============================================================================
//type=15
boolean TVTPolygon::PaintObjTo(TVT_ViewRect *pViewRect,TVT_Net *pVT_Net) {
boolean valid=(getVTObjectListSize(pVT_Net)>0),setFill=false;
uint8_t optn=0,aIdx=2,bb[2]={0,0};
int16_t x=pVT_Net->x, y=pVT_Net->y;
uint16_t w=0,h=0,i=0,oLevel=pVT_Net->level;
uint8_t err=0x00;
int16_t objIdx=pVT_Net->objNr; 
uint8_t* buff=pVT_Net->streamStr.getBuffer();
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
   //
   //VTPoints=getVTObjectAttributeInt32("VTPoints", pVT_Net);
     if (VTPoints<3) return false;   
   //paint
   w=VTWidth;h=VTHeight;optn=VTPolygonType; pVT_Net->w=w;pVT_Net->h=h;
   getViewport(pVT_Net,&vRect,x,y,w,h);
   getVTDrawListAddObj(pVT_Net,true,&vRect);

     for (i=0;i<pVT_Net->VT_AID_Nr;i++) aIdx+=pVT_Net->VTAttrAID[i].byteAID;
     //
   TVTPoints points[VTPoints];
    for (i=0;i<VTPoints;i++){
     bb[0]=buff[aIdx+4*i+0];
     bb[1]=buff[aIdx+4*i+1];
     points[i].pointX=bb[0] + (bb[1]<<8);
     bb[0]=buff[aIdx+4*i+2];
     bb[1]=buff[aIdx+4*i+3];
     points[i].pointY=bb[0] + (bb[1]<<8);
    }//for i
     //
   //get ref VTLineAttributes and VTFillAttributes
   setFill=getLineAndFill(VTLineAttributes,VTFillAttributes,pVT_Net);
   //
   pVT_Net->lineArt=lineArt;
   setFill=((setFill) && (optn!=3)); //open polygon
     //paint
     //VTFillAttribute, set fill polygon not Open type
     if (setFill) PaintObjToFillBitmap(&vvRect,pVT_Net,points,VTPoints);
     //
   //set polygon line
   pVT_Net->options=optn; 
   getViewport(pVT_Net,&vRect,x,y,w,h);
   SetLinePolygon(pVT_Net,points);
   pVT_Net->lineWidth=1; pVT_Net->options=0;pVT_Net->lineArt=0xFFFF; 
   getVTObjectFromList(pVT_Net,objIdx); 
   pVT_Net->level=oLevel;
  }//valid  
 return valid;  
};


//------------------------------------------------------------------------------
void TVTPolygon::SetLinePolygon(TVT_Net *pVT_Net,TVTPoints points[]){
int16_t x=pVT_Net->x, y=pVT_Net->y, x0=0,y0=0,x1=0,y1=0,x2=0,y2=0, j=0;
uint8_t optn=pVT_Net->options; 
      for (j=0;j<VTPoints;j++) {
        if ((j>0) && (lineWidth>0) && (lineArt>0)) {
          x0=points[j-1].pointX; y0=points[j-1].pointY;
          x1=points[j].pointX;   y1=points[j].pointY;
          Set_drawWideLine(pVT_Net,x0+x,y0+y,x1+x,y1+y,lineWidth,lineColor);
           //
           if ((j==VTPoints-1) && (optn!=3)) {
             x0=points[j].pointX; y0=points[j].pointY;
             x1=points[0].pointX; y1=points[0].pointY;
             Set_drawWideLine(pVT_Net,x0+x,y0+y,x1+x,y1+y,lineWidth,lineColor);
           }
        }
      }//for j
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
boolean TVTPolygon::writeToStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
 return writeToStreamDirect(pVT_Net,pStream);
};

//------------------------------------------------------------------------------
boolean TVTPolygon::readFromStream(TVT_Net *pVT_Net,LoopbackStream *pStream){
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
    for (i=6;i<VT_AID_Nr;i++) {
      nn=VTAttrAID[i].valueAID.toInt(); 
       if (i==6) getVTPoints(pVT_Net,nn, pStream);
       if (i==7) getVTMacros(pVT_Net,nn, pStream);
    }//for i
   getVTObjectListAdd(pVT_Net);
   pVT_Net->streamStr.clear();
  }//valid
 return valid; 
};

//------------------------------------------------------------------------------
void TVTPolygon::getAID(){
uint32_t ww=0;
uint16_t i=0,j=0;
 VTEvent=3;
    for (i=1;i<VT_AID_Nr;i++){
     ww=0;
       for (j=0;j<VTAttrAID[i].valueAID.length();j++){
        ww+=char(VTAttrAID[i].valueAID[j])<<8*j;  
       }//for j
       switch (i) {
        case 1:VTWidth      =ww;VTEvent+=2;break;
        case 2:VTHeight     =ww;VTEvent+=2;break;
        case 3:VTLineAttributes =ww;VTEvent+=2;break;
        case 4:VTFillAttributes=ww;VTEvent+=2;break;
        case 5:VTPolygonType=ww;VTEvent+=1;break;
        //list
        case 6:VTPoints=ww;VTEvent+=1;VTEvent+=4*ww;break;
        case 7:VTMacros=ww;VTEvent+=1;break; 
       }//switch i
    }//for i
    if (VTMacros==0) VTEvent=0;
};



//------------------------------------------------------------------------------
void TVTPolygon::setAID(){
 VTAttrAID[0].numAID=0;  VTAttrAID[0].byteAID=1; VTAttrAID[0].typeAID=0; VTAttrAID[0].nameAID="VTObjType";       VTAttrAID[0].valueAID=String(VTObjType);
 VTAttrAID[1].numAID=1;  VTAttrAID[1].byteAID=2; VTAttrAID[1].typeAID=1; VTAttrAID[1].nameAID="VTWidth";         VTAttrAID[1].valueAID=String(VTWidth);
 VTAttrAID[2].numAID=2;  VTAttrAID[2].byteAID=2; VTAttrAID[2].typeAID=1; VTAttrAID[2].nameAID="VTHeight";        VTAttrAID[2].valueAID=String(VTHeight);
 VTAttrAID[3].numAID=3;  VTAttrAID[3].byteAID=2; VTAttrAID[3].typeAID=1; VTAttrAID[3].nameAID="VTLineAttributes";VTAttrAID[3].valueAID=String(VTLineAttributes);
 VTAttrAID[4].numAID=4;  VTAttrAID[4].byteAID=2; VTAttrAID[4].typeAID=1; VTAttrAID[4].nameAID="VTFillAttributes";VTAttrAID[4].valueAID=String(VTFillAttributes);
 VTAttrAID[5].numAID=5;  VTAttrAID[5].byteAID=1; VTAttrAID[5].typeAID=1; VTAttrAID[5].nameAID="VTPolygonType";   VTAttrAID[5].valueAID=String(VTPolygonType);
 //
 VTAttrAID[6].numAID=6;  VTAttrAID[6].byteAID=1; VTAttrAID[6].typeAID=2; VTAttrAID[6].nameAID="VTPoints";         VTAttrAID[6].valueAID=VTPoints;
 VTAttrAID[7].numAID=7;  VTAttrAID[7].byteAID=1; VTAttrAID[7].typeAID=2; VTAttrAID[7].nameAID="VTMacros";         VTAttrAID[7].valueAID=VTMacros;
 VT_AID_Nr=8;
};


//==============================================================================
//==============================================================================
