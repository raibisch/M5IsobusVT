//==============================================================================
//Definition objects TVTMacroCommandObject
//TVTESC                    , TVTHideShowObject       , TVTEnableDisableObject,TVTSelectInputObject,
//TVTControlAudioDevice     , TVTSetAudioVolume       ,
//TVTChangeChildLocation    , TVTChangeChildPosition  , TVTChangeSize
//TVTChangeBackgroundColour , TVTChangeNumericValue   , TVTChangeEndPoint,
//TVTChangeFontAttributes   , TVTChangeLineAttributes , TVTChangeFillAttributes
//TVTChangeActiveMask       , TVTChangeSoftKeyMask    , TVTChangeAttribute,
//TVTChangePriority         , TVTChangeListItem       , TVTChangeStringValue
//
//ISOVersion 3 and later
//TVTLockMaskObject         , TVTExecuteMacroObject   , TVTExtendedExecuteMacroObject
//TVTChangeObjectLabel      , TVTChangePolygonPoint   , TVTChangePolygonScale
//TVTGraphicContextCommand  ,
//TVTGetAttribute           ,TVTSelectColourMapObject
//TVTExtendedInputAttributeCommand
//TVTSelectActiveWorkingSet



//==============================================================================
//unit UnitVTMacroCommandObject.cpp
//==============================================================================

#include "UnitVTMacroCommandObject.h"


//==============================================================================
//Macro Commands
//==============================================================================
boolean MacroCANMsgPGN(TVT_Net *pVT_Net,CANMsg *pMsg,boolean macro){
boolean sendMSG=false;  
TVTObject msgObj;
  pVT_Net->VT_ChangeAttr=false;  
    //MACRO-COMMANDS
    if ((!sendMSG) && (pMsg->DATA[0]==0x92)){   
     TVTESC msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }   
    if ((!sendMSG) && (pMsg->DATA[0]==0xA0)){   
     TVTHideShowObject msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }   
    if ((!sendMSG) && (pMsg->DATA[0]==0xA1)){   
     TVTEnableDisableObject msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }   
    if ((!sendMSG) && (pMsg->DATA[0]==0xA2)){   
     TVTSelectInputObject msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }   
    if ((!sendMSG) && (pMsg->DATA[0]==0xA3)){   
     TVTControlAudioDevice msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }   
    if ((!sendMSG) && (pMsg->DATA[0]==0xA4)){   
     TVTSetAudioVolume msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }   
    if ((!sendMSG) && (pMsg->DATA[0]==0xA5)){   
     TVTChangeChildLocation msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xA6)){   
     TVTChangeSize msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xA7)){   
     TVTChangeBackgroundColour msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xA8)){   
     TVTChangeNumericValue msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xA9)){   
     TVTChangeEndPoint msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xAA)){   
     TVTChangeFontAttributes msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xAB)){   
     TVTChangeLineAttributes msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xAC)){   
     TVTChangeFillAttributes msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xAD)){   
     TVTChangeActiveMask msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xAE)){   
     TVTChangeSoftKeyMask msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    //type ChangeAttribute=0xAF  GetAttribute=0xB9
    if ((!sendMSG) && ((pMsg->DATA[0]==0xAF) || (pMsg->DATA[0]==0xB9))){   
     TVTChangeAttribute msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xB0)){   
     TVTChangePriority msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xB1)){   
     TVTChangeListItem msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xB3)){   
     TVTChangeStringValue msgObj; pVT_Net->stream_TP[pVT_Net->listNr].clear();
      for (int i=0;i<pMsg->LEN;i++) pVT_Net->stream_TP[pVT_Net->listNr].write(pMsg->DATA[i]);
     pMsg->DATA[0]=0x13;//pMsg->LEN>8
     pMsg->ID=TP_PGN + ((pMsg->ID & 0xFF)<<8) + ((pMsg->ID>>8) & 0xFF); //togglr src/dst
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xB4) && (macro)){   
     TVTChangeChildPosition msgObj; pVT_Net->stream_TP[pVT_Net->listNr].clear();
       for (int i=0;i<pMsg->LEN;i++) pVT_Net->stream_TP[pVT_Net->listNr].write(pMsg->DATA[i]);
     pMsg->DATA[0]=0x13; //pMsg->LEN=9
     pMsg->ID=TP_PGN + ((pMsg->ID & 0xFF)<<8) + ((pMsg->ID>>8) & 0xFF); //togglr src/dst
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
    }  
    //    
    if ((!sendMSG) && (pMsg->DATA[0]==0xB6)){   
     TVTChangePolygonPoint msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xB7)){   
     TVTChangePolygonScale msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xBE)){   
     TVTExecuteMacroObject msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if ((!sendMSG) && (pMsg->DATA[0]==0xBC)){   
     TVTExtendedExecuteMacroObject msgObj;
     sendMSG=msgObj.setMsgToAttr(pMsg,pVT_Net);
    }  
    if (macro) {
      //Macro TODO
    } else {
      if (sendMSG) {
        Serial.println(getMsgFrameStr(pMsg));
        Serial.println(delm0);
        pVT_Net->VTPoolRefresh=((msgObj.VTError==0) && (pVT_Net->VT_ChangeAttr));
      }
    }
   //
 return sendMSG;
};



