//==============================================================================
//Definition objects VTActivation Messages
//TVTMaintenanceMessage   , TVT_VTStatusMessage,
//TVT_VTSoftKeyActivation , TVT_VTButtonActivation  , TVT_VTPointingEvent
//TVT_VTSelectInput       , TVT_VTESC               , TVT_VTChangeNumericValue
//TVT_VTChangeActiveMask  , TVT_VTChangeSoftKeyMask , TVT_VTInputStringValue
//TVT_VTDisplayActivation , TVT_VTControlAudioDeviceTermination
//TVTUnsupportedFunction

//==============================================================================
//unit UnitVTActivationMsg.cpp
//==============================================================================

#include "UnitVTActivationMsg.h"

//==============================================================================
//TVTMaintenanceMessage Implementaion   255 0xFF
boolean TVTMaintenanceMessage::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   pMsg->DATA[1]=0x00;
   pMsg->DATA[2]=0x06;
   pMsg->DATA[3]=0xFF;
   pMsg->DATA[4]=0xFF;
   pMsg->DATA[5]=0xFF;
   pMsg->DATA[6]=0xFF;
   pMsg->DATA[7]=0xFF;
   valid=true;
  }
 return valid; 
};//TVTMaintenanceMessage::setMsgToAttr


//==============================================================================
//TVT_VTStatusMessage Implementaion   0xFE
boolean TVT_VTStatusMessage::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (0xFF<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=pVT_Net->VT_ActiveSRC;
   pMsg->DATA[2]=(pVT_Net->VT_ActiveMask>>0) & 0xFF;
   pMsg->DATA[3]=(pVT_Net->VT_ActiveMask>>8) & 0xFF;
   pMsg->DATA[4]=(pVT_Net->VT_ActiveSoftKeyMask>>0) & 0xFF;
   pMsg->DATA[5]=(pVT_Net->VT_ActiveSoftKeyMask>>8) & 0xFF;
   pMsg->DATA[6]=0x00;
   pMsg->DATA[7]=0xFF;
   valid=true;
 return valid; 
};//TVT_VTStatusMessage::setMsgToAttr


//==============================================================================
//TVT_VTSoftKeyActivation Implementaion   0x00
boolean TVT_VTSoftKeyActivation::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint16_t objID=pVT_Net->VTObjID;
uint8_t keyNr=pVT_Net->TFT_KeySelect+1,eventID=VTOnSoftKeyPress;//24;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
    if (pVT_Net->VTObjID==0xFFFF) keyNr=0; //ACKN
   //
   pMsg->DATA[0]=VT0PCommFunction;
   //pMsg->DATA[1]=(str=="D"); 1=pressed, 0=released, 2=stillpressed
   pMsg->DATA[2]=((pVT_Net->VTObjID>>0) & 0xFF);
   pMsg->DATA[3]=((pVT_Net->VTObjID>>8) & 0xFF);
   pMsg->DATA[4]=((pVT_Net->VT_ActiveMask>>0) & 0xFF);
   pMsg->DATA[5]=((pVT_Net->VT_ActiveMask>>8) & 0xFF);
   pMsg->DATA[6]=keyNr;
   pMsg->DATA[7]=0x0F; //TAN number
     //
     if (pMsg->DATA[1]==0) eventID=VTOnSoftKeyRelease;//25;
     //key pressed
     if (eventID==VTOnSoftKeyPress) pVT_Net->stream_Cmd[pVT_Net->listNr].clear();
   getVTMacrosListEvents(pVT_Net,pVT_Net->VTObjID,eventID);
   valid=true;
   return valid; 
};//TVT_VTSoftKeyActivation::setMsgToAttr

