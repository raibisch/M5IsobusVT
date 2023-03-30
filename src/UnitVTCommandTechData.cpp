//==============================================================================
//Definition objects TVTCommandTechDataObject
//TVTGetHardware, TVTGetNumberOfSoftKeys, TVTGetTextFontData,TVTGetMemory,
//TVTGetSupportedWideChars,
//
//TVTGetVersion, TVTStoreVersion, TVTLoadVersion, TVTDeleteVersion,TVTDeleteObjectPool,
//TVTExtendedGetVersion, TVTExtendedStoreVersion, TVTExtendedLoadVersion, TVTExtendedDeleteVersion,
//
//TVTGetWindowMaskData,TVTGetSupportedObjects,TVTIdentifyVT, TVTScreenCapture
//TVTProprietaryCommand
//TVT_DiagConformance
//==============================================================================
//unit UnitVTCommandTechData.cpp
//==============================================================================

#include "UnitVTCommandTechData.h"



//==============================================================================
boolean VT_CAN_Transmit(TVT_Net *pVT_Net,CANMsg *pMsg){
boolean sendMSG=false;
String str="";
String ss="";
String objStr="";
uint16_t objID=0xFFFF,w=pVT_Net->w,h=pVT_Net->h,ww=w,hh=h;
int16_t x=pVT_Net->x,y=pVT_Net->y, xx=x,yy=y;
boolean serialOut_old=pVT_Net->serialOut;
TVT_ViewRect ViewRect;
 if (pVT_Net->CAN_active)
 {
  //send VTStatus
  if (millis()-pVT_Net->pTime>=1000) {
    pVT_Net->Flash++;
      //
      if (pVT_Net->FlashList.length()>0) {
         if ((pVT_Net->Flash==1) || (pVT_Net->Flash==3)) {
          str=pVT_Net->FlashList; pVT_Net->FlashList="";
           //
           while (str.indexOf("\n")>0) {
            ss=str.substring(0,str.indexOf("\n")); str.remove(0,str.indexOf("\n")+1);
            objID=ss.substring(0,ss.indexOf(",")).toInt(); ss.remove(0,ss.indexOf(",")+1);
            //
            xx=ss.substring(0,ss.indexOf(",")).toInt(); ss.remove(0,ss.indexOf(",")+1);
            yy=ss.substring(0,ss.indexOf(",")).toInt(); ss.remove(0,ss.indexOf(",")+1);
            ww=ss.substring(0,ss.indexOf(",")).toInt(); ss.remove(0,ss.indexOf(",")+1);
            hh=ss.substring(0,ss.indexOf(",")).toInt(); ss.remove(0,ss.indexOf(",")+1);
            //  
            TVT_ViewRect ViewRect;
            ViewRect.viewX=xx; ViewRect.viewY=yy;
            ViewRect.viewW=ww; ViewRect.viewH=hh;
            pVT_Net->x=xx;pVT_Net->y=yy; pVT_Net->w=ww;pVT_Net->h=hh;
            getVTObjectFromList(pVT_Net,getVTObjID(pVT_Net,objID));
           }//while
           //
          pVT_Net->VTPoolRefresh=true;
          Serial.println("FLASH");
          //setSerialPrint(pVT_Net,delm0);
          pVT_Net->x=x;pVT_Net->y=y; pVT_Net->w=w;pVT_Net->h=h;
         }
      }//flash list>0
      //    
      if (pVT_Net->Flash>=4) pVT_Net->Flash=0;
      //
      //VT-Instance number to tft
      if (pVT_Net->VTInstance>=0){
       Set_resetViewport(pVT_Net);
       Set_fillRect(pVT_Net,0,0,16,16,1);
        if (pVT_Net->Flash==1) {
          //TEST
          Serial.println("VTInstance=" + String(pVT_Net->VTInstance));          
          Set_setTextColor(pVT_Net,0);
          Set_setCursor(pVT_Net,2,2);
          Set_drawGlyph(pVT_Net,0x0030 + pVT_Net->VTInstance);
        }
      }//instance number
      //
    //TX1 1CE6FF26  8  FE 26 FF FF FF FF 00 FF
    //VT_Status message
    TVT_VTStatusMessage msgObj;
    sendMSG=msgObj.setMsgToAttr (pMsg,pVT_Net);
    getVTActiveMask(pVT_Net);
    pVT_Net->pTime=millis();
  } //millis
  //
  //send TP DATA
  if (pVT_Net->TP_DATA_ACTIVE) {
     pMsg->ID=(TP_ETP_PRIO<<24) + (TP_DT) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) +  pVT_Net->VT_SRC;
     //TP-DATA active
    TVTTransportProtocol msgObj;
    sendMSG=msgObj.getTransportProtcol(pMsg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
  }
  //
  // by JG: Klammern bei If Abfrge
  if (sendMSG) {VT_CAN_MsgSend(pVT_Net,pMsg); }  //CAN0.sendMsgBuf(0x1CE6FF26, 1, 8, data);
  //
  
 return sendMSG;
 }//can_active 
 // by JG: return auch auserhalb von can active
 return sendMSG;
}//VT_CAN_Transmit



//==============================================================================
//write/read Image
//==============================================================================
//get screenshot picture from screen
boolean writeImageDirect(TVT_Net *pVT_Net){
uint8_t  bitDef=2;
uint16_t w=pVT_Net->w,h=pVT_Net->h;
int16_t  x=pVT_Net->x, y=pVT_Net->y,yy=y;
uint32_t wh=bitDef*w*h, sz=wh+54;
   pVT_Net->streamStr.clear(); 
   //signature
   pVT_Net->streamStr.writeBytesVal(0x4D42,2);
    //filesize
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //reserved
   sz=0;        
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //DataOffset
   sz=0x36; //54        
   pVT_Net->streamStr.writeBytesVal(sz,4);
    //
   //size of info header
   sz=0x28; //40
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //Width
   sz=w;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //Height
   sz=h;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //Planes
   sz=1;
   pVT_Net->streamStr.writeBytesVal(sz,2);
   //BitCount
   sz=8*bitDef;
   pVT_Net->streamStr.writeBytesVal(sz,2);
   //Compression
   sz=0;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //ImageSize=0
   sz=0;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //xPixelsPerM
   sz=0;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //yPixelsPerM
   sz=0;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //ColorsUsed
   sz=0;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //ColorsImportant
   sz=0;
   pVT_Net->streamStr.writeBytesVal(sz,4);
   //
   //getArray8Info(pVT_Net,(uint8_t*) buff,64);
   //
   uint8_t tft_buffer[bitDef*w];
   Set_alpha(pVT_Net,0x00);
   Set_setScreenShot(pVT_Net,true);
   yy=y+h-1;   
     /*
     while(yy>=y) {
      //pVT_Net->tft.readRectRGB(x, yy, w, 1, (uint8_t *)tft_buffer);
      pVT_Net->tft.readRect(x, yy, w, 1, (uint16_t *)tft_buffer);
      pVT_Net->streamStr.writeBytes((uint8_t*) tft_buffer,bitDef*w);
      yy--;
     }//while    
     */
   yy=y+h-1;   
     while(yy>=y) {
       Set_pushImageAlpha(pVT_Net,x,yy,w,1,(uint16_t*) tft_buffer,NULL,0xFFFF);
       pVT_Net->streamStr.writeBytes((uint8_t*) tft_buffer,bitDef*w);
       yy--;
     }
   
   pVT_Net->streamStr.setPos(0);
   Set_alpha(pVT_Net,0xFF);
   Set_setScreenShot(pVT_Net,false);
 return (pVT_Net->streamStr.available()>0);
};//writeImageDirect


//------------------------------------------------------------------------------
boolean writeImageScreen(fs::FS &fs, const char * path,TVT_Net *pVT_Net){
boolean valid=false;
uint32_t nn=0;
 getHeapStatus(pVT_Net,10);
  if (writeImageDirect(pVT_Net)) {
   uint8_t* buff=pVT_Net->streamStr.getBuffer();
   nn=pVT_Net->streamStr.available();
   Serial.println(nn);
     //
     if (fs.exists(path)) fs.remove(path);
   //   
   File file = fs.open(path, FILE_WRITE);
     if (file) {
       Serial.println(path);
       
       
       valid=file.write((uint8_t*) buff,nn);
       file.close();
     }//file  
     //
    Serial.println("writeImage");
  }
 getHeapStatus(pVT_Net,30);
 return valid;
};//writeImageScreen


//------------------------------------------------------------------------------
boolean readImageScreen(fs::FS &fs, const char *path, TVT_Net *pVT_Net) {
boolean valid=false;
uint32_t len=0;
uint8_t* buff=pVT_Net->streamStr.getBuffer();
TVTPixelXY pXY;
 getHeapStatus(pVT_Net,10);
 File bmpFS = fs.open(path,FILE_READ);
 Serial.println(path);
  //
  if ((bmpFS) && (bmpFS.available())){
   
   pVT_Net->streamStr.clear();
   len=bmpFS.available();
   len=bmpFS.readBytes((char*) buff,len); 
   pVT_Net->streamStr.setSize(len);
   
   pVT_Net->streamStr.setPos(0);
   Serial.println(pVT_Net->streamStr.available());
   //getArray8Info(pVT_Net,(uint8_t*) buff,16);
   //
   getHeapStatus(pVT_Net,20);
   Set_setScreenShot(pVT_Net,true);
   valid=GraphicDataPaintObjTo(pVT_Net,&pXY, 0);
   Set_setScreenShot(pVT_Net,false);
   //
   pVT_Net->streamStr.clear();
   bmpFS.close();
  }
 getHeapStatus(pVT_Net,30);
 return valid;
};//readImageScreen