//==============================================================================
//Commands and Messages
//==============================================================================
//TVTESCObject Implementaion   146 $92
boolean TVTESC::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0,eventID=VTOnESC;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
int16_t objIdx=-1;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   //get input
   pMsg->DATA[1]=(VTObjectReference>>0) & 0xFF;
   pMsg->DATA[2]=(VTObjectReference>>8) & 0xFF;
   VTError=0x01;
   //Set new ObjectAttribute
   objIdx=getVTDrawID(pVT_Net,VTObjectReference);
     //
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTSelect";valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
   pMsg->DATA[3]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTHideShowObject Implementaion   160 $A0
boolean TVTHideShowObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0,eventID=VTOnShow;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) +  (pMsg->DATA[2]<<8);
   VTHidden=(pMsg->DATA[3]==0); //not visible
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTHidden";valueAttr="";
        valueAttr+=char(VTHidden);
          if (VTHidden) eventID=VTOnHide;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
    //
   pMsg->DATA[4]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTEnableDisableObject Implementaion   161 $A1
boolean TVTEnableDisableObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, optn=0, eventID=VTOnEnable;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
int16_t objIdx=-1;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=((pMsg->DATA[1]<<0)) +  ((pMsg->DATA[2]<<8));
   VTEnabled=pMsg->DATA[3];
   VTError=0x01;
   //Set new ObjectAttribute
   objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (VTEnabled==0) eventID=VTOnDisable;
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTEnabled";valueAttr="";valueAttr+=char(pMsg->DATA[3]);
         //check inputlist
         if (pVT_Net->VTObjType==10){
           nameAttr="VTOptions";
           valueAttr=getVTObjectAttributeDirect(nameAttr,pVT_Net);
           optn=char(valueAttr[0]) & 0xFE; valueAttr="";
           optn=optn | pMsg->DATA[3];
           valueAttr+=char(optn);
         }
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
   pMsg->DATA[4]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};
  