//==============================================================================
//TVT_VTButtonActivation Implementaion   0x01
boolean TVT_VTButtonActivation::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint16_t objID=pVT_Net->VTObjID;
uint8_t keyNr=pVT_Net->TFT_KeySelect+1,eventID=VTOnSoftKeyPress;//24;
uint8_t optn=0,la=0,ls=0,ds=0;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
    if (pVT_Net->VTObjID==0xFFFF) keyNr=0; //ACKN
   //
   pMsg->DATA[0]=VT0PCommFunction;
   //pMsg->DATA[1]=(str=="D"); 1=pressed, 0=released, 2=stillpressed
   pMsg->DATA[2]=((pVT_Net->VTObjID>>0) & 0xFF);
   pMsg->DATA[3]=((pVT_Net->VTObjID>>8) & 0xFF);
   pMsg->DATA[4]=((pVT_Net->VT_ActiveMask>>0) & 0xFF);
   pMsg->DATA[5]=((pVT_Net->VT_ActiveMask>>8) & 0xFF);
   pMsg->DATA[6]=keyNr;
   pMsg->DATA[7]=0x0F; //TAN number
   
   //get VTOptions of button object
   int16_t objIdx=getVTObjID(pVT_Net,pVT_Net->VTObjID);
     if (objIdx>=0) {
       //getStreamStrInfo(pVT_Net); 
       optn=pVT_Net->streamStr.readBytesVal(1,10);
       la=(optn>>0) & 0x01; //latchable
       ls=(optn>>1) & 0x01; //latchstate
       ds=(optn>>4) & 0x01; //disable
         //check not disabled
         if (!ds) {  
           if (pMsg->DATA[1]==0) eventID=VTOnSoftKeyRelease;//25;
           //
           if (la) {
             if ((ls) && (eventID==VTOnSoftKeyPress))  {
              pMsg->DATA[1]=0; 
              eventID=VTOnSoftKeyRelease;//25;
              valid=true;
             }
             if ((!ls) &&  (eventID==VTOnSoftKeyRelease)) {
              pMsg->DATA[1]=1;
              eventID=VTOnSoftKeyPress;//24;
              valid=true;
             }
           } else valid=true;//la 
           //
           if (valid){
             pVT_Net->TFT_ButtonPressed=pMsg->DATA[1];
             pVT_Net->VTPoolRefresh=true;
             getVTMacrosListEvents(pVT_Net,pVT_Net->VTObjID,eventID);
           } 
         } //not ds
     }//objIdx>=0
     //   
   return valid;
};//TVT_VTButtonActivation::setMsgToAttr


//==============================================================================
//TVT_VTPointingEvent Implementaion   0x02
boolean TVT_VTPointingEvent::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint16_t objID=pVT_Net->VTObjID;
uint8_t keyNr=pVT_Net->TFT_KeySelect+1, eventID=VTOnPointingPress;//27;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=((pVT_Net->VTxPos>>0) & 0xFF);
   pMsg->DATA[2]=((pVT_Net->VTxPos>>8) & 0xFF);
   pMsg->DATA[3]=((pVT_Net->VTyPos>>0) & 0xFF);
   pMsg->DATA[4]=((pVT_Net->VTyPos>>8) & 0xFF);
   //
   pMsg->DATA[5]=0x00 + pVT_Net->VTsPos ; //TAN number=0 + released=0, pressed=1, held=2
   pMsg->DATA[6]=((pVT_Net->VT_ActiveMask>>0) & 0xFF);
   pMsg->DATA[7]=((pVT_Net->VT_ActiveMask>>8) & 0xFF);
     if ((pMsg->DATA[5] & 0x0F)==0) eventID=VTOnPointingRelease;//28;
   getVTMacrosListEvents(pVT_Net,pVT_Net->VTObjID,eventID);
   
   valid=true;
   return valid; 
};//TVT_VTPointingEvent::setMsgToAttr


//==============================================================================

//TVT_VTSelectInput Implementaion    0x03
boolean TVT_VTSelectInput::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint16_t objID=pVT_Net->VTObjID;
uint8_t keyNr=0xFF,eventID=VTOnInputFieldSelection; //19
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
     //
     if (pVT_Net->VTObjType==5) {
      keyNr=pVT_Net->TFT_KeySelect;
       if (keyNr==255) pVT_Net->TFT_KeySelectObjID=0xFFFF;
     }
     if (pVT_Net->VTObjType==6) {
      keyNr=pVT_Net->TFT_ButtonSelect;
       if (keyNr==255) pVT_Net->TFT_ButtonSelectObjID=0xFFFF;
     }
     if (HasInArray(pVT_Net->VTObjType,inpAllObjSet)) {
      keyNr=pVT_Net->TFT_InputSelect;
       if (keyNr==255) pVT_Net->TFT_InputSelectObjID=0xFFFF;
     }
     //   
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=((pVT_Net->VTObjID>>0) & 0xFF);
   pMsg->DATA[2]=((pVT_Net->VTObjID>>8) & 0xFF);
   pMsg->DATA[3]=(keyNr<255); //select/deselect
   pMsg->DATA[4]=0x00; //edit open 
   pMsg->DATA[5]=0xFF;
   pMsg->DATA[6]=0xFF;
   pMsg->DATA[7]=0x0F; //TAN number
     if (keyNr==255) eventID=VTOnInputFieldDeselection;//20
     //
   getVTMacrosListEvents(pVT_Net,pVT_Net->VTObjID,eventID);
   pVT_Net->VTPoolRefresh=true;
   valid=true;
   return valid; 
};//TVT_VTSelectInput::setMsgToAttr

