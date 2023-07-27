//==============================================================================
//Main program
//==============================================================================
/*
  Please add MCP_CAN_LIB to your library first........
*/
//==============================================================================
#define STACK_MODE
//==============================================================================
//------------------------------------------------------------------------------
// by JG: Stack Definition hier bringt nichts. 
// besser: Definition in platformio.ini  
// -DARDUINO_LOOP_STACK_SIZE=32768 
#if defined(ESP32) && defined(STACK_MODE)
/*
 #if !(USING_DEFAULT_ARDUINO_LOOP_STACK_SIZE)
  uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 32768; //32768; //16384; //max 32768, default=8192
  //uint16_t CONFIG_ARDUINO_LOOP_STACK_SIZE=32768;
 #endif
 */
 // ADC_MODE(ADC_VCC);
#endif //STACK_MODE

//==============================================================================
//create start pool data
#include "UnitVTObjCreatePool.h"

#ifndef BUILD_MINIMAL
#include "UnitVTOled.h"    // OLED support define OLED_MODE
#include "UnitVTServer.h"  // WebServer    define WEB_MODE
#endif

#include <ESP32Time.h>

ESP32Time rtc(0);  // offset in seconds GMT+1,3600

//==============================================================================
void init_can();
void test_can_transmit();
void test_can_receive();
//
void CANMsgPGN(CANMsg msg);
//void getInputSerialString();

//==============================================================================
//SCREEN KEYBOARD
//==============================================================================
//Set screen keyboard input
void SetVTKeyboardInput(boolean setUniCode){
String   fntName="arial3-0",str=input_text;     
int16_t  dxx=0,dyy=0,dxx_pos=0;
uint16_t len=str.length(),xMax=key_max*key_size+1 ,un=0;
boolean  TEST=false;  
   if (setUniCode){
    pVT_Net->fntType=getVTInpFontType(pVT_Net);
    pVT_Net->uniCode=false;
      //       
      if (len>=2){
       un+=char(str[0]); un+=char(str[1])<<8;
       pVT_Net->uniCode=(un==0xFEFF); un=0;
         if (pVT_Net->uniCode) str.remove(0,2);        
         if ((len>=2)&& (pVT_Net->uniCode)) {
           un+=char(str[0]); un+=char(str[1])<<8;
          //TEST
          //setSerialPrint(pVT_Net,getStringHEX(un,4));
          pVT_Net->VTKeyboardOffs=(un & 0xFFF0) + 0x20;
          drawVTKeyboard();
         }//len>=2
      }//len>=2
      //
      if (!pVT_Net->uniCode){
        if (pVT_Net->fntType>0) {
         str.clear(); 
           for (uint16_t k=0;k<len;k++){
            un=getUniCodeFontIndex(input_text,k,pVT_Net); 
            str+=char((un>>0) & 0xFF); str+=char((un>>8) & 0xFF);
           }//for k
        } else str=setASCIItoUniCodeStr(str);
      }//not pVT_Net->uniCode
   }//setUniCode
   //
 //TEST TEXT
 //getStringHEXInfo(str,true);
 //getStringHEXInfo(input_text,true);
 //
 // Clear input text area, set input viewport
 Set_setViewport(pVT_Net,0, 0, xMax, KEYBOARD_Y - 1);
 Set_fillRect(pVT_Net,0, 0, xMax, KEYBOARD_Y - 1, cl_black);
 Set_setTextDatum(pVT_Net,0);
 Set_loadFont(pVT_Net,fntName);
 len=str.length()/2;
   //   
   if (key_pos>(key_start+key_max))key_start++;
   if (key_pos<key_start) key_start--;
   //
 dxx=-key_size*key_start;dxx_pos=key_size*(key_pos-key_start);  
   if (TEST){
    setSerialPrint(pVT_Net,"key_pos=" + String(key_pos));
    setSerialPrint(pVT_Net,"dxx=" + String(dxx));
    setSerialPrint(pVT_Net,"dxx_pos=" + String(dxx_pos));
   }
 //set input text
 pVT_Net->fntType=0; pVT_Net->fntNr=3;
 pVT_Net->x=dxx;pVT_Net->y=dyy+1;
 setUniCodeTextDirect(str,pVT_Net);
 //set cursor position
 Set_fillRect(pVT_Net,dxx_pos,2,1,KEYBOARD_Y-4,cl_white);
 //
 input_text=str;
 //TEST TEXT
 getStringHEXInfo(input_text,true);
 Set_resetViewport(pVT_Net);
}//SetVTKeyboardInput


//==============================================================================
void reset_VTKeyboard(){
int i=0,r=0,c=0;
  #ifdef M5CORE2_MODE
   for (r = 0; r < ROWS; r++){
     for (c = 0; c < COLS; c++) {
      if(touch_button_list[i] != NULL)  {
       touch_button_list[i]->delHandlers();
       delete(touch_button_list[i]); touch_button_list[i]=NULL;
      }
     i++; 
     }//for c  
   }//for r  
   //
   for(i= 0; i < 3; i++) {
    if(touch_button_uniCode[i]!= NULL)  {
     touch_button_uniCode[i]->delHandlers();
     delete(touch_button_uniCode[i]); touch_button_uniCode[i]=NULL;
    }
   }//for i
  #endif
 key_pos=0; key_start=0;
 pVT_Net->VTKeyboardOffs=0; //reset unicode offset
}//resetKeyboard


//==============================================================================
void fontVTKeyboard(String fntStr,uint8_t i, uint8_t fntType,wchar_t ch, wchar_t tch, String key, boolean TEST){
 if (TEST){
   setSerialPrint(pVT_Net,String(i) + "\t" + String(fntType) + fntStr,true); setSerialPrint(pVT_Net,String(wchar_t(ch)),true); 
   setSerialPrint(pVT_Net,"->",true);setSerialPrint(pVT_Net,String(uint16_t(ch)),true) ;setSerialPrint(pVT_Net,"/",true); 
   setSerialPrint(pVT_Net,"0x" + getStringHEX(uint16_t(ch),4),true);
   setSerialPrint(pVT_Net,"->",true);setSerialPrint(pVT_Net,String(uint16_t(tch)),true);setSerialPrint(pVT_Net,"/",true); 
   setSerialPrint(pVT_Net,"0x" + getStringHEX(uint16_t(tch),4),4);
   setSerialPrint(pVT_Net,"\t" + key,true);
   setSerialPrint(pVT_Net,"");
 }
}//fontVTKeyboard

//==============================================================================
void drawVTKeyboard(){
String fntName="arial3-0",key="",vStr="",kStr="";
//std::wstring wKey="";
int i=0, x=0,y=0,r=0,c=0, objIdx=-1;
boolean  setKey=false,vType=false, smFont=false, strType=false, oCode=false,TEST=false;
uint16_t un=0,nn=0,len=0,k=0,dxx=12, dyy=12,offs=0;
uint16_t plane[3]={0x0000,0x0000,0x0000};
uint8_t  fntType=0,key_page=0;
wchar_t  wch,tch;
 Set_setTextDatum(pVT_Net,0);
 fntType=getVTInpFontType(pVT_Net);
 Set_loadFont(pVT_Net,fntName);
  //
  if (pVT_Net->VTInpAttr<0xFFFF){
    objIdx=getVTObjID(pVT_Net,pVT_Net->VTInpAttr);
     if (objIdx>=0){ 
       getStreamStrInfo(pVT_Net);nn=3;
       vType=pVT_Net->streamStr.readBytesVal(1,nn); nn++;
       len=pVT_Net->streamStr.readBytesVal(1,nn);   nn++;
         if (len>0){
           vStr=pVT_Net->streamStr.readBytesString(len,nn);nn+=len;
            //TEST
             if (TEST){
              setSerialPrint(pVT_Net,String(vType));
              setSerialPrint(pVT_Net,String(len));
              setSerialPrint(pVT_Net,vStr);
             }
         }//len>0
     }//objIdx>=0 
  }//<0xFFFF
  //
  if (HasInArray(pVT_Net->TFT_InputSelectObjType,inpNumObjSet)) {
    key_mode=KEY_MODE_NUMBER;
    shift_mode=false;
  }
  //
  if(key_mode == KEY_MODE_NUMBER) key_page += 2;
  if (shift_mode) key_page += 1;
  //
  //
 setSerialPrint(pVT_Net,delm1);
  // 
  for (r = 0; r < ROWS; r++){
    for (c = 0; c < COLS; c++) {
     setKey=false;
     x = (KEYBOARD_X + (c * KEY_W));  y = (KEYBOARD_Y + (r * KEY_H));
     wch=keymap[key_page][r][c]; tch=wch; kStr="";
     key.clear(); key+=char(tch & 0xFF); key+=char((tch>>8) & 0xFF);
      //shift,blank,LF,CR,TAB,LR,ESC,ESC
      if (r==ROWS-1) {
        fntName="arial2-0"; Set_loadFont(pVT_Net,fntName); kStr=keyStr[c];
      }
      //
      #ifdef M5CORE2_MODE
        if(touch_button_list[i] != NULL)  {
          touch_button_list[i]->delHandlers();
          delete(touch_button_list[i]); touch_button_list[i]=NULL;
        }
      #endif   
      //
      //check input type numeric
      if (HasInArray(pVT_Net->TFT_InputSelectObjType,inpNumObjSet)) {
       setKey=((setKey) || (r<2) || ((r==ROWS-1) && (c>5)));
       //setSerialPrint(pVT_Net,"setKey=" + String(setKey));
      }else{
        //Letter_Mode
        setKey=true;
         //check object InputAttribute
         if (vStr.length()>0) {
           if (vType) {
              if (vStr.indexOf(key)>=0) setKey=false;
           } else {
              if (vStr.indexOf(key)<0) setKey=false;
           }
         }
      }
      //
      if (setKey) {
        Set_fillRect(pVT_Net,x + 1, y + 1, KEY_W - 2, KEY_H - 2, TFT_BLACK);
        Set_drawRoundRect(pVT_Net,x, y, KEY_W, KEY_H, 10, TFT_DARKGREY);
        //check string input Fonttype,gInpStringType=8
        strType=(pVT_Net->TFT_InputSelectObjType==gInpStringType);
        //TEST
        //fntType=5; strType=true;TEST=true;
        smFont=((fntType>0) && (kStr.length()==0) && (strType));
         //
         if (smFont) pVT_Net->VTKeyboardOffs=0;
         //
         if (key_mode==KEY_MODE_LETTER) {
             if ((pVT_Net->VTKeyboardOffs>=0x0020) && (r<4)) {
              offs=pVT_Net->VTKeyboardOffs-0x0020;
              tch=uint16_t(offs+i); key.clear();
              key+=char(tch & 0xFF); key+=char((tch>>8) & 0xFF);
             }//offset>0
             //
         }//Letter
         //
         if (!Set_smFont(pVT_Net)) smFont=false;
         //
         if (smFont) {
              //start A-Z  0x41-0x60
              if ((uint16_t(wch)>=0x0041) && (uint16_t(wch)<=0x0060)) tch=uint16_t(wch)+0x006F; 
              //start a-z  0x61-0x80
              if ((uint16_t(wch)>=0x0061) && (uint16_t(wch)<=0x0080)) tch=uint16_t(wch)+0x006F; 
            key.clear(); key+=char(tch & 0xFF); key+=char((tch>>8) & 0xFF);
            //
            //TEST info
            fontVTKeyboard(".smFont=",i,fntType,wch,tch,key,TEST);
            //
            oCode=pVT_Net->uniCode; pVT_Net->uniCode=false; pVT_Net->fntType=fntType;k=0;
            un=getUniCodeFontIndex(key,k,pVT_Net);
            pVT_Net->uniCode=oCode;
            Set_setCursor(pVT_Net,x + dxx, y + dyy);
            Set_drawGlyph(pVT_Net,un);
            key.clear(); key+=char(un & 0xFF); key+=char((un>>8) & 0xFF);
         }else{
            key.clear(); key+=char(tch & 0xFF); key+=char((tch>>8) & 0xFF);
             if (kStr.length()==0) {
              //TEST info
              fontVTKeyboard(".Font=",i,fntType,wch,tch,key,TEST);
              //
              pVT_Net->fntType=fntType; pVT_Net->fntNr=3;
              pVT_Net->x=x+dxx; pVT_Net->y=y+dyy;
              setUniCodeTextDirect(key,pVT_Net);    
             }else {
                if (kStr.length()>2) dxx=4;else dxx=8;
              Set_drawString(pVT_Net,kStr, x + dxx, y + dyy);
             }
         }//smFont
         //
         if (setKey){
          plane[0]=fntType;
            if ((i>=0) && (plane[1]==0x0000)) plane[1]=uint16_t(tch);
            if (i<32) plane[2]=uint16_t(tch);
         }
         //
         //Set touch button
         #ifdef M5CORE2_MODE
           touch_button_list[i] = new Button(x, y + 5, KEY_W, KEY_H, false, key.c_str());
         #endif 
        //
      } else{
        //clear Key
        Set_drawRoundRect(pVT_Net,x, y, KEY_W, KEY_H, 10, TFT_DARKGREY);
        Set_fillRect(pVT_Net,x + 1, y + 1, KEY_W - 2, KEY_H - 2, TFT_BLACK);
        //Set_fillRect(pVT_Net,x, y, KEY_W, KEY_H, TFT_BLACK);
      }//setKey
      // 
     i++; 
    }//for c
  }//for r
  //
 setSerialPrint(pVT_Net,"fntType=",true); setSerialPrint(pVT_Net,"\t",true); setSerialPrint(pVT_Net,getStringHEX(plane[0],2),true);
 setSerialPrint(pVT_Net,"\t",true);
 Serial.print("PLANE=");   Serial.print("\t"); Serial.print(getStringHEX(plane[1],4));Serial.print("\t");
 setSerialPrint(pVT_Net,getStringHEX(plane[2],4));
 dxx=1;dyy=6;
 fntName="arial1-0"; Set_loadFont(pVT_Net,fntName);dxx=4;dyy=8;
 uint8_t fkt=1;

  //get unicode plane viewer
  for (c = 5; c < COLS; c++) {
    if (c==5) fkt=2;else fkt=1;   
   x = (KEYBOARD_X + (c * KEY_W) + (fkt-1)*KEY_W/2); y = 0;
   Set_fillRect(pVT_Net,x + 1, y + 1, KEY_W/fkt - 2, KEYBOARD_Y - 2, TFT_BLACK);
   Set_drawRoundRect(pVT_Net,x, y, KEY_W/fkt, KEYBOARD_Y, 10, TFT_DARKGREY);
   //
   key=getStringHEX(plane[c-5],4/fkt); 
   Set_drawString(pVT_Net,key, x + dxx, y + dyy);
     //Set touch button
     #ifdef M5CORE2_MODE
        if(touch_button_uniCode[c-5]!= NULL)  {
          touch_button_uniCode[c-5]->delHandlers();
          delete(touch_button_uniCode[c-5]); touch_button_uniCode[c-5]=NULL;
        }
        //
      key.clear(); key+="plane" + String(c-5); key+=(plane[c-5]>>0) & 0xFF; key+=(plane[c-5]>>8) & 0xFF;
      touch_button_uniCode[c-5] = new Button(x+2,y + 2, KEY_W, KEY_H, false, key.c_str());
     #endif 
  }//for c
 setSerialPrint(pVT_Net,delm1);
}//drawVTKeyboard