//==============================================================================
//Transport protocol
//==============================================================================
//SEND MODE
boolean TVTTransportProtocol::getTransportProtcol(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream,uint32_t rPGN){
boolean sendMSG=false,setEOMA=false, respMSG=false;
uint8_t src=0x0A, dst=pVT_Net->VT_SRC; //0x26;
uint32_t reqPGN=VTtoECU_PGN;
int i=0;
   if (rPGN>0) reqPGN=rPGN;
 // 
 pMsg->MSG_TX=1; pMsg->MSGTYPE=1;pMsg->LEN=8;
 dst=(pMsg->ID>>8) & 0xFF; 
 respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
 //
 src=(pMsg->ID>>0) & 0xFF;
  if (src>=0xFE) src=0xFF;
  //
  //send protocol
  ///---------------------------------------
  //TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
  //[TP.CM_RTS_E600_00_0]  Size=9  Packets=2
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC)) {
      //
      //setECUAddress(pVT_Net,dst);
      //
      pMsg->DATA[0]=TP_RTS;     
      pStream->setPos(0);
      pVT_Net->TP_SIZE=pStream->available(); 
      //
      pVT_Net->TP_DATA=0x0001;
      pVT_Net->TP_DATA_NR=0;
      pMsg->DATA[1]=(pVT_Net->TP_SIZE>>0) & 0xFF;
      pMsg->DATA[2]=(pVT_Net->TP_SIZE>>8) & 0xFF;
      pVT_Net->TP_SIZE=(pMsg->DATA[2]<<8) + (pMsg->DATA[1]<<0);
      pVT_Net->TP_SIZE0=pVT_Net->TP_SIZE;      
      pVT_Net->TP_Block=pVT_Net->TP_SIZE/7;  
       if (pVT_Net->TP_SIZE % 7>0) pVT_Net->TP_Block++;      
      pMsg->DATA[3]=pVT_Net->TP_Block;
      pMsg->DATA[4]=0xFF;
      
      pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
      pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
      pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
      
      pVT_Net->TP_DATA_ACTIVE=false;
      sendMSG=true;
  }//TP_RTS

  //TP_DATA
  //RX3 18EB260A  8  01 11 00 00 00 0F 01 64
  if (((pMsg->ID & 0x00FF0000)==TP_DT) && (src=pVT_Net->VT_SRC)){
     if (pStream->available()) {
        pMsg->DATA[0]=pVT_Net->TP_DATA;pVT_Net->TP_DATA++; 
        //pVT_Net->TP_Block;
        //
        //TP_DATA_stream
        for (i=1; i<pMsg->LEN;i++) {
          pMsg->DATA[i]=0xFF;
            if (pStream->available()) pMsg->DATA[i]=pStream->read();
        }
      sendMSG=true;
     }
    pVT_Net->TP_DATA_ACTIVE=(pStream->available());
  }//TP_DATA
  
  //
  //ETP_RTS
  //TX1 1CC8260A  8  14 7F 13 00 00 00 E7 00
  //[ETP.CM_RTS_E700_00_0]  Size=4991
  if (((pMsg->ID & 0x00FF0000)==ETP_PGN) && (src=pVT_Net->VT_SRC)) {
    pMsg->DATA[0]=ETP_RTS;
    pVT_Net->ETP_SIZE=pStream->available(); 
    //      
    pVT_Net->ETP_DATA=0x0001;
    pVT_Net->ETP_DATA_NR=0;
    pVT_Net->ETP_Block=0xFF;
    pMsg->DATA[4]=(pVT_Net->ETP_SIZE>>24) & 0xFF;
    pMsg->DATA[3]=(pVT_Net->ETP_SIZE>>16) & 0xFF;
    pMsg->DATA[2]=(pVT_Net->ETP_SIZE>>8) & 0xFF;
    pMsg->DATA[1]=(pVT_Net->ETP_SIZE>>0) & 0xFF;
    pVT_Net->ETP_SIZE0=pVT_Net->ETP_SIZE;      
    //
    pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
    pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
    pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
    pVT_Net->ETP_DATA_ACTIVE=false;
    sendMSG=true;
  }//ETP_RTS
  //  
 return sendMSG;
};//TVTTransportProtocol::getTransportProtcol



//==============================================================================
//RECEIVE MODE
boolean TVTTransportProtocol::setTransportProtcol(CANMsg *pMsg,TVT_Net *pVT_Net,uint32_t rPGN){
boolean sendMSG=false,setEOMA=false, respMSG=false;
uint8_t src=0x0A, dst=pVT_Net->VT_SRC; //0x26;
uint8_t mdata[7];
uint32_t reqPGN=ECUtoVT_PGN;
int i=0;
  if (rPGN>0) reqPGN=rPGN;
 pMsg->MSG_TX=1; pMsg->MSGTYPE=1;pMsg->LEN=8;
 dst=(pMsg->ID>>8) & 0xFF;
 respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
 //
 src=(pMsg->ID>>0) & 0xFF;
  if (src>=0xFE) src=0xFF;
  // 
  //---------------
  //TP-PROTOCOL
  //---------------
  //TX1 1CEC260A  8  10 F9 06 FF FF 00 E7 00
  //[TP.CM_RTS_E700_00_0]  Size=1785  Packets=255
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (dst=pVT_Net->VT_SRC)) {
    if ((pMsg->DATA[0]==TP_RTS) && (reqPGN==ECUtoVT_PGN)) {   
      pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
      //
      pVT_Net->TP_DATA=0x0001;
      pVT_Net->TP_DATA_NR=0;
      pVT_Net->TP_SIZE=(pMsg->DATA[2]<<8) + (pMsg->DATA[1]<<0);
      pVT_Net->TP_SIZE0=pVT_Net->TP_SIZE;      
      pVT_Net->TP_Block=pVT_Net->TP_SIZE/7;  
       if (pVT_Net->TP_SIZE % 7>0) pVT_Net->TP_Block++;      
      pVT_Net->stream_TP[pVT_Net->listNr].clear();
      //
      pMsg->DATA[0]=TP_CTS; //TP_CTS
      pMsg->DATA[1]=pVT_Net->TP_Block;
      pMsg->DATA[2]=(pVT_Net->TP_DATA & 0xFF);
      pMsg->DATA[3]=0xFF;
      pMsg->DATA[4]=0xFF;
      pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
      pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
      pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
      sendMSG=true;
    }   
  }//TP_RTS
  //      uint8_t*  ptr= buff;      

  //TP_.CM_CTS
  //TX1 18EC260A  8  11 04 01 FF FF 00 E6 00
  //[TP.CM_CTS_E600_00_0]  Packets=4  Next=1
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (dst=pVT_Net->VT_SRC)) {
   reqPGN=VTtoECU_PGN; 
    if (pMsg->DATA[0]==TP_CTS) {   
     //send TP data
     pVT_Net->TP_DATA_ACTIVE=pVT_Net->stream_TP[pVT_Net->listNr].available();
    }
  }//TP_CTS
  //
  //TP_DATA
  //RX3 18EB260A  8  01 11 00 00 00 0F 01 64
  if (((pMsg->ID & 0x00FF0000)==TP_DT) && (dst=pVT_Net->VT_SRC)){
    //TP_DATA_stream
    for (i=1; i<pMsg->LEN;i++) pVT_Net->stream_TP[pVT_Net->listNr].write(pMsg->DATA[i]); 
    //
    if (pMsg->DATA[0]==pVT_Net->TP_Block) {   
      pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
      //
      pVT_Net->TP_DATA=pVT_Net->TP_DATA + pVT_Net->TP_Block;
      pMsg->DATA[0]=TP_CTS; //TP_CTS
      pMsg->DATA[1]=pVT_Net->TP_Block;
      pMsg->DATA[2]=(pVT_Net->TP_DATA & 0xFF);
      pMsg->DATA[3]=0xFF;
      pMsg->DATA[4]=0xFF;
      pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
      pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
      pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
       //
       if (pVT_Net->TP_SIZE-7*pVT_Net->TP_Block>=0) {
         pVT_Net->TP_SIZE=pVT_Net->TP_SIZE-7*pVT_Net->TP_Block;
       }
       //
       if ((pVT_Net->TP_SIZE>0) && (pVT_Net->TP_SIZE<=7*pVT_Net->TP_Block)) {
        pVT_Net->TP_Block=pVT_Net->TP_SIZE/7; //trunc 
          if (pVT_Net->TP_SIZE % 7>0) pVT_Net->TP_Block++;
        pMsg->DATA[1]=pVT_Net->TP_Block;
        setEOMA=true;
       }
       //
       if (pVT_Net->TP_SIZE<=7*pVT_Net->TP_Block) pVT_Net->TP_EOMA_ACTIVE=setEOMA;
       //
       if ((pVT_Net->TP_SIZE<=0) || (pVT_Net->TP_EOMA_ACTIVE)) {
        //pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
        pMsg->DATA[0]=0x13;  //TP_EOMA_E700  
        pMsg->DATA[1]=(pVT_Net->TP_SIZE0 >> 0)  & 0xFF;  //TP_SIZE0  
        pMsg->DATA[2]=(pVT_Net->TP_SIZE0 >> 8)  & 0xFF;  //TP_SIZE0  
        pMsg->DATA[3]=0xFF;  //TP_SIZE0  
        pMsg->DATA[4]=0xFF;  //TP_SIZE0 
        //
        pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
        pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
        pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
        setEOMA=false;
       }
       //
      pVT_Net->TP_EOMA_ACTIVE=setEOMA;       
      sendMSG=true; 
    }//TP_Block   
  }//TP_DATA
  //
  //---------------
  //ETP-PROTOCOL
  //---------------
  //
  //TX1 1CC8260A  8  14 7F 13 00 00 00 E7 00
  //[ETP.CM_RTS_E700_00_0]  Size=4991
  if (((pMsg->ID & 0x00FF0000)==ETP_PGN) && (dst=pVT_Net->VT_SRC)) {
    if ((pMsg->DATA[0]==ETP_RTS) && (reqPGN==ECUtoVT_PGN)) {   
      pMsg->ID=(TP_ETP_PRIO<<24) + ETP_PGN + (src<<8) + pVT_Net->VT_SRC;
      //      
      pVT_Net->ETP_DATA=0x0001;
      pVT_Net->ETP_DATA_NR=0;
      pVT_Net->ETP_Block=0xFF;
      pVT_Net->ETP_SIZE=(pMsg->DATA[4]<<24) + (pMsg->DATA[3]<<16) + (pMsg->DATA[2]<<8) + (pMsg->DATA[1]<<0);
      pVT_Net->ETP_SIZE0=pVT_Net->ETP_SIZE;      
      //
      pVT_Net->stream_ETP[pVT_Net->listNr].clear();
      //
      pMsg->DATA[0]=ETP_CTS; //ETP_CTS
      pMsg->DATA[1]=pVT_Net->ETP_Block;
      pMsg->DATA[2]=(pVT_Net->ETP_DATA & 0xFF);
      pMsg->DATA[3]=(pVT_Net->ETP_DATA >> 8) & 0xFF;
      pMsg->DATA[4]=0x00;
      pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
      pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
      pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
      sendMSG=true;
    }   
  }//ETP_RTS
  //
  //
  //ETP_.CM_CTS
  //TX1 18EC260A  8  11 04 01 FF FF 00 E6 00
  //[TP.CM_CTS_E600_00_0]  Packets=4  Next=1
  if (((pMsg->ID & 0x00FF0000)==ETP_PGN) && (dst=pVT_Net->VT_SRC)) {
    if ((pMsg->DATA[0]==ETP_CTS) && (reqPGN==VTtoECU_PGN)) {   
     //send ETP data
     //pVT_Net->ETP_DATA_ACTIVE=pVT_Net->streamETP[pVT_Net->listNr]->available();
     
     pVT_Net->ETP_DATA_ACTIVE=pVT_Net->stream_ETP[pVT_Net->listNr].available();
     
    }
  }//ETP_CTS
  //  
  //
  //
  //VT_Net.ETP_DATA
  //TX1 18C7260A  8  01 11 3E 00 1C 18 00 AF
  if ((pMsg->ID & 0x00FF0000)==ETP_DT) {
    //pVT_Net->ETP_DATA_stream
    for (i=1; i<pMsg->LEN;i++) pVT_Net->stream_ETP[pVT_Net->listNr].write(pMsg->DATA[i]);
    //
    if (pMsg->DATA[0]==pVT_Net->ETP_Block) {   
      pMsg->ID=(TP_ETP_PRIO<<24) + ETP_PGN + (src<<8) + pVT_Net->VT_SRC;
      //
      pVT_Net->ETP_DATA=pVT_Net->ETP_DATA + pVT_Net->ETP_Block;
      pMsg->DATA[0]=ETP_CTS; //ETP_CTS
      pMsg->DATA[1]=pVT_Net->ETP_Block;
      pMsg->DATA[2]=(pVT_Net->ETP_DATA & 0xFF);
      pMsg->DATA[3]=(pVT_Net->ETP_DATA >> 8) & 0xFF;
      pMsg->DATA[4]=0x00;
      pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
      pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
      pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
       //
       if (pVT_Net->ETP_SIZE-7*pVT_Net->ETP_Block>=0) {
         pVT_Net->ETP_SIZE=pVT_Net->ETP_SIZE-7*pVT_Net->ETP_Block;
       };
       //
       if ((pVT_Net->ETP_SIZE>0) && (pVT_Net->ETP_SIZE<7*pVT_Net->ETP_Block)) {
        pVT_Net->ETP_Block=pVT_Net->ETP_SIZE/7; //trunc 
          if (pVT_Net->ETP_SIZE % 7>0) pVT_Net->ETP_Block++;
        pMsg->DATA[1]=pVT_Net->ETP_Block;
        setEOMA=true;
       }
       //
       if ((pVT_Net->ETP_SIZE<=0) || (pVT_Net->ETP_EOMA_ACTIVE)) {
        pMsg->DATA[0]=ETP_EOMA;  //ETP_EOMA_E700  
        pMsg->DATA[1]=(pVT_Net->ETP_SIZE0 >> 0)  & 0xFF;  //ETP_SIZE0  
        pMsg->DATA[2]=(pVT_Net->ETP_SIZE0 >> 8)  & 0xFF;  //ETP_SIZE0  
        pMsg->DATA[3]=(pVT_Net->ETP_SIZE0 >> 16) & 0xFF;  //ETP_SIZE0  
        pMsg->DATA[4]=(pVT_Net->ETP_SIZE0 >> 24) & 0xFF;  //ETP_SIZE0  
        pMsg->DATA[5]=(reqPGN>>8)  & 0xFF;
        pMsg->DATA[6]=(reqPGN>>16) & 0xFF;
        pMsg->DATA[7]=(reqPGN>>24) & 0xFF;
        setEOMA=false;
       }
       //
      pVT_Net->ETP_EOMA_ACTIVE=setEOMA;
      sendMSG=true; 
    }//VT_Net.ETP_Block   
  }//ETP_DATA
  
 return sendMSG;
  //
};//TVTTransportProtocol::setTransportProtcol