//------------------------------------------------------------------------------
//TVTSelectInputObject Implementaion   162 $A2
boolean TVTSelectInputObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t  err=0,objType=0xFF,eventID=VTOnInputFieldSelection;
int16_t  objIdx=-1,keyNr=-1,keyMax=-1,nn=0,i=-1;
uint16_t objID=0xFFFF;
String   ss="",str="";
uint8_t* buff;
boolean lCount=getVTObjectListSize(pVT_Net),valid=false,change=false;
VTError=0x01;
  if ((lCount>0) && (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net))) {   
   VTObjectReference=((pMsg->DATA[1]<<0)) +  ((pMsg->DATA[2]<<8));
   VTSelect=pMsg->DATA[3]; 
   VTError=0x01; pVT_Net->VT_ChangeAttr=false;
   //
   objIdx=getVTDrawID(pVT_Net,VTObjectReference);
     //
     if (objIdx<0){
      VTSelect=0;err=0;
      change=((change) || (pVT_Net->TFT_KeySelectObjID<0xFFFF));
      change=((change) || (pVT_Net->TFT_ButtonSelectObjID<0xFFFF));
      change=((change) || (pVT_Net->TFT_InputSelectObjID<0xFFFF));
     }
     if (VTSelect==0) eventID=VTOnInputFieldDeselection;
     //
     if (objIdx>=0) {
       pVT_Net->VTObjID=VTObjectReference;
         //Key-object 
         if (pVT_Net->VTObjType==5) {
           objIdx=getVTObjID(pVT_Net,pVT_Net->VT_ActiveSoftKeyMask);
           //getStreamStrInfo(pVT_Net);
           nn=4;
             if (objIdx>=0) keyMax=pVT_Net->streamStr.readBytesVal(1,nn);
             if (keyMax>0) {
                 objIdx=getVTDrawID(pVT_Net,VTObjectReference);
                  if (objIdx<0) {
                    pVT_Net->TFT_KeySelect=0; VTPoolDataRefreshDirect(pVT_Net,false);
                    objIdx=getVTDrawID(pVT_Net,VTObjectReference);
                  }
                  //
                  while ((objIdx<0) && (keyMax>0)){
                    objIdx=getVTDrawID(pVT_Net,VTObjectReference);
                     if (objIdx<0) {
                       keyMax=keyMax-pVT_Net->TFT_KeyNr;                  
                         //Serial.println(keyMax);
                         if (keyMax>0){
                          pVT_Net->TFT_KeySelect+=pVT_Net->TFT_KeyNr;
                          //Serial.println(pVT_Net->TFT_KeySelect);
                          VTPoolDataRefreshDirect(pVT_Net,false);
                         } 
                     }
                  }//while
             }//keyNax>0
             //
             if (objIdx>=0){
              keyNr=pVT_Net->TFT_SelectNr;
              //Serial.println("Focus");
              pVT_Net->TFT_ButtonSelectObjID=0xFFFF;
              pVT_Net->TFT_InputSelectObjID=0xFFFF;
              change=(VTObjectReference!=pVT_Net->TFT_KeySelectObjID);
              pVT_Net->TFT_KeySelect=pVT_Net->TFT_KeyPage*pVT_Net->TFT_KeyNr + keyNr;
                if (pVT_Net->TFT_KeySelectObjID<0xFFFF)    objID=pVT_Net->TFT_KeySelectObjID;
                if (pVT_Net->TFT_ButtonSelectObjID<0xFFFF) objID=pVT_Net->TFT_ButtonSelectObjID;
                if (pVT_Net->TFT_InputSelectObjID<0xFFFF)  objID=pVT_Net->TFT_InputSelectObjID;
              change=((change) || (objID!=pVT_Net->VTObjID));
              pVT_Net->TFT_KeySelectObjID=VTObjectReference;
              pVT_Net->TFT_KeySelectObjType=5;
              pVT_Net->VTSelectMode="C";
              err=0x00;
             }
         }//type==5
         //
         //mask inputs and button=6
         if ((pVT_Net->VTObjType==6) || (HasInArray(pVT_Net->VTObjType,inpAllObjSet))) {
            pVT_Net->VTSelectMode="A";  
             if (pVT_Net->VTObjType==6) pVT_Net->VTSelectMode="B";
             //
             if (pMsg->DATA[3]>0) {
               if (pVT_Net->VTObjType==6){
                   if (pVT_Net->TFT_KeySelectObjID<0xFFFF)    objID=pVT_Net->TFT_KeySelectObjID;
                   if (pVT_Net->TFT_ButtonSelectObjID<0xFFFF) objID=pVT_Net->TFT_ButtonSelectObjID;
                   if (pVT_Net->TFT_InputSelectObjID<0xFFFF)  objID=pVT_Net->TFT_InputSelectObjID;
                 change=((change) || (objID!=pVT_Net->VTObjID));
                 pVT_Net->TFT_ButtonSelectObjID=pVT_Net->VTObjID;
                 pVT_Net->TFT_ButtonSelectObjType=6;
               }else {
                   if (pVT_Net->TFT_KeySelectObjID<0xFFFF)    objID=pVT_Net->TFT_KeySelectObjID;
                   if (pVT_Net->TFT_ButtonSelectObjID<0xFFFF) objID=pVT_Net->TFT_ButtonSelectObjID;
                   if (pVT_Net->TFT_InputSelectObjID<0xFFFF)  objID=pVT_Net->TFT_InputSelectObjID;
                 change=((change) || (objID!=pVT_Net->VTObjID));
                 pVT_Net->TFT_InputSelectObjID=pVT_Net->VTObjID;
                 pVT_Net->TFT_InputSelectObjType=pVT_Net->VTObjType;
               }
             } else {
               if (pVT_Net->VTObjType==6){
                change=((change) || (pVT_Net->TFT_ButtonSelectObjID!=0xFFFF));
                pVT_Net->TFT_ButtonSelectObjID=0xFFFF;
                pVT_Net->TFT_ButtonSelectObjType=0xFF;
               }else {
                change=((change) || (pVT_Net->TFT_InputSelectObjID!=0xFFFF));
                pVT_Net->TFT_InputSelectObjID=0xFFFF;
                pVT_Net->TFT_InputSelectObjType=0xFF;
               }
              eventID=VTOnInputFieldDeselection;
             }
             if (pVT_Net->VTObjType==6) pVT_Net->VTSelectMode="B";
             // 
           err=0x00;
         }//mask inputs
         //
         //
     }//objIdx>=0
     //
     if (VTObjectReference==0xFFFF) {
        if (pVT_Net->TFT_KeySelectObjID<0xFFFF)    objID=pVT_Net->TFT_KeySelectObjID;
        if (pVT_Net->TFT_ButtonSelectObjID<0xFFFF) objID=pVT_Net->TFT_ButtonSelectObjID;
        if (pVT_Net->TFT_InputSelectObjID<0xFFFF)  objID=pVT_Net->TFT_InputSelectObjID;
      change=(objID!=VTObjectReference);
      err=0;
     }
     //
   VTError=err;
   pMsg->DATA[4]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
        if (objID<0xFFFF) {
         getVTMacrosListEvents(pVT_Net,objID,VTOnInputFieldDeselection);
           if (objID==pVT_Net->TFT_KeySelectObjID)    pVT_Net->TFT_KeySelectObjID=0xFFFF;
           if (objID==pVT_Net->TFT_ButtonSelectObjID) pVT_Net->TFT_ButtonSelectObjID=0xFFFF;
           if (objID==pVT_Net->TFT_InputSelectObjID)  pVT_Net->TFT_InputSelectObjID=0xFFFF;
        }
        //        
        if ((VTObjectReference<0xFFFF) && (eventID==VTOnInputFieldSelection)) {
          getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
        }
     }//VTError
     //
   pVT_Net->VT_ChangeAttr=change;
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//TVTControlAudioDevice Implementaion   163 $A3
boolean TVTControlAudioDevice::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false,TEST=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   pVT_Net->VTRepetition=pMsg->DATA[1];
   pVT_Net->VTFrequency =(pMsg->DATA[2]<<0) + (pMsg->DATA[3]<<8);
   pVT_Net->VTOnTimeDuration =(pMsg->DATA[4]<<0) + (pMsg->DATA[5]<<8);
   pVT_Net->VTOffTimeDuration=(pMsg->DATA[6]<<0) + (pMsg->DATA[7]<<8);
   pVT_Net->VTAudioActive=1;
   pMsg->DATA[1]=0;
     //
     if (TEST){
      Serial.print(pVT_Net->VTRepetition);Serial.print(" ");
      Serial.print(pVT_Net->VTFrequency) ;Serial.print(" ");
      Serial.print(pVT_Net->VTOnTimeDuration);Serial.print(" ");
      Serial.println(pVT_Net->VTOffTimeDuration);
     }
     //
     for (int i=2; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   VTError=pMsg->DATA[1];
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTControlAudioDevice Implementaion   164 $A4
boolean TVTSetAudioVolume::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false,TEST=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   pVT_Net->VTAudioVolume=pMsg->DATA[1];
   pVT_Net->VTAudioActive=1;
   pMsg->DATA[1]=0;
     if (TEST){
      Serial.println(pVT_Net->VTAudioVolume);
     }
     //
     for (int i=2; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   VTError=pMsg->DATA[1];
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//TVTChangeChildLocation Implementaion   165 $A5
boolean TVTChangeChildLocation::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeChildLocation;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTParentReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTObjectReference=(pMsg->DATA[3]<<0) + (pMsg->DATA[4]<<8);
   VTxChange=pMsg->DATA[5]-0x7F;
   VTyChange=pMsg->DATA[6]-0x7F;
   pVT_Net->VT_ChangeAttr=((VTxChange!=0) || (VTyChange!=0)); 
   VTError=0x01;
   //
   //Set new ObjectAttribute
   if (pVT_Net->VT_ChangeAttr){
    int16_t objIdx=getVTObjID(pVT_Net,VTParentReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTObjects=0"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
      }
     }else VTError=0x00;
     //
     for (int i=6; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTParentReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeChildPosition Implementaion   180 $B4
boolean TVTChangeChildPosition::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
uint8_t err=0, eventID=VTOnChangeChildPosition;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
uint8_t ctrlByte=0xFF, src=((pMsg->ID)>>0) & 0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if ((pStream!=NULL) && (pStream->available()>8)) ctrlByte=pStream->read();
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
     if (pStream->available()>=8) {
      VTParentReference=(pStream->read()<<0) + (pStream->read()<<8); 
      VTObjectReference=(pStream->read()<<0) + (pStream->read()<<8); 
      VTxPos=(pStream->read()<<0) + (pStream->read()<<8); 
      VTyPos=(pStream->read()<<0) + (pStream->read()<<8); 
     }
   pMsg->DATA[1]=(VTParentReference>>0) & 0xFF;
   pMsg->DATA[2]=(VTParentReference>>8) & 0xFF;
   pMsg->DATA[3]=(VTObjectReference>>0) & 0xFF;
   pMsg->DATA[4]=(VTObjectReference>>8) & 0xFF;
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTParentReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTObjects=1"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char((VTxPos>>0) & 0xFF);
        valueAttr+=char((VTxPos>>8) & 0xFF);
        valueAttr+=char((VTyPos>>0) & 0xFF);
        valueAttr+=char((VTyPos>>8) & 0xFF);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=6; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTParentReference,eventID);
     }
   valid=true;
  }