//==============================================================================
//TVT_VTESC Implementaion   0x04
boolean TVT_VTESC::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
uint16_t objID=pVT_Net->VTObjID;
uint8_t keyNr=pVT_Net->TFT_KeySelect,  eventID=VTOnESC; //21;
   VTError=pMsg->DATA[3];
   VTError=((VTError) || (keyNr<255));
   // 
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
    //if (pVT_Net->VTObjID==0xFFFF) keyNr=0; //ACKN
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=((pVT_Net->VTObjID>>0) & 0xFF);
   pMsg->DATA[2]=((pVT_Net->VTObjID>>8) & 0xFF);
   pMsg->DATA[3]=VTError; //select/deselect
   pMsg->DATA[4]=0xFF;  
   pMsg->DATA[5]=0xFF;
   pMsg->DATA[6]=0xFF;
   pMsg->DATA[7]=0x0F; //TAN number
   //
   getVTMacrosListEvents(pVT_Net,pVT_Net->VTObjID,eventID);
   valid=true;
   return valid; 
};//TVT_VTESC::setMsgToAttr



//==============================================================================
//TVT_VTChangeNumericValue Implementaion   0x05
boolean TVT_VTChangeNumericValue::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean  valid=false,TEST=false;
uint8_t  keyNr=0xFF,nn=0,mm=0, eventID=VTOnEntryOfValue;//=22   //VTOnEntryOfNewValue;//23 
int16_t  refIdx=-1;
uint16_t refObj=0xFFFF,objID=0xFFFF;
uint32_t vMin=0,vMax=0xFFFFFFFF,int4=0;
int32_t  offs=0;
float rr=1.000, erg=0.000;
  if (HasInArray(pVT_Net->VTObjType,inpNumObjSet)){ 
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   refIdx=getVTObjID(pVT_Net,pVT_Net->VTObjID); objID=pVT_Net->VTObjID;
     //
     if (refIdx>=0) {
        //TEST
        //getStreamStrInfo(pVT_Net);
        //input checkbox Field
        if (pVT_Net->VTObjType==7) {
         nn=8;
         refObj =pVT_Net->streamStr.readBytesVal(2,nn); nn+=2;
         VTValue=pVT_Net->streamStr.readBytesVal(1,nn);  
           if (refObj<0xFFFF) {
             refIdx=getVTObjID(pVT_Net,refObj); refObj=0xFFFF;
               if (refIdx>=0) {
                refObj=pVT_Net->VTObjID;
                VTValue=pVT_Net->streamStr.readBytesVal(4,3); 
               }
           }
           //
           if (pVT_Net->VTValueStr.length()==0) {
            VTValue=(VTValue!=0); pVT_Net->VTValue=!VTValue; 
           } else pVT_Net->VTValue=pVT_Net->VTValueStr.toInt(); 
           //
           if (VTValue!=pVT_Net->VTValue) {
              VTValue=pVT_Net->VTValue;
              eventID=VTOnEntryOfNewValue;
              nn=10; mm=1;
                if (refObj<0xFFFF) {
                  nn=3; mm=4;
                } else {
                  if (VTValue!=0) VTValue=1; 
                }
              pVT_Net->streamStr.writeBytesVal(VTValue,mm,nn);
           }
         valid=true;
        }//input checkbox
        //
        //InputList
        if (pVT_Net->VTObjType==10) {
         nn=7;
         refObj =pVT_Net->streamStr.readBytesVal(2,nn); nn+=2;
         VTValue=pVT_Net->streamStr.readBytesVal(1,nn); nn++;
         vMax=pVT_Net->streamStr.readBytesVal(1,nn);
           if (refObj<0xFFFF) {
             refIdx=getVTObjID(pVT_Net,refObj); refObj=0xFFFF;
               if (refIdx>=0) {
                refObj=pVT_Net->VTObjID;
                VTValue=pVT_Net->streamStr.readBytesVal(4,3); 
               }
           }
           //
           if (pVT_Net->VTValueStr.length()==0) {
            pVT_Net->VTValue=VTValue;
             if ((pVT_Net->VTValue>=255) || (pVT_Net->VTValue>=vMax)) pVT_Net->VTValue=0;
           }else {
             if ((VTValue<255) && (pVT_Net->VTValueStr=="+")){
              pVT_Net->VTValue=VTValue;pVT_Net->VTValue++; 
             }
             if ((VTValue>0) && (pVT_Net->VTValueStr=="-")){
              pVT_Net->VTValue=VTValue; pVT_Net->VTValue--; 
             }
             if (pVT_Net->VTValue>0xFF) pVT_Net->VTValue=0xFF;
           }
           //
           if (VTValue!=pVT_Net->VTValue) {
              VTValue=pVT_Net->VTValue;
              eventID=VTOnEntryOfNewValue;
              nn=9; mm=1;
                 if (refObj<0xFFFF) {nn=3; mm=4;}
              pVT_Net->streamStr.writeBytesVal(VTValue,mm,nn);
           }
         valid=true;
        }//inputlist
        //
        //input numeric Field
        if (pVT_Net->VTObjType==9) {
          nn=11;
          //check refObj
          refObj=pVT_Net->streamStr.readBytesVal(2,nn);nn+=2;
          //actual value
          VTValue=pVT_Net->streamStr.readBytesVal(4,nn);nn+=4;
          //minValue
          vMin=pVT_Net->streamStr.readBytesVal(4,nn);nn+=4;
          //maxValue
          vMax=pVT_Net->streamStr.readBytesVal(4,nn);nn+=4;
          //offs
          offs=pVT_Net->streamStr.readBytesVal(4,nn);nn+=4;
          //Scale
          int4=pVT_Net->streamStr.readBytesVal(4,nn);nn+=4;
          rr=getFloatFromInt(int4);
             //
             if (refObj<0xFFFF) {
               refIdx=getVTObjID(pVT_Net,refObj); refObj=0xFFFF;
                if (refIdx>=0) {
                    refObj=pVT_Net->VTObjID;
                    VTValue=pVT_Net->streamStr.readBytesVal(4,3); 
                }
             }
             if (pVT_Net->VTValueStr.length()>0){
                //input
                erg=pVT_Net->VTValueStr.toFloat();
                //calc
                rr= erg/rr;rr=rr-offs;
                pVT_Net->VTValue=int32_t(rr);
             } else {
                erg=VTValue;
                pVT_Net->VTValue=VTValue;
             }
            valid=((pVT_Net->VTValue>=vMin) && (pVT_Net->VTValue<=vMax));
             if ((valid) && (VTValue!=pVT_Net->VTValue)) {
              VTValue=pVT_Net->VTValue;
              eventID=VTOnEntryOfNewValue; nn=13;
                if (refObj<0xFFFF) nn=3;
              pVT_Net->streamStr.writeBytesVal(VTValue,4,nn);
             }
        }//input numeric field
        //     
        //
        //TEST
        if (TEST) { 
          Serial.println(delm0);
          Serial.println(valid);
          Serial.println(erg);
          Serial.println(vMin);
          Serial.println(vMax);
          Serial.println(VTValue);
          Serial.println(pVT_Net->VTValue);
        }
        //
        if (TEST) getStreamStrInfo(pVT_Net);
        if ((valid) && (eventID==VTOnEntryOfNewValue)) setVTObjectListValue(pVT_Net,refIdx);
     }//refIdx>=0
     //
     if (valid) {
      VTObjectReference=pVT_Net->VTObjID;
      //Serial.println(VTObjectReference);
      pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);
      pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
      pMsg->DATA[3]=0x0F; //TAN number
      pMsg->DATA[4]=((VTValue>>0)  & 0xFF);  
      pMsg->DATA[5]=((VTValue>>8)  & 0xFF);
      pMsg->DATA[6]=((VTValue>>16) & 0xFF);
      pMsg->DATA[7]=((VTValue>>24) & 0xFF);
      //
      getVTMacrosListEvents(pVT_Net,objID,VTOnEntryOfValue);
        //
        if (eventID==VTOnEntryOfNewValue) {
         //Serial.println("VTOnEntryOfNewValue=" + String(eventID));
         getVTMacrosListEvents(pVT_Net,objID,eventID);
         pVT_Net->VTPoolRefresh=true;
        }
      valid=true;
     } 
  }
  //
 return valid; 
};//TVT_VTChangeNumericValue::setMsgToAttr