//==============================================================================
//==============================================================================
//AddrClm_PGN to WorkingSetName
//TVTAddressClaim Implementaion    WorkingSetName
String TVTAddressClaim::setWorkingSetName(CANMsg *pMsg,TVT_Net *pVT_Net){
String wsName="",ws="";
boolean setWS=true;
int i=0;
uint8_t src=(pMsg->ID) & 0xFF;
   for (i=0;i<2;i++) pVT_Net->VT_DST[i]=0xFF;
   //set WorkingSetName
   for (i=0;i<pMsg->LEN;i++) ws=getStringHEX(pMsg->DATA[i],2) + ws;     
 ws=getStringHEX(src,2) + " " + ws;
 //TEST
 //Serial.println("WS_List=" + ws);
   //for (i=0;i<WS_ListCount;i++) Serial.println(pVT_Net->WS_List[i]);
   //
   for (i=0;i<WS_ListCount;i++) {
     if (pVT_Net->WS_List[i]==ws) {
      setWS=false;
      break;
     }  
   }
   if (setWS) {
     for (i=0;i<WS_ListCount;i++) {
      if ( pVT_Net->WS_List[i]=="") {
        wsName=ws;
        pVT_Net->WS_List[i]=wsName;
        if (src==pVT_Net->VT_SRC) pVT_Net->VT_SRC_WS=wsName;
        if (src==pVT_Net->VT_DST[pVT_Net->listNr]) pVT_Net->VT_DST_WS[pVT_Net->listNr]=wsName;
        break;
      }
     }
   }
   if (pVT_Net->VT_InfoMode>0) {
     for (i=0;i<WS_ListCount;i++) Serial.println(pVT_Net->WS_List[i]);
   }
   //
  return wsName;     
};//TVTAddressClaim::setWorkingSetName



//------------------------------------------------------------------------------
//RX1 18EEFE26  8  64 00 60 1F 00 1D 00 20
//WorkingSetName=20001D001F600064
String TVTAddressClaim::setWSNameSetToMsgAndStr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t i=0;
String msgStr="";
  // copy nameId to array
  pMsg->DATA[0]=  (VT_ID & 0x000000FF) & 0xFF;
  pMsg->DATA[1]= ((VT_ID & 0x0000FF00) >> 8) & 0xFF;
  pMsg->DATA[2]= ((VT_ID >> 16) & 0x1F);
  // copy nameManufacturer to array
  pMsg->DATA[2]=pMsg->DATA[2] | ((VT_Manufacture & 0x07) << 5);
  pMsg->DATA[3]= ((VT_Manufacture >> 3) & 0xFF);
  // ecu instance
  pMsg->DATA[4]= (VT_ECUInstance & 0x07);
  // function instance
  pMsg->DATA[4]=pMsg->DATA[4] | ((VT_FunctionInstance & 0x1F) << 3);
  // function
  pMsg->DATA[5]= VT_Function;
  // system
  pMsg->DATA[6]= ((VT_DeviceClass & 0x7F) << 1);
  // Reserve
  pMsg->DATA[6]= pMsg->DATA[6] | (VT_ReserveBit & 0xFF);
  // system instance
  pMsg->DATA[7]= (VT_DevClassInst & 0x1F);
  // industry group
  pMsg->DATA[7]=pMsg->DATA[7] | ((VT_IndustryGroup & 0x07) << 4);
  // arbitrary
  //deviceName[7] |= ((arbitrary & 0x1) << 7);
  pMsg->DATA[7]=pMsg->DATA[7] | ((VT_Arbitrary & 0x01) << 7);
  //
    for (i=0;i<pMsg->LEN;i++) msgStr+=getStringHEX(pMsg->DATA[pMsg->LEN-1-i],2);
  //Serial.println(msgStr);  
return msgStr;
};//TVTAddressClaim::setWSNameSetToMsgAndStr


//------------------------------------------------------------------------------
//RX1 18EEFE26  8  64 00 60 1F 00 1D 00 20
//WorkingSetName=20001D001F600064
void TVTAddressClaim::getWSNameFromMsg(CANMsg *pMsg,TVT_Net *pVT_Net){
    //arbitrary
    VT_Arbitrary=(pMsg->DATA[7] >> 7) & 0x01;
    //Reserve Bit
    VT_ReserveBit=(pMsg->DATA[6]) & 0x01;
    //industryGroup
    VT_IndustryGroup=(pMsg->DATA[7] >>  4) & 0x07;
    //systemInstance
    VT_DevClassInst=(pMsg->DATA[7] & 0x0F);
    //system
    VT_DeviceClass=(pMsg->DATA[6] >> 1) & 0x7F;
    //ecuFunc
    VT_Function=(pMsg->DATA[5]);
    //ecuFuncInstance
    VT_FunctionInstance=(pMsg->DATA[4] >> 3) & 0x1F;
    //ecuInstance
    VT_ECUInstance=(pMsg->DATA[4]) & 0x07;

    //nameManufacturer
    VT_Manufacture=256*((pMsg->DATA[3] >> 5) & 0x07) + ((pMsg->DATA[3] << 0x03) & 0xF8) +((pMsg->DATA[2] >> 5) & 0x07);
    //
    // copy nameId to array
    //nameId
    VT_ID=256*256*(pMsg->DATA[2] & 0x1F) + 256*pMsg->DATA[1] + pMsg->DATA[0];
};//TVTAddressClaim::getWSNameFromMsg