//==============================================================================
//get input vs ScreenKeyboard
void setup_VTKeyboard(String str="") {
CANMsg   msg;
String   fntName="arial2-0";
boolean  TEST=false;
uint16_t un=0,len=0,yOffs=228;
  //M5.background.delHandlers();
  setSerialPrint(pVT_Net,"0.last_fntName=",true);setSerialPrint(pVT_Net,pVT_Net->fntName);
  reset_VTKeyboard();
    //
    if (pVT_Net->VTPageSelect==2) {
     pVT_Net->fntNamePool[pVT_Net->listNr]=pVT_Net->fntName;
     //
     resetKeyButtonInputList(pVT_Net);
     Set_setTextDatum(pVT_Net,0);
     pVT_Net->ImgMode=0; 
       if (Set_fontLoaded) Set_unloadFont(pVT_Net); 
     //
     Set_fillScreen(pVT_Net,TFT_BLACK);
     Set_setTextColorBg(pVT_Net,TFT_GREEN, TFT_BLACK);
     Set_loadFont(pVT_Net,fntName);
     //
     // Button A
     Set_drawString(pVT_Net,"delete", 16, yOffs);
     // Button B
     Set_drawString(pVT_Net,"done",  138, yOffs);
     // Button C
     Set_drawString(pVT_Net,"cancel",240, yOffs);
     //
     key_mode = KEY_MODE_LETTER;  shift_mode = false;  input_text="";
     //TEST
        if (TEST){
         setSerialPrint(pVT_Net,String(pVT_Net->TFT_InputSelectObjID));
         setSerialPrint(pVT_Net,String(pVT_Net->TFT_InputSelectObjType));
         setSerialPrint(pVT_Net,delm0);
        }
        //
        if (str.length()>0) {
         //selected input ssid_w, set password
         input_text=str; 
        }else{
         if (pVT_Net->TFT_InputSelectObjID<0xFFFF) {
          input_text=String(pVT_Net->TFT_InputSelectObjID); 
           //
           if (HasInArray(pVT_Net->TFT_InputSelectObjType,inpAllObjSet)){
            pVT_Net->getVTValue=true; pVT_Net->VTValueStr="";pVT_Net->VTValue=0; 
            pVT_Net->VTObjType=pVT_Net->TFT_InputSelectObjType;
            pVT_Net->VTObjID=pVT_Net->TFT_InputSelectObjID;
            pVT_Net->VTInpAttr=0xFFFF;
            //TEST
            //setSerialPrint(pVT_Net,String(pVT_Net->VTObjID));
            //setSerialPrint(pVT_Net,String(pVT_Net->VTObjType));
              //gInpStringType=8
              if (pVT_Net->TFT_InputSelectObjType==gInpStringType) {
                TVT_VTInputStringValue mObj;
                  if (mObj.setMsgToAttr(&msg,pVT_Net)){
                    if (TEST){
                     setSerialPrint(pVT_Net,String(pVT_Net->VTValue));
                     setSerialPrint(pVT_Net,pVT_Net->VTValueStr);
                     setSerialPrint(pVT_Net,String(pVT_Net->VTInpAttr));
                    }
                   //trimRight blanks
                   pVT_Net->VTValueStr=getStringRightTrim(pVT_Net->VTValueStr);
                   input_text=pVT_Net->VTValueStr;
                  }
              }else{
                key_mode = KEY_MODE_NUMBER;
                TVT_VTChangeNumericValue mObj;
                  if (mObj.setMsgToAttr(&msg,pVT_Net)){
                    if (TEST){
                     setSerialPrint(pVT_Net,String(pVT_Net->VTValue));
                     setSerialPrint(pVT_Net,pVT_Net->VTValueStr);
                    }
                    //
                   pVT_Net->VTValueStr.trim();
                   input_text=pVT_Net->VTValueStr;
                  }
              }
              //
            pVT_Net->getVTValue=false;  
           } //all inputs
           //
         }//<0xFFFF
        }//str.length()
        //
     drawVTKeyboard();
     SetVTKeyboardInput(true);
     //
     pVT_Net->VTPageSelect=2;
     pVT_Net->VTPoolRefresh=false;    
     setSerialPrint(pVT_Net,"1.last_fntName=",true);setSerialPrint(pVT_Net,pVT_Net->fntNamePool[pVT_Net->listNr]);
    }else{
     reset_FontImageMode(pVT_Net);
    }
    //
}//setup_VTKeyBoard


//==============================================================================
#ifdef MSTACK_MODE
//==============================================================================
//get status
void getStatusInfo() {
int i=0, j=-1;
String str="",ss="";
String oStr[2]={"OFF","ON"};
 str+="#SerSpeed="  + String(115200); //String(ser_speed);
 str+="#Bluetooth=" + String(0);      //String(bluetooth);
 str+="#CAN_SPEED=" + String(250);    //String(can_speed);
     if (pVT_Net->CAN_active) str+="#CAN:OPEN"; else str+="#CAN:CLOSE";
 str+="#";
   //
   #ifdef OLED_MODE
    getStatusOLEDDisplay_Info(str, 2);
   #endif //OLED_MODE  
   //
   #ifdef MSTACK_MODE
    if (pVT_Net->ImgMode==0) {
     VT_CAN_ResetStatusInfo(pVT_Net,200);
     j=str.indexOf("#");
       if (j==0) str.remove(0,1);
       // 
       for (i=0;i<4;i++){
        j=str.indexOf("#");
         if (j>=0) {
          ss=str.substring(0,j); str.remove(0,j+1);
           if (ss.length()>0) pVT_Net->tft.drawString(ss,10,10+i*30);
         } 
       }//for i
       //
     ss="CR=" + String(oStr[pVT_Net->CR_active]);
     ss+="  Trace=" + String(oStr[pVT_Net->Trace_active]);
     ss+="  FLT=" + String(pVT_Net->VTTraceFilter);
     pVT_Net->tft.drawString(ss,10,i*30+20);
     //
     ss=pVT_Net->ssid_w;
     ss+=" [" + pVT_Net->ip_w + "]";
     pVT_Net->tft.drawString(ss,10,i*30+40);
     ss=pVT_Net->ssid_a;
     ss+=" [" + String(pVT_Net->ip_a) + "]";
     pVT_Net->tft.drawString(ss,10,i*30+60);
     delay(100);
    }//pVT_Net->ImgMode==0
    //
   #endif //MSTACK_MODE
   //
}//getStatusInfo


//==============================================================================
void reset_VTTrace(){
String fntName_trace="arial2-0";  
 Set_resetViewport(pVT_Net);
 Set_setTextColor(pVT_Net,cl_white);
 Set_setTextDatum(pVT_Net,0);
 Set_setCursor(pVT_Net,0, 0);
   if (pVT_Net->VTStampActive)fntName_trace="arial1-0";
 Set_loadFont(pVT_Net,fntName_trace);
}//reset_VTTrace


//==============================================================================
void SetVTTraceMode(){
String str[3]={"TIME","TRACE","INFO"},fntName="arial3-0";
uint16_t cc=TFT_DARKGREEN,w=320/3,h=40,x=0,y=200;
boolean setMarker=false;
 Set_resetViewport(pVT_Net);
 //Set_setTextColorBg(pVT_Net,TFT_YELLOW, TFT_BLACK);
 Set_setTextColor(pVT_Net,cl_yellow);
 Set_loadFont(pVT_Net,fntName);
 //
 setMarker=((pVT_Net->VTStampActive) || (pVT_Net->VTTraceActive) || (pVT_Net->VTTraceFilter==0));
  //
  if (setMarker){
    pVT_Net->SSID_List.clear();
    pVT_Net->SSID_ListIdx=-1;
    pVT_Net->SSID_ListIdxMax=-1;
  } else {
    str[0]="WLAN";
  }
  // 
  for (int i=0;i<3;i++) {
   setMarker=false; cc=TFT_DARKGREEN;  x=i*w;
   pVT_Net->tft.fillRect(x,y,w,h,cc);
   pVT_Net->tft.drawRect(x,y,w,h,TFT_RED);
     //
     if ((i==0) && (pVT_Net->VTStampActive))    setMarker=true;
     if ((i==1) && (pVT_Net->VTTraceActive))    setMarker=true;
     if ((i==2) && (pVT_Net->VTTraceFilter==0)) setMarker=true;
     //
     if (setMarker){
       cc=TFT_RED;
       pVT_Net->tft.drawRect(x+1,y+1,w-2,h-2,cc); 
       pVT_Net->tft.drawRect(x+2,y+2,w-4,h-4,cc); 
     }
    Set_drawString(pVT_Net,str[i],x+15,y+10);
  }//for i
  //
 reset_VTTrace();
}//SetVTTraceMode 


//==============================================================================
void setup_VTTrace() {
  if (pVT_Net->VTPageSelect==3){
     pVT_Net->fntNamePool[pVT_Net->listNr]=pVT_Net->fntName;
     pVT_Net->VTTimeStamp=millis(); 
     //     
     resetKeyButtonInputList(pVT_Net);
     pVT_Net->ImgMode=0; 
       if (Set_fontLoaded) Set_unloadFont(pVT_Net); 
       //
     Set_fillScreen(pVT_Net,TFT_BLUE);
     //
     SetVTTraceMode();
     getStatusInfo();
  }else {
    reset_FontImageMode(pVT_Net);
  }//pVT_Net->VTPageSelect==3
}//setup_VTTrace 
//  
//==============================================================================
#endif //MSTACK_MODE
//==============================================================================



//==============================================================================
//Button EVENT
//==============================================================================
void buttonEventDirect(String str,boolean selectMode=true){
    if (selectMode) str=pVT_Net->VTSelectMode + "=" + str; 
   pVT_Net->inputSerialComplete=true;
   pVT_Net->inputSerialString.clear();
   pVT_Net->inputSerialString+=str;
   getInputSerialString();
}//buttonEventDirect


//==============================================================================
#ifdef M5CORE2_MODE
//==============================================================================
void eventDisplay(Event& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d) --> (%3d, %3d)   ",
                e.typeName(), e.finger, e.objName(), e.from.x, e.from.y,e.to.x, e.to.y);
  Serial.printf("( dir %d deg, dist %d, %d ms )\n", e.direction(),e.distance(), e.duration);
}//eventDisplay