return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeSize Implementaion   166 $A6
boolean TVTChangeSize::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0,eventID=VTOnChangeSize;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTWidth =(pMsg->DATA[3]<<0) + (pMsg->DATA[4]<<8);
   VTHeight=(pMsg->DATA[5]<<0) + (pMsg->DATA[6]<<8);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTWidth"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
          //check objects Meter,CheckBox,Picture
          if (HasInArray(pVT_Net->VTObjType,widthObjSet)){
           //height not allowed  
          }else {
           nameAttr="VTHeight", valueAttr="";
           valueAttr+=char(pMsg->DATA[5]);
           valueAttr+=char(pMsg->DATA[6]);
           err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
           pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
          }
        VTError=err;    
     }
     //
     for (int i=4; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeBackgroundColour Implementaion   167 $A7
boolean TVTChangeBackgroundColour::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeBackgroundColour;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTBackgroundColour =(pMsg->DATA[3]<<0);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTBackgroundColour"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=5; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[4]=VTError; 
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};

//------------------------------------------------------------------------------
//TVTChangeNumericValue Implementaion   168 $A8
boolean TVTChangeNumericValue::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeValue;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTValue=(pMsg->DATA[4]<<0) + (pMsg->DATA[5]<<8) + + (pMsg->DATA[6]<<16) + (pMsg->DATA[7]<<24);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTValue"; valueAttr="";
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        valueAttr+=char(pMsg->DATA[7]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     //for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeEndPoint Implementaion  169      $A9
boolean TVTChangeEndPoint::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeEndPoint;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTWidth=(pMsg->DATA[3]<<0) + (pMsg->DATA[4]<<8);
   VTHeight=(pMsg->DATA[5]<<0) + (pMsg->DATA[6]<<8);
   VTLineDirection=(pMsg->DATA[7]<<0);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTWidth"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTHeight"; valueAttr="";
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTLineDirection"; valueAttr="";
        valueAttr+=char(pMsg->DATA[7]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeFontAttributes Implementaion  170      $AA
boolean TVTChangeFontAttributes::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeFontAttributes;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTFontColour=(pMsg->DATA[3]<<0);
   VTFontSize  =(pMsg->DATA[4]<<0);
   VTFontType  =(pMsg->DATA[5]<<0);
   VTFontStyle =(pMsg->DATA[6]<<0);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        //        
        nameAttr="VTFontColour"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //        
        nameAttr="VTFontSize"; valueAttr="";
        valueAttr+=char(pMsg->DATA[4]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTFontType"; valueAttr="";
        valueAttr+=char(pMsg->DATA[5]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTFontStyle"; valueAttr="";
        valueAttr+=char(pMsg->DATA[6]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        //
        pVT_Net->VT_SetEvent=true;
        VTError=err;
     }
     //
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeLineAttributes Implementaion  171      $AB
boolean TVTChangeLineAttributes::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeLineAttributes;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTLineColour=(pMsg->DATA[3]<<0);
   VTLineWidth =(pMsg->DATA[4]<<0);
   VTLineArt   =(pMsg->DATA[5]<<0) + (pMsg->DATA[6]<<8);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTLineColour"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTLineWidth"; valueAttr="";
        valueAttr+=char(pMsg->DATA[4]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTLineArt"; valueAttr="";
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//TVTChangeFillAttributes Implementaion  172      $AC
boolean TVTChangeFillAttributes::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeFillAttributes;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTFillType   =(pMsg->DATA[3]<<0);
   VTFillColour =(pMsg->DATA[4]<<0);
   VTFillPattern=(pMsg->DATA[5]<<0) + (pMsg->DATA[6]<<8);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTFillType"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTFillColour"; valueAttr="";
        valueAttr+=char(pMsg->DATA[4]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        change=((change) || (pVT_Net->VT_ChangeAttr));
        //
        nameAttr="VTFillPattern"; valueAttr="";
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeActiveMask Implementaion   173     $AD
boolean TVTChangeActiveMask::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeActiveMask;
String nameAttr="", valueAttr="";
uint16_t activeMask=pVT_Net->VT_ActiveMask;
uint16_t activeSoftKeyMask=pVT_Net->VT_ActiveSoftKeyMask;
int16_t  objIdx=-1;
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTWorkingSet=(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTNewActiveMask=(pMsg->DATA[3]<<0) + (pMsg->DATA[4]<<8);
   pMsg->DATA[1]=(VTNewActiveMask>>0) & 0xFF;
   pMsg->DATA[2]=(VTNewActiveMask>>8) & 0xFF;
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTWorkingSet);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTActiveMask";valueAttr="";
        valueAttr+=char(pMsg->DATA[1]);
        valueAttr+=char(pMsg->DATA[2]);
        pVT_Net->VT_SetEvent=false;
        VTError=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
     }
     //
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTWorkingSet,eventID);
      //
      getVTMacrosListEvents(pVT_Net,activeMask,VTOnHide);
      getVTMacrosListEvents(pVT_Net,activeSoftKeyMask,VTOnHide);
      //
      getVTMacrosListEvents(pVT_Net,VTNewActiveMask,VTOnShow);
      objIdx=getVTObjID(pVT_Net,VTNewActiveMask,false);
        if (objIdx>=0) {
         activeSoftKeyMask=pVT_Net->streamStr.readBytesVal(2,4);
         getVTMacrosListEvents(pVT_Net,activeSoftKeyMask,VTOnShow);
        }
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeSoftKeyMask Implementaion   174     $AE
boolean TVTChangeSoftKeyMask::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeSoftKeyMask;
String nameAttr="", valueAttr="";
uint16_t activeSoftKeyMask=pVT_Net->VT_ActiveSoftKeyMask;
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTDataMaskType =(pMsg->DATA[1]<<0);
   VTMaskID   =(pMsg->DATA[2]<<0) + (pMsg->DATA[3]<<8);
   VTNewMaskID=(pMsg->DATA[4]<<0) + (pMsg->DATA[5]<<8);
   pMsg->DATA[1]=((VTMaskID>>0) & 0xFF);   pMsg->DATA[2]=((VTMaskID>>8) & 0xFF);
   pMsg->DATA[3]=((VTNewMaskID>>0) & 0xFF);pMsg->DATA[4]=((VTNewMaskID>>8) & 0xFF);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTMaskID);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTSoftKeyMask";valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=5; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTMaskID,eventID);
      
      getVTMacrosListEvents(pVT_Net,activeSoftKeyMask,VTOnHide);
      getVTMacrosListEvents(pVT_Net,VTNewMaskID,VTOnShow);
     }
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangeAttribute Implementaion   175     $AF=changeAttribute or $B9=getAttribute
boolean TVTChangeAttribute::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0,eventID=VTOnChangeAttribute;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
 VT0PCommFunction=pMsg->DATA[0];
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference =(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTAttributeID=(pMsg->DATA[3]<<0);
   VTValue=(pMsg->DATA[4]<<0) + (pMsg->DATA[5]<<8) + (pMsg->DATA[6]<<16) + (pMsg->DATA[7]<<24);
   //
   pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);   pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[3]=((VTAttributeID>>0) & 0xFF);
   //
   pVT_Net->VT_CMD=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=(VT0PCommFunction==0xAF);
   //
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        //getStreamStrInfo(pVT_Net);
        //get AttrbuteList(objType) 
        nameAttr="VT_AID=" + String(VTAttributeID); valueAttr="";
        //check SetAttribute
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        valueAttr+=char(pMsg->DATA[7]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }//objIdx>=0
     //
     //check GetAttribute
     if (VT0PCommFunction==0xB9) {
      //Serial.println(pVT_Net->VT_AttrValue);
      pMsg->DATA[4]=(pVT_Net->VT_AttrValue>>0)  & 0xFF;
      pMsg->DATA[5]=(pVT_Net->VT_AttrValue>>8)  & 0xFF;
      pMsg->DATA[6]=(pVT_Net->VT_AttrValue>>16) & 0xFF;
      pMsg->DATA[7]=(pVT_Net->VT_AttrValue>>24) & 0xFF;
     } else {
        for (int i=4; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
      pMsg->DATA[4]=VTError;   
     }
     //TEST
     //Serial.println(VTObjectReference);
     //Serial.println(VTError);
     //Serial.println(String(VT0PCommFunction,HEX));
     //getMacro events and commands
     if ((VTError==0x00) && (VT0PCommFunction==0xAF)) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
     //  
   valid=true;
  }
 return valid; 
};

//------------------------------------------------------------------------------
//TVTChangePriority Implementaion   176     $B0
boolean TVTChangePriority::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangePriority;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference =(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTNewPriority=(pMsg->DATA[3]<<0);
   pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);   pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[3]=((VTNewPriority>>0) & 0xFF);
   //
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTPriority";valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=4; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[4]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//TVTChangeListItem Implementaion   177     $B1
boolean TVTChangeListItem::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0, eventID=VTOnChangeValue;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference =(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTNewListIndex=(pMsg->DATA[3]<<0);
   VTNewObjectID =(pMsg->DATA[4]<<0) + (pMsg->DATA[5]<<8);
   //
   pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);   pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[3]=((VTNewListIndex>>0) & 0xFF);
   pMsg->DATA[4]=((VTNewObjectID>>0) & 0xFF);   pMsg->DATA[5]=((VTNewObjectID>>8) & 0xFF);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTItemList"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char(pMsg->DATA[5]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=6; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[6]=VTError;   
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//TVTChangeStringValue Implementaion   179 $B3
boolean TVTChangeStringValue::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
uint8_t err=0, eventID=VTOnChangeValue;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
uint16_t len=0,i=0;
uint8_t ctrlByte=0xFF, src=((pMsg->ID)>>0) & 0xFF,objType=0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
  if ((pStream!=NULL) && (pStream->available())) ctrlByte=pStream->read();
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=false;
   VTError=0x01;
       //
       if (pStream->available()) VTObjectReference=pStream->readBytesVal(2); 
      //Set new ObjectAttribute
      int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
       //
       if (objIdx>=0) {
          getVTObjectFromList(pVT_Net,objIdx);
          nameAttr="VTLength"; 
          len=pStream->readBytesVal(2);
          VTNewValue+=pStream->readBytesString(len);
          valueAttr+=VTNewValue;
          //TEST
          //getStringHEXInfo(valueAttr);
          //getStringHEXInfo(VTNewValue);
          //
          //Serial.println(len);
          //Serial.println(VTNewValue);
          //Serial.println(nameAttr);
          //Serial.println(valueAttr);
          
          pVT_Net->VT_SetEvent=false;
          err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
          pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
          VTError=err;
       }//objIdx
     //
   pMsg->DATA[1]=0xFF;pMsg->DATA[2]=0xFF;
   pMsg->DATA[3]=(VTObjectReference>>0) & 0xFF;
   pMsg->DATA[4]=(VTObjectReference>>8) & 0xFF;
     //
     for (int i=5; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[5]=VTError;
     //getMacro events and commands
     if (VTError==0x00) {
      getVTMacrosListEvents(pVT_Net,VTObjectReference,eventID);
     }
   valid=true;
  }//msg
 return valid; 
};