//------------------------------------------------------------------------------
//TVTAddressClaim Implementaion    AddrClm_PGN
boolean TVTAddressClaim::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false,setWS=true;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
boolean  respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
int i=0;
uint32_t reqPGN=(pMsg->DATA[2]<<24) + (pMsg->DATA[1]<<16) + (pMsg->DATA[0]<<8);
     //
     //AddrClm_PGN to WorkingSetName
     if ((respMSG) && (reqPGN==AddrClm_PGN)) {
       pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
        for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
       pMsg->ID=(AddrClm_PRIO<<24) + reqPGN + (src<<8) + pVT_Net->VT_SRC;
       //
       setWSNameSetToMsgAndStr(pMsg,pVT_Net);
       //set WorkingSetName
       setWorkingSetName(pMsg,pVT_Net);
       //
       valid=true;
     }   
 return valid; 
};//TVTAddressClaim::setMsgToAttr


//==============================================================================
//TVTAddressClaimResponse Implementaion    AddrClm_PGN
boolean TVTAddressClaimResponse::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
  //set WorkingSetName
  setWorkingSetName(pMsg,pVT_Net);
  //
  return valid; 
};//TVTAddressClaimResponse::setMsgToAttr


//==============================================================================
//TVTLanguage Implementaion    request_PGN
boolean TVTLanguage::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
boolean  respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
int i=0;
uint32_t reqPGN=(pMsg->DATA[2]<<24) + (pMsg->DATA[1]<<16) + (pMsg->DATA[0]<<8);
     //Language
     if ((respMSG) && (reqPGN==LANGUAGE_PGN)) {
      pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
        for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
      pMsg->ID=(AddrClm_PRIO<<24) + reqPGN + pVT_Net->VT_SRC;
      pMsg->DATA[0]=0x65;
      pMsg->DATA[1]=0x6E;
      pMsg->DATA[2]=0x00;
      pMsg->DATA[3]=0x00;
      pMsg->DATA[4]=0x00;
      pMsg->DATA[5]=0x00;
      pMsg->DATA[6]=0xFF;
      pMsg->DATA[7]=0xFF;
      //
      valid=true;  
     }
 return valid; 
};//TVTLanguage::setMsgToAttr



//------------------------------------------------------------------------------
/*
246.800 RX1 18EAFFFE  3  E6 FE 00
Request_PGN (00)=00FEE6
[FEE6] TimeDate (00)

246.800 TX1 18FEE626  8  15 08 13 01 68 26 7D 7D
[FEE6] TimeDate (00)
TimeDate_LOC=26.01.2023 19:08:05
TimeDate_UTC=26.01.2023 19:08:05*
Byte0=[15]  21    Seconds=5.25
Byte1=[08]  8     Minutes=8
Byte2=[13]  19    Hour=19
Byte3=[01]  1     Month=1
Byte4=[68]  104   Day=26.75  PM
Byte5=[26]  38    Year(+1985)=38->2023
Byte6=[7D]  125   MinuteOffset=0 [valid]
Byte7=[7D]  125   HourOffset=0 [Local time offset]
//
pVT_Net->RTCDate.Date,pVT_Net->RTCDate.Month, pVT_Net->RTCDate.Year, 
pVT_Net->RTCtime.Hours, pVT_Net->RTCtime.Minutes,pVT_Net->RTCtime.Seconds);
*/
//TVT_TimeDate Implementaion    Request PGN=0x00EA0000
boolean TVT_TimeDate::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
boolean  respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
int i=0;
uint32_t reqPGN=(pMsg->DATA[2]<<24) + (pMsg->DATA[1]<<16) + (pMsg->DATA[0]<<8);
     //TimeDate
     if ((respMSG) && (reqPGN==TIME_PGN)) {
      VTFlushTime(pVT_Net);
      pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
        for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
      pMsg->ID=(AddrClm_PRIO<<24) + reqPGN + pVT_Net->VT_SRC;
      pMsg->DATA[0]=4*pVT_Net->RTCtime.Seconds;   //Seconds
      pMsg->DATA[1]=pVT_Net->RTCtime.Minutes;     //minutes
      pMsg->DATA[2]=pVT_Net->RTCtime.Hours-pVT_Net->hourOffset; //hour- offset
      pMsg->DATA[3]=pVT_Net->RTCDate.Month;     //month
      pMsg->DATA[4]=4*pVT_Net->RTCDate.Date;    //day
      pMsg->DATA[5]=pVT_Net->RTCDate.Year-1985; //year 
      pMsg->DATA[6]=0x7D; //minutes_Offs valid
      pMsg->DATA[7]=0x7D + pVT_Net->hourOffset; //hour_Offs
      //
      valid=true;  
     }
 return valid; 
};//TVT_TimeData::setMsgToAttr




//==============================================================================
//Commands to VT
//==============================================================================
//TVTGetHardware Implementaion    199 $C7
boolean TVTGetHardware::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (int i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=VTBootTime;
   pMsg->DATA[2]=VTGraphicType;
   pMsg->DATA[3]=VTHardware;
   pMsg->DATA[4]=(VTXPixel>>0) & 0xFF;
   pMsg->DATA[5]=(VTXPixel>>8) & 0xFF;
   pMsg->DATA[6]=(VTYPixel>>0) & 0xFF;
   pMsg->DATA[7]=(VTYPixel>>8) & 0xFF;
   valid=true;
  }
 return valid; 
};//TVTGetHardware::setMsgToAttr




//==============================================================================
//TVTGetNumberOfSoftKeys Implementaion   194  $C2
boolean TVTGetNumberOfSoftKeys::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (int i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[4]=VTXDots;
   pMsg->DATA[5]=VTYDots;
   pMsg->DATA[6]=VTVirtualSoftKeys;
   pMsg->DATA[7]=VTPhysicalSoftKeys;
   valid=true;
  }
 return valid; 
};//TVTGetNumberOfSoftKeys::setMsgToAttr

//------------------------------------------------------------------------------
//==============================================================================
//TVTGetTextFontData Implementaion   195  $C3
boolean TVTGetTextFontData::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (int i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[5]=VTSmallFontSize;
   pMsg->DATA[6]=VTLargeFontSize;
   pMsg->DATA[7]=VTFontStyle;
   valid=true;
  }
 return valid; 
};//TVTGetTextFontData::setMsgToAttr




//==============================================================================
//TVTGetMemory Implementaion   192 $C0
boolean TVTGetMemory::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   VTMemory+=(pMsg->DATA[2]<<0);
   VTMemory+=(pMsg->DATA[3]<<8);
   VTMemory+=(pMsg->DATA[4]<<16);
   VTMemory+=(pMsg->DATA[5]<<24);
   //
   pMsg->DATA[1]=VTVersionNumber;
   pMsg->DATA[2]=VTStatus;
    for (int i=3;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   valid=true;
  }
 return valid; 
};//TVTGetMemory::setMsgToAttr


//==============================================================================
//TVTDeleteObjectPool Implementaion   $B2
boolean TVTDeleteObjectPool::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (int i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=VTError;
   //TODO clear pooldata
   valid=true;
  }
 return valid; 
};//TVTDeleteObjectPool::setMsgToAttr


//------------------------------------------------------------------------------
//TVTEndOfObjectPool Implementaion   $12 VT_Net.ETP_EOP
boolean TVTEndOfObjectPool::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (int i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=VTError;
   pMsg->DATA[6]=VTError;
   //TODO end of pooldata
   valid=true;
  }
 return valid; 
};//TVTEndOfObjectPool::setMsgToAttr


//==============================================================================
//==============================================================================
//TVTVersion Implementaion
//==============================================================================
//check not valid characters
//------------------------------------------------------------------------------
/*
Not Valid Version Label Characters:
00 $5C: \  Reverse Solidus (Back slash)
01 $22: " Quotation mark (Double quote)
02 $27: ' Apostrophe (Single quote)
03 $60: ` Grave Accent  (Back tic)
04 $2F: / Solidus (Forward slash)
05 $3A: : Colon
06 $2A: * Asterisk
07 $3C: < Less-than sign
08 $3E: > Greater-than sign
09 $7C: | Vertical line
10 $3F: ? Question mark
*/
int8_t TVTVersion::getValidCharacters(TVT_Net *pVT_Net){
int8_t i=0, j=0,k=-1;
String sStr="",tStr,VersionLabel=VTVersionLabel;
  //Serial.println(VersionLabel);
  while (VersionLabel.length()>=2) {
    sStr=VersionLabel.substring(0,2); VersionLabel.remove(0,2);
    //Serial.println(sStr); 
     if (sStr=="20") k--;
     //
     for (j=0;j<0x20;j++) {
      tStr=getStringHEX(j,2);
       if (tStr==sStr) {
         k=i;
         break;
       }
     }//for j
     //
     if (k>=0) break;
     //
     for (j=0;j<16;j++) {
      tStr=getStringHEX(VTCharLabel[j],2);
       if (tStr==sStr) {
         k=i;
         break;
       }
     }//for j
     //
     if (k>=0) break;
    i++;
  }//while
 return k;
};//TVTVersion::getValidCharacters


//==============================================================================
//check not valid characters
int8_t TVTVersion::getLastVersionLabel(TVT_Net *pVT_Net){
int8_t k=-1;
String str="";
  if ((VTVersionFolder.length()==16) && (VTVersionLabel.length()==pVT_Net->VT_VersionSize)) {
    //check not valid characters
    k=getValidCharacters(pVT_Net);
      //
      if (k<0){
         if (abs(k)==(VTVersionLabel.length()/2+1)){
          //get last version
          pVT_Net->VT_VersionList="";   pVT_Net->VT_VersionCount=0;
          pVT_Net->VT_VersionDateTime=0;pVT_Net->VT_VersionLast="";

          pVT_Net->VT_VersionSize=-pVT_Net->VT_VersionSize;
          str="/" + VTVersionFolder;
          listDir(SD, str.c_str(), 1,pVT_Net);
          Serial.println(pVT_Net->VT_VersionList);
          pVT_Net->VT_VersionSize=abs(pVT_Net->VT_VersionSize);
          VTVersionLabel=pVT_Net->VT_VersionLast;
          //
          Serial.print("pVT_Net->VT_VersionLast=");
          Serial.println(pVT_Net->VT_VersionLast);
         }
      }//k<0
  }
  if ((VTVersionFolder.length()==16) && (VTVersionLabel.length()==pVT_Net->VT_VersionSize)) {
    if (k<0) VTError=0x00; 
  }
  //
 return k;     
};//TVTVersion::getLastVersionLabel         