//==============================================================================
void buttonEvent(Event& e){
String str="",ss="",key="",fntName_trace="arial2-0";
uint16_t dCount=getVTDrawListSize(pVT_Net),un=0,len=0,k=0;
boolean TEST=false;
int i=-1;
Button& b = *e.button;
 str=String(b.getName());
  //
  if (setGesture) {
   setGesture=false;
   return; 
  }
  //
  if (TEST){
   Serial.println("pVT_Net->VTPageSelect=" + String(pVT_Net->VTPageSelect));
   Serial.println(e); Serial.println(str);
   eventDisplay(e);
  }
  //background events
  if(str.indexOf("background")==0) {
      //send pointing event message
      if ((pVT_Net->VTPageSelect==0) && (dCount>0)) {
        str="P=" + String(e.from.x) + "," + String(e.from.y);
        buttonEventDirect(str,false);
      }
    return;
  }
  // 
  if (pVT_Net->VTPressedFor){
   pVT_Net->VTPressedFor=false; 
   return;
  }
  //  
  if (pVT_Net->VTPageSelect==0) {
    //
    if(e.button == &M5.BtnA){
      buttonEventDirect("B");
      return;
    }  
    if(e.button == &M5.BtnB){
       if ((pVT_Net->TFT_KeySelectObjID<0xFFFF) || (pVT_Net->TFT_ButtonSelectObjID<0xFFFF)){
         buttonEventDirect("PR");
       }else{
          if (pVT_Net->TFT_InputSelectObjID<0xFFFF) {
           getVTObjID(pVT_Net,pVT_Net->TFT_InputSelectObjID,false,true);
           /*
           //TEST
           setSerialPrint(pVT_Net,String(pVT_Net->TFT_InputSelectObjID)); 
           setSerialPrint(pVT_Net,String(pVT_Net->VTObjType));
           setSerialPrint(pVT_Net,String(pVT_Net->VTObjID));
           setSerialPrint(pVT_Net,delm0);
           */
            // 
            //check gAuxPtnType==33
            if ((pVT_Net->listNr==2) && (pVT_Net->VTObjType==gAuxPtnType)) {
             setSerialPrint(pVT_Net,String(pVT_Net->VTObjType));
             setSerialPrint(pVT_Net,String(pVT_Net->VTObjID));
             setSerialPrint(pVT_Net,delm0);
             //TODO
            }else {
              buttonEventDirect("V");
            }
          }
       } 
      return;
    }  
    if(e.button == &M5.BtnC){
      buttonEventDirect("N");
      return;
    }  
    //
    if(str.indexOf("KEY")==0) {
      setSerialPrint(pVT_Net,str);
      str.replace("KEY",""); i=str.toInt();
      i=i+6*pVT_Net->TFT_KeyPage;
      //Serial.println(i);
      //Serial.println(pVT_Net->TFT_KeyPage);
      pVT_Net->VTSelectMode="C";
      //set serial manual touch SoftKey
      buttonEventDirect(String(i));
      buttonEventDirect("PR");
      return;
    }
    //
    if(str.indexOf("BUTTON")==0) {
      setSerialPrint(pVT_Net,str);
      str.replace("BUTTON",""); i=str.toInt();
      //Serial.println(i);
      pVT_Net->VTSelectMode="B";
      //set serial manual touch button
      buttonEventDirect(String(i));
      buttonEventDirect("PR");
      return;
    }
    //
    if(str.indexOf("INPUT")==0) {
      setSerialPrint(pVT_Net,str);
      str.replace("INPUT",""); i=str.toInt();
        //Serial.println(i);
      pVT_Net->VTSelectMode="A";
      //set serial manual touch Input,select input
      buttonEventDirect(String(i));
      
        //check gAuxPtnType==33     
        if ((pVT_Net->listNr==(listMax-1)) && (pVT_Net->VTObjType==gAuxPtnType)) {
          setSerialPrint(pVT_Net,String(pVT_Net->VTObjType));
          setSerialPrint(pVT_Net,String(pVT_Net->VTObjID));
          setSerialPrint(pVT_Net,delm0);
          //TODO
          //get all aux inputs list
          //Serial.println("get all aux inputs list"); 
          //getVTAuxObjectList(pVT_Net);
          //
          //edit aux assignment
          return;
        }
      //edit input with keyboard or direct
      buttonEventDirect("V"); //toggle value
      return;
    }
    //
   return;
  }//keyboard deactive,pVT_Net->VTPageSelect==0
  //
  if (pVT_Net->VTPageSelect==2) {
   // Delete or Backspace
   if(e.button == &M5.BtnA) {
    //uniCode delete/delete backward
    len=input_text.length()/2;
      if (len>0){
         if (key_pos>=len) key_pos--;
       input_text.remove(2*key_pos,2); 
       SetVTKeyboardInput(false);
      }
    return;
   };
   // Done keyboard with uniCode
   if(e.button == &M5.BtnB){
     //
     if (pVT_Net->SSID_List.length()>0){
       #ifdef WEB_MODE 
         ss=pVT_Net->ssid_w;
         ssid_w= new char[ss.length()+1];
         ss.toCharArray(ssid_w,ss.length()+1);  
         setSerialPrint(pVT_Net,"ssid_w=",true);  setSerialPrint(pVT_Net,String(ssid_w));
         //
         ss=setUniCodeStrToASCII(input_text);
         password_w=new char[ss.length()+1];
         ss.toCharArray(password_w,ss.length()+1);  
         setSerialPrint(pVT_Net,"password_w=",true); setSerialPrint(pVT_Net,String(password_w));
         getStringHEXInfo(ss,true);
         //    
         //Try connect to ssid_w, password_w
         WiFi_ConnectDirect();
         //
         pVT_Net->VTPageSelect=3;
         setup_VTTrace();
       #endif //WEB_MODE
       //
      return;
     }
     //
     if (dCount>0) {
       if ((pVT_Net->TFT_InputSelectObjID<0xFFFF) && (HasInArray(pVT_Net->TFT_InputSelectObjType,editObjSetK))){
          //check input string type=8
          if (pVT_Net->TFT_InputSelectObjType==gInpStringType){
            pVT_Net->fntType=getVTInpFontType(pVT_Net);
            str.clear();
            //getStringHEXInfo(input_text,true);
            //setSerialPrint(pVT_Net,"pVT_Net->uniCode=" + String(pVT_Net->uniCode));  
            //setSerialPrint(pVT_Net,"pVT_Net->fntType=" + String(pVT_Net->fntType));  
              //
              if (pVT_Net->uniCode){
                getStringHEXInfo(input_text,true);           
                str+=char(0xFF); str+=char(0xFE); str+=input_text;   
                getStringHEXInfo(str,true);
              } else{ 
               pVT_Net->uniCode=false;
               str=input_text;
                 // 
                 if (pVT_Net->fntType>0) {
                   pVT_Net->uniCode=true;
                   str.clear(); len=input_text.length()/2;
                   //TEST
                   //getStringHEXInfo(input_text,true);
                   setSerialPrint(pVT_Net,"fntType=" + String(pVT_Net->fntType));
                     //
                     for (k=0;k<len;k++) {
                      un=setUniCodeFontIndex(input_text,k, pVT_Net);
                      str+=char(un & 0xFF);
                     }//for k
                     //
                   //TEST
                   //getStringHEXInfo(str,true);
                 } else {
                   //TEST
                   getStringHEXInfo(input_text,true);
                   str=setUniCodeStrToASCII(input_text);
                   //TEST
                   getStringHEXInfo(str,true);
                 }
               pVT_Net->uniCode=false;  
              }//uniCode
          } else str=setUniCodeStrToASCII(input_text);
          //
        //set input value to selected object
        //TEST
        getStringHEXInfo(str,true);
        str="V" + str; 
        buttonEventDirect(str,true);
       }
      
      setSerialPrint(pVT_Net,"done=" + input_text);
      pVT_Net->VTPageSelect=0;
      setup_VTKeyboard();
      pVT_Net->VTPoolRefresh=(pVT_Net->VTPageSelect==0);
     } else {
      str=setUniCodeStrToASCII(input_text);
      buttonEventDirect(str,false);
     }
     //
    //pVT_Net->uniCode=false;
    return;
   }//done
   //
   // Cancel keyboard
   if(e.button == &M5.BtnC){
     if (pVT_Net->SSID_List.length()>0){
      pVT_Net->VTPageSelect=3;
      setup_VTTrace();
       #ifdef WEB_MODE
        pVT_Net->ssid_w=String(ssid_w);
        pVT_Net->ssid_a=String(ssid_a);
        WiFi_setNetworksList();
       #endif 
     
     }else{
      setSerialPrint(pVT_Net,"cancel=" + input_text);
      ss+="E";
      buttonEventDirect(ss,true);
     }
    return;
   }//cancel
   //
   //get buttons Letters and Ctrl Buttons, 001=shift, 002=mode,003=left,004=right 
   un=getArray8Info(pVT_Net,(uint8_t*) b.getName(),2,false);
   setSerialPrint(pVT_Net,getStringHEX(un,4)); 
    //shift
    if (un==0x0001) {
     shift_mode = !shift_mode; 
     drawVTKeyboard();
     return;
    }
    //mode
    if (un==0x0002) {
        switch(key_mode) {
         case KEY_MODE_LETTER:
          key_mode = KEY_MODE_NUMBER;
          break;
         default:
         case KEY_MODE_NUMBER:
          key_mode = KEY_MODE_LETTER;
          break;
       }
     shift_mode = false;
     drawVTKeyboard();
     return;
    }
    //left=0003,right=0004
    if ((un==0x0003) || (un==0x0004)) {
     len=input_text.length()/2;
       if (un==0x0003) key_pos--;else key_pos++;
       if (key_pos<0) key_pos=0;
       if (key_pos>len) key_pos=len;
     SetVTKeyboardInput(false);
     return;
    }
    //
   //get key input
   //setSerialPrint(pVT_Net,getStringHEX(un,4)); 
   key+=char((un >>0) & 0xFF); key+=char((un >>8) & 0xFF);
   len=input_text.length()/2;
     //insert/add input key  
     if (key_pos<len){
       //insert mode
       ss=input_text.substring(0,2*key_pos); input_text.remove(0,2*key_pos);
       ss+=key; ss+=input_text;
       input_text=ss;
     }else { 
       //append mode
       input_text+=key; key_pos++;
     }
   SetVTKeyboardInput(false);
   return;
  }//pVT_Net->VTPageSelect==2
  //
  //
  //TimeView mode
  if (pVT_Net->VTPageSelect==1) {
    //
    //set Time minute -1
    if(e.button == &M5.BtnA){
      setSerialPrint(pVT_Net,"TimeMinute+1");
      VTDateTimeMinute(pVT_Net,false,1);
      return;
    }  
    if(e.button == &M5.BtnB){
      setSerialPrint(pVT_Net,"TimeView OFF");
      pVT_Net->VTPageSelect=0;
      setup_VTDateTime(pVT_Net);
      pVT_Net->VTPoolRefresh=(pVT_Net->VTPageSelect==0);
      return;
    }  
    //set Time minute +1
    if(e.button == &M5.BtnC){
      setSerialPrint(pVT_Net,"TimeMinute-1");
      VTDateTimeMinute(pVT_Net,false,-1);
      return;
    }  
  }//pVT_Net->VTPageSelect==1
  //
  //TraceView mode
  if (pVT_Net->VTPageSelect==3) {
    //
    //set TimeStamp
    if(e.button == &M5.BtnA){
      boolean setMarker=((pVT_Net->VTStampActive) || (pVT_Net->VTTraceActive) || (pVT_Net->VTTraceFilter==0));
      //
      if (setMarker){
       setSerialPrint(pVT_Net,"TIME");
       pVT_Net->VTStampActive=!pVT_Net->VTStampActive; SetVTTraceMode();
         if (pVT_Net->VTStampActive) {
          pVT_Net->VTTimeStamp=millis();
          fntName_trace="arial1-0";
         }
       Set_loadFont(pVT_Net,fntName_trace);
      }else {
       setSerialPrint(pVT_Net,"WLAN");
        #ifdef WEB_MODE
          if (pVT_Net->SSID_List.length()>0){
           //set SSID
           str=WiFi_setNetworksIdx();
             if (str.length()>0){ 
              pVT_Net->VTPageSelect=2;
              //set Password for ssid_w  
              str=String(password_w);
              setup_VTKeyboard(str);
             }
          } else WiFi_scanNetworks(); 
        #endif
      }
     return;
    }//BtnA
    //  
    //Trace
    if(e.button == &M5.BtnB){
      if (pVT_Net->SSID_List.length()>0){
       #ifdef WEB_MODE
        WiFi_getNetworksIdx(); 
       #endif
       // 
      }else{
       setSerialPrint(pVT_Net,"TRACE");
       pVT_Net->VTTraceActive=!pVT_Net->VTTraceActive;
        if (pVT_Net->VTTraceLine==0) VT_CAN_ResetStatusInfo(pVT_Net,200);
       SetVTTraceMode();
      
      }
      return;
    } //BtnB
    // 
    //set Info
    if(e.button == &M5.BtnC){
      
      setSerialPrint(pVT_Net,"INFO");
        if (pVT_Net->VTTraceFilter==0x00) pVT_Net->VTTraceFilter=0xFF; else pVT_Net->VTTraceFilter=0x00;
      pVT_Net->VTTraceActive=false;pVT_Net->VTStampActive=false;
      Set_loadFont(pVT_Net,fntName_trace);
      SetVTTraceMode();
      pVT_Net->VTTraceLine=0;
      getStatusInfo();
      WiFi_resetNetworks(pVT_Net,""); 
       //      
      return;
    }  
  }//pVT_Net->VTPageSelect==1
 //   
}//buttonEvent

//==============================================================================
#endif //M5CORE2_MODE
//==============================================================================


//==============================================================================
//TIME AND DATE
//==============================================================================

//==============================================================================
void loop_VTDateTime() {
String fntName="arial4-0";
uint32_t tDiff=millis()-pVT_Net->TargetTime;
String str="";
int xpos=0,ypos=0;
 if ((pVT_Net->VTPageSelect==1) && (tDiff>=1000)) {
   // Set next update for 1 second later
   pVT_Net->TargetTime = millis();
   VTFlushTime(pVT_Net);
  //
  //fntName="arial3-0";
  Set_loadFont(pVT_Net,fntName);
  //
  // Update digital time
  xpos  = 160; ypos  = 10;  // Top left corner ot clock text, about half way down
  // Draw actual Date
  str=String(pVT_Net->timeStrbuff);
  String ss=str.substring(0,str.indexOf(" "));ss.trim(); 
  Set_drawCentreString(pVT_Net,ss, xpos, ypos);   
  //  
  fntName="arial4-0";
  Set_loadFont(pVT_Net,fntName);
  // Draw actual Time
  str.remove(0,str.indexOf(" ")); str.trim(); ypos=45;  
  //
  str+=" ";
  Set_drawCentreString(pVT_Net,str, xpos, ypos);
  //
  //UTC-Time
  fntName="arial3-0";
  Set_loadFont(pVT_Net,fntName);
  // 
  xpos=0;ypos=100;
  //get offset Time UTC
  VTDateTime(pVT_Net,false,pVT_Net->hourOffset);
  str="UTC:" + String(pVT_Net->timeStrbuff);
  str+=" ";
  Set_drawString(pVT_Net,str, xpos, ypos);// Draw hour offset
    //
    if (pVT_Net->VTPageSelect==1){
     ypos=125;
     str="UTC-HourOffset: " + String(pVT_Net->hourOffset);
     Set_drawString(pVT_Net,str, xpos, ypos);// Draw hour offset
     //compiled data
     xpos=0; ypos=160;
     //
     str="vers:" + String(__VERSION__) + " RTC[0x51]=" + String(pVT_Net->RTC_active);
     Set_drawString(pVT_Net,str, xpos, ypos);// Draw compiled version
     //
     str="date:" + String(__DATE__); ypos+=25;
     Set_drawString(pVT_Net,str, xpos, ypos);// Draw compiled date
     //
     str="time:" + String(__TIME__);ypos+=25;
     Set_drawString(pVT_Net,str, xpos, ypos);// Draw compiled time
    }
    //
  //pVT_Net->VTPageSelect=2;  
  pVT_Net->VTPoolRefresh=false;    
 }
}//loop_VTDateTime


//==============================================================================
//#define DHT_Mode
//#include <SHT3x.h>
//SHT3x Sensor(0x5C);
#ifdef DHT_Mode
  #include "DHT12.h"
  DHT12 dht12;
//DHT12 dht12(&I2Ctwo);
//DHT12 dht12(&I2Ctwo, 21,22);
//DHT12 dht12(&I2Ctwo, 0x5C);
//DHT12 dht12(&I2Ctwo, 21,22,0x5C);
#endif


//==============================================================================
void loop_TempHumSensor() {
#ifdef DHT_Mode
uint32_t tDiff=millis()-pVT_Net->TargetTime;
 //
 if (pVT_Net->VTPageSelect==0) {
   //
   if (tDiff>=2000) {
     
     // Set next update for 1 second later
     pVT_Net->TargetTime = millis();
     /*
     Sensor.UpdateData();
     setSerialPrint(pVT_Net,"Temperature: ",true); setSerialPrint(pVT_Net,String(Sensor.GetTemperature()),true);
     Serial.write("\xC2\xB0"); //The Degree symbol
     setSerialPrint(pVT_Net,"C");
     //
     setSerialPrint(pVT_Net,"Humidity: ",true); setSerialPrint(pVT_Net,String(Sensor.GetRelHumidity()),true);
     setSerialPrint(pVT_Net,"%");
     */
     //
     float tmp = dht12.readTemperature();
     float hum = dht12.readHumidity();
     Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%\r\n", tmp, hum);    
     setSerialPrint(pVT_Net,delm0);
   }
 } 
#endif //DHT_Mode    
}//loop_TempHumSensor 


//==============================================================================
//==============================================================================
void ResetSelectMode() {
String str="";
 pVT_Net->VTSelectMode="C";
 str=VT_Net.VTSelectMode + "=0";
 setSerialPrint(pVT_Net,"VTSelectMode:" + str);
 setSerialPrint(pVT_Net,delm0);
 pVT_Net->inputSerialComplete=true; pVT_Net->inputSerialString+=str.c_str(); 
 getInputSerialString();
}    


//==============================================================================
void VTPoolDataRead(TVT_Net *pVT_Net,uint8_t dataMode,LoopbackStream *pStream) {
uint16_t objID=0xFFFF;
uint8_t  objType=0xFF,ctrlByte=0xFF;
uint32_t hStatus=0xFFFFFFFF,sCount=pStream->available(),pSize=0, ppSize=0,i=0;
uint16_t lCount=getVTObjectListSize(pVT_Net);
String str="";
boolean setBreak=false, TEST=false;
 logMemory();
 //
 setSerialPrint(pVT_Net,"0.VTPoolDataRead=" + String(pStream->available()) + "/" + String(lCount));
 getHeapStatus(pVT_Net,5); pVT_Net->objNr=0;pVT_Net->VTUpLoad=false;
   //
   if (pStream->available()) {
       //get 0x11 pooldata or loadversion data
       if (dataMode==0) {
          pVT_Net->VTUpLoad=(lCount>0);
          ctrlByte=pStream->read();
       }//dataMode
       //
       while (pStream->available()){
        //pVT_Net->ETP_DATA++;
        //pVT_Net->TP_DATA++;
        ppSize=pStream->available();
          //
          if (ppSize>3) {
            objID=pStream->read() + (pStream->read()<<8); objType=pStream->read();
              //TEST
              //setSerialPrint(pVT_Net,String(objType));
              //      
              if ((objID<0xFFFF) && (HasInArray(objType,_objTypeSet))) {
               //UnitVTObjCreatePool.h
               getPoolObjectCreate(pVT_Net,objType,objID,pStream);
              } else setBreak=true;
          } else setBreak=true;
          //
          //default=4000;       
          if (pVT_Net->objNr>=pVT_Net->VTObjMax[pVT_Net->listNr]) setBreak=true;
          //
          TEST=true;
          if ((setBreak) || (TEST)) {
           pSize=ppSize-pStream->available();
           str=String(pVT_Net->objNr) + "\t" + String(objID) + "\t" + String(objType) + "\t";
           str+=String(ppSize) + "\t" + String(pSize) + "\t" + String(pStream->available());
           setSerialPrint(pVT_Net,str);
             //
             if (setBreak) break;
          }   
          //
        pVT_Net->objNr++;
       } //while
  }
  // 
  //send ESC close Keyboard
  if (pVT_Net->VTPageSelect==2) buttonEventDirect("E");
 pVT_Net->VTPageSelect=0; 
 //VTPoolDataRefresh(pVT_Net,false,true);        
 // 
 setSerialPrint(pVT_Net,"Objects=" + String(getVTObjectListSize(pVT_Net)));
 setSerialPrint(pVT_Net,delm0);
 logMemory();
 //
 getHeapStatus(pVT_Net,15);
 pStream->clear();
 //getVTObjectListInfo(pVT_Net,-6); //list all
 getHeapStatus(pVT_Net,16);
 //
 //getStreamHEX(&pVT_Net->stream_Pool[pVT_Net->listNr],pVT_Net);
 //UnitVTCreateObjPool
 VTPoolDataRefresh(pVT_Net,true);
 //pVT_Net->VTPoolRefresh=true;
 //
 pVT_Net->VTUpLoad=false;
}