//------------------------------------------------------------------------------
//TVTChangePolygonPoint Implementaion   182 $B6
boolean TVTChangePolygonPoint::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
TVTObjIDPoints points;
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference =(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTPointIndex=(pMsg->DATA[3]<<0);
   VTxPos =(pMsg->DATA[4]<<0) + (pMsg->DATA[5]<<8);
   VTyPos =(pMsg->DATA[6]<<0) + (pMsg->DATA[7]<<8);
   pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);   pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        //TEST
        //getStreamStrInfo(pVT_Net);    
        nameAttr="VTPoints"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        valueAttr+=char(pMsg->DATA[7]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//TVTChangePolygonScale Implementaion   183 $B7
boolean TVTChangePolygonScale::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   VTObjectReference =(pMsg->DATA[1]<<0) + (pMsg->DATA[2]<<8);
   VTNewWidth =(pMsg->DATA[3]<<0) + (pMsg->DATA[4]<<8);
   VTNewHeight=(pMsg->DATA[5]<<0) + (pMsg->DATA[6]<<8);
   //
   pMsg->DATA[1]=((VTObjectReference>>0) & 0xFF);   pMsg->DATA[2]=((VTObjectReference>>8) & 0xFF);
   pMsg->DATA[3]=((VTNewWidth>>0) & 0xFF);   pMsg->DATA[4]=((VTNewWidth>>8) & 0xFF);
   pMsg->DATA[5]=((VTNewHeight>>0) & 0xFF);   pMsg->DATA[6]=((VTNewHeight>>8) & 0xFF);
   //
   VTError=0x01;
   //Set new ObjectAttribute
   int16_t objIdx=getVTObjID(pVT_Net,VTObjectReference);
     if (objIdx>=0) {
        getVTObjectFromList(pVT_Net,objIdx);
        nameAttr="VTScale"; valueAttr="";
        valueAttr+=char(pMsg->DATA[3]);
        valueAttr+=char(pMsg->DATA[4]);
        valueAttr+=char(pMsg->DATA[5]);
        valueAttr+=char(pMsg->DATA[6]);
        pVT_Net->VT_SetEvent=false;
        err+=SetVTObjectAttribute(nameAttr,valueAttr,pVT_Net);
        pVT_Net->VT_ChangeAttr=((change) || (pVT_Net->VT_ChangeAttr));
        VTError=err;
     }
     //
     for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[7]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//Begin Messages ISO Version 3
//------------------------------------------------------------------------------
//type    //189  $BD
boolean TVTLockMaskObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   //
     //
     for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[7]=VTError;   
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//type    //190  $BE
boolean TVTExecuteMacroObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   uint16_t objID=(pMsg->DATA[1]);
   pVT_Net->stream_Cmd[pVT_Net->listNr].clear();
   VTError=runMacroCommands(pVT_Net,objID);
     for (int i=3; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[2]=VTError;   
   valid=true;
  }
 return valid; 
};