//==============================================================================
boolean TVTVersion::writeObjectFile(fs::FS &fs, const char * path,TVT_Net *pVT_Net){
uint16_t lSize=pVT_Net->streamStr.available();
boolean valid=(lSize>0);
  if (valid) {
   //Serial.print(pVT_Net->objNr); Serial.println(path);

    File file=fs.open(path,FILE_WRITE);
      if (file){
        valid=file.write((uint8_t*)pVT_Net->streamStr.getBuffer(),lSize);
        
          if (valid) {
            //Serial.print(pVT_Net->objNr); Serial.println(path);
          }
        file.close();
      }//file  
  }
 return valid;
};//TVTVersion::writeObjectFile


//------------------------------------------------------------------------------
boolean TVTVersion::storeFile(fs::FS &fs,TVT_Net *pVT_Net){
boolean valid=(getVTObjectListSize(pVT_Net)>0);
  if (valid) {
     String str="/" + VTVersionFolder; valid=false;
       if (pVT_Net->VT_InfoMode>0) Serial.println(str);
       //
       if (createDir(fs,str.c_str(),pVT_Net)) {
         str=str + "/" + VTVersionLabel + ".iop";
         deleteFile(fs,str.c_str(),NULL,pVT_Net);
           //
           if (pVT_Net->VT_InfoMode>0)Serial.println(str);
           if (writeFile(fs,str.c_str(),pVT_Net)) {
              File file = fs.open(str.c_str(),FILE_READ);
                if (file){
                 time_t t= file.getLastWrite();
                 pVT_Net->tmstruct=*localtime(&t);
                 file.close(); 
                 printDateTimeStructure("LastWrite",pVT_Net);
                }
              valid=true;
           }
       }
  }//valid
 return valid;
};//TVTVersion::storeFile



//------------------------------------------------------------------------------
boolean TVTVersion::writeFile(fs::FS &fs, const char * path,TVT_Net *pVT_Net){
uint16_t lSize=getVTObjectListSize(pVT_Net);
uint32_t len=0;
uint8_t* buff;
boolean valid=(lSize>0);
  if (valid) {
    File file = fs.open(path, FILE_WRITE);
      if (file) {
       pVT_Net->streamObj[pVT_Net->listNr].setPos(0);
       len=pVT_Net->streamObj[pVT_Net->listNr].available();
       buff=pVT_Net->streamObj[pVT_Net->listNr].getBuffer();
       valid=file.write((uint8_t*) buff,len);
       //
       file.close();
      }//file  
  }
 return valid;
};//TVTVersion::writeFile



//==============================================================================
boolean TVTVersion::readFile(fs::FS &fs, const char *path, LoopbackStream *pStream,TVT_Net *pVT_Net) {
uint8_t* buff;
uint32_t i=0,len=0,buffSize=0;
String str=String(path);
boolean valid=false;
  File file = fs.open(path,FILE_READ);
    if (file){
      len=file.available();
      pStream->clear();
      Serial.print(str + "\tbytes="); Serial.println(len);
       //
       if (str.indexOf(".iop")>0) {
         if (len>255*7+1) {
           buffSize=pVT_Net->stream_ETP[pVT_Net->listNr].getBufferSize();
             if (len<=buffSize){
              pVT_Net->stream_ETP[pVT_Net->listNr].clear(); 
              buff=pVT_Net->stream_ETP[pVT_Net->listNr].getBuffer();
              //Serial.println(pVT_Net->stream_ETP[pVT_Net->listNr].getBufferSize());
              len=file.readBytes((char*) buff,len); 
              //Serial.println(len);
              pVT_Net->stream_ETP[pVT_Net->listNr].setSize(len);
              pVT_Net->stream_ETP[pVT_Net->listNr].setPos(0);
              valid=true;
             } 
         }else{
           buffSize=pVT_Net->stream_TP[pVT_Net->listNr].getBufferSize();
             if (len<=buffSize){
              pVT_Net->stream_TP[pVT_Net->listNr].clear(); 
              buff=pVT_Net->stream_TP[pVT_Net->listNr].getBuffer();
              len=file.readBytes((char*) buff,len); 
              pVT_Net->stream_TP[pVT_Net->listNr].setSize(len);
              pVT_Net->stream_TP[pVT_Net->listNr].setPos(0);
              valid=true;
             } 
         }
       } else{
        buffSize=pStream->getBufferSize();        
          if (len<=buffSize) {
            pStream->clear();
            buff=pStream->getBuffer();
            len=file.readBytes((char*) buff,len); 
            pStream->setSize(len);pStream->setPos(0);
            valid=true;
          }
       }
       //
       if (!valid) {
         Serial.println("fileSize>bufferSize=" + String(len) + ">" + String(buffSize));
       }
       //
      file.close();
    }//file
 return ((valid) && (len>0));
};//TVTVersion::readFile



//==============================================================================
boolean TVTVersion::deleteRecursive(fs::FS &fs, const char *path,TVT_Net *pVT_Net) {
boolean isDir=false, valid=false;
 File root=fs.open(path);
   if (root){
     isDir=root.isDirectory();
       if (!isDir) {
        root.close();
        valid=fs.remove(path);
         if (valid) setSerialPrint(pVT_Net,"RemoveFile=" + String(path));
        return valid;
       }
       //
    Serial.println("DIR=" + String(path));
     //root.close();
     File file = root.openNextFile(); 
      while (file) {
        Serial.println(String(file.name()));
        valid=fs.remove(file.name());
        file = root.openNextFile();
      }//while
      //
     return valid; 
     //
     file=fs.open(path);
      if (file) {
        file.close();
        valid=fs.rmdir(path);
         if (valid) setSerialPrint(pVT_Net,"RemoveDir=" + String(path));
      } 
   }
 return valid;
};//TVTVersion::deleteRecursive



//==============================================================================
boolean TVTVersion::deleteFile(fs::FS &fs, const char * path,LoopbackStream *pStream,TVT_Net *pVT_Net) {
boolean valid=false;
  if (fs.exists(path)) {
    setSerialPrint(pVT_Net,"Remove=" + String(path));
    //valid=fs.remove(path);
    valid=deleteRecursive(fs, path,pVT_Net);
  }
 return valid; 
};//TVTVersion::deleteFile

//==============================================================================
String TVTVersion::formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes) + "B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0) + "KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0) + "MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0) + "GB";
  }
};//TVTVersion::formatBytes


//=============================================================================
boolean TVTVersion::createDir(fs::FS &fs, const char * dirname,TVT_Net *pVT_Net){
boolean valid=false;
String str=String(dirname);
int16_t idx=str.indexOf(".");
  if (idx>0) {
    File file=fs.open(dirname,FILE_WRITE);   
      if (file){
        file.close();
        valid=true;
      }
  }else {
   valid=(fs.mkdir(dirname));
  }
 return valid;
};//TVTVersion::createDir

//=============================================================================
boolean TVTVersion::listDir(fs::FS &fs, const char * dirname, uint8_t levels, TVT_Net *pVT_Net){
boolean valid=true;
uint8_t len=0;
String str="";
time_t t=0;
  //Serial.println(dirname);
  File root = fs.open(dirname);
    if(!root){
          if (pVT_Net->VT_InfoMode>0) Serial.println("Failed to open directory");
        return false;
    }
    if(!root.isDirectory()){
          if (pVT_Net->VT_InfoMode>0) Serial.println("Not a directory");
        return false;
    }

    File file = root.openNextFile();
    
    while(file){
        if(file.isDirectory()){
            if (pVT_Net->VT_InfoMode>0){
              Serial.print("DIR:"); Serial.println(file.name());
            }
            //
            if(levels){
                listDir(fs, file.name(), levels,pVT_Net);
            }
        } else {
             if (pVT_Net->VT_InfoMode>0){
              Serial.print("FILE:"); Serial.print(file.name());
             } 
            str=String(file.name());
            //Serial.println(str);
            str.replace(".iop","");
            int i=str.lastIndexOf("/");
              if (i>=0) str.remove(0,i);
            str.replace("/","");
            len=str.length();
            //TEST
            //Serial.println(len);
            //Serial.println(str);
            //Serial.println(pVT_Net->VT_VersionSize);
            //Serial.println(delm0);  
              //
              if (pVT_Net->VT_VersionSize<0) {
                file.getLastWrite();    
                t= file.getLastWrite();
                  //
                  if (t>pVT_Net->VT_VersionDateTime) {
                   pVT_Net->VT_VersionDateTime=t;
                   pVT_Net->VT_VersionLast=str;
                  }
                pVT_Net->tmstruct = *localtime(&t);
                //  if (pVT_Net->VT_InfoMode>0) printDateTimeStructure("LastWrite",pVT_Net);
              }
              //
              if (len==abs(pVT_Net->VT_VersionSize)) {
               pVT_Net->VT_VersionCount++;
                 if (pVT_Net->VT_VersionSize<0) {
                  pVT_Net->VT_VersionList+=String(t);
                  pVT_Net->VT_VersionList+="\t";
                 }
               pVT_Net->VT_VersionList+=str;
               pVT_Net->VT_VersionList+="\n";
              }
              //
              if (pVT_Net->VT_InfoMode>0) Serial.print("\t SIZE:");
            str=formatBytes(file.size()) + "\t" + String(file.size());
              if (pVT_Net->VT_InfoMode>0) {
                Serial.println(str);
                 if (pVT_Net->VT_InfoMode>0) printDateTimeStructure("LastWrite",pVT_Net);
              }
              //
        }
        file = root.openNextFile();
    }
  return valid;  
};//TVTVersion::listDir



//==============================================================================
//==============================================================================
/*
RX3 1CE7260A  8  DF FF FF FF FF FF FF FF
GetVersion (00)
TIME00 [ms]=0

TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
[TP.CM_RTS_E600_00_0]  Size=9  Packets=2
VTtoECU_PGN
TIME00 [ms]=0

RX3 1CEC260A  8  11 02 01 FF FF 00 E6 00
[TP.CM_CTS_E600_00_0]  Packets=2  Next=1
VTtoECU_PGN
TIME00 [ms]=7

TX3 18EB0A26  8  01 E0 01 56 45 52 53 49
TX3 18EB0A26  8  02 4F 4E FF FF FF FF FF
RX3 18EC260A  8  13 09 00 02 FF 00 E6 00
[TP.CM_EOMA_E600_E0_0]  Size=9  Packets=2
VTtoECU_PGN
TIME00 [ms]=6

GetVersionResponse (00)
NumberOfVersionLabels=1
VersionLabels=VERSION
TIME00 [ms]=25
*/