//==============================================================================
void VTPoolData(TVT_Net *pVT_Net, uint8_t dataMode) {
 getHeapStatus(pVT_Net,1); 
 resetSelectInput(pVT_Net);
   //   
   if (pVT_Net->stream_ETP[pVT_Net->listNr].available()) VTPoolDataRead(pVT_Net,dataMode,&pVT_Net->stream_ETP[pVT_Net->listNr]); 
   if (pVT_Net->stream_TP[pVT_Net->listNr].available())  VTPoolDataRead(pVT_Net,dataMode,&pVT_Net->stream_TP[pVT_Net->listNr]); 
 //getHeapStatus(pVT_Net,2);
}//VTPoolData


//==============================================================================
boolean clearPoolData(TVT_Net *pVT_Net){
boolean valid=false;
uint8_t oListNr=0;
 //UnitVTCreateObjPool
   //send ESC close Keyboard
   if (pVT_Net->VTPageSelect==2) buttonEventDirect("E");
 pVT_Net->VTPageSelect=0; 
 valid=getPoolObjectClear(pVT_Net);
   //
   if (valid){
     oListNr=pVT_Net->listNr;
     pVT_Net->listNr=(pVT_Net->listNr+1) % 2;
       //
       if (getVTObjectListSize(pVT_Net)==0) pVT_Net->listNr=oListNr;
   }//valid
   // 
 return valid;
}//clearPoolData


//==============================================================================
//UnitVTCommandTechData.h
void test_can_transmit(){
CANMsg msg;
 if (pVT_Net->CAN_active){
   if (VT_CAN_Transmit(pVT_Net,&msg)) {
    //
   }
   //
 }//CAN_active
}//test_can_transmit


//==============================================================================
void test_can_receive(){
CANMsg msg;
 if (pVT_Net->CAN_active){
   if (VT_CAN_MsgReceive(pVT_Net,&msg)) {
     CANMsgPGN(msg);
   }
 }//CAN_active  
}//test_can_receive



//==============================================================================
void SetAudioCommand(){
uint32_t aTime=millis(),dTime=aTime-pVT_Net->VTActiveTime;
  if ((pVT_Net->VTRepetition>0) && (pVT_Net->VTFrequency>0) && (pVT_Net->VTOnTimeDuration>0)){
    if (pVT_Net->VTAudioActive==1) {
     setSerialPrint(pVT_Net,String(pVT_Net->VTRepetition),true);
     setSerialPrint(pVT_Net,".SetON=",true); setSerialPrint(pVT_Net,String(dTime));
      #ifdef M5STACK_MODE
        M5.Speaker.tone(pVT_Net->VTFrequency,pVT_Net->VTOnTimeDuration);
      #endif
      //
      #ifdef M5CORE2_MODE
        //M5.Spk.tone(0,0);
        M5.Spk.DingDong();
      #endif 
     pVT_Net->VTActiveTime=millis(); dTime=0;
     pVT_Net->VTAudioActive=2; 
    }
    //
    if ((pVT_Net->VTAudioActive==2)  && (dTime>=pVT_Net->VTOnTimeDuration)) {
     pVT_Net->VTAudioActive=0;
     setSerialPrint(pVT_Net,String(pVT_Net->VTRepetition),true);
     setSerialPrint(pVT_Net,".SetOFF=",true); setSerialPrint(pVT_Net,String(dTime));
      #ifdef M5STACK_MODE
        M5.Speaker.tone(0,0);
      #endif
      //
      #ifdef M5CORE2_MODE
        //M5.Spk.tone(0,0);
        M5.Spk.DingDong();
      #endif 
     pVT_Net->VTActiveTime=millis();dTime=0;
     pVT_Net->VTRepetition--;
    }
    //
    if ((pVT_Net->VTAudioActive==0)  && (dTime>=pVT_Net->VTOffTimeDuration)) {
     //Start Audio
     pVT_Net->VTAudioActive=1;
     setSerialPrint(pVT_Net,String(pVT_Net->VTRepetition),true);
     setSerialPrint(pVT_Net,".SetON=",true);  setSerialPrint(pVT_Net,String(dTime));
     pVT_Net->VTActiveTime=millis();dTime=0;
    }
  } else {
      if (pVT_Net->VTAudioActive>0) {    
        #ifdef M5STACK_MODE
          M5.Speaker.tone(0,0);
        #endif
        // 
        #ifdef M5CORE2_MODE
          //M5.Spk.tone(0,0);
          M5.Spk.DingDong();
        #endif
      }
    pVT_Net->VTAudioActive=0;    
    pVT_Net->VTActiveTime=millis();
  }
}//SetAudioCommand 



//==============================================================================
void CANMsgPGN(CANMsg msg){
boolean sendMSG=false, respMSG=false,setEOMA=false,setWS=true;; 
uint8_t  ctrlByte=0xFF,src=0x0A, dst=pVT_Net->VT_SRC; //0x26;
uint32_t reqPGN;
String ss="",str="";
int i=0;
TVT_ViewRect viewRect;
msg.TimeStamp=millis();
 //
 if (pVT_Net->CAN_active){
  //setSerialPrint(pVT_Net,getMsgFrameStr(&msg));
  //
  src=(msg.ID>>0) & 0xFF;
    if (src>=0xFE) src=0xFF;
  dst=(msg.ID>>8) & 0xFF;
  respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
  //
  //get TECU messages to TECU Monitor
  if (pVT_Net->listNr==(listMax-1)){
    getTECUMonitor(&msg,pVT_Net);
  } 
  //
  //Request
  if ((respMSG) && ((msg.ID & 0x00FF0000)==Request_PGN)) {
     //RX1 18EEFE26  8  64 00 60 1F 00 1D 00 20
     //WorkingSetName=20001D001F600064
     //TX1 18EAFFFE  3  00 EE 00
     //Request_PGN (00)=00EE00
     if (!sendMSG) {
       TVTAddressClaim msgObj;
       sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
     } //AddressClaim
     //
     if (!sendMSG) {
       TVTLanguage msgObj;
       sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
     } //Language
     //
     //Diagnostic 
     if (!sendMSG) {
       TVT_DiagConformance msgObj;
       sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
     } //Diagnostic
     //
     //TimeDate 
     //246.800 RX1 18EAFFFE  3  E6 FE 00
     //Request_PGN (00)=00FEE6
     if (!sendMSG) {
       TVT_TimeDate msgObj;
       sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
     } //TimeDate
     //
  }//Request
  //
  
  //AddrClm_PGN global 
  if (!sendMSG) {
    if (((msg.ID & 0x00FF0000)==AddrClm_PGN) && (dst=0xFF) && (src<0xFE)){
       TVTAddressClaimResponse msgObj;
       msgObj.setMsgToAttr(&msg,pVT_Net);
       //sendMSG=false;
    }   
  }
  //
  //-----
  //
  //GetHardware...//PGN 0x00E700
  if (((msg.ID & 0x00FF0000)==ECUtoVT_PGN) && (dst=pVT_Net->VT_SRC)){
    //
    //ECU Status message set ECU-address
    if ((!sendMSG) && (msg.DATA[0]==0xFF)){   
      if (src<0xFE) {
        //setSerialPrint(pVT_Net,String(src,HEX));
        setECUAddress(pVT_Net,msg);
        return;
      }
    }  
    //
   setECUListNumber(pVT_Net,msg);
    //
    //MACRO-COMMANDS
    if (!sendMSG) {
      sendMSG=MacroCANMsgPGN(pVT_Net,&msg,false);
    }//macro commands
    //
    //.....
    // 
    //--------
    //TECHDATA
    //TX1 1CE7260A  8  C7 FF FF FF FF FF FF FF
    //GetHardware (00)
    if ((!sendMSG) && (msg.DATA[0]==0xC7)){   
     TVTGetHardware msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
    }   
    //TX1 1CE7260A  8  C2 FF FF FF FF FF FF FF
    //GetNumberOfSoftKeys (00)
    if ((!sendMSG) && (msg.DATA[0]==0xC2)){   
     TVTGetNumberOfSoftKeys msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
    }   
    //RX1 1CE7260A  8  C3 FF FF FF FF FF FF FF
    //GetTextFontData (00)
    if ((!sendMSG) && (msg.DATA[0]==0xC3)){   
     TVTGetTextFontData msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
    }  
    //TX1 1CE7260A  8  C0 FF 00 00 00 00 FF FF
    //GetMemory (00)  MemoryByte=0
    if ((!sendMSG) && (msg.DATA[0]==0xC0)){   
     TVTGetMemory msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
     uint32_t mem=msgObj.VTMemory;
       if (mem>0) {
         //setSerialPrint(pVT_Net,"MEMORY=" + String(mem));
       }
    }  
    //
    //TX1 1CE7260A  8  C1 FF FF FF FF FF FF FF
    //TVTGetSupportedWideChars (00)
    if ((!sendMSG) && (msg.DATA[0]==0xC1)){   
     TVTGetSupportedWideChars msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
    }  
    //
    //TX1 1CE7260A  8  C4 FF FF FF FF FF FF FF
    //GetWindowMaskData (00)
    if ((!sendMSG) && (msg.DATA[0]==0xC4)){   
     TVTGetWindowMaskData msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
    }  
    //
    //TX1 1CE7260A  8  C5 FF FF FF FF FF FF FF
    //GetSupportedObjects (00)
    if ((!sendMSG) && (msg.DATA[0]==0xC5)){   
     TVTGetSupportedObjects msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
    }  
    //
    //TX1 1CE7260A  8  BB FF FF FF FF FF FF FF
    //IdentifyVT (00)
    if ((!sendMSG) && (msg.DATA[0]==0xBB)){   
     TVTIdentifyVT msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
       if ((msgObj.VTError==0) && (pVT_Net->VT_ChangeAttr)) VTPoolDataRefresh(pVT_Net,false);
    } 
    //
    //TX1 1CE7260A  8  C6 00 01 FF FF FF FF FF
    //ScreenCapture (00)
    //ItemRequested=0 [ScreenImage]
    //Path=1 [Storage/Media]
    if ((!sendMSG) && (msg.DATA[0]==0xC6)){   
     TVTScreenCapture msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
       //Clear screen
       if (msgObj.VTError==0xC0) VTPoolDataRefresh(pVT_Net,false,true);        
     getHeapStatus(pVT_Net,23);
    } 
    //
    //TX1 1CE7260A  8  60 00 01 FF FF FF FF FF
    //ProprietaryCommand (00)
    if ((!sendMSG) && ((msg.DATA[0]>=0x60) && (msg.DATA[0]<=0x7F))){   
     TVTProprietaryCommand msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
    } 
    // 
    //
    //TX1 1CE7260A  8  DF FF FF FF FF FF FF FF
    //GetVersion (00) //Label with TP
    if ((!sendMSG) && (msg.DATA[0]==0xDF)){   
     TVTGetVersion msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
    }  
    //
    //TX1 1CE7260A  8  D3 FF FF FF FF FF FF FF
    //ExtendedGetVersion (00)
    if ((!sendMSG) && (msg.DATA[0]==0xD3)){   
     TVTExtendedGetVersion msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
    }  
    // 
    //TX1 1CE7260A  8  D0 56 45 52 53 49 4F 4E
    //StoreVersion (00)  VersionLabel=VERSION
    //FileName(VTData/*.iop)=
    //200C80001F600064_56455253494F4E
    if ((!sendMSG) && (msg.DATA[0]==0xD0)){   
     //UnitVTCreateObjPool.cpp
     VTPoolDataStore(pVT_Net);
     //save to file
     TVTStoreVersion msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_ETP[pVT_Net->listNr]);
     pVT_Net->stream_ETP[pVT_Net->listNr].clear();  
     pVT_Net->VT_InfoMode=false;
    } 
    // 
    //TX1 1CE7260A  8  D1 56 45 52 53 49 4F 32
    //LoadVersion (00)  VersionLabel=VERSIO2
    //200C80001F600064_56455253494F32
    if ((!sendMSG) && (msg.DATA[0]==0xD1)){   
     TVTLoadVersion msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_ETP[pVT_Net->listNr]);
     //setSerialPrint(pVT_Net,"LOADVERSION=" + String(pVT_Net->stream_ETP[pVT_Net->listNr].available()));
       //
       if ((sendMSG) && (msgObj.VTError==0)) {
         clearPoolData(pVT_Net);
         VTPoolData(pVT_Net,1);
       }
     pVT_Net->stream_ETP[pVT_Net->listNr].clear();   
     pVT_Net->VT_InfoMode=false;
    } 
    //
    //TX1 1CE7260A  8  D2 56 45 52 53 49 4F 32
    //DeleteVersion (00)  VersionLabel=VERSIO2
    //200C80001F600064_56455253494F32
    if ((!sendMSG) && (msg.DATA[0]==0xD2)){   
     TVTDeleteVersion msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_ETP[pVT_Net->listNr]);
     pVT_Net->VT_InfoMode=false;
    } 
    //
    //===== 
    //TX1 1CE7260A  8  B2 FF FF FF FF FF FF FF
    //DeleteObjectPool (00)
    if ((!sendMSG) && (msg.DATA[0]==0xB2)){   
     TVTDeleteObjectPool msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
        if (getVTObjectListSize(pVT_Net)>0) {
          pVT_Net->VTPoolRefresh=true; pVT_Net->VTPoolClear=true;
           if (clearPoolData(pVT_Net)) msg.DATA[1]=0x00;
        }
    }  
    //TX3 1CE7260A  8  12 FF FF FF FF FF FF FF
    //EndOfObjectPool_1
    if ((!sendMSG) && (msg.DATA[0]==ETP_EOP)){   
     VTPoolData(pVT_Net,0);
     pVT_Net->VT_DST[pVT_Net->listNr]=src;
     TVTEndOfObjectPool msgObj;
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
    }  
    //
    //AuxiliaryPrefAssignment=0x22 VTInputUnitNumber=0
    //TX1 1CE7260A  2  22 00
    //AuxiliaryPrefAssignment (00)
    //Unit_Nr=0
    if ((!sendMSG) && (msg.DATA[0]==AUX3_ASSIGN_PREF)){   
     TVTAuxiliaryPrefAssignment msgObj; 
     sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,NULL);
    }
    //  
  }//0x00E700
  //  
  //Transport protocol
  if (!sendMSG) {
   TVTTransportProtocol msgObj;
   sendMSG=msgObj.setTransportProtcol(&msg,pVT_Net);
  }
  //
  if (sendMSG) {
    VT_CAN_MsgSend(pVT_Net,&msg); sendMSG=false;
  }
  //
  //GetTP_Protocole...//PGN TP_PGN
  //MACRO-COMMANDS for TP_EOMA
  if ((!sendMSG) && ((msg.ID & 0x00FF0000)==TP_PGN) && (msg.DATA[0]==0x13) && (pVT_Net->stream_TP[pVT_Net->listNr].available())){   
      ctrlByte=pVT_Net->stream_TP[pVT_Net->listNr].peek();
        //ChangeChildPosition
        if (ctrlByte==0xB4) {
           TVTChangeChildPosition msgObj;
           sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
            if ((msgObj.VTError==0) && (pVT_Net->VT_ChangeAttr)) VTPoolDataRefresh(pVT_Net,false);
        }
        //ChangeStringValue
        if (ctrlByte==0xB3) {
           TVTChangeStringValue msgObj;
           sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
            if ((msgObj.VTError==0) && (pVT_Net->VT_ChangeAttr)) VTPoolDataRefresh(pVT_Net,false);
        }
        //SelectActiveWorkingSet
        if (ctrlByte==0x90) {
           TVTSelectActiveWorkingSet msgObj;
           sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
             if ((msgObj.VTError==0) && (pVT_Net->VT_ChangeAttr)) VTPoolDataRefresh(pVT_Net,false);
        }
        //
        //ExtendedStoreVersion
        if (ctrlByte==0xD4) {
          TVTExtendedStoreVersion msgObj;
          sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
          pVT_Net->stream_TP[pVT_Net->listNr].clear();   
          pVT_Net->VT_InfoMode=false;
        }
        //
        //ExtendedLoadVersion
        if (ctrlByte==0xD5) {
           TVTExtendedLoadVersion msgObj;
           sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
            //
            if ((sendMSG) && (msgObj.VTError==0)) {
             clearPoolData(pVT_Net);
             VTPoolData(pVT_Net,1);
            }
          pVT_Net->stream_TP[pVT_Net->listNr].clear();   
          pVT_Net->VT_InfoMode=false;
        }
        //
        //
        //ExtendedDeleteVersion
        if (ctrlByte==0xD6) {
          TVTExtendedDeleteVersion msgObj;
          sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
          pVT_Net->stream_TP[pVT_Net->listNr].clear();   
          pVT_Net->VT_InfoMode=false;
        }
        //
        //Auxiliary messages
        //AuxiliaryPrefAssignment=0x22
        if (ctrlByte==AUX3_ASSIGN_PREF) {
          TVTAuxiliaryPrefAssignment msgObj; //msgObj.VT0PCommFunction=ctrlByte;
          sendMSG=msgObj.setMsgToAttr(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
          pVT_Net->stream_TP[pVT_Net->listNr].clear();   
          pVT_Net->VT_InfoMode=false;
          //Serial.println("AuxiliaryPrefAssignment");
        }
        if (sendMSG) VT_CAN_MsgSend(pVT_Net,&msg);
  }//TP_EOMA

 }//can_active 
}//CANMsgPGN


