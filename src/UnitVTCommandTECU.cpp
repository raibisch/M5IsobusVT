//==============================================================================
//Definition objects TECU and Diagnostic Messages
//TVTAcknowledgeMessage, TVTWorkingSetMaster, TVTWorkingSetMember
//TVT_TECUFacilities, TVT_TECURemoteControl, TVT_TECURepetitionRate,
//TVT_DiagConformance,
//Functions
//SetBasedSpeedMessage, SetEngineSpeedMessage
//SetHitchStatusMessage, SetPTOOutShaftMessage,
//SetAuxiliaryValveMessage
//==============================================================================
//unit UnitVTCommandTECU.cpp
//==============================================================================

#include "UnitVTCommandTECU.h"


//==============================================================================
//TVTAcknowledgeMessage Implementaion
boolean TVTAcknowledgeMessage::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net){
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
       //setWSNameSetToMsgAndStr(pMsg,pVT_Net);
       //set WorkingSetName
       //setWorkingSetName(pMsg,pVT_Net);
       //
       valid=true;
     }   
 return valid; 
};//TVTAcknowledgeMessage::setMsgToAttr




//==============================================================================
//TECU FUNCTIONS Messages
//==============================================================================
/*
//------------------------------------------------------------------------------
//pVT_Net->tecuData.wheelSpeed
class TVT_TECU_Data {
 public:
 uint8_t  Direction=0;
 uint8_t  StartStopState=0;
 uint8_t  wgKey=0;
 uint8_t  MaxTimeOfTractorPower=0;
 uint32_t wheelSpeed=0;
 uint32_t groundSpeed=0;
 uint32_t Distance=0;
 //
 uint16_t engineSpeed=0;
 //
 uint8_t rearHitch=0;
 uint8_t frontHitch=0;
 //
 uint16_t rearPTO=0;
 uint16_t frontPTO=0;
};
*/

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//getSpeedMessage Wheel=0x00FE4800, Ground=0x00FE4900
String getSpeedMessage(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t tMax=0,Direction=0,StartStopState=0,k=0;
uint32_t bSpeed=0,Distance=0;
    bSpeed=(pMsg->DATA[1]<<8) + pMsg->DATA[0];
    //
    Distance+=pMsg->DATA[2]<<0;
    Distance+=pMsg->DATA[3]<<8;
    Distance+=pMsg->DATA[4]<<16;
    Distance+=pMsg->DATA[5]<<24;
    //
    tMax=pMsg->DATA[6]; //MaxTimeOfTractorPower
    Direction=pMsg->DATA[7] & 0x03;
    StartStopState=pMsg->DATA[7] & 0x10;
      //
      if ((pMsg->ID & 0x00FFFF00)==TECU_WHEEL_PGN){
       pVT_Net->tecuData.MaxTimeOfTractorPower=tMax;
       pVT_Net->tecuData.Direction=Direction;
       pVT_Net->tecuData.StartStopState=StartStopState;
      }else {
       pVT_Net->tecuData.Distance=Distance;k=1;
      }
   pVT_Net->tecuData.objValue1[k]=bSpeed;
   String str=String(round(3.6*bSpeed/1000)) + " km/h";
   return str;
}; //getSpeedMessage



//------------------------------------------------------------------------------
//getEngineSpeedMessage 0x00F00400
String getEngineSpeedMessage(CANMsg *pMsg,TVT_Net *pVT_Net){
uint16_t  eSpeed=0,k=2;
  eSpeed+=pMsg->DATA[3]<<0;
  eSpeed+=pMsg->DATA[4]<<8;
  pVT_Net->tecuData.objValue1[k]=eSpeed;
  String str=String(round(eSpeed/8)) + " rpm";
  return str;
};//getEngineSpeedMessage