//==============================================================================
//TVTGetVersionResponse Implementaion   224 $E0 GetVersionResponse or 223 DF GetVersion
boolean TVTGetVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
int i=0,j=0;
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
uint8_t cc=0;
String str="",sStr="",tStr;
//char lab[6] 
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=VTNumberOfVersionLabels;
   //TODO String
   VTVersionFolder=getWSNameFromAddress(pVT_Net,src);
   pVT_Net->VT_VersionList="";   pVT_Net->VT_VersionCount=0;
   valid=true;
     //     
     //
     if (VTVersionFolder.length()==16) {
       str="/" + VTVersionFolder;
       //
       //pVT_Net->VT_InfoMode=1;
       pVT_Net->VT_VersionSize=14;
       
       listDir(SD, str.c_str(), 1,pVT_Net);
         if (pVT_Net->VT_InfoMode>0) Serial.println(delm1);
       //listDir(SD, "/", 0,pVT_Net);
       //pVT_Net->VT_InfoMode=0;
        //
         if (pVT_Net->VT_InfoMode>0){
          Serial.println(pVT_Net->VT_VersionList);
          Serial.println(pVT_Net->VT_VersionCount);
         }
       pVT_Net->VT_VersionSize=14;
       //pVT_Net->VT_InfoMode=0;
     }
     //pVT_Net->VT_VersionCount=0;
     pMsg->DATA[1]=pVT_Net->VT_VersionCount;
     //
     //Start TP-protocol
     if (pVT_Net->VT_VersionCount>0) {
      str=pVT_Net->VT_VersionList;
      str.replace("\n","");
      cc=str.length()/2;
      byte byteArray[cc] = {0};
      hexCharacterStringToBytes(byteArray, str.c_str());
      //
      pStream->clear();
      pStream->write(0xE0);pStream->write(pVT_Net->VT_VersionCount);
      pStream->writeBytes((uint8_t*) byteArray,cc,-1); 
      getStreamInfo(pStream,pVT_Net);
      //Serial.println(pStream->available());
      //
      //TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
      //[TP.CM_RTS_E600_00_0]  Size=9  Packets=2
      //VTtoECU_PGN
      pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
      TVTTransportProtocol msgObj;
      valid=msgObj.getTransportProtcol(pMsg,pVT_Net,pStream);     
     }    
  }
 return valid; 
};//TVTGetVersion::setMsgToAttr


//==============================================================================
//TVTStoreVersionResponse Implementaion   $D0
boolean TVTStoreVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
String str="";
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
    //
    for (i=1;i<pMsg->LEN;i++) { 
      VTVersionLabel+=String(pMsg->DATA[i],HEX);
        if (pVT_Net->VT_InfoMode>0) Serial.print(char(pMsg->DATA[i]));
    }
    if (pVT_Net->VT_InfoMode>0) Serial.println("");
   VTVersionLabel.toUpperCase();
    if (pVT_Net->VT_InfoMode>0) Serial.println(VTVersionLabel);
    //get WSName from address 
    VTVersionFolder=getWSNameFromAddress(pVT_Net,src);
    //if (pVT_Net->VT_InfoMode>0) Serial.println("VTVersionFolder=" + VTVersionFolder);
   VTError=0x01;
   pVT_Net->VT_VersionSize=14;
   getLastVersionLabel(pVT_Net);
     //
     if (VTError==0x00) {
      VTError=0x01;
       if (storeFile(SD,pVT_Net)) {
        str="/" + VTVersionFolder;
        listDir(SD, str.c_str(), 1,pVT_Net);
        VTError=0x00;
       }
     }
     //
     //set Response  
     for (i=1;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
   valid=true;
  }
 return valid; 
};//TVTStoreVersion::setMsgToAttr


//========================================================================
//TVTLoadVersionResponse Implementaion   $D1
boolean TVTLoadVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
String str="";
//pVT_Net->VT_InfoMode=1;
  
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
    //
    for (i=1;i<pMsg->LEN;i++) { 
      VTVersionLabel+=String(pMsg->DATA[i],HEX);
        if (pVT_Net->VT_InfoMode>0) Serial.print(char(pMsg->DATA[i]));
    }
    if (pVT_Net->VT_InfoMode>0)Serial.println("");
   VTVersionLabel.toUpperCase();
    if (pVT_Net->VT_InfoMode>0)Serial.println(VTVersionLabel);
   // 
   VTVersionFolder=getWSNameFromAddress(pVT_Net,src);
     if (pVT_Net->VT_InfoMode>0) Serial.println("VTVersionFolder=" + VTVersionFolder + " VTVersionLabel=" + VTVersionLabel);
     //
   VTError=0x01;
   pVT_Net->VT_VersionSize=14;
   getLastVersionLabel(pVT_Net);
     //
     if (VTError==0x00) {
       VTError=0x01;
       str="/" + VTVersionFolder; 
       listDir(SD, str.c_str(), 1,pVT_Net);
       str=str + "/" + VTVersionLabel + ".iop";
       //Serial.println(str);
         if (readFile(SD,str.c_str(),pStream,pVT_Net)) VTError=0x00;
     }
     //
     //set Response  
     for (i=1;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
   valid=true;
  }
 return valid; 
};//TVTLoadVersion::setMsgToAttr


//========================================================================
//TVTDeleteVersionResponse Implementaion   $D2
boolean TVTDeleteVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
String str="";
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
    //
    for (i=1;i<pMsg->LEN;i++) { 
      VTVersionLabel+=String(pMsg->DATA[i],HEX);
        if (pVT_Net->VT_InfoMode>0) Serial.print(char(pMsg->DATA[i]));
    }
    if (pVT_Net->VT_InfoMode>0)Serial.println("");
   VTVersionLabel.toUpperCase();
    if (pVT_Net->VT_InfoMode>0) Serial.println(VTVersionLabel);
   // 
   VTVersionFolder=getWSNameFromAddress(pVT_Net,src);
     //if (pVT_Net->VT_InfoMode>0) Serial.println(VTVersionFolder);
   VTError=0x01;
   pVT_Net->VT_VersionSize=14;
   getLastVersionLabel(pVT_Net);
     //
     //if (VTError==0x00) {
       VTError=0x01;
       str="/" + VTVersionFolder; 
       str=str + "/" + VTVersionLabel + ".iop";
        if (deleteFile(SD,str.c_str(),pStream,pVT_Net)) {
          str="/" + VTVersionFolder;
          listDir(SD, str.c_str(), 1,pVT_Net);          
          VTError=0x00;
        }
     //}
     //
     //set Response  
     for (i=1;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
   valid=true;
  }
 return valid; 
};//TVTDeleteVersion::setMsgToAttr



//==============================================================================
//TVTExtendedGetVersionResponse Implementaion   211 $D3 GetExtendedVersionResponse
boolean TVTExtendedGetVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
int i=0;
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
uint8_t len=0;
String str="";
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
     for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=VTNumberOfVersionLabels;
   //TODO String
   VTVersionFolder=getWSNameFromAddress(pVT_Net,src);
   pVT_Net->VT_VersionList="";   pVT_Net->VT_VersionCount=0;
   valid=true;
     //     
     if (VTVersionFolder.length()==16) {
       String str="/" + VTVersionFolder;
       //
       pVT_Net->VT_VersionSize=2*32;
       //pVT_Net->VT_InfoMode=1;
       listDir(SD, str.c_str(), 1,pVT_Net);
         //
         if (pVT_Net->VT_InfoMode>0){
          Serial.println(pVT_Net->VT_VersionList);
          Serial.println(pVT_Net->VT_VersionCount);
         }
       //pVT_Net->VT_InfoMode=0;
       pVT_Net->VT_VersionSize=14;
       pMsg->DATA[1]=pVT_Net->VT_VersionCount;
     }
     //
     //
     //Start TP-protocol
     if (pVT_Net->VT_VersionCount>0) {
      str=pVT_Net->VT_VersionList;
      str.replace("\n","");
      len=str.length()/2;
      byte byteArray[len] = {0};
      hexCharacterStringToBytes(byteArray, str.c_str());
      //
      pStream->clear();
      //pStream->setNewBufferSize(2+cc); 
      //
      pStream->write(0xD3);pStream->write(pVT_Net->VT_VersionCount);
        for (i=0;i<len;i++) pStream->write(byteArray[i]);   
      getStreamInfo(pStream,pVT_Net);
      //
      //TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
      //[TP.CM_RTS_E600_00_0]  Size=9  Packets=2
      //VTtoECU_PGN
      pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
      TVTTransportProtocol msgObj;
      valid=msgObj.getTransportProtcol(pMsg,pVT_Net,pStream);     
     }    
  }
 return valid; 
};//TVTExtendedGetVersion::setMsgToAttr



//==============================================================================
//TVTExtendedStoreVersion Implementaion   212  $D4
boolean TVTExtendedStoreVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=(getVTObjectListSize(pVT_Net)>0);
uint8_t src=((pMsg->ID)>>0) & 0xFF, dst=((pMsg->ID)>>8) & 0xFF, ctrlByte=0xFF;
String str="",ss="";
int i=0,j=-1;
  if (pStream->available()) ctrlByte=pStream->read();
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=false;
     //
     while ((i<32) && (pStream->available())) {
       VTVersionLabel+=getStringHEX(char(pStream->read()),2); i++;
     }
   //get WSName from address 
   VTVersionFolder=getWSNameFromAddress(pVT_Net,dst);
   VTError=0x01;
   pVT_Net->VT_VersionSize=2*32;
   getLastVersionLabel(pVT_Net);
     //
     if (VTError==0x00) {
      VTError=0x01;
       if (storeFile(SD,pVT_Net)) {
        str="/" + VTVersionFolder;
        listDir(SD, str.c_str(), 1,pVT_Net);        
        VTError=0x00;
       }
     }
     //
     for (i=1; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
   Serial.println(getMsgFrameStr(pMsg));
   valid=true;
  }//msg
 return valid; 
};//TVTExtendedStoreVersion::setMsgToAttr