//==============================================================================
//SETUP CAN
//==============================================================================
void init_can(){
 VT_CAN_Init(pVT_Net);
 //
 //UnitVTCreateObjPool.cpp
 getPoolObjectReset(pVT_Net,true); 
}//init_can



//==============================================================================
#ifdef ESP32
  //Brownout handler
  //#include "soc/soc.h"
  //#include "soc/rtc_cntl_reg.h"  
  //
#endif


//==============================================================================
//==============================================================================
i2cDevice_t i2cParentptr;
//
void addI2cDevice(String name, uint8_t addr) {
  i2cDevice_t *lastptr = &i2cParentptr;

  while (lastptr->nextPtr != nullptr) {
    lastptr = lastptr->nextPtr;
  }
  //
  i2cDevice_t *ptr = (i2cDevice_t *)calloc(1, sizeof(i2cDevice_t));
  ptr->Name = name;
  ptr->addr = addr;
  ptr->nextPtr = nullptr;
  lastptr->nextPtr = ptr;
}//addI2cDevice


//==============================================================================
int scani2caddr() {
int cnt=0;
 setSerialPrint(pVT_Net,delm1); 
 setSerialPrint(pVT_Net,"Scan I2C addresses[HEX]:"); 
  //  
  for (int i = 0; i < 120; i++) {
    Wire1.beginTransmission(i);
    if (Wire1.endTransmission() == 0) {
      Serial.printf("%02X   |FIND", i);
      setSerialPrint(pVT_Net,".");
      cnt++;
       //check RTC address=0x51
       if (i==0x51) pVT_Net->RTC_active=true;   
    }
  }
 setSerialPrint(pVT_Net,delm0); 
 return cnt;
}//scani2caddr



//==============================================================================
int checkI2cAddr() {
uint8_t cnt = 0;
String str="";
  i2cDevice_t *lastptr = &i2cParentptr;
  do {
    lastptr = lastptr->nextPtr;
    Wire1.beginTransmission(lastptr->addr);
    if (Wire1.endTransmission() == ESP_OK) {
      Serial.printf("Addr:0x%02X - Name:%s\t", lastptr->addr,lastptr->Name.c_str());
      str=lastptr->Name;
      str=str.substring(0,str.indexOf(" "));
      str="\tI2C " + str + " Found";
      setSerialPrint(pVT_Net,str);   
      cnt++;
    } else {
      //Serial.printf("Addr:0x%02X - Name:%s\t", lastptr->addr,lastptr->Name.c_str());
      //str="I2C " + lastptr->Name + " Find failed";
    }
    //
  } while (lastptr->nextPtr != nullptr);
  return cnt;
}//checkI2cAddr


//==============================================================================
void addI2cDeviceAll() {
  addI2cDevice("AXP192 Power", 0x34);
  addI2cDevice("FT6336U CST Touch", 0x38);
  addI2cDevice("MPU6886 IMU", 0x68);
  addI2cDevice("BM8563 RTC", 0x51);
  addI2cDevice("AT608A Fingerprint", 0x35);
  addI2cDevice("AK8963 Magnetometer", 0x0C);
  addI2cDevice("SH200Q 6-axis sensor",0x6C);
  addI2cDevice("IP5306 Power managment",0x75);
  addI2cDevice("BMM150 Magnetometer",0x10);
  addI2cDevice("SHT30 Temp/Humidity",0x5C);
}//addI2cDeviceAll


//==============================================================================
void SetDateAndTime(String str,boolean setCompile) {
boolean setDateTime=false;
uint8_t k=2;
int i=-1,j=-1;
String ss="",cc="";
//
uint8_t tt[2][3]={{0,0,0},{0,0,0}};
uint16_t yy=0;
      i=str.indexOf("TIME=");
       if (i==0) {
        pVT_Net->VTPageSelect=0;
        setup_VTDateTime(pVT_Net);VTFlushTime(pVT_Net);
        setSerialPrint(pVT_Net,"TIME=" + String(pVT_Net->timeStrbuff));
        pVT_Net->VTPoolRefresh=true;
        return;
       }
       //      
      i=str.indexOf("HOUR=");
       if (i==0) {
        str.replace("HOUR=","");
          if (str.length()>0) pVT_Net->hourOffset=str.toInt();
        setSerialPrint(pVT_Net,"hourOffset=" + String(pVT_Net->hourOffset));
        return;
       }
       //
      i=str.indexOf("."); j=str.lastIndexOf(".");
        if ((i>0) && (j>0) &&(i<j)) {k=0; cc="."; setDateTime=true;}
      //
      i=str.indexOf(":"); j=str.lastIndexOf(":");
        if ((i>0) && (j>0) &&(i<j)) {k=1; cc=":"; setDateTime=true;}
      j=0;  
        //
        if (setDateTime) {
         //get/set time or date "TIME="
          if (pVT_Net->RTC_active) {
            M5.Rtc.GetTime(&pVT_Net->RTCtime);  // Gets the time in the real-time clock.
            M5.Rtc.GetDate(&pVT_Net->RTCDate);  // Gets the date in the real-time clock.
          }else {
            VTFlushTime(pVT_Net,false);
          }
         //
         tt[0][0]=pVT_Net->RTCDate.Date;  tt[0][1]=pVT_Net->RTCDate.Month; yy=pVT_Net->RTCDate.Year; tt[0][2]=yy % 2000; 
         tt[1][0]=pVT_Net->RTCtime.Hours; tt[1][1]=pVT_Net->RTCtime.Minutes; tt[1][2]=pVT_Net->RTCtime.Seconds;
          //
          if (setCompile) {
           //TODO  
          }
          //
          while ((str.length()>0) && (j<3) && (k<2)) {
           i=str.indexOf(cc);
            if (i>0) {
             ss=str.substring(0,i); str.remove(0,i+1);
             //setSerialPrint(pVT_Net,ss);
             tt[k][j]=ss.toInt();
            } else {
             tt[k][j]=str.toInt();
             //setSerialPrint(pVT_Net,str);
               //get year
               if ((k==0) && (j==2)) yy=str.toInt();
             str="";
            }
           j++; 
          }//while
        }//setDateTime 
        //
        if (setDateTime) {
          //setSerialPrint("SetDateTime:");
          setSerialPrint(pVT_Net,"SetDate:" + String(tt[0][0]) + "." + String(tt[0][1]) + "." + String(yy));
          setSerialPrint(pVT_Net,"SetTime:" + String(tt[1][0]) + ":" + String(tt[1][1]) + ":" + String(tt[2][2]));
          // Set the time.
          pVT_Net->RTCtime.Hours   = tt[1][0];    
          pVT_Net->RTCtime.Minutes = tt[1][1];
          pVT_Net->RTCtime.Seconds = tt[1][2];
          pVT_Net->RTCDate.Date    = tt[0][0];    
          pVT_Net->RTCDate.Month   = tt[0][1];
          pVT_Net->RTCDate.Year    = yy; //tt[0][2];
            
           if (!M5.Rtc.SetTime(&pVT_Net->RTCtime)) setSerialPrint(pVT_Net,"wrong time set!");
           if (!M5.Rtc.SetDate(&pVT_Net->RTCDate)) setSerialPrint(pVT_Net,"wrong date set!");
        }//setDateTime
        //
} //SetDateAndTime


//==============================================================================
//==============================================================================
void serialEventInput() {
  if (pVT_Net->notSerialReceive) return;  
  //
  while (Serial.available()) {
   //get the new byte:
   uint8_t bb = Serial.read();
     if  (!pVT_Net->inputSerialComplete) {
       if ((bb == 0x0D) || (bb == 0x0A)) {
        if (pVT_Net->inputSerialString.length()>0) pVT_Net->inputSerialComplete=true;
       }else {
         pVT_Net->inputSerialString+=char(bb);
       }
     }
     //
     if ((bb == 0x0D) || (bb == 0x0A)) {
       if (pVT_Net->inputSerialString.length()>0) pVT_Net->inputSerialComplete=true;
     }
  } //while
}//serialEventInput