//------------------------------------------------------------------------------
//type    //188 $BC
boolean TVTExtendedExecuteMacroObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   uint16_t objID=(pMsg->DATA[1]) + (pMsg->DATA[2]<<8);
   pVT_Net->stream_Cmd[pVT_Net->listNr].clear();
   VTError=runMacroCommands(pVT_Net,objID);
     for (int i=4; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[3]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//type    //181  $B5
boolean TVTChangeObjectLabel::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
     //
     for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[7]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//type   //184   $B8
boolean TVTGraphicContextCommand::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   //
     //
     for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[7]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//type    //186  $BA
boolean TVTSelectColourMapObject::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   //
     //
     for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[7]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//type   //0 .. 15 , $00   $0F, CodePlanes
boolean TVTExtendedInputAttributeCommand::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t err=0;
String nameAttr="", valueAttr="";
boolean valid=false,change=false;
  if (getMsgToAttr(VT0PCommFunction,pMsg,pVT_Net)) {   
   //
     //
     for (int i=7; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[7]=VTError;   
   valid=true;
  }
 return valid; 
};


//------------------------------------------------------------------------------
//type  //144  $90
boolean TVTSelectActiveWorkingSet::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net, LoopbackStream *pStream){
uint8_t i=0,oList=pVT_Net->listNr;
boolean valid=false;
uint8_t ctrlByte=0xFF, src=((pMsg->ID)>>0) & 0xFF,objType=0xFF;
uint8_t dst=((pMsg->ID)>>8) & 0xFF,bb=0;
  if ((pStream!=NULL) && (pStream->available())) ctrlByte=pStream->read();
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
   pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
   pMsg->DATA[0]=VT0PCommFunction;
   pVT_Net->VT_ChangeAttr=false;
   VTError=0x01;
     //
     for (i=0;i<8;i++) {
         if (pStream->available()) VTWorkingSetName=getStringHEX(pStream->read(),2) + VTWorkingSetName; 
     }//for i
   //reset selected Input
   resetSelectInput(pVT_Net);
   //TEST
   Serial.println("VTWorkingSetName"); 
   Serial.println(VTWorkingSetName); 
   Serial.println(pVT_Net->VT_DST_WS[0]); 
   Serial.println(pVT_Net->VT_DST_WS[1]); 
     //
     for (i=0;i<listMax;i++) {
       if (VTWorkingSetName==pVT_Net->VT_DST_WS[i]) {
         pVT_Net->listNr=i; 
         //
         if (getVTObjectListSize(pVT_Net)>0) {
          pVT_Net->VT_ChangeAttr=(pVT_Net->listNr!=oList);
          VTError=0;
         } else pVT_Net->listNr=oList;
        break; 
       }
     }//for i
     //
     for (int i=1; i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
   pMsg->DATA[1]=VTError;   
   valid=true;
  }
 return valid; 
};


//==============================================================================
//==============================================================================
