//==============================================================================
//Definition objects
//TVTAuxiliaryAssignment
//TVTAuxiliaryPrefAssignment, TVTAuxiliaryInputStatus,
//TVTAuxiliaryInputMaintenance, TVTAuxiliaryInputStatusEnable
//==============================================================================
//unit UnitVTAuxAssignment.cpp
//==============================================================================

#include "UnitVTAuxAssignment.h"

/*
TX1 1CEC260A  8  10 11 00 03 FF 00 E7 00
[TP.CM_RTS_E700_00_0]  Size=17  Packets=3
ECUtoVT_PGN
TIME00 [ms]=0

RX1 18EC0A26  8  11 03 01 FF FF 00 E7 00
[TP.CM_CTS_E700_00_0]  Packets=3  Next=1
ECUtoVT_PGN
TIME00 [ms]=2

TX1 18EB260A  8  01 22 01 65 00 60 1F 00
TX1 18EB260A  8  02 80 0C 21 65 00 01 94
TX1 18EB260A  8  03 75 94 75 FF FF FF FF
RX1 18EC0A26  8  13 11 00 FF FF 00 E7 00
[TP.CM_EOMA_E700_22_0]  Size=17  Packets=255
ECUtoVT_PGN
TIME00 [ms]=3

AuxiliaryPrefAssignment (00)
Unit_Nr=1 / 1
 WorkingSetName_hex=210C80001F600065
 ModelIdentCode_hex=0065
  PrefFuncNumber=1
   FuncInpID00=30100 [7594] <- 30100 [7594]
*/


//========================================================================
//TVTAuxiliaryPrefAssignment Implementaion 0x22
uint8_t TVTAuxiliaryPrefAssignment::setVTAuxAssignList(TVT_Net *pVT_Net,uint8_t dst){
uint8_t err=0x00,olistNr=pVT_Net->listNr;
String  wsFuncName=getWSNameFromAddress(pVT_Net,dst), str=wsFuncName;
   //clear 
   if ((wsFuncName=="FFFFFFFFFFFFFFFF") || (VTWSetName=="FFFFFFFFFFFFFFFF")){
    pVT_Net->VTAuxAssignList.clear();
    return err;
   }
   //
   for (int i=0;i<listMax;i++){
    pVT_Net->listNr=i;     
     if (wsFuncName==pVT_Net->VT_DST_WS[i]){
       if (getVTObjID(pVT_Net,VTFunctionReference)<0){
         err=0x01;break;
       }   
     }
     if (VTWSetName==pVT_Net->VT_DST_WS[i]){
       if (getVTObjID(pVT_Net,VTInputReference)<0){
        err=0x01;break;
       }   
     }
   }//for i
   //
 pVT_Net->listNr=olistNr;     
   //
   if (err==0x00){
    str+=getStringHEX(VTFunctionReference,4);
    str+=VTWSetName; str+=getStringHEX(VTInputReference,4);
    //Serial.println(str);
      if (pVT_Net->VTAuxAssignList.indexOf(str)<0){
        pVT_Net->VTAuxAssignList+=str + "\n";
      }
   }//err=0x00
   //
 return err;
};//TVTAuxiliaryPrefAssignment::setVTAuxAssignList