//--------------------------------------------------------
//set serial manual Date or Time
void getInputSerialString() {
CANMsg msg;
boolean  setMSG=false,refresh=false,setFocus=false;
String   str="",ss="",aStr="ADDRESS_INFO:",lStr="";
uint8_t  showAttr=1,i=0,bMode=0,nn=0,olist=pVT_Net->listNr;
uint16_t objID=0xFFFF,inpID=0xFFFF,objCount=0;
uint32_t oValue=0;
int16_t  objIdx=-1,k=-1,j=1;
  if (pVT_Net->inputSerialComplete) {
   pVT_Net->inputSerialComplete=false; str+=pVT_Net->inputSerialString; pVT_Net->inputSerialString.clear(); 
   ss=str.substring(0,3);ss.toUpperCase();
      //VTChangeValue
      if (ss.indexOf("A=V")==0) {
       str.remove(0,3); str=ss + str;
      }else {
       str.trim();lStr=str; str.toUpperCase();
      }//A=V
      //
      if (str.indexOf("SC=R")==0){
       //get filestream to serial
       //setSerialPrint(pVT_Net,"InputSerialString:\t",true);
      } else {
        setSerialPrint(pVT_Net,"InputSerialString:\t",true);
      }
      //Change source address VT or destination address of ECU0/ECU1
      if ((str.indexOf("VT=")==0) || (str.indexOf("ECU0=")==0) || (str.indexOf("ECU1=")==0)){
        setSerialPrint(pVT_Net,str); k=-1;
        str.replace("VT=",""); str.replace("ECU",""); 
           //get all addresses
           if ((str.length()==0) || (str=="0=") || (str=="1=")) {
            //setSerialPrint(pVT_Net,"ADDRESS_INFO:");
           }else{
             //Check ECU0/ECU1
             if ((str.indexOf("0=")==0) || (str.indexOf("1=")==0)) {
              ss=str.substring(0,1); i=ss.toInt(); str.remove(0,2);
                if ((str.length()>0) && ((i==0) || (i==1))){
                 str.trim();
                 k=getIntFromHEX(str);
                   if (k>=0) pVT_Net->VT_DST[i]=k; 
                }
             } else {
               str.trim();k=getIntFromHEX(str);
                if (k>=0) pVT_Net->VT_SRC=k; 
             }
           }
           if (k>=0) aStr="ADDRESS_CHANGE:";
           //
       setSerialPrint(pVT_Net,delm0);    
       setSerialPrint(pVT_Net,aStr);
       getAddressInfo(pVT_Net);
       return;
      }
      //      
      //set ScreenShot, download ScreenShot file
      if (str.indexOf("SC=")==0) {
       boolean oSerialOut=pVT_Net->serialOut; 
           if (str.indexOf("SC=R")==0) pVT_Net->serialOut=false;
           //
        setSerialPrint(pVT_Net,str);
        str.replace("SC=",""); 
           //Clear screen
           if (str.length()==0) {
             str="Clear screen";
                if (pVT_Net->VTPageSelect==2) buttonEventDirect("E");
             pVT_Net->VTPageSelect=0; 
             VTPoolDataRefresh(pVT_Net,false,true);        
               //send ESC close Keyboard
             setSerialPrint(pVT_Net,str);
             return;
           } else {
              if ((str.indexOf("R")==0) || (str.indexOf("S")==0)) {
               bMode=true; //readMode
               pVT_Net->SD_DownLoad=(str.indexOf("R")==0);
               str.remove(0,1);  
              }
            j=str.toInt();
           }
           //
           if (str.length()<=2) {
              //auto image_number=pVT_Net->VTPageSelect;
              if (str=="A") j=pVT_Net->VTPageSelect;
            str=getStringDEC(j,4);
           }
           //
        pVT_Net->x=0;pVT_Net->y=0; pVT_Net->w=pVT_Net->tft.width(); pVT_Net->h=pVT_Net->tft.height();
        String path="/img" + str + ".bmp"; 
          if (bMode) {
           str="ScreenShot read from=" + path + "->NOT OK";
            if (readImageScreen(SD, path.c_str(),pVT_Net)) str.replace("NOT","");
          }else{
           str="ScreenShot write to=" + path + "->NOT OK";
            if (writeImageScreen(SD, path.c_str(),pVT_Net)) str.replace("NOT","");
          }
          //
        pVT_Net->SD_DownLoad=false;
        setSerialPrint(pVT_Net,str); 
        pVT_Net->serialOut=oSerialOut;
        return;
      }//SC=
      //set/get CAN message
      if (str.indexOf("SETMSG=")==0) {
        setSerialPrint(pVT_Net,str);
        setMSG=getMsgFromCmd(pVT_Net, &msg,str);
        setSerialPrint(pVT_Net,getMsgFrameStr(&msg));
          //read/send Messages simu
          if ((setMSG) && (pVT_Net->CAN_active)) {
            if (msg.MSG_TX) {
              msg.MSGTYPE=1;
              setSerialPrint(pVT_Net,"SEND->CANMsgSend");
              VT_CAN_MsgSend(pVT_Net,&msg);
            } else{
              setSerialPrint(pVT_Net,"RECEIVE->CANMsgPGN");
              msg.MSGTYPE=0;
              CANMsgPGN(msg);
            }
          }
        return;
      }//SETMSG=
      //
      //"S=" TVT_VTSelectInput message, S=6000,1
      if (str.indexOf("S=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("S=",""); str.trim();       
        j=str.indexOf(",");
          if ((j>0) && (pVT_Net->VT_ActiveMask<0xFFFF)) {
           ss=str.substring(0,j); str.remove(0,j+1);
           objID=ss.toInt(); oValue=str.toInt();
           objIdx=getVTDrawID(pVT_Net,objID);
           //getStreamDrawInfo(pVT_Net);
             //
             if (objIdx>=0) {
                //key
                if (pVT_Net->VTObjType==5) {pVT_Net->TFT_KeySelect==pVT_Net->TFT_SelectNr; setFocus=true;}
                //button
                if (pVT_Net->VTObjType==6) {pVT_Net->TFT_ButtonSelect==pVT_Net->TFT_SelectNr;setFocus=true;}
                //input
                if (HasInArray(pVT_Net->VTObjType,inpAllObjSet)) {pVT_Net->TFT_InputSelect=pVT_Net->TFT_SelectNr;setFocus=true;}
                //
                if (setFocus){
                 TVT_VTSelectInput msgObj;     
                  if (msgObj.setMsgToAttr(&msg,pVT_Net)) {     
                      //check TVT_VTSelectInput message
                      if (msg.MSG_TX) VT_CAN_MsgSend(pVT_Net,&msg);
                  }//msg
               }//setFocus
             } else oValue=255; //objIdx>=0  
             //
             if (oValue==255){
                pVT_Net->TFT_KeySelect=0;
                pVT_Net->TFT_ButtonSelect=0;
                pVT_Net->TFT_InputSelect=0;
                pVT_Net->TFT_KeySelectObjID=0xFFFF;
                pVT_Net->TFT_ButtonSelectObjID=0xFFFF;
                pVT_Net->TFT_InputSelectObjID=0xFFFF;
             } else setVTDrawListClearSelect(pVT_Net,objID);
           pVT_Net->VTPoolRefresh=true;
          }//j>0
        return;
      }//S=
      //
      //"N=" TVT_VTChangeNumericValue message N=9002,4 or TVT_VTInputStringValue N=8000,WERTE 
      if (str.indexOf("N=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("N=",""); str.replace("n=",""); str.trim();       
        j=str.indexOf(",");
          if ((j>0) && (pVT_Net->VT_ActiveMask<0xFFFF)) {
           ss=str.substring(0,j); str.remove(0,j+1);
           objID=ss.toInt(); oValue=str.toInt();
           objIdx=getVTObjID(pVT_Net,objID);
             //
             if (objIdx>=0) {
              pVT_Net->VTValue=oValue;
              pVT_Net->VTValueStr=str;
              //TEST
              //setSerialPrint(pVT_Net,String(pVT_Net->VTObjID));
              //setSerialPrint(pVT_Net,String(pVT_Net->VTObjType));
              //setSerialPrint(pVT_Net,String(pVT_Net->VTValue));
              //setSerialPrint(pVT_Net,pVT_Net->VTValueStr);
              //
              str="V"+ pVT_Net->VTValueStr;
              buttonEventDirect(str,true);
             }//objIdx>=0  
          }//j>0
        return;
      }//N=
      //
      //"P=" pointing event message
      if (str.indexOf("P")==0) {
        setSerialPrint(pVT_Net,str);
        j=str.indexOf("="); pVT_Net->VTsPos=1;
          if (j==2){
           ss=str[1]; pVT_Net->VTsPos=ss.toInt();        
          }
          if (j>0) str.remove(0,j+1); 
        str.trim(); j=str.indexOf(",");
          if ((j>0) && (pVT_Net->VT_ActiveMask<0xFFFF)) {
           ss=str.substring(0,j); str.remove(0,j+1);
           pVT_Net->VTxPos=ss.toInt(); pVT_Net->VTyPos=str.toInt();
           //setSerialPrint(pVT_Net,String(pVT_Net->VTxPos));
           //setSerialPrint(pVT_Net,String(pVT_Net->VTyPos));
           TVT_VTPointingEvent msgObj;     
           msgObj.setMsgToAttr(&msg,pVT_Net);     
            //check PointingEvent message
            if (msg.MSG_TX) VT_CAN_MsgSend(pVT_Net,&msg);
          }//j>0
        return;
      }//P=
      //
      //with VTActivation VTAudio Terminierung
      if (str.indexOf("T=")==0)  {
        setSerialPrint(pVT_Net,str);
        TVT_VTControlAudioDeviceTermination msgObj;
        setMSG=msgObj.setMsgToAttr(&msg,pVT_Net);
          if ((setMSG) && (pVT_Net->CAN_active)) {
            if (msg.MSG_TX) VT_CAN_MsgSend(pVT_Net,&msg);
          }
        return;
      }//T=
      //    
      //with ACKN Key "C=", button "B=", Input "A=",
      if ((str.indexOf("C=")==0) || (str.indexOf("B=")==0) || (str.indexOf("A=")==0)) {
        VT_Net.VTSelectMode=str[0];
        setSerialPrint(pVT_Net,str);
        bMode+=1*(str.indexOf("B=")==0);       
        bMode+=3*(str.indexOf("A=")==0);       
        str.remove(0,2); str.trim();       
        //getStringHEXInfo(str,true);
        j=str.length();
         if (j>0){
          msg.DATA[0]=bMode; refresh=(msg.DATA[0]>0);
          //check is numeric 
          nn=char(str[0]);
          //setSerialPrint(pVT_Net,String(nn,HEX));
            if ((nn>=0x30) && (nn<=0x39)) {
              msg.DATA[0]=0xFF;
              //set deselect
              ss="D";
                if (setKeyNumberSelect(pVT_Net,&msg,ss)) {
                  if (msg.MSG_TX) VT_CAN_MsgSend(pVT_Net,&msg);
                }
              j=1;
            }
            //
          msg.DATA[0]=bMode; refresh=(msg.DATA[0]>0);
            //
            //ESC to screen keyboard
            if ((str[0]=='E') && (pVT_Net->VTPageSelect==2)) {
              setSerialPrint(pVT_Net,"ESC-Keyboard");
              pVT_Net->VTPageSelect=0;pVT_Net->VTKeyboardOffs=0; //reset unicode offset
              setup_VTKeyboard(); 
              pVT_Net->VTPoolRefresh=true;    
            }
            //
            if (str[0]=='V') {
              j=1;
                //get screen keyboard for input
                if (pVT_Net->TFT_InputSelectObjID<0xFFFF){  
                  if ((str.length()==1) && (HasInArray(pVT_Net->TFT_InputSelectObjType,editObjSetK))) {
                    pVT_Net->VTPageSelect=2;setup_VTKeyboard(); ss=str;
                    //pVT_Net->TFT_InputSelect=1;
                    pVT_Net->VTObjID=pVT_Net->TFT_InputSelectObjID;
                    pVT_Net->VTObjType=pVT_Net->TFT_InputSelectObjType;
                    //check edit mode active, TVT_VTSelectInput set msg edit open/close
                    TVT_VTSelectInput msgObj;
                      if (msgObj.setMsgToAttr(&msg,pVT_Net)){
                       VT_CAN_MsgSend(pVT_Net,&msg);
                      }
                    return;
                  }//length=1
                }//<0xFFFF  
            }
            //  
            for (i=0;i<j;i++) {
             ss.clear(); 
              if (j==1) ss+=str; else ss+=str[i];
              //getStringHEXInfo(ss,true);
              //getStringHEXInfo(str,true);
              //
              if (setKeyNumberSelect(pVT_Net,&msg,ss)) {
                //check KeyActivation message
                if (msg.MSG_TX) {
                   if ((msg.ID & 0x00FF0000)==TP_PGN) {
                    //setSerialPrint(pVT_Net,"TVTTransportProtocol");
                    //getArray8Info(pVT_Net,(uint8_t*)pVT_Net->stream_TP[pVT_Net->listNr].getBuffer(),pVT_Net->stream_TP[pVT_Net->listNr].available());
                    TVTTransportProtocol msgObj;
                    msgObj.getTransportProtcol(&msg,pVT_Net,&pVT_Net->stream_TP[pVT_Net->listNr]);
                   }
                 VT_CAN_MsgSend(pVT_Net,&msg);
                }
              }
            }//for i
           //            
         }//j>0
        return;
      }//A=, B=, C=
      //
      if ((str.indexOf("IMAGEMODE=")==0) || (str.indexOf("I=")==0)) {
        setSerialPrint(pVT_Net,str);
        str.replace("IMAGEMODE=",""); str.replace("I=",""); str.trim();
        VT_Net.ImgMode=str.toInt();
        VTPoolDataRefresh(pVT_Net,false);
        return;
      }//I=
      //
      if ((str.indexOf("INFOMODE=")==0) || (str.indexOf("M=")==0)) {
        setSerialPrint(pVT_Net,str);
        str.replace("INFOMODE=","");  str.replace("M=",""); str.trim();
        VT_Net.VT_InfoMode=str.toInt();
        return;
      }//M=
      //
      //get info DrawList
      if (str.indexOf("D=")==0) {
        setSerialPrint(pVT_Net,str);
        setSerialPrint(pVT_Net,"DrawListSize=" + String(getVTDrawListSize(pVT_Net)));
        str.replace("D=",""); str.trim();
          if (str.length()>0) objIdx=str.toInt();
          if (objIdx>=0) showAttr=3; 
          if (objIdx<0)  showAttr=0; 
          //
          if (objIdx<-1) {
            getArray8Info(pVT_Net,pVT_Net->stream_Draw[pVT_Net->listNr].getBuffer(),pVT_Net->stream_Draw[pVT_Net->listNr].available());
          } else getVTDrawListInfo(pVT_Net,objIdx,showAttr);
          //
        getHeapStatus(pVT_Net,110);
        return;
      }//D=
      //
      //get/set TECU monitor file
      if (str.indexOf("OM=")==0) {
        str.replace("OM=",""); str.trim(); ss="";
          if ((str.length()>0) && (str.indexOf(".IOP")>0)){
            if (getFileExists(pVT_Net,1,str.c_str())) pVT_Net->TECU_Monitor=str;
          }
       setSerialPrint(pVT_Net,pVT_Net->TECU_Monitor);  
       return;
      }
      //
      //get info object list
      if (str.indexOf("O=")==0) {
        objCount=getVTObjectListSize(pVT_Net);
        setSerialPrint(pVT_Net,str);
        setSerialPrint(pVT_Net,"ObjListSize=" + String(objCount));
        // 
        str.replace("O=",""); str.trim(); ss="";
          //
          if (str.indexOf(".IOP")>0) {
            //check normal iop-file
           setSerialPrint(pVT_Net,str); 
            if (str.indexOf("/")>=0) ss=str; 
            //
            if (ss.length()==0){
              str.replace(".IOP","");
              //get WorkingSetName
              ss=getWSNameFromAddress(pVT_Net,pVT_Net->VT_DST[pVT_Net->listNr]);
                //
                if (ss.length()==0) {
                  ss="200C80001F600064"; //default WorkingSetName
                  setSerialPrint(pVT_Net,"Default WorkingSet=" + ss);
                }
               //Serial.println(str);
              str=getHexCharacterString(str);
              str="/" + ss + "/" + str + ".IOP";
              //Serial.println(str);
            }
            //
           pVT_Net->stream_ETP[pVT_Net->listNr].clear();  
           TVTVersion mObj;
              //
              if (mObj.readFile(SD,str.c_str(),&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net)) {
                setSerialPrint(pVT_Net,"Version_File found on SD:");
                setSerialPrint(pVT_Net,str);
                clearPoolData(pVT_Net);
                VTPoolData(pVT_Net,1);
                pVT_Net->stream_ETP[pVT_Net->listNr].clear();   
                pVT_Net->VT_InfoMode=false;
              }else {
               //IOP/_aafac_000.iop
               setSerialPrint(pVT_Net,"IOP or Version_File NOT found on SD!");
               setSerialPrint(pVT_Net,str);
              }
              //
            return;
          }
          //set active maskID of actual pooldata
          if (str.indexOf(".MSK")>0) {
            str.replace(".MSK",""); str.trim();
              if (objCount>0) {
                objID=str.toInt();
                objIdx=getVTObjID(pVT_Net,objID);
                 if (objIdx>=0) {
                   objIdx=getVTObjID(pVT_Net,0,true);
                    if (objIdx>=0){
                      //setActive Mask to WorkingSet
                      pVT_Net->streamStr.writeBytesVal(objID,2,5);
                      //getStreamStrInfo(pVT_Net);  
                      setVTObjectListValue(pVT_Net,objIdx);
                      pVT_Net->VTPoolRefresh=true;
                    }
                 }
              }
           return;
          }
          //
          if (str.length()>0) objIdx=str.toInt();
          if (objIdx>=0){
           pVT_Net->objNr=objIdx;
           showAttr=7;
          }else {
            showAttr=abs(objIdx);
          }
        getVTObjectListInfo(pVT_Net,objIdx,showAttr);
        getHeapStatus(pVT_Net,111);
        return;
      }//O=
      // 
      //get/set  maximum info object list
      if (str.indexOf("X=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("X=",""); str.trim();
        pVT_Net->VTObjMax[pVT_Net->listNr]=VTObjMaxDef;
          if (str.length()>0) objIdx=str.toInt();
          if (objIdx>=16) pVT_Net->VTObjMax[pVT_Net->listNr]=objIdx;
        setSerialPrint(pVT_Net,"pVT_Net->VTObjMax[" + String(pVT_Net->listNr) + "]=" + String(pVT_Net->VTObjMax[pVT_Net->listNr]));
        getHeapStatus(pVT_Net,112);
        return;
      }//X=
      //
      //SD filelist
      if (str.indexOf("SD=")==0) {
        setSerialPrint(pVT_Net,str);
        pVT_Net->SD_Mode=true;
        str.replace("SD=",""); str.trim(); 
        //SP_SD ViewModus
        getSP_SD_List(str);
        getHeapStatus(pVT_Net,112);
        return;
      }//SD=
      //
      //SPIFFS filelist
      if (str.indexOf("SP=")==0) {
        setSerialPrint(pVT_Net,str);
        pVT_Net->SD_Mode=false;
        str.replace("SP=",""); str.trim(); 
        getSP_SD_List(str);
        getHeapStatus(pVT_Net,113);
        return;
      }//SP=
      //
      //Get file text
      if (str.indexOf("GET=")==0) {
        setSerialPrint(pVT_Net,str);  pVT_Net->inputLineString.clear();
        TVTVersion msgObj;
        str.replace("GET=",""); str.trim(); 
         if ((str.length()==0) || (str.indexOf("LINE")==0))   {
           if (str.indexOf("LINE")==0){
            k=lStr.indexOf("=");lStr.remove(0,k+1);
            pVT_Net->inputLineString=lStr;
           }
          str="/auto.txt";
         }
         //
         //
         if (pVT_Net->SD_Mode) {
           msgObj.readFile(SD,str.c_str(),&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net);
         }else{
           msgObj.readFile(SPIFFS,str.c_str(),&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net);
         }
         //
         if (str.indexOf(".txt")>0){
           str=getStreamTextInfo(&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net);
            if (pVT_Net->inputLineString.length()>0){
              str=getInsertLineValue(pVT_Net,str);
              setSerialPrint(pVT_Net,delm0);
              setSerialPrint(pVT_Net,str);
              setSerialPrint(pVT_Net,delm0);
              pVT_Net->stream_ETP[pVT_Net->listNr].clear();
              pVT_Net->stream_ETP[pVT_Net->listNr].writeBytes((uint8_t*)str.c_str(),str.length());
              str="/auto.txt";
               // 
               if (pVT_Net->SD_Mode) {
                msgObj.writeStreamToFile(SD,str.c_str(),&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net);
               }else{
                msgObj.writeStreamToFile(SPIFFS,str.c_str(),&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net);
               }
            } 

         } else{
           getStreamInfo(&pVT_Net->stream_ETP[pVT_Net->listNr],pVT_Net);
         }
        pVT_Net->inputLineString.clear();
        return;
      }//GET=
      //
      //Delete file or folder
      if (str.indexOf("DEL=")==0) {
        setSerialPrint(pVT_Net,str);
        TVTVersion msgObj;
        str.replace("DEL=",""); str.trim(); 
         if (pVT_Net->SD_Mode) {
           msgObj.deleteFile(SD,str.c_str(),NULL,pVT_Net);
           listDir(SD, "/", 1); 
         }else{
           msgObj.deleteFile(SPIFFS,str.c_str(),NULL,pVT_Net);
           listDir(SPIFFS, "/", 1); 
         }
        return;
      }//DEL=
      //
      //make Dir or file
      if (str.indexOf("MKD=")==0) {
        setSerialPrint(pVT_Net,str);
        TVTVersion msgObj;
        str.replace("MKD=",""); str.trim(); 
         if (pVT_Net->SD_Mode) {
           if (msgObj.createDir(SD,str.c_str(),pVT_Net)) {
             listDir(SD, "/", 0); 
           }
         }else{
           if (msgObj.createDir(SPIFFS,str.c_str(),pVT_Net)) {
             listDir(SPIFFS, "/", 0); 
           }
         }
        return;
      }//MKD=
      //
      //get/set TraceFilter value
      if (str.indexOf("FLT=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("FLT=",""); str.trim(); 
           if (str.length()>0) pVT_Net->VTTraceFilter=str.toInt();
        setSerialPrint(pVT_Net,"FLT=",true); setSerialPrint(pVT_Net,String(pVT_Net->VTTraceFilter));
        return;
      } 
      //get heap status
      if (str.indexOf("H=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("H=",""); str.trim(); 
        getHeapStatus(pVT_Net,str.toInt());
        return;
      } 
      //get Keyboard
      if (str.indexOf("KB=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("KB=",""); str.trim(); 
          if (str.length()==0) {
            if (pVT_Net->VTPageSelect==0) pVT_Net->VTPageSelect=2;else pVT_Net->VTPageSelect=0; 
          } else {
           pVT_Net->VTPageSelect=str.toInt();  
          }
          //
          if (pVT_Net->VTPageSelect==2) {
            pVT_Net->VTPoolRefresh=false;
            setup_VTKeyboard();
          }else {
            setup_VTKeyboard();
            pVT_Net->VTPoolRefresh=true;
          }
          //
        return;
      } 
      //set/get serialOut
      if (str.indexOf("SR=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("SR=",""); str.trim(); 
          if (str.length()>0) pVT_Net->serialOut=str.toInt();  
        Serial.println("serialOut=" + String(pVT_Net->serialOut));
        //
        return;
      } 
      //set/get Auxiliary Assignment
      if (str.indexOf("AX=")==0) {
        setSerialPrint(pVT_Net,str);
        str.replace("AX=",""); str.trim(); j=str.indexOf(",");
          //clear all VTAuxAssignList
          if ((str.length()>0) && (j<0)) {
             if (str=="X") {
               getVTAuxObjectList(pVT_Net);
               return;
             } else {
               if (str=="0") setVTAuxAssignListClear(pVT_Net);
             }//str=="X" 
           str.clear(); 
          }//j<0
          //
          if ((str.length()>0) && (j>0)) {
            pVT_Net->listNr=0;                     
            ss=str.substring(0,j);str.remove(0,j+1);
            objID=ss.toInt();
            objIdx=getVTObjID(pVT_Net,objID);
            
            /*
            Serial.println(getStringHEX(pVT_Net->VT_DST[pVT_Net->listNr],2));
            Serial.println(pVT_Net->VTObjType);
            Serial.println(pVT_Net->VTObjID);
            Serial.println(objIdx);
            */
            //
            ss.clear();

              //gAuxFuncType=31,gAuxInpType=32;
              if ((objIdx>=0) && (pVT_Net->VTObjType==gAuxFuncType)) {
               Serial.println("FuncType=" + String(pVT_Net->VTObjType));
               ss+=getWSNameFromAddress(pVT_Net,pVT_Net->VT_DST[pVT_Net->listNr]);
               ss+=getStringHEX(objID,4);
                 //
                 if (str.length()>0) {
                   pVT_Net->listNr=1;                     
                   inpID=str.toInt();
                   objIdx=getVTObjID(pVT_Net,inpID);
                     if ((objIdx>=0) && (pVT_Net->VTObjType==gAuxInpType)) {
                      Serial.println("AuxInpType=" + String(pVT_Net->VTObjType));
                      ss+=getWSNameFromAddress(pVT_Net,pVT_Net->VT_DST[pVT_Net->listNr]);
                      ss+=getStringHEX(inpID,4);
                     }else inpID=0xFFFF;
                 }
              } else objID=0xFFFF;
              //
            pVT_Net->listNr=olist;
              //
              if (ss.length()>0) {
                Serial.println(ss);
                //Serial.println("VTAuxAssignList");
                //Serial.println(pVT_Net->VTAuxAssignList);
                j=pVT_Net->VTAuxAssignList.indexOf(ss);    
                  //
                  if (inpID<0xFFFF) {
                    if ((j<0) || (pVT_Net->VTAuxAssignList.length()==0)) pVT_Net->VTAuxAssignList+= ss + "\n";                    
                  }else{
                    if (j>=0) {
                      //k=pVT_Net->VTAuxAssignList.indexOf("\n",j);
                      pVT_Net->VTAuxAssignList.remove(j,41);
                    }

                  }
              }//length>0
              //
            pVT_Net->listNr=olist;  
          }//j>0
          //        
        Serial.println("VTAuxAssignList");
        Serial.println(pVT_Net->VTAuxAssignList);
        Serial.println(delm0);
        //
        return;
      } 
      //
      //not compiled time
     SetDateAndTime(str,false);
   //
  }//inputSerialComplete
}//getInputSerialString 


//==============================================================================
void loop_ButtonPressFor() {
uint16_t pressTime=2000;
String str="";
  //pressedFor
  if (pVT_Net->VTPageSelect==0){
   //
   if (M5.BtnA.pressedFor(pressTime)) {
    setSerialPrint(pVT_Net,"M5.BtnA.pressedFor:" + String(pressTime));
    pVT_Net->VTPressedFor=true;
    pVT_Net->VTSelectMode="A";
    str=VT_Net.VTSelectMode + "=0";
    setSerialPrint(pVT_Net,"VTSelectMode:" + str);
    pVT_Net->inputSerialComplete=true; pVT_Net->inputSerialString+=str.c_str(); 
    getInputSerialString();
   }//A
   //
   if (M5.BtnB.pressedFor(pressTime)) {
    setSerialPrint(pVT_Net,"M5.BtnB.pressedFor:" + String(pressTime));
    pVT_Net->VTPressedFor=true;
    pVT_Net->VTSelectMode="B";
   str=VT_Net.VTSelectMode + "=0";
    setSerialPrint(pVT_Net,"VTSelectMode:" + str);
    pVT_Net->inputSerialComplete=true; pVT_Net->inputSerialString+=str.c_str(); 
    getInputSerialString();
   }//B
   //
   if (M5.BtnC.pressedFor(pressTime)) {
    setSerialPrint(pVT_Net,"M5.BtnC.pressedFor:" + String(pressTime));
    pVT_Net->VTPressedFor=true;
    ResetSelectMode();
   }//C
  }//pVT_Net->VTPagedActive==0 
}//loop_ButtonPressFor
  


//==============================================================================
/*
        M5.BtnA/B/C.read();
        M5.BtnA/B/C.isPressed();
        M5.BtnA/B/C.isReleased();
        M5.BtnA/B/C.wasPressed();
        M5.BtnA/B/C.wasReleased();
        M5.BtnA/B/C.wasreleasedFor()
        M5.BtnA/B/C.pressedFor(uint32_t ms);
        M5.BtnA/B/C.releasedFor(uint32_t ms);
        M5.BtnA/B/C.lastChange();
*/
void loop_Button() {
 loop_ButtonPressFor();
 // 
 #if defined(M5STACK_MODE)
   //two button pressed
   //to normal screen
   if ((pVT_Net->VTPageSelect==2) && (M5.BtnA.isPressed()) && (M5.BtnC.isPressed())) {
      pVT_Net->VTPressedABC=true; pushStr="M5.BtnB/C.isPressed: VTPageSelect=";
      pVT_Net->VTPageSelect=0; setup_VTDateTime(pVT_Net);
 
   }//A/C
   //to time screen
   if ((pVT_Net->VTPageSelect==0) && (M5.BtnA.isPressed()) && (M5.BtnB.isPressed())) {
      pVT_Net->VTPressedABC=true; pushStr="M5.BtnA/B.isPressed: VTPageSelect=";
      pVT_Net->VTPageSelect=1;setup_VTDateTime(pVT_Net);
 
   }//A/B
   //to keyboard screen
   if ((pVT_Net->VTPageSelect==0) && (M5.BtnB.isPressed()) && (M5.BtnC.isPressed())) {
      pVT_Net->VTPressedABC=true; pushStr="M5.BtnA/C.isPressed: VTPageSelect=";
      pVT_Net->VTPageSelect=1; setup_VTKeyboard();
   }//B/C
     //  
  pVT_Net->VTPoolRefresh=(pVT_Net->VTPageSelect==0);
  
   //released/wasPressed
   if (M5.BtnA.wasPressed())  {
     if ((pVT_Net->VTPageSelect==0) && (!pVT_Net->VTPressedFor) && (!pVT_Net->VTPressedABC)) buttonEventDirect("B");
    pVT_Net->VTPressedFor=false; pVT_Net->VTPressedABC=false;
   }//ButtonA
   //
   if (M5.BtnB.wasPressed())  {
     if ((!pVT_Net->VTPressedFor) && (!pVT_Net->VTPressedABC)) {
       //
       if (pVT_Net->VTPageSelect==0) {
        str="";
         if ((pVT_Net->TFT_KeySelectObjID<0xFFFF) || (pVT_Net->TFT_ButtonSelectObjID<0xFFFF)) str="PR";
         if (pVT_Net->TFT_InputSelectObjID<0xFFFF)str="V";
         //
         if (str.length()>0) buttonEventDirect(str);
       }
     }//pressFor
     //
    pVT_Net->VTPressedFor=false; pVT_Net->VTPressedABC=false;
   }//Button B
   //
   if (M5.BtnC.wasPressed())  {
      if ((pVT_Net->VTPageSelect==0) && (!pVT_Net->VTPressedFor) && (!pVT_Net->VTPressedABC)) buttonEventDirect("N");
    pVT_Net->VTPressedFor=false; pVT_Net->VTPressedABC=false;
   }//Button C
  #endif //M5STACK_MODE
} //loop_Button  



//==============================================================================
void loop_Touch() {
uint16_t dCount=dCount=getVTDrawListSize(pVT_Net);  
 #if defined(M5CORE2_MODE)
   if ((pVT_Net->VTPageSelect==0) && (dCount==0)) {
     if (M5.Touch.changed ) {
       TouchPoint_t pos = M5.Touch.getPressPoint();  
        if ((pos.x>0) && (pos.y>0)){ 
          if (pos.x>240){
            for (int i=0;i<6;i++){
              if ((pos.y>40*i) && (pos.y<40*(i+1))) {
                setSerialPrint(pVT_Net,String(i));
                //                
                switch (i) {
                  case 0 : pVT_Net->VTPageSelect=1; 
                           setup_VTDateTime(pVT_Net);
                           break;
                           
                  case 1 : pVT_Net->VTPageSelect=2;
                           setup_VTKeyboard();
                           break;
                  /*
                  case 2 : //Auxiliary, pVT_Net->VTPageSelect=1;setup_VTAuxiliary();
                           break;

                  case 5 : //to pool back, pVT_Net->VTPageSelect=1;setup_VTAuxiliary();
                           break;
                  
                  default:break;
                  */
                }//switch
                //
               break;   
              }
            }//for i
          
          
          }//>240
          //
         setSerialPrint(pVT_Net,String(pos));
        }
     }
   }
   //
  #endif //M5CORE2_MODE
} //loop_Touch



//==============================================================================
// Defines gestures
//==============================================================================
#ifdef M5CORE2_MODE
Gesture swipeRight("Swipe Right", 40, DIR_RIGHT, 20, true);
Gesture swipeLeft( "Swipe Left",  40, DIR_LEFT,  20, true);
Gesture swipeDown( "Swipe Down",  40, DIR_DOWN,  20, true);
Gesture swipeUp(   "Swipe Up",    40, DIR_UP,    20, true);


//==============================================================================
// Lazy output routine. Just enough to do the job.
// str should fit on the screen (320 pixels) and not contain a \n
void output_info(const char* name, const char* info) {
  Serial.printf("%-15s: %s\n", name, info);
}


//==============================================================================
void gestureHandler(Event& e) {
CANMsg msg;
uint8_t setPush=0,setESC=0,i=0;
uint8_t* buff=pVT_Net->streamStr.getBuffer();
int8_t pPageSelect=-1; 
String str=String(e.gesture->getName()),ss="";
 output_info(e.typeName(), e.gesture->getName());
 setGesture=true;
  //  
  if ((str=="Swipe Right") || (str=="Swipe Left")) {
   WiFi_resetNetworks(pVT_Net,""); 
   pPageSelect=pVT_Net->VTPageSelect;
    if (str=="Swipe Right") pVT_Net->VTPageSelect++;
    if (str=="Swipe Left")  pVT_Net->VTPageSelect--;
    //
    if (pVT_Net->VTPageSelect>pVT_Net->VTPageMax) pVT_Net->VTPageSelect=0; 
    if (pVT_Net->VTPageSelect<0) pVT_Net->VTPageSelect=pVT_Net->VTPageMax; 
    //
    if (pPageSelect!=pVT_Net->VTPageSelect){
     pVT_Net->VTTraceLine=0;
      if (pPageSelect==1) setup_VTDateTime(pVT_Net);     
      if (pPageSelect==2) setup_VTKeyboard();     
      if (pPageSelect==3) setup_VTTrace();     
    }
    //
    switch (pVT_Net->VTPageSelect) {
             //pooldata
     case 0:   //if (pPageSelect==1) setup_VTDateTime(pVT_Net);
               //if (pPageSelect==2) setup_VTKeyboard();
               //if (pPageSelect==pVT_Net->VTPageMax) setup_VTDateTime(pVT_Net);
             break;
             //DateTime
     case 1: setup_VTDateTime(pVT_Net);
             break;
             //Keyboard
     case 2: setup_VTKeyboard();
             break;
             //Trace and Info
     case 3: setup_VTTrace();
             break;
    }//switch
   //return;
  }//gesture Right/Left
  // 
  //
  if ((str=="Swipe Down") || (str=="Swipe Up"))  {
    if (pVT_Net->VTPageSelect==2) {
       //set unicode letter offset to VTKeyboard
       if (key_mode==KEY_MODE_LETTER) {
         if ((str=="Swipe Down") && (pVT_Net->VTKeyboardOffs<0x4000)) pVT_Net->VTKeyboardOffs+=0x20;
         if ((str=="Swipe Up")   && (pVT_Net->VTKeyboardOffs>=0x20))   pVT_Net->VTKeyboardOffs-=0x20;
        drawVTKeyboard();
        return;
       } 
    } else{
     setup_VTKeyboard();
       if (pVT_Net->VTPageSelect==0){
         resetSelectInput(pVT_Net);
         
         //
         pVT_Net->fntNamePool[pVT_Net->listNr]=pVT_Net->fntName;
         Serial.println(String(pVT_Net->listNr) + ".Swipe Mode=" + str);   
         Serial.println("AuxlistNr=" + String(pVT_Net->AuxlistNr));   
         Serial.println(delm1);   
            //
            if (str=="Swipe Up"){
                if (pVT_Net->listNr<listMax-1) {
                  pVT_Net->AuxlistNr=pVT_Net->listNr;
                  getVTAuxObjectList(pVT_Net);
                  //
                  pVT_Net->listNr=listMax-1;
                } else pVT_Net->listNr=pVT_Net->AuxlistNr;
                //
            } else {
             switch (pVT_Net->listNr) {
              case 0:pVT_Net->listNr=1;break;
              case 1:pVT_Net->listNr=0;break;
              default:pVT_Net->listNr=0;break;
             }//switch
             //
             if (getVTObjectListSize(pVT_Net)==0) pVT_Net->listNr=(pVT_Net->listNr+1) % (listMax-1);
            }
            //
            //
            if (pVT_Net->listNr==(listMax-1)){
              Serial.println("load _tecu_monitor.iop");

              //load _tecu_monitor.iop 
              pVT_Net->inputSerialString.clear();
              //
              pVT_Net->inputSerialString+="O=" + pVT_Net->TECU_Monitor;
              //pVT_Net->inputSerialString+="O=/_tecu_monitor.iop";
              //pVT_Net->inputSerialString+="O=/_v6_opc50_4.iop";
              //
              pVT_Net->inputSerialComplete=true;
              Serial.println("getInputSerialString");
              getInputSerialString();
              //
              Serial.println("get TECU-monitor objects");
              //
              //get TECU-monitor objects
              setTECUMonitor(pVT_Net);
              //
              Serial.println("TVTAuxiliaryInputStatusEnable");
              //VT send Enabled CAN message to all assign AUX-Input
              TVTAuxiliaryInputStatusEnable mObj;
              mObj.setMsgToAttr (&msg,pVT_Net,NULL); 
              pVT_Net->listNr=listMax-1;
              //
              return;
            }else{
              if (getVTObjectListSize(pVT_Net)>0) {
                setSerialPrint(pVT_Net,"Change Workingset");
                //send VTResponse
                getVTActiveMask(pVT_Net);
                TVT_VTDisplayActivation mObj; 
                   //
                   if (mObj.setMsgToAttr(&msg,pVT_Net)) {
                     setSerialPrint(pVT_Net,getMsgFrameStr(&msg));
                     VT_CAN_MsgSend(pVT_Net,&msg);
                   }
              } else pVT_Net->listNr=0;
             VTPoolDataRefresh(pVT_Net,false);
             return;
            }//listNr==2    
       }//pVT_Net->VTPageSelect==0
    }//pVT_Net->VTPageSelect==2  
  }//Down/Up
  //  
  //
  if (pVT_Net->VTPageSelect==0){
     pVT_Net->VTPoolRefresh=true;
     //
     setESC+=(pVT_Net->VTPageSelect==0);
      //Check ESC send CAN-message, Cancel input
      if (setESC==2) {
        setSerialPrint(pVT_Net,"SET_ESC");
        TVT_VTESC mObj;
         if (mObj.setMsgToAttr(&msg,pVT_Net)) {
           setSerialPrint(pVT_Net,getMsgFrameStr(&msg));
           VT_CAN_MsgSend(pVT_Net,&msg);
         }
      }//setESC
      //
  }//pVT_Net->VTPageSelect==0
}//gestureHandler


//==============================================================================
// Gestures only need to be set up once. I'll protect against multiple initializations.
void setupGestures() {
static bool done = false;
  if(!done) {
    done = true;
    swipeRight.addHandler(gestureHandler, E_GESTURE);
    swipeLeft.addHandler(gestureHandler, E_GESTURE);
    swipeUp.addHandler(gestureHandler, E_GESTURE);
    swipeDown.addHandler(gestureHandler, E_GESTURE);
  }
}//setupGestures
#endif //M5CORE2_MODE



//==============================================================================
//SETUP
//==============================================================================
void setup() {
String str="";
int i=0,j=0;
struct tm tmstruct ;
boolean setWiFi=false;
boolean Set_Stream[listMax];
uint8_t oImgMode=pVT_Net->ImgMode;
 //
 pVT_Net->ImgMode=1; oImgMode=pVT_Net->ImgMode;
  //
  for (i=0;i<listMax;i++){
    pVT_Net->fntNamePool[i]="arial2-0";
    // 
    pVT_Net->VT_DST_TIME[i]=millis();         //default ECU-TimeStampDiff 0..listMax
    pVT_Net->VT_DST[i]=0xFF;                  //default ECU-addresses 0..listMax
    pVT_Net->VT_DST_WS[i]="FFFFFFFFFFFFFFFF"; //default ECU-Workingset Name 0..listMax
    //max objects in objectlist
    pVT_Net->VTObjMax[i]=VTObjMaxDef;         //default max objects in pooldata 0..listMax
  } //for i
   //
  //ViewLog
  if (!pVT_Net->notSerial) {
      Serial.begin(115200); 
       //
       while ((!Serial) && (i < 300)) {
        delay(10);  
        i++;
      }
     //
    pVT_Net->serialOut=(Serial);
  } else pVT_Net->serialOut=false;
  //
 //TEST
 //pVT_Net->serialOut=false;
 
 setupStack();
  //
  #ifdef STACK_MODE
    M5.begin();
      #ifdef M5STACK_MODE
        M5.Power.begin();
        pVT_Net->tft.setBrightness(255);
      #endif
      //
      #ifdef M5CORE2_MODE
        //M5.Power.begin();
        pVT_Net->tft.setBrightness(255);
        //M5.Buttons.addHandler(buttonEvent,E_TOUCH);
        M5.Buttons.addHandler(buttonEvent,E_RELEASE);
        setupGestures();   
      #endif
      //
    pVT_Net->VTPageSelect=3;
    setup_VTTrace();
    pVT_Net->ImgMode=oImgMode;
  #endif //STACK_MODE
  //
  #ifdef OLED_MODE
    OLED_Init();
  #endif
 //
 //M5Stack set/get PSRAM mode
 Set_M5Stack_PSRAM(pVT_Net);
 //Sketch info
 setSerialPrint(pVT_Net,delm0);
 setSerialPrint(pVT_Net,"ESP_SketchSize=",true); setSerialPrint(pVT_Net,String(ESP.getSketchSize()));
 setSerialPrint(pVT_Net,"ESP_FreeSketchSpace=",true); setSerialPrint(pVT_Net,String(ESP.getFreeSketchSpace()));
 setSerialPrint(pVT_Net,delm0);
 // 
 oImgMode=pVT_Net->ImgMode;
 pVT_Net->ImgMode=0;
 Set_setTextColor(pVT_Net,0);
 setClearScreen(pVT_Net,true);
 setup_log();
 logMemory();
 //
 pVT_Net->streamStr.setNewBufferSize(300000,pVT_Net->PSRam);
 pVT_Net->streamDraw.setNewBufferSize(16000,pVT_Net->PSRam);
   //
   for (i=0;i<listMax;i++) {
     if (i==0) Set_Stream[i]=true;else Set_Stream[i]=pVT_Net->PSRam;
     //
     if (Set_Stream[i]) {
      pVT_Net->stream_Cmd[i].setNewBufferSize(20000,pVT_Net->PSRam);
      pVT_Net->stream_Draw[i].setNewBufferSize(20000,pVT_Net->PSRam);
      pVT_Net->streamObj[i].setNewBufferSize(300000,pVT_Net->PSRam);
      pVT_Net->stream_Pool[i].setNewBufferSize(32000,pVT_Net->PSRam);
      pVT_Net->stream_ETP[i].setNewBufferSize(300000,pVT_Net->PSRam);
      pVT_Net->stream_TP[i].setNewBufferSize(2000,pVT_Net->PSRam);
     }
   }//for i
   //
  setSerialPrint(pVT_Net,delm1);
  setSerialPrint(pVT_Net,"streamStr=",true);  setSerialPrint(pVT_Net,String(pVT_Net->streamStr.getBufferSize()));
  setSerialPrint(pVT_Net,"streamDraw=",true); setSerialPrint(pVT_Net,String(pVT_Net->streamDraw.getBufferSize()));
  setSerialPrint(pVT_Net,delm0);
   //
   for (i=0;i<listMax;i++) {
    setSerialPrint(pVT_Net,"streamObj" + String(i) + "=",true); setSerialPrint(pVT_Net,String(pVT_Net->streamObj[i].getBufferSize()));
    setSerialPrint(pVT_Net,"Draw" + String(i) + "=",true);      setSerialPrint(pVT_Net,String(pVT_Net->stream_Draw[i].getBufferSize()));
    setSerialPrint(pVT_Net,"Cmd"  + String(i) + "=",true);      setSerialPrint(pVT_Net,String(pVT_Net->stream_Cmd[i].getBufferSize()));
    setSerialPrint(pVT_Net,"Pool" + String(i) + "=",true);      setSerialPrint(pVT_Net,String(pVT_Net->stream_Pool[i].getBufferSize()));
    setSerialPrint(pVT_Net,"ETP"  + String(i) + "=",true);      setSerialPrint(pVT_Net,String(pVT_Net->stream_ETP[i].getBufferSize()));
    setSerialPrint(pVT_Net,"TP"   + String(i) + "=",true);      setSerialPrint(pVT_Net,String(pVT_Net->stream_TP[i].getBufferSize()));
    setSerialPrint(pVT_Net,delm0);
   }
 setSerialPrint(pVT_Net,delm1);
 //
 logMemory();
 // 
 str="\nSETUP START ImgMode=" + String(pVT_Net->ImgMode) +  "\n" + delm1;
 setSerialPrint(pVT_Net,str); 
 getHeapStatus(pVT_Net,0);
 str="\nFontsLoaded=" +  String(pVT_Net->tft.fontsLoaded(),BIN);
 setSerialPrint(pVT_Net,str); 
 str="ActiveFont=" +  String(pVT_Net->tft.textfont);
 setSerialPrint(pVT_Net,str); 
   //
   if (SPIFFS.begin()) {
     str="SPIFFS initialisation OK";
     setSerialPrint(pVT_Net,str); 
     pVT_Net->valid_sp=true;
   }else {  
     str="SPIFFS initialisation failed!";
     setSerialPrint(pVT_Net,str); 
     pVT_Net->valid_sp=false;
   }
   //
   #ifndef STACK_MODE
    if (!SD.begin(TFCARD_CS_PIN, SPI, 40000000)){
     str="SD initialisation failed!";
     setSerialPrint(pVT_Net,str); 
     pVT_Net->valid_sd=false;
    }   
   #endif  
   //
   if (pVT_Net->valid_sd){
     pVT_Net->fs_SD=&SD;
     //listDir(*pVT_Net->fs_SD, "/", 1,"SD:");
   } 
   if (pVT_Net->valid_sp){
     pVT_Net->fs_SP=&SPIFFS;
     //listDir(*pVT_Net->fs_SP, "/", 1,"SPIFFS:");
   } 
   // 
 str="\r\nInitialisation done.\n" + delm1;
 setSerialPrint(pVT_Net,str); 
 //  
 pVT_Net->pTime=millis();
 //
 pVT_Net->VT_InfoMode=true;
 str="\n" + delm1 +"\nSD-CARD_INFO\n" + delm1;
 setSerialPrint(pVT_Net,str); 
 //
 pVT_Net->VT_InfoMode=true;
   if ((!pVT_Net->valid_sp) && (pVT_Net->valid_sd)) {
    str="/";
    //listDir(*pVT_Net->fs_SD, str.c_str(), 1,"SD:");
   }
   //
   if (pVT_Net->VT_InfoMode) getSdFat_Info();
   //
   //list fontfiles of SPIFFS
   if (pVT_Net->valid_sp) {
    str="/";
    listDir(*pVT_Net->fs_SP, str.c_str(), 1,"SPIFFS:");
   }
   //
   //set WEB_MODE 
   #if defined(ESP32) && defined(STACK_MODE)
     #if defined(WEB_MODE)
      setServerModeActive();
     #endif
   #endif //STACK_MODE
   //
   //
   if (pVT_Net->VT_InfoMode) {
     getSPIFFS_Info();
     showFontSet();
     getUserSetup();  
   }
   //
 pVT_Net->VT_InfoMode=false;
  //
  for (i=0;i<2;i++) {
   pVT_Net->stream_ETP[i].clear(); 
   pVT_Net->stream_TP[i].clear();
  }
  //
 pVT_Net->ImgMode=oImgMode;
 //
 //default draw
 init_can();
 //
 //Check I2C devices 
 addI2cDeviceAll(); 
 scani2caddr();
 checkI2cAddr();
 //
 VTDateTimeInit(pVT_Net,true,setWiFi);
 rtc.setTimeStruct(pVT_Net->tmstruct);
 pVT_Net->tmstruct=rtc.getTimeStruct();
 printDateTimeStructure("RTC",pVT_Net);
 //
 getHeapStatus(pVT_Net,0); 
 // 
 logMemory();
  //clear Serial
  while (Serial.available()) Serial.read();
 //
 getStatusInfo();
 // 
 pVT_Net->VTPageSelect=0;
 pVT_Net->VTPoolRefresh=true;pVT_Net->VTPoolClear=true;
 // 
 str="\nSETUP END ImgMode=" + String(pVT_Net->ImgMode) +  "\n" + delm1;
 setSerialPrint(pVT_Net,str);
} //setup


//==============================================================================
//LOOP
//==============================================================================
void loop() {
String str="";
CANMsg msg;
int16_t  i=0,dw=0,dh=0;
uint8_t  setPush=0;
String pushStr="M5.Buttons.event: VTPageSelect=";
 loop_TempHumSensor();
 //
 loop_VTDateTime();
 //
 M5.update();
 loop_Button();
 loop_Touch();
 //
 serialEventInput();
 getInputSerialString();
 // 
 test_can_transmit();
 test_can_receive();
 //
  //Server
  #ifdef WEB_MODE
    server.handleClient(); 
    //webSocketloop();
  #endif //WEB_MODE
  // 
  //
  //set macro commands
  if (pVT_Net->stream_Cmd[pVT_Net->listNr].available()>8) {
   msg.ID=0x1CE7260A;
   //get command 
   msg.LEN=pVT_Net->stream_Cmd[pVT_Net->listNr].read();
   pVT_Net->stream_Cmd[pVT_Net->listNr].readBytes((uint8_t*) msg.DATA,msg.LEN);
   pVT_Net->stream_Cmd[pVT_Net->listNr].removeBytes(0,msg.LEN+1);
   setSerialPrint(pVT_Net,getMsgFrameStr(&msg));
   MacroCANMsgPGN(pVT_Net,&msg,true);
     if (pVT_Net->stream_Cmd[pVT_Net->listNr].available()<8) {
       pVT_Net->stream_Cmd[pVT_Net->listNr].clear();
       pVT_Net->VTPoolRefresh=true;
     }
  }
  //
  if (pVT_Net->VTPageSelect>0) pVT_Net->VTPoolRefresh=false;
  //
  //Refresh paint pool data
  if (pVT_Net->VTPoolRefresh) {
    pVT_Net->VTPoolRefresh=false;
    str=setVTDrawListSoftKey(pVT_Net);
    //setSerialPrint(pVT_Net,str);
    VTPoolDataRefreshDirect(pVT_Net);  
    setSerialPrint(pVT_Net,"VTPoolDataRefreshDirect:ImgMode=" + String(pVT_Net->ImgMode));
      if (!pVT_Net->VTChangeSelect) {
        pVT_Net->VTChangeSelect=(str!=setVTDrawListSoftKey(pVT_Net));
        //setSerialPrint(pVT_Net,str);
      }
      //
      if (pVT_Net->VTChangeSelect){
       pVT_Net->VTChangeSelect=false;
       //Serial.println("VTChangeSelect");
       ResetSelectMode();
      }
  }//pVT_Net->VTPoolRefresh
  //
 SetAudioCommand();
}//loop



//==============================================================================
//==============================================================================