//------------------------------------------------------------------------------
//getHitchStatusMessage 0x00FE4500, Front 0x00FE4600
String getHitchStatusMessage(CANMsg *pMsg,TVT_Net *pVT_Net){
uint8_t hitch=0,k=3;
   hitch=pMsg->DATA[0];
    if ((pMsg->ID & 0x00FFFF00)==TECU_FrontHitchStatus_PGN) k=4;
   pVT_Net->tecuData.objValue1[k]=hitch;
   String str=String(round(hitch)) + " %";
   return str;
};//getHitchStatusMessage


//------------------------------------------------------------------------------
//getPTOOutShaftMessage 0x00FE4300, Front 0x00FE4500
String getPTOOutShaftMessage(CANMsg *pMsg,TVT_Net *pVT_Net){
uint16_t pto=0,k=5;
   pto+=pMsg->DATA[0]<<0;
   pto+=pMsg->DATA[1]<<8;
    if ((pMsg->ID & 0x00FFFF00)==TECU_FrontPTOOutShaft_PGN) k=6;
   pVT_Net->tecuData.objValue1[k]=pto;
   String str=String(round(pto)) + " rpm";
   return str;
};//getPTOOutShaftMessage


//==============================================================================
//==============================================================================
boolean setTECUMonitor(TVT_Net *pVT_Net){
boolean TEST=false;
uint8_t i=0;
uint16_t objID=0xFFFF;
  for (i=0;i<8;i++){
    objID=pVT_Net->tecuData.pObjID[i];
    pVT_Net->tecuData.pObjIdx[i]=getVTObjID(pVT_Net,objID,false,true);
      if (TEST){
       setSerialPrint(pVT_Net,String(i) + ".pObjID="  + String(pVT_Net->tecuData.pObjID[i]));
       setSerialPrint(pVT_Net,String(i) + ".pObjIdx=" + String(pVT_Net->tecuData.pObjIdx[i]));
      }
    //
    objID=pVT_Net->tecuData.objID[i];
    pVT_Net->tecuData.objIdx[i]=-1;
    pVT_Net->tecuData.objValue0[i]=0;
    pVT_Net->tecuData.objValue1[i]=0;
      if (objID<0xFFFF) {
        pVT_Net->tecuData.objIdx[i]=getVTObjID(pVT_Net,objID,false,true);
          //get VariableNumericReference
          if (pVT_Net->tecuData.objIdx[i]>=0){
             if (pVT_Net->streamStr.available()>=7){
              pVT_Net->tecuData.objValue0[i]=pVT_Net->streamStr.readBytesVal(4,3);
              pVT_Net->tecuData.objValue1[i]=pVT_Net->tecuData.objValue0[i];
              //TEST
              setSerialPrint(pVT_Net,String(i) + ".ObjID=" + String(objID));
              setSerialPrint(pVT_Net,String(i) + ".ObjIdx=" + String(pVT_Net->tecuData.objIdx[i]));
              setSerialPrint(pVT_Net,String(i) + ".Value=" + String(pVT_Net->tecuData.objValue0[i]));
              getStreamStrInfo(pVT_Net);
              //setSerialPrint(pVT_Net,delm0);
             }
          }
      }
  }//for i
 return true; 
};//setTECUMonitor

//==============================================================================
//Set new ObjectAttribute of VariableReference
boolean setTECUDataValue(int8_t k, TVT_Net *pVT_Net){
int8_t objIdx=-1;
uint32_t objValue0=0,objValue1=0;
  pVT_Net->VT_ChangeAttr=false;
     if (k>=0) {
      objIdx=pVT_Net->tecuData.objIdx[k];
      objValue1=pVT_Net->tecuData.objValue1[k];
       //
       if (objIdx>=0) {
        getVTObjectListValue(pVT_Net,objIdx);
        
        //old value getStreamStrInfo(pVT_Net);
        objValue0=pVT_Net->streamStr.readBytesVal(4,3);
        pVT_Net->VT_ChangeAttr=(objValue1!=objValue0);
        //TEST
        //setSerialPrint(pVT_Net,String(objValue0));
        //setSerialPrint(pVT_Net,String(objValue1));
          //
          if (pVT_Net->VT_ChangeAttr){
           pVT_Net->tecuData.objValue0[k]=objValue0;
           pVT_Net->streamStr.writeBytesVal(objValue1,4,3);
           setVTObjectListValue(pVT_Net,objIdx);
          }
          //
        //getStreamStrInfo(pVT_Net);
       }//objIdx>=0
     }//k>=0
     //
  return pVT_Net->VT_ChangeAttr;
};//setTECUDataValue