//========================================================================
//TVTExtendedLoadVersion Implementaion   213  $D5
boolean TVTExtendedLoadVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF, dst=((pMsg->ID)>>8) & 0xFF, ctrlByte=0xFF;
String str="";
int i=0;
  if (pStream->available()) ctrlByte=pStream->read();
  
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=false;
   VTError=0x01;
     //
     while ((i<32) && (pStream->available())) {
       VTVersionLabel+=getStringHEX(char(pStream->read()),2); i++;
     }
   //get WSName from address 
   VTVersionFolder=getWSNameFromAddress(pVT_Net,dst);
   VTError=0x01;
   pVT_Net->VT_VersionSize=2*32;
   getLastVersionLabel(pVT_Net);
     //
     if (VTError==0x00) {
       VTError=0x01;
       str="/" + VTVersionFolder; 
       str=str + "/" + VTVersionLabel + ".iop";
         if (readFile(SD,str.c_str(),pStream,pVT_Net)) {
          str="/" + VTVersionFolder;
          listDir(SD, str.c_str(), 1,pVT_Net);        
          VTError=0x00;
         }
     }
     //
     //set Response  
     for (i=1;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
   valid=true;
  }//msg
 return valid; 
};//TVTExtendedLoadVersion::setMsgToAttr


//========================================================================
//TVTExtendedDeleteVersion Implementaion    214 $D6
boolean TVTExtendedDeleteVersion::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF, dst=((pMsg->ID)>>8) & 0xFF, ctrlByte=0xFF;
String str="";
int i=0;
  if (pStream->available()) ctrlByte=pStream->read();
  
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=false;
   VTError=0x01;
     //
     while ((i<32) && (pStream->available())) {
       VTVersionLabel+=getStringHEX(char(pStream->read()),2); i++;
     }
   //get WSName from address 
   VTVersionFolder=getWSNameFromAddress(pVT_Net,dst);
   VTError=0x01;
   pVT_Net->VT_VersionSize=2*32;
   getLastVersionLabel(pVT_Net);
     //
     if (VTError==0x00) {
       VTError=0x01;
       str="/" + VTVersionFolder; 
       str=str + "/" + VTVersionLabel + ".iop";
        if (deleteFile(SD,str.c_str(),pStream,pVT_Net)) {
         str="/" + VTVersionFolder;
         listDir(SD, str.c_str(), 1,pVT_Net);        
         VTError=0x00;
        }
     }
     //
     //set Response  
     for (i=1;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
   valid=true;
  }//msg
 return valid; 
};//TVTExtendedDeleteVersion::setMsgToAttr



//========================================================================
//TX1 1CE7260A  8  C1 00 00 00 FF FF FF FF
//GetSupportedWideChars (00)
//CodePlane=0
//FirstChar_hex=0000  LastChar_hex=FFFF
//TVTGetSupportedWideChars Implementaion    193 $C1
boolean TVTGetSupportedWideChars::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
     if (pStream!=NULL) {
       pStream->clear(); VTError=0; 
       pStream->write(VT0PCommFunction);   
       pStream->write(VTCodePlane);
       pStream->write((VTFirstChar>>0) & 0xFF);
       pStream->write((VTFirstChar>>8) & 0xFF);
       pStream->write((VTLastChar>>0) & 0xFF);
       pStream->write((VTLastChar>>8) & 0xFF);
       pStream->write(VTError);
       pStream->write(VTRangeNumber);
        for (i=0; i<VTRangeNumber;i++) {
          pStream->write((VTRange[2*i]>>0) & 0xFF);
          pStream->write((VTRange[2*i]>>8) & 0xFF);
          pStream->write((VTRange[2*i+1]>>0) & 0xFF);
          pStream->write((VTRange[2*i+1]>>8) & 0xFF);
        }
      //getStreamInfo(pStream,pVT_Net);
      //
      //TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
      //[TP.CM_RTS_E600_00_0]  Size=9  Packets=2
      //VTtoECU_PGN
      pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
      TVTTransportProtocol msgObj;
      valid=msgObj.getTransportProtcol(pMsg,pVT_Net,pStream);     
     }    
  }
 return valid; 
};//TVTGetSupportedWideChars::setMsgToAttr


//========================================================================
//TVTGetWindowMaskData Implementaion    196 $C4
boolean TVTGetWindowMaskData::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
String str="";
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   //
   pMsg->DATA[1]=VTDataMaskColour;
   pMsg->DATA[2]=VTKeyColour;
     //set Response  
     for (i=3;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   //
   valid=true;
  }
 return valid; 
};//TVTGetWindowMaskData::setMsgToAttr


//========================================================================
//TVTGetSupportedObjects Implementaion    197 $C5
boolean TVTGetSupportedObjects::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
String str="";
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   //  
   //Start TP-protocol
   VTDataCount=gPoolObjMax0+1;
   VTStructure="";
     
     if ((pStream!=NULL) && (VTDataCount>0)) {
       pStream->clear(); 
       pStream->write(VT0PCommFunction);   pStream->write(VTDataCount);
        //
        for (i=0;i<VTDataCount;i++) {
          VTStructure+=char(i);
          pStream->write(i);   
        }
      //getStreamInfo(pStream,pVT_Net);
      //
      //TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
      //[TP.CM_RTS_E600_00_0]  Size=9  Packets=2
      //VTtoECU_PGN
      pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
      TVTTransportProtocol msgObj;
      valid=msgObj.getTransportProtcol(pMsg,pVT_Net,pStream);     
     }    
  }
 return valid; 
};//TVTGetSupportedObjects::setMsgToAttr


//========================================================================
//TVTIdentifyVT Implementaion    187  $BB
boolean TVTIdentifyVT::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
int i=0;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=false;

     //set Response  
     for (i=1;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
     //
     if (pVT_Net->VTInstance>=0) {
       pVT_Net->VTInstance=-1;
       pVT_Net->VT_ChangeAttr=true;
     } else pVT_Net->VTInstance=VTInstance;
     //   
   //
   VTError=0x00;
   valid=true;
  }
 return valid; 
};//TVTIdentifyVT::setMsgToAttr


//========================================================================
//TVTScreenCapture Implementaion    198  $C6
boolean TVTScreenCapture::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
String  str="/img";
int i=0,j=0;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   //
   VTItemRequested=pMsg->DATA[1]; VTPath=pMsg->DATA[2];
     //
     if (((VTItemRequested==0x00) || (VTItemRequested>=0xF0)) && ((VTPath==0x01) || (VTPath>=0xF0))) {
       VTImageID=VTItemRequested;
       pVT_Net->x=0;pVT_Net->y=0; 
         //
         pVT_Net->w=M5.Lcd.width(); pVT_Net->h=M5.Lcd.height();
          
          if (pMsg->DATA[4]<0xFF) pVT_Net->x=pMsg->DATA[4]; 
          if (pMsg->DATA[5]<0xFF) pVT_Net->y=pMsg->DATA[5]; 
          if (pMsg->DATA[6]<0xFF) pVT_Net->w=pMsg->DATA[6]; 
          if (pMsg->DATA[7]<0xFF) pVT_Net->h=pMsg->DATA[7]; 
 
        VTError=0x00; j=6;
        str+=getStringHEX(VTImageID,4) + ".bmp";
          //Clear screen
          if ((pVT_Net->w==0) || (pVT_Net->h==0)) {
           valid=true;
           VTError=0xC0;
          } else {
           Set_setScreenShot(pVT_Net,true);
            //
            if (pMsg->DATA[3]==0) {
             valid=readImage(SD,str.c_str(),pVT_Net);  
            } else {
             valid=writeImage(SD,str.c_str(),pVT_Net);
             //pVT_Net->VT_InfoMode=1;
             listDir(SD, "/", 0,pVT_Net);
             //pVT_Net->VT_InfoMode=0;
            }
           Set_setScreenShot(pVT_Net,false);
          }
          //
          if (!valid) {
            VTError=0x01; j=4;
          }
          //
     } else {
      VTError=0x01;j=4;
     }
     //set Response  
     for (i=j;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
     //
   pMsg->DATA[3]=VTError;
   pMsg->DATA[4]=pMsg->DATA[1];
   pMsg->DATA[5]=0x00;
      
   valid=true;
  }
 return valid; 
};//TVTScreenCapture::setMsgToAttr



//------------------------------------------------------------------------------
//get screenshot picture from screen img0000.bmp all
//SetMsg=#TX1 1CE7260A  8  C6 00 01 FF FF FF FF FF
//SetMsg=$TX1 1CE7260A  8  C6 00 01 FF FF FF FF FF
boolean TVTScreenCapture::writeImage(fs::FS &fs, const char * path,TVT_Net *pVT_Net){
return writeImageScreen(fs, path,pVT_Net);
};//TVTScreenCapture::writeImage


//------------------------------------------------------------------------------
File bmpFS;
//------------------------------------------------------------------------------
boolean TVTScreenCapture::readImage(fs::FS &fs, const char *path, TVT_Net *pVT_Net) {
 return readImageScreen(fs, path, pVT_Net);
};//TVTScreenCapture::readImage



//========================================================================
//TVTProprietaryCommand Implementaion   $60 to $7F
boolean TVTProprietaryCommand::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF, dst=((pMsg->ID)>>8) & 0xFF, len=0;
uint32_t r_PGN=AddrClm_PGN;
String str="";
int i=0;
  if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (src<<8) + pVT_Net->VT_SRC;
   VT0PCommFunction=pMsg->DATA[0];
     //get VT_SRC and VT_WS_Name
     if ((VT0PCommFunction>=0x60) && (VT0PCommFunction<=0x7F)) {
       TVT_DiagConformance msgObj;
       str+=msgObj.getDiagHS_Str(pVT_Net,r_PGN);
       str.replace("SourceAddress=","");
       str.replace("WorkingSetName=","");
       str=str.substring(0,18); len=str.length()/2;
         //
         if (pMsg->DATA[1]<0xFE){
           if (pMsg->DATA[2]>=0xFE) {
            pVT_Net->VT_SRC=pMsg->DATA[1];
           }else{
            pVT_Net->VT_SRC_WS=""; 
              for (i=0;i<pMsg->LEN;i++){
                pVT_Net->VT_SRC_WS+=getStringHEX(pMsg->DATA[pMsg->LEN-i-1],2);
              }
           }
         }else{  
           uint8_t byteArray[len];
           hexCharacterStringToBytes(byteArray, str.c_str()); str.remove(0,2);
           pVT_Net->VT_SRC=byteArray[0];
           pVT_Net->VT_SRC_WS=str;
           //
           getArray8Info(pVT_Net,(uint8_t*) byteArray,len);
           Serial.println(pVT_Net->VT_SRC_WS);
         }
     }
   //
   VTError=0x00;
   valid=true;
  }
 return valid; 
};//TVTProprietaryCommand::setMsgToAttr