//==============================================================================
//TVT_VTChangeActiveMask Implementaion   0x06 send as error detection
boolean TVT_VTChangeActiveMask::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);
   pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[3]=VTError; 
   pMsg->DATA[4]=((VTObjectReference>>0) & 0xFF);  
   pMsg->DATA[5]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[6]=((pVT_Net->VT_ActiveMask>>0)  & 0xFF);
   pMsg->DATA[7]=((pVT_Net->VT_ActiveMask>>8)  & 0xFF);
   valid=true;
   return valid; 
};//TVT_VTChangeActiveMask::setMsgToAttr

//==============================================================================
//TVT_VTChangeSoftKeyMask Implementaion    0x07 send as error detection
boolean TVT_VTChangeSoftKeyMask::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=((pVT_Net->VT_ActiveMask>>0)  & 0xFF); 
   pMsg->DATA[2]=((pVT_Net->VT_ActiveMask>>8)  & 0xFF);  
   pMsg->DATA[3]=((VTObjectReference>>0) & 0xFF);
   pMsg->DATA[4]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[5]=VTError;
   pMsg->DATA[6]=0xFF;
   pMsg->DATA[7]=0xFF;
   valid=true;
   return valid; 
};//TVT_VTChangeSoftKeyMask::setMsgToAttr