//==============================================================================
boolean getTECUMonitor(CANMsg *pMsg,TVT_Net *pVT_Net){
boolean valid=false;
int16_t objIdx=-1,k=-1;
uint32_t objValue0=0,objValue1=0;
String str="";
  if (pVT_Net->listNr==(listMax-1)){
    //
    //Wheel=0x00FE4800
    if ((pMsg->ID & 0x00FFFF00)==TECU_WHEEL_PGN){
     k=0;str=getSpeedMessage(pMsg,pVT_Net);
     valid=(str.length()>0);
    }
    //Ground=0x00FE4900
    if ((pMsg->ID & 0x00FFFF00)==TECU_GROUND_PGN){
     k=1;str=getSpeedMessage(pMsg,pVT_Net);
     valid=(str.length()>0);
    }
    //getEngineSpeedMessage $00F00400
    if ((pMsg->ID & 0x00FFFF00)==TECU_ENGINE_PGN){
      k=2;str=getEngineSpeedMessage(pMsg,pVT_Net);
     valid=(str.length()>0);
    }
    //Rear getHitchStatusMessage 0x00FE4500
    if ((pMsg->ID & 0x00FFFF00)==TECU_RearHitchStatus_PGN){
     k=3;str=getHitchStatusMessage(pMsg,pVT_Net);
     valid=(str.length()>0);
    }
    //Front getHitchStatusMessage 0x00FE4600
    if ((pMsg->ID & 0x00FFFF00)==TECU_FrontHitchStatus_PGN){
     k=4;str=getHitchStatusMessage(pMsg,pVT_Net);
     valid=(str.length()>0);
    }
    //Rear getPTOOutShaftMessage 0x00FE4300;
    if ((pMsg->ID & 0x00FFFF00)==TECU_RearPTOOutShaft_PGN){
     k=5;str=getPTOOutShaftMessage(pMsg,pVT_Net); 
     valid=(str.length()>0);
    }
    //Front getPTOOutShaftMessage 0x00FE4400;
    if ((pMsg->ID & 0x00FFFF00)==TECU_FrontPTOOutShaft_PGN){
     k=6;str=getPTOOutShaftMessage(pMsg,pVT_Net); 
     valid=(str.length()>0);
    }
    //
    //
    if (valid) {
      if (setTECUDataValue(k, pVT_Net)){
        if (k>=0){
          objValue0=pVT_Net->tecuData.objValue0[k];
          objValue1=pVT_Net->tecuData.objValue1[k];
        }
        //refresh
        if (k==0) {
          pVT_Net->tecuData.wheel++;
           if (pVT_Net->tecuData.wheel>0){
              pVT_Net->tecuData.wheel=0;
              /*
              //Meter fast mode
              uint16_t r=130/2-10;
              uint16_t x=5+16+r;
              uint16_t y=12 + r;
              //              
              float ww0=M_PI*objValue0/22222;
              float ww1=M_PI*objValue1/22222;
              
              //
              uint16_t x0=x - r*cos(ww0);
              uint16_t y0=y - r*sin(ww0);
              //
              uint16_t x1=x - r*cos(ww1);
              uint16_t y1=y - r*sin(ww1);
              
              pVT_Net->tft.drawLine(x, y,x0, y0, TFT_WHITE);
              pVT_Net->tft.drawLine(x, y,x1, y1, TFT_BLACK);
              */
              pVT_Net->VTPoolRefresh=true;
              //Serial.println(str);
           }//wheel>0
        }//k=0
      }
    }//valid
  }//listNr
  //
 return valid;
};//getTECUMonitor



//==============================================================================
//==============================================================================