//==============================================================================
//========================================================================
//TVT_DiagConformance Implementaion    PGN=0x00EA0000
  //WSName
  //Request_PGN  =0x00EA0000;
  //AddrClm_PGN  =0x00EE0000;
  //
  //Diagnostics
  //DIAG_CONFORM_PGN  =0x00FD4200;
  //DIAG_PROT_PGN     =0x00FD3200;
  //
  //DIAG_SOFTW_PGN    =0x00FEDA00;
  //DIAG_HARDW_PGN    =0x00FDC500;
  //DIAG_ACT_PGN      =0x00FECA00;
  //DIAG_PRV_PGN      =0x00FECB00;
  //DIAG_FUN_PGN      =0x00FC8E00;
  //DIAG_PID_PGN      =0x00FC8D00;
boolean TVT_DiagConformance::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false,setWS=true,getDiagn=false;
uint8_t src=((pMsg->ID)>>0) & 0xFF, dst=((pMsg->ID)>>8) & 0xFF, len=0;
boolean  respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
int i=0;
String str="",ss="";
uint32_t reqPGN=(pMsg->DATA[2]<<24) + (pMsg->DATA[1]<<16) + (pMsg->DATA[0]<<8);
 //
 getDiagn=(getDiagn) || (reqPGN==DIAG_CONFORM_PGN) || (reqPGN==DIAG_PROT_PGN);
 getDiagn=(getDiagn) || (reqPGN==DIAG_SOFTW_PGN) || (reqPGN==DIAG_HARDW_PGN);
 getDiagn=(getDiagn) || (reqPGN==DIAG_ACT_PGN) || (reqPGN==DIAG_PRV_PGN);
 getDiagn=(getDiagn) || (reqPGN==DIAG_FUN_PGN) || (reqPGN==DIAG_PID_PGN);
  //
  if ((getDiagn) && (respMSG)) {
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(AddrClm_PRIO<<24) + reqPGN  + pVT_Net->VT_SRC;
     //   
     for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0x00;
     //  
     //get Diagnostic PGN
     if ((!valid) && (reqPGN==DIAG_CONFORM_PGN)) {
      str+=getDiagHS_Str(pVT_Net,reqPGN);
      //Serial.println(str);
       //
       while (str.indexOf("[")>=0) {
        str.remove(0,str.indexOf("[")+1);
        ss+=str.substring(0,str.indexOf("]"));
       }
      len=ss.length()/2;
      uint8_t byteArray[len];
      hexCharacterStringToBytes(byteArray, ss.c_str());
      //getArray8Info(pVT_Net,(uint8_t*) byteArray,len);
        for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=byteArray[i];
      /*
      Serial.println(ss);
      
       //default
       //String str="00 00 00 00 87 1F 62 16"
       pMsg->DATA[0]=0x16;
       pMsg->DATA[1]=0x62;
       pMsg->DATA[2]=0x1F;
       pMsg->DATA[3]=0x87;
       */
       valid=true;
     }   
     if ((!valid) && (reqPGN==DIAG_PROT_PGN)) {
         for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
       reqPGN=DIAG_FUN_PGN;
       str=getDiagHS_Str(pVT_Net,reqPGN);
       pMsg->DATA[0]=char(str[0]); //protocol 0
       valid=true;
     }   
     if (!valid) {
       str=getDiagHS_Str(pVT_Net,reqPGN);
         if ((pStream!=NULL) && (str.length()>0)) {
          pStream->clear();
            for (i=0;i<str.length();i++) pStream->write(char(str[i]));
          //getStreamInfo(pStream,pVT_Net);
          //
          //TX3 1CEC0A26  8  10..
          //[TP.CM_RTS_DIAG_PGN_00_0]
          pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (src<<8) + pVT_Net->VT_SRC;
          TVTTransportProtocol msgObj;
          valid=msgObj.getTransportProtcol(pMsg,pVT_Net,pStream,reqPGN);     
         }
     }
  }//respMSG     
 return valid; 
};//TVT_DiagConformance::setMsgToAttr


//------------------------------------------------------------------------------
//Diagnostic Info
String TVT_DiagConformance::getDiagInfo(TVT_Net *pVT_Net,String mStr, LoopbackStream *pStream){
String str="",streamStr="";
int16_t a=-1,b=0,i=0;
 pStream->setPos(0);
   //
   while (pStream->available()) streamStr+=char(pVT_Net->streamStr.read());
 a=streamStr.indexOf(mStr); 
  if (a>=0){
    streamStr.remove(0,a + mStr.length()); mStr.replace("<PREF","</PREF");
    b=streamStr.indexOf(mStr);
    streamStr.remove(b,0xFFFF);
    str+=streamStr.c_str();
    //str.replace("\n","");str.replace("\r","");
   }
 return str;
};//TVT_DiagConformance::getDiagInfo



//------------------------------------------------------------------------------
//Diagnostic
//String Software Identfication, ECU Information Identfication
String TVT_DiagConformance::getDiagHS_Str(TVT_Net *pVT_Net, uint32_t r_PGN){
String   str="",mStr="", ss="/diagn.txt",wStr="A=1",streamStr="";
int16_t  a=0,b=0;
uint8_t  nn=0xFF,len=0;
pVT_Net->streamStr.clear();
//Serial.println(ss);
boolean valid=readFile(SD,ss.c_str(),&pVT_Net->streamStr,pVT_Net);
  //
  if (!valid) valid=readFile(SPIFFS,ss.c_str(),&pVT_Net->streamStr,pVT_Net);
  //  
  if (valid) {
      while (pVT_Net->streamStr.available()) streamStr+=char(pVT_Net->streamStr.read()); 
  } else ss=""; 
  //
  if (r_PGN==DIAG_PRV_PGN) {
    r_PGN=DIAG_ACT_PGN;wStr="A=0";
  }
  //
  switch (r_PGN) {
   //AddrClm_PGN  =0x00EE0000;
   case AddrClm_PGN: 
     mStr="<PREF_WORKINGSETNAME>";
     str+=getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     str.replace("\n","");str.replace("\r","");
     break;
   //
   //ConformanceInformation
   case DIAG_CONFORM_PGN: 
     mStr="<PREF_CONFORMANCE>";
     str+=getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     break;
   //
   //DIAG_PRV_PGN  
   case DIAG_ACT_PGN:
     mStr="<PREF_DTC>";
     getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     streamStr+=char(pVT_Net->streamStr.read());
     //
     //TEST
     //Serial.println(pStream);
     //
     a=streamStr.indexOf("SPN=");   
     str="";  
     nn=0xFF;str+=char(nn);
     nn=0xFF;str+=char(nn);
       //
       while (a>=0) {
        streamStr.remove(0,a+4);
        a=streamStr.indexOf("SPN=");
        b=streamStr.indexOf("/>");   
          if (b<0) break;              
        ss=streamStr.substring(0,b); 
        ss.replace("\"","");
        ss.replace("$","");
        ss.replace(" ","");
        ss.replace("FMI=","");
        ss.replace("SPNMode=","0");
        ss.replace("Count=","0");
        //
        b=ss.indexOf(wStr);  
        ss.replace("A=","0");
          //
          if (b>0) {
            ss.remove(b,255);
            //TEST
            //Serial.println(ss);
            len=ss.length()/2;
            uint8_t byteArray[len];
            hexCharacterStringToBytes(byteArray, ss.c_str());
            getArray8Info(pVT_Net,(uint8_t*) byteArray,len);
            ss="";
            ss+=char(byteArray[2]);
            ss+=char(byteArray[1]);
            ss+=char(((byteArray[0] << 5) & 0xE0) + byteArray[3]);
            ss+=char(((byteArray[4] << 7) & 0x80) + byteArray[5]);
            str+=ss;  
          }
          //
       }//while
      getArray8Info(pVT_Net,(uint8_t*) str.c_str(),str.length());
     break;
     //  
   //SoftwareInformation
   case DIAG_SOFTW_PGN: 
     mStr="<PREF_SOFTWAREINFO>";
     //3 Fields of Information
     str+=char(3);
     str+=getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     str.replace("\n","");str.replace("\r","");
     break;
   //
   //HardwareInformation
   case DIAG_HARDW_PGN:
     mStr="<PREF_HARDWAREINFO>";
     str+=getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     str.replace("\n","");str.replace("\r","");
     break;
   //  
   //ProductIdentification
   case DIAG_PID_PGN:
     mStr="<PREF_PRODUCTINFO>";
     str+=getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     str.replace("\n","");str.replace("\r","");
     break;
   //
   //Functionality
   case DIAG_FUN_PGN:
     mStr="<PREF_DPR>"; ss="";
     ss+=getDiagInfo(pVT_Net,mStr,&pVT_Net->streamStr);
     ss.replace("\n","");ss.replace("\r","");
     ss.replace("\"","");
     //TEST
     //Serial.println(ss);
     mStr="Function=";
     a=ss.indexOf(mStr);
       //
       while (a>=0) {
         ss.remove(0,a + mStr.length());
         str+=ss.substring(0,2);
         a=ss.indexOf(mStr);
       } //while
     
     //TEST
     //Serial.println(str);
     len=str.length()/2;
     uint8_t byteArray0[len];
     hexCharacterStringToBytes(byteArray0, str.c_str()); str=""; 
       for (int i=0;i<len;i++) str+=char(byteArray0[i]); 
     break;
     //
  }//switch
 return str;
};//TVT_DiagConformance::getDiagHS_Str




//==============================================================================
//==============================================================================