//------------------------------------------------------------------------------
boolean TVTAuxiliaryPrefAssignment::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false,setClear=false;
uint8_t ctrlByte=0xFF, src=((pMsg->ID)>>0) & 0xFF,i=0,j=0;
uint8_t dst=((pMsg->ID)>>8) & 0xFF;
uint16_t funcRef=0xFFFF;
String  msgStr="",str="",wsName="";
 VTError=0x00;
  //
  if ((pStream!=NULL) && (pStream->available()>8)) ctrlByte=pStream->read();
  //
  //TP_EOMA
  if (((pMsg->ID & 0x00FF0000)==TP_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==0x13) && (ctrlByte==VT0PCommFunction)){
   VTInputUnitNumber=pStream->read();
     //
     if (VTInputUnitNumber>0){
       for (i=0;i<VTInputUnitNumber;i++) {
         VTWSetName=pStream->readBytesString(8);
         VTWSetName=getHexCharacterString(VTWSetName,true); //get reverse=true
         //
         setClear=(VTWSetName=="FFFFFFFFFFFFFFFF");
         //
         VTModelIdentCode=pStream->readBytesVal(2);
         msgStr+="Unit_Nr=" + String(i+1) + " / " + String(VTInputUnitNumber) + "\n";
         msgStr+=" WorkingSetName_hex=" + VTWSetName       + "\n";
         msgStr+=" ModelIdentCode_hex=" + getStringHEX(VTModelIdentCode,4)  + "\n";
         //
         VTPrefFuncNumber=pStream->read();
         msgStr+="  PrefFuncNumber=" + String(VTPrefFuncNumber) + "\n";
          //          
          if (VTPrefFuncNumber>0){
            for (j=0;j<VTPrefFuncNumber;j++) {
              VTFunctionReference=pStream->readBytesVal(2);
              VTInputReference=pStream->readBytesVal(2);
              //
              msgStr+="   FuncInpID" + getStringHEX(j,2) + "=" + String(VTFunctionReference) + " ";
              msgStr+="[" + getStringHEX(VTFunctionReference,4) + "]";
              msgStr+=" <- " + String(VTInputReference) + " ";
              msgStr+="[" + getStringHEX(VTInputReference,4) + "]\n";
              //Check Valid PreferedAssignment and AddInsert() to VTAuxAssignList
              VTError=VTError | setVTAuxAssignList(pVT_Net,dst);
                if ((VTError>0) && (funcRef==0xFFFF)) funcRef=VTFunctionReference; //get fault aux function             
                if (setClear) break;
            }//for j
          }//>0
          //
          if (VTPrefFuncNumber==0) setClear=true;
          //
          if (setClear) {
           pVT_Net->VTAuxAssignList.clear();
           break; 
          }
       }//for i
     }//>0
     //
     if (msgStr.length()>0) {
      setSerialPrint(pVT_Net,msgStr);
      setSerialPrint(pVT_Net,delm0);
      valid=true;
     }
     //
  }else {
   src=((pMsg->ID)>>8) & 0xFF; //check VT address
    if (((pMsg->ID & 0x00FF0000)==ECUtoVT_PGN) && (src==pVT_Net->VT_SRC) && (pMsg->DATA[0]==VT0PCommFunction)){
     VTInputUnitNumber=pMsg->DATA[1]; i=0;
     msgStr+="Unit_Nr=" + String(i+1) + " / " + String(VTInputUnitNumber) + "\n";
     Serial.println(msgStr);
       //
       //clear aux assignment          
       if (VTInputUnitNumber==0){
         funcRef=0xFFFF;pVT_Net->VTAuxAssignList.clear();
         valid=true;
       }//VTInputUnitNumber==0

    }//ECUtoVT_PGN
   //    
  }//ctrlByte and stream
  //
 //VT-response
 pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
 pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
  for (i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF; 
  // 
 pMsg->DATA[0]=VT0PCommFunction;
 pMsg->DATA[1]=VTError;
 pMsg->DATA[2]=(funcRef>>0) & 0xFF;
 pMsg->DATA[3]=(funcRef>>8) & 0xFF;
 //
 setSerialPrint(pVT_Net,"VTAuxAssignList");
 setSerialPrint(pVT_Net,pVT_Net->VTAuxAssignList);
   if (VTError==0x00) {
    pVT_Net->VTPoolRefresh=true;
    setSerialPrint(pVT_Net,"pVT_Net->VTPoolRefresh=true");
   }
 setSerialPrint(pVT_Net,delm1);
 //
 return valid; 
};//TVTAuxiliaryPrefAssignment::setMsgToAttr