//==============================================================================
//TVT_VTInputStringValue Implementaion      0x08
boolean TVT_VTInputStringValue::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean  valid=false;
uint8_t  olen=0,nlen=0,nn=0,i=0, eventID=VTOnEntryOfValue;//=22   //VTOnEntryOfNewValue;//23 
uint16_t refObj=0xFFFF;
int16_t  refIdx=-1;
  if (pVT_Net->VTObjType==gInpStringType){ 
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   VTObjectReference=pVT_Net->VTObjID;
   refIdx=getVTObjID(pVT_Net,pVT_Net->VTObjID);
     //input string Field
     if ((refIdx>=0) && (pVT_Net->VTObjType==8)){
         nn=13;
         refObj=pVT_Net->streamStr.readBytesVal(2,nn);nn+=3; 
         olen=pVT_Net->streamStr.readBytesVal(1,nn);nn++;
         //actual value
         VTNewValue="";
           for (i=0;i<olen;i++) VTNewValue+=char(0x20); 
         pVT_Net->streamStr.readBytes((uint8_t*)VTNewValue.c_str(),olen,nn);
          //
          if (refObj<0xFFFF) {
            refIdx=getVTObjID(pVT_Net,refObj); refObj=0xFFFF;
              if (refIdx>=0) {
                refObj=pVT_Net->VTObjID; nn=3;
                olen=pVT_Net->streamStr.readBytesVal(2,nn);nn+=2;
                //actual value
                VTNewValue="";
                   for (i=0;i<olen;i++) VTNewValue+=char(0x20); 
                   if (olen>255) olen=255;               
                pVT_Net->streamStr.readBytes((uint8_t*)VTNewValue.c_str(),olen,nn);
              }
          }
          if (pVT_Net->VTValueStr.length()==0) {
            pVT_Net->VTValueStr=VTNewValue;  
          }else {
            nlen=pVT_Net->VTValueStr.length();
              if (nlen>olen) pVT_Net->VTValueStr=pVT_Net->VTValueStr.substring(0,olen); 
              //
              if (VTNewValue!=pVT_Net->VTValueStr) {
                  while (pVT_Net->VTValueStr.length()<olen) pVT_Net->VTValueStr+=char(0x20); 
                VTNewValue=pVT_Net->VTValueStr; nn=17;
                eventID=VTOnEntryOfNewValue;//23  
                  if (refObj<0xFFFF) nn=5;  
                pVT_Net->streamStr.writeBytes((uint8_t*)VTNewValue.c_str(),olen,nn);
                setVTObjectListValue(pVT_Net,refIdx);
              }
            VTNewValue.trim(); nlen=VTNewValue.length(); 
          }
         valid=true;
     }//refIdx>=0
     //
     if (valid) {
      nlen=VTNewValue.length(); 
      VTObjectReference=pVT_Net->VTObjID;
        //check TP-protocol
        if (nlen>4){
           pVT_Net->stream_TP[pVT_Net->listNr].clear();
           pVT_Net->stream_TP[pVT_Net->listNr].writeBytesVal(VT0PCommFunction,1);
           pVT_Net->stream_TP[pVT_Net->listNr].writeBytesVal(VTObjectReference,2);
           pVT_Net->stream_TP[pVT_Net->listNr].writeBytesVal(nlen,1);
           pVT_Net->stream_TP[pVT_Net->listNr].writeBytes((uint8_t*)VTNewValue.c_str(),nlen);
           //
           //TX3 1CEC0A26  8  10 09 00 02 FF 00 E6 00
           //[TP.CM_RTS_E600_00_0]  Size=nn9  Packets=2
           //VTtoECU_PGN
           pMsg->ID=(TP_ETP_PRIO<<24) + TP_PGN + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
           //TVTTransportProtocol msgObj;
           //valid=msgObj.getTransportProtcol(pMsg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);     
        } else {
         pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);
         pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
         pMsg->DATA[3]=nlen; //string length
           //set String or TP
           for (i=0;i<nlen;i++) pMsg->DATA[4+i]=char(VTNewValue[i]);  
        }
        //
      refObj=pVT_Net->VTObjID;
      getVTMacrosListEvents(pVT_Net,refObj,VTOnEntryOfValue);
        if (eventID==VTOnEntryOfNewValue){
          getVTMacrosListEvents(pVT_Net,refObj,eventID);
          pVT_Net->VTPoolRefresh=true;
        }
      valid=true;
     } 
  }
  //
 return valid; 
};//TVT_VTInputStringValue::setMsgToAttr