//==============================================================================
//TVTAuxiliaryInputStatus Implementaion   0x23
boolean TVTAuxiliaryInputStatus::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
 //TODO
 return valid; 
};//TVTAuxiliaryInputStatus::setMsgToAttr



//==============================================================================
//type  //35 $23
boolean TVTAuxiliaryInputMaintenance::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean valid=false;
 //TODO
 return valid; 
};//TVTAuxiliaryInputMaintenance::setMsgToAttr


//==============================================================================
//type   //35 $25
boolean TVTAuxiliaryInputStatusEnable::setMsgToAttr(CANMsg *pMsg,TVT_Net *pVT_Net,LoopbackStream *pStream){
boolean  valid=false,TEST=false;
uint16_t auxFuncID=0xFFFF,auxInpID=0xFFFF;
uint8_t  dst=0xFF,oList=pVT_Net->listNr;
int8_t   WS_auxFunc_listNr=-1,WS_auxInp_listNr=-1;
String   str=pVT_Net->VTAuxAssignList,WS_auxFunc="",WS_auxInp="",auxFunc="",auxInp="";
   //
   while (str.length()>=40){
     WS_auxFunc=str.substring(0,16); 
     auxFunc=str.substring(16,20); 
     auxFuncID=hexCharacterToObjID(auxFunc);
     WS_auxFunc_listNr=getWSlistNrFromName(pVT_Net,WS_auxFunc);
     //
     WS_auxInp=str.substring(20,36); 
     auxInp=str.substring(36,40); 
     auxInpID=hexCharacterToObjID(auxInp);
     WS_auxInp_listNr=getWSlistNrFromName(pVT_Net,WS_auxInp);
       //
       if (TEST){
         Serial.println("WS_auxFunc=" + WS_auxFunc);
         Serial.println("auxFunc="    + auxFunc + "/" + String(auxFuncID));
         Serial.println("WS_auxFunc_listNr=" + String(WS_auxFunc_listNr));
         auxFuncID=hexCharacterToObjID(auxFunc);
         //
         Serial.println("WS_auxInp="  + WS_auxInp);
         Serial.println("auxInp="     + auxInp + "/" + String(auxInpID));
         Serial.println("WS_auxInp_listNr=" + String(WS_auxInp_listNr));
       }
       //
       //FOUND AUX_ASSIGN
       //found auxInp assignment for auxFunc
       if ((auxInpID<0xFFFF) && (auxFuncID<0xFFFF)) {
        VTObjectReference=auxInpID; VTEnabled=true;
        Serial.print("FOUND auxInpID=" + String(auxInpID));
        Serial.println("->WS_auxInp_listNr=" + String(WS_auxInp_listNr));
          //
          if ((WS_auxInp_listNr<2) && (oList!=WS_auxInp_listNr)) {
            pVT_Net->listNr=(pVT_Net->listNr+1) % 2;
          }
        //send VT CAN-message  
        dst=pVT_Net->VT_DST[pVT_Net->listNr];  
        pMsg->MSG_TX=1; pMsg->MSGTYPE=1; pMsg->LEN=8;
        pMsg->ID=(ECU_VT_PRIO<<24) + VTtoECU_PGN + (dst<<8) + pVT_Net->VT_SRC;
          for (int i=0;i<pMsg->LEN;i++) pMsg->DATA[i]=0xFF;
          //
        pMsg->DATA[0]=VT0PCommFunction;
        pMsg->DATA[1]=(VTObjectReference>>0) & 0xFF;  
        pMsg->DATA[2]=(VTObjectReference>>8) & 0xFF;  
        pMsg->DATA[3]=VTEnabled;  
        //
        //
        //VT send CAN-message
        VT_CAN_MsgSend(pVT_Net,pMsg);
        //
        pVT_Net->listNr=oList;
        valid=true;       
       }//AuxInp/AuxFunc found
       //
     str.remove(0,41);  
   }//while
  //
 return valid; 
};//TVTAuxiliaryInputStatusEnable::setMsgToAttr


//==============================================================================
//==============================================================================