//==============================================================================
//TVT_VTDisplayActivation Implementaion   0x09  for WindowMask or KeyGroup
boolean TVT_VTDisplayActivation::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=((pVT_Net->VT_ActiveMask>>0) & 0xFF);
   pMsg->DATA[2]=((pVT_Net->VT_ActiveMask>>8) & 0xFF);
   pMsg->DATA[3]=0x01; 
   pMsg->DATA[4]=((pVT_Net->VT_ActiveMask>>0) & 0xFF);  
   pMsg->DATA[5]=((pVT_Net->VT_ActiveMask>>8) & 0xFF);
   pMsg->DATA[6]=0x01;
   pMsg->DATA[7]=0x0F; //TAN number
   valid=true;
   return valid; 
};//TVT_VTDisplayActivation::setMsgToAttr


//==============================================================================
//TVT_VTControlAudioDeviceTermination Implementaion   0x0A
boolean TVT_VTControlAudioDeviceTermination::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=0x01; //was terminated 
   pMsg->DATA[2]=0x0F; //TAN number
     for (int i=3;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
     //
   //pVT_Net->VTAudioActive=0;
   pVT_Net->VTAudioVolume=100;
   pVT_Net->VTRepetition=0;
   pVT_Net->VTFrequency =0;
   pVT_Net->VTOnTimeDuration =0;
   pVT_Net->VTOffTimeDuration=0;
   pVT_Net->VTActiveTime=millis();
   
   
   valid=true;
   return valid; 
};//TVT_VTControlAudioDeviceTermination


//==============================================================================
//TVTUnsupportedFunction Implementaion   253 0xFD
boolean TVTUnsupportedFunction::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
   pMsg->ID=VTtoECU_PGN + (ECU_VT_PRIO<<24) + (pVT_Net->VT_DST[pVT_Net->listNr]<<8) + pVT_Net->VT_SRC;
   pMsg->MSG_TX=1;  pMsg->MSGTYPE=1; pMsg->LEN=8;
   //
   pMsg->DATA[0]=VT0PCommFunction;
   pMsg->DATA[1]=0xF0;
   pMsg->DATA[2]=0xFF;
   pMsg->DATA[3]=0xFF; 
   pMsg->DATA[4]=0xFF;  
   pMsg->DATA[5]=0xFF;
   pMsg->DATA[6]=0xFF;
   pMsg->DATA[7]=0xFF;
   valid=true;
   return valid; 
};//TVTUnsupportedFunction::setMsgToAttr




//==============================================================================
//==============================================================================
