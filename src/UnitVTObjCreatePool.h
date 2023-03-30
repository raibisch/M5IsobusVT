//==============================================================================
//Definition and Create Pooldata objects
//==============================================================================
//unit UnitVTObjCreatePool.h;
//==============================================================================
#if defined(ESP32)
 //void * StackPtrAtStart;
 //void * StackPtrEnd;
 //UBaseType_t watermarkStart;
#endif


//------------------------------------------------------------------------------
#ifndef UnitVTObjConsts_h
 #define UnitVTObjConsts_h
   #include "UnitVTObjConsts.h"
   TVT_Net VT_Net;
   TVT_Net *pVT_Net=&VT_Net;
#endif

//------------------------------------------------------------------------------
#ifndef UnitVTObject_h
 #define UnitVTObject_h
 #include "UnitVTObject.h"
#endif

//==============================================================================
#ifndef UnitVTMacroCommandObject_h
 #define UnitVTMacroCommandObject_h
 #include "UnitVTMacroCommandObject.h"
#endif


//==============================================================================
//CAN-Messages
//==============================================================================
#ifndef UnitVTCommandTechData_h
 #define UnitVTCommandTechData_h
 #include "UnitVTCommandTechData.h"
#endif


//==============================================================================
//VTScreen Keyboard
//==============================================================================
#include <Free_Fonts.h>

#define KEYBOARD_X (2)
#define KEYBOARD_Y (26)

#define KEY_W (45)
#define KEY_H (50)

#define COLS (7)
#define ROWS (4)

#define MAX_SHIFT_MODE (4)

//==============================================================================
char keymap[MAX_SHIFT_MODE][ROWS][COLS] =
{
  {
    {'a', 'b', 'c', 'd', 'e', 'f', 'g'},
    {'h', 'i', 'j', 'k', 'l', 'm', 'n'},
    {'o', 'p', 'q', 'r', 's', 't', 'u'},
    {'v', 'w', 'x', 'y', 'z', ' ', '\002'}, // 002 = shift
  },
  {
    {'A', 'B', 'C', 'D', 'E', 'F', 'G'},
    {'H', 'I', 'J', 'K', 'L', 'M', 'N'},
    {'O', 'P', 'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z', ' ', '\002'}, // 002 = shift
  },
  {
    {'`', '1', '2', '3', '4', '5', '6'},
    {'7', '8', '9', '0', '-', '=', '['},
    {']', '\\', ';', '\'', ',', '.', '/'},
    {' ', ' ', ' ', ' ', ' ', ' ', '\002'}, // 002 = shift
  },
  {
    {'~', '!', '@', '#', '$', '%', '^'},
    {'&', '*', '(', ')', '_', '+', '{'},
    {'}', '|', ':', '"', '<', '>', '?'},
    {' ', ' ', ' ', ' ', ' ', ' ', '\002'}, // 002 = shift
  },
};

typedef enum {
  KEY_MODE_LETTER = 0,
  KEY_MODE_NUMBER = 1,
} key_mode_t;

//==============================================================================
//global variable
String input_text="";
key_mode_t key_mode = KEY_MODE_LETTER;
bool shift_mode = false;
Button *touch_button_list[COLS*ROWS];

//==============================================================================
//==============================================================================
setup_t user; // The library defines the type "setup_t" as a struct
              // Calling tft.getSetup(user) populates it with the settings


//==============================================================================
//global variable
//==============================================================================
//StreamString  inputSerialString;      // a String to hold incoming data
String  inputSerialString;      // a String to hold incoming data

boolean inputSerialComplete = false;  // whether the string is complete
boolean notSerialReceive=false;
boolean SD_Mode=true;

//==============================================================================
//PROTOTYPE
//==============================================================================
//------------------------------------------------------------------------------
void getStackStatus(TVT_Net *pVT_Net) {
  #if defined(ESP32) && defined(STACK_MODE)
   void* SpActual = NULL;
   Serial.printf("Free Stack at actual position is: %d \r\n", (uint32_t)&SpActual - (uint32_t)pVT_Net->StackPtrEnd);
  #endif//STACK_MODE
};//getStackStatus


//------------------------------------------------------------------------------
void setupStack() {
  #if defined(ESP32)
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable   detector brownout  
    void* SpStart = NULL;
    pVT_Net->StackPtrAtStart = (void *)&SpStart;
    pVT_Net->watermarkStart =  uxTaskGetStackHighWaterMark(NULL);
    pVT_Net->StackPtrEnd = pVT_Net->StackPtrAtStart - pVT_Net->watermarkStart;
    //
    getStackStatus(pVT_Net);
  #endif//ESP32
};//setupStack


//==============================================================================
//==============================================================================
void logMemory() {
  Serial.println("USED_LOG_MEMORY");
  Serial.print("Used PSRAM:");  Serial.println(ESP.getPsramSize() - ESP.getFreePsram());
};//logMemory

//==============================================================================
void setup_log() {
  Serial.println("SETUP_LOG_MEMORY");
  Serial.print("Total heap:");  Serial.println(ESP.getHeapSize());
  Serial.print("Free heap:");   Serial.println(ESP.getFreeHeap());
  Serial.print("Total PSRAM:"); Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM:");  Serial.println(ESP.getFreePsram());
};//setup_log

//==============================================================================
void printProcessorName(){
  setSerialPrint(pVT_Net,"Processor    = ",true);
  if ( user.esp == 0x8266) setSerialPrint(pVT_Net,"ESP8266");
  if ( user.esp == 0x32)   setSerialPrint(pVT_Net,"ESP32");
  if ( user.esp == 0x32F)  setSerialPrint(pVT_Net,"STM32");
  if ( user.esp == 0x2040) setSerialPrint(pVT_Net,"RP2040");
  if ( user.esp == 0x0000) setSerialPrint(pVT_Net,"Generic");
};//printProcessorName

//------------------------------------------------------------------------------
// Get pin name
int8_t getPinName(int8_t pin){
  // For ESP32 and RP2040 pin labels on boards use the GPIO number
  if (user.esp == 0x32 || user.esp == 0x2040) return pin;

  if (user.esp == 0x8266) {
    // For ESP8266 the pin labels are not the same as the GPIO number
    // These are for the NodeMCU pin definitions:
    //        GPIO       Dxx
    if (pin == 16) return 0;
    if (pin ==  5) return 1;
    if (pin ==  4) return 2;
    if (pin ==  0) return 3;
    if (pin ==  2) return 4;
    if (pin == 14) return 5;
    if (pin == 12) return 6;
    if (pin == 13) return 7;
    if (pin == 15) return 8;
    if (pin ==  3) return 9;
    if (pin ==  1) return 10;
    if (pin ==  9) return 11;
    if (pin == 10) return 12;
  }

  if (user.esp == 0x32F) return pin;

  return -1; // Invalid pin
};//getPinName



//==============================================================================
void getUserSetup() {
pVT_Net->tft.getSetup(user);
//
 if (!pVT_Net->serialOut) return;
Serial.print("\n[code]\n");

Serial.print ("TFT_eSPI ver = "); Serial.println(user.version);
printProcessorName();
#if defined (ESP32) || defined (ESP8266)
  if (user.esp < 0x32F000 || user.esp > 0x32FFFF) { Serial.print("Frequency    = "); Serial.print(ESP.getCpuFreqMHz());Serial.println("MHz"); }
#endif
#ifdef ESP8266
  Serial.print("Voltage      = "); Serial.print(ESP.getVcc() / 918.0); Serial.println("V"); // 918 empirically determined
#endif
Serial.print("Transactions = "); Serial.println((user.trans  ==  1) ? "Yes" : "No");
Serial.print("Interface    = "); Serial.println((user.serial ==  1) ? "SPI" : "Parallel");
#ifdef ESP8266
if (user.serial ==  1){ Serial.print("SPI overlap  = "); Serial.println((user.overlap == 1) ? "Yes\n" : "No\n"); }
#endif
if (user.tft_driver != 0xE9D) // For ePaper displays the size is defined in the sketch
{
  Serial.print("Display driver = "); Serial.println(user.tft_driver, HEX); // Hexadecimal code
  Serial.print("Display width  = "); Serial.println(user.tft_width);  // Rotation 0 width and height
  Serial.print("Display height = "); Serial.println(user.tft_height);
  Serial.println();
}
else if (user.tft_driver == 0xE9D) Serial.println("Display driver = ePaper\n");

if (user.r0_x_offset  != 0)  { Serial.print("R0 x offset = "); Serial.println(user.r0_x_offset); } // Offsets, not all used yet
if (user.r0_y_offset  != 0)  { Serial.print("R0 y offset = "); Serial.println(user.r0_y_offset); }
if (user.r1_x_offset  != 0)  { Serial.print("R1 x offset = "); Serial.println(user.r1_x_offset); }
if (user.r1_y_offset  != 0)  { Serial.print("R1 y offset = "); Serial.println(user.r1_y_offset); }
if (user.r2_x_offset  != 0)  { Serial.print("R2 x offset = "); Serial.println(user.r2_x_offset); }
if (user.r2_y_offset  != 0)  { Serial.print("R2 y offset = "); Serial.println(user.r2_y_offset); }
if (user.r3_x_offset  != 0)  { Serial.print("R3 x offset = "); Serial.println(user.r3_x_offset); }
if (user.r3_y_offset  != 0)  { Serial.print("R3 y offset = "); Serial.println(user.r3_y_offset); }

if (user.pin_tft_mosi != -1) { Serial.print("MOSI    = "); Serial.print("GPIO "); Serial.println(getPinName(user.pin_tft_mosi)); }
if (user.pin_tft_miso != -1) { Serial.print("MISO    = "); Serial.print("GPIO "); Serial.println(getPinName(user.pin_tft_miso)); }
if (user.pin_tft_clk  != -1) { Serial.print("SCK     = "); Serial.print("GPIO "); Serial.println(getPinName(user.pin_tft_clk)); }

#ifdef ESP8266
if (user.overlap == true)
{
  Serial.println("Overlap selected, following pins MUST be used:");

                             Serial.println("MOSI     = SD1 (GPIO 8)");
                             Serial.println("MISO     = SD0 (GPIO 7)");
                             Serial.println("SCK      = CLK (GPIO 6)");
                             Serial.println("TFT_CS   = D3  (GPIO 0)\n");

  Serial.println("TFT_DC and TFT_RST pins can be user defined");
}
#endif
String pinNameRef = "GPIO ";
#ifdef ESP8266
  pinNameRef = "PIN_D";
#endif

if (user.esp == 0x32F) {
  Serial.println("\n>>>>> Note: STM32 pin references above D15 may not reflect board markings <<<<<");
  pinNameRef = "D";
}
if (user.pin_tft_cs != -1) { Serial.print("TFT_CS   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_cs)); }
if (user.pin_tft_dc != -1) { Serial.print("TFT_DC   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_dc)); }
if (user.pin_tft_rst!= -1) { Serial.print("TFT_RST  = " + pinNameRef); Serial.println(getPinName(user.pin_tft_rst)); }

if (user.pin_tch_cs != -1) { Serial.print("TOUCH_CS = " + pinNameRef); Serial.println(getPinName(user.pin_tch_cs)); }

if (user.pin_tft_wr != -1) { Serial.print("TFT_WR   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_wr)); }
if (user.pin_tft_rd != -1) { Serial.print("TFT_RD   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_rd)); }

if (user.pin_tft_d0 != -1) { Serial.print("\nTFT_D0 = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d0)); }
if (user.pin_tft_d1 != -1) { Serial.print("TFT_D1   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d1)); }
if (user.pin_tft_d2 != -1) { Serial.print("TFT_D2   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d2)); }
if (user.pin_tft_d3 != -1) { Serial.print("TFT_D3   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d3)); }
if (user.pin_tft_d4 != -1) { Serial.print("TFT_D4   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d4)); }
if (user.pin_tft_d5 != -1) { Serial.print("TFT_D5   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d5)); }
if (user.pin_tft_d6 != -1) { Serial.print("TFT_D6   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d6)); }
if (user.pin_tft_d7 != -1) { Serial.print("TFT_D7   = " + pinNameRef); Serial.println(getPinName(user.pin_tft_d7)); }

#if defined (TFT_BL)
  Serial.print("\nTFT_BL           = " + pinNameRef); 
  //Serial.println(getPinName(user.pin_tft_led));
  #if defined (TFT_BACKLIGHT_ON)
    Serial.print("TFT_BACKLIGHT_ON = "); Serial.println(user.pin_tft_led_on == HIGH ? "HIGH" : "LOW");
  #endif
#endif

Serial.println();

uint16_t fonts = Set_fontsLoaded(pVT_Net);
if (fonts & (1 << 1))        Serial.print("Font GLCD   loaded\n");
if (fonts & (1 << 2))        Serial.print("Font 2      loaded\n");
if (fonts & (1 << 4))        Serial.print("Font 4      loaded\n");
if (fonts & (1 << 6))        Serial.print("Font 6      loaded\n");
if (fonts & (1 << 7))        Serial.print("Font 7      loaded\n");
if (fonts & (1 << 9))        Serial.print("Font 8N     loaded\n");
else
if (fonts & (1 << 8))        Serial.print("Font 8      loaded\n");
if (fonts & (1 << 15))       Serial.print("Smooth font enabled\n");
Serial.print("\n");

if (user.serial==1)        { Serial.print("Display SPI frequency = "); Serial.println(user.tft_spi_freq/10.0); }
if (user.pin_tch_cs != -1) { Serial.print("Touch SPI frequency   = "); Serial.println(user.tch_spi_freq/10.0); }

Serial.println("[/code]");
Serial.println(delm1);
};//getUserSetup



//------------------------------------------------------------------------------
// List files ESP32 SPIFFS memory or SD
//------------------------------------------------------------------------------
void listDir(fs::FS &fs, const char * dirname, uint8_t levels,String str="") {
uint16_t lCount=0;
String fileStr="",dirStr=String(dirname),fileSize="";
 if ((pVT_Net->sp_available) || (pVT_Net->sd_available)) {
  if (dirStr.indexOf('/')==0) {
   //TODO 
  }else {
   dirStr='/' + dirStr; 
  }
  //
  setSerialPrint(pVT_Net,str + "Listing " + str + " directory:" + dirStr);
  //
  fs::File root = fs.open(dirStr.c_str());
  //
  if (!root) {
    setSerialPrint(pVT_Net,"Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    setSerialPrint(pVT_Net,"Not a directory");
    return;
  }
  //pVT_Net->fs_SP=root;
  
  fs::File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      setSerialPrint(pVT_Net,delm0);
      setSerialPrint(pVT_Net,"DIR : ",true);
      fileStr = String(file.name());
      setSerialPrint(pVT_Net,fileStr);
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      lCount++;
      fileStr = String(file.name());
      setSerialPrint(pVT_Net,"  " + fileStr,true);
      int spaces = 32 - fileStr.length(); // Tabulate nicely
        if (spaces < 1) spaces = 1;
        while (spaces--) setSerialPrint(pVT_Net," ",true);
      fileSize = String(file.size());
      spaces = 8 - fileSize.length(); // Tabulate nicely
        if (spaces < 1) spaces = 1;
        while (spaces--) setSerialPrint(pVT_Net," ",true);
      setSerialPrint(pVT_Net,fileSize + " bytes [" + String(lCount) + "]",true);
      //
      time_t t= file.getLastWrite();
      pVT_Net->tmstruct = *localtime(&t);
      printDateTimeStructure("LastWrite",pVT_Net);
    }
    file = root.openNextFile();
  }//while
 } 
};//listDir


//--------------------------------------------------------
String getSdFat_Info() {
String str="";
uint64_t cardSize0=0,cardSize=0; 
 if (pVT_Net->sd_available) {
  uint8_t cardType = SD.cardType();
   if(cardType == CARD_NONE){
    str="No SD card attached";
    setSerialPrint(pVT_Net,str); 
    return str;
   }
  str="SdFat_Info SD Card Type: ";
   //
   if(cardType == CARD_MMC){
    str+="MMC";
   } else if(cardType == CARD_SD){
    str+="SDSC";
   } else if(cardType == CARD_SDHC){
    str+="SDHC";
   } else {
    str+="UNKNOWN";
   }
  setSerialPrint(pVT_Net,str); 
  str="";
  uint64_t cardSize=0; 
  cardSize0 = SD.cardSize() / (1024 * 1024);
  printf("SD Card Size: %lluMB\n", cardSize0,str);
  //
  cardSize0 = SD.totalBytes();
  printf("SD total bytes: %lluMB\n", cardSize0/(1024 * 1024),str);
  cardSize = SD.usedBytes();
  printf("SD used bytes: %lluMB\n", cardSize/(1024 * 1024),str);
  cardSize=cardSize0-cardSize;
  printf("SD free bytes: %lluMB\n", cardSize/(1024 * 1024),str);
  //
  setSerialPrint(pVT_Net,str); 
  setSerialPrint(pVT_Net,delm1);
 }
 return str; 
 //
};//getSdFat_Info
 

//------------------------------------------------------------------------------
String getSPIFFS_Info() {
String ssOut="SPIFFS_Info:";
float totalB,usedB;
 //
  if (pVT_Net->sp_available) {
   totalB=SPIFFS.totalBytes()/1024.0/1024.0;
   usedB=SPIFFS.usedBytes()/1024.0/1024.0;
   //
   ssOut += "\ntotalBytes:"    + String(totalB,3) + "MB";
   ssOut += "\nusedBytes:"     + String(usedB,3)  + "MB";
   ssOut += "\nfreeBytes:"     + String(totalB - usedB,3)  + "MB";
  } else {
   ssOut+="not available"; 
  }
 //
 ssOut=delm0 + "\n" + ssOut + "\n" + delm0;
 setSerialPrint(pVT_Net,ssOut);
 return ssOut; 
};//getSPIFFS_Info  


//------------------------------------------------------------------------------
// Modus List files ESP32 SPIFFS memory or SD
void getSP_SD_List(String str){
uint8_t j=str.toInt();
int8_t k=-1; 
   if ((SD_Mode) && (!pVT_Net->sd_available)) {
    Serial.println("SD_MODE not available");
    return;
   }
   if ((!SD_Mode) && (!pVT_Net->sp_available)) {
    Serial.println("SPIFFS_MODE not available");
    return;
   }
   //
   if (SD_Mode) getSdFat_Info(); else getSPIFFS_Info();
   //
   if (str.indexOf("/")==0){
     k=str.lastIndexOf("/");
       if (k>0) {
         str.remove(k,1);
          if (SD_Mode) listDir(*pVT_Net->fs_SD, str.c_str(), 1,"SD:"); else listDir(*pVT_Net->fs_SP, str.c_str(), 1,"SPIFFS:"); 
       } else {
         #ifdef WEB_MODE
            if (SD_Mode) str=getFileList_SD(str,false); else str=getFileList_SP(str,false);
          Serial.println(str);
         #else
           if (SD_Mode) listDir(*pVT_Net->fs_SD, str.c_str(), 0,"SD:");else listDir(*pVT_Net->fs_SP, str.c_str(), 0,"SPIFFS:");
         #endif
       }//j>0 
   } else {
    if (SD_Mode) listDir(*pVT_Net->fs_SD, "/", j,"SD:"); else listDir(*pVT_Net->fs_SP, "/", j,"SPIFFS:");   
   }
};//getSP_SD_List



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Create Pooldata Objects
//------------------------------------------------------------------------------
uint16_t getPoolObjectCreate(TVT_Net *pVT_Net,uint8_t objType,uint16_t objID,LoopbackStream *pStream) {
boolean valid=(pStream->available());
int16_t objIdx=-1;
TVT_ViewRect VT_ViewRect;
  if (valid){
     //UnitVTObject.cpp
     if (HasInArray(objType,_objTypeSet)) {
       //read from stream
       pVT_Net->VTObjID=objID;pVT_Net->VTObjType=objType;
       valid=runClassObj(pVT_Net,&VT_ViewRect,pStream);
     }//HasInArray
  }//valid 
  //   
 return pVT_Net->objNr;
};//getPoolObjectCreate



//------------------------------------------------------------------------------
boolean VTPoolDataStore(TVT_Net *pVT_Net) {
uint16_t listSize=getVTObjectListSize(pVT_Net); 
boolean valid=(listSize>0);
uint8_t objType=0xFF,runType=0;
uint16_t objID=0xFFFF, objNr=0,pSize=pVT_Net->VT_PoolSize; // 16384;
String str="";
  if ((valid) && (pSize>0)){
      pVT_Net->stream_ETP[pVT_Net->listNr].clear(); 
  }//valid    
 return valid; 
};//VTPoolDataStore  



//------------------------------------------------------------------------------
//const uint16_t TFT_Width=320;
//const uint16_t TFT_Height=240;
void VTPoolDataRefresh(TVT_Net *pVT_Net,boolean poolInfo=true,boolean pClear=false) {
uint16_t x=0,y=0,w=pVT_Net->TFT_Width,h=pVT_Net->TFT_Height;
String str="",fntName="";
 getHeapStatus(pVT_Net,1);
 pVT_Net->level=0; pVT_Net->x=x;pVT_Net->y=y;pVT_Net->pVisible=true;
 pVT_Net->w=w; pVT_Net->h=h;
 getStackStatus(pVT_Net);
     for (int i=0;i<15;i++) {
       //for (int j=0;j<4;j++) pVT_Net->fontSet[i][j]=0;
     }
     //
     if (pVT_Net->fontSet[pVT_Net->fntNr][pVT_Net->fntSr]==1) {
      fntName="arial" + String(pVT_Net->fontSet[pVT_Net->fntNr][6]) + "-" + String(pVT_Net->fntSr);
        //Load new font
        if (!Set_loadFont(pVT_Net,fntName)) {
         pVT_Net->fntNr=0; pVT_Net->fntSr=0;
         fntName="arial" + String(pVT_Net->fontSet[pVT_Net->fntNr][6]) + "-" + String(pVT_Net->fntSr);
         Set_loadFont(pVT_Net,fntName);
        }
     }
     //
     if (pVT_Net->ImgMode) {
       if (pVT_Net->ImgMode==1){
         pVT_Net->ImgMode=2;
         pVT_Net->ImgTFT.setColorDepth(8);
         // Create the sprite and clear background
         pVT_Net->ImgTFT.createSprite(pVT_Net->TFT_Width,pVT_Net->TFT_Height);
       } 
     }
     //
   pVT_Net->VTPoolRefresh=true; pVT_Net->VTPoolClear=pClear;  
  //
  getHeapStatus(pVT_Net,2);
  getStackStatus(pVT_Net);
};//VTPoolDataRefresh


//------------------------------------------------------------------------------
void getPoolObjectReset(TVT_Net *pVT_Net,boolean pClear=false) {
uint16_t lCount=getVTObjectListSize(pVT_Net);
 setVTObjectListClear(pVT_Net);
 Serial.println("VTObjectList=" + String(lCount));
 VTPoolDataRefresh(pVT_Net,pClear);
 logMemory();
 //getHeapStatus(pVT_Net,3);
};//getPoolObjectReset


//------------------------------------------------------------------------------
boolean getPoolObjectClear(TVT_Net *pVT_Net) {
uint16_t lCount=getVTObjectListSize(pVT_Net);
boolean valid=false;
   if (lCount>0) {
     getPoolObjectReset(pVT_Net);
     valid=true;
   }
   //
 return valid;
};//getPoolObjectClear


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
boolean Set_loadFontToPSRAM(fs::FS &fs, const char *path, LoopbackStream *pStream){
uint32_t len=0;
uint8_t* buff;
boolean valid=getFileExists(pVT_Net,2,path);
  //  
  if (pStream!=NULL) pStream->clear();
  //  
  if (valid){ 
   fs::File file = fs.open(path,FILE_READ);
    if ((file) && (pStream!=NULL)) {
      Serial.print(path); Serial.print(" FileSize="); Serial.print(file.size());
        if (pVT_Net->PSRam) {
          pStream->setNewBufferSize(file.size(),pVT_Net->PSRam);
          buff=pStream->getBuffer();
          len=file.available();
          len=file.readBytes((char*) buff,len); 
          pStream->setSize(len);pStream->setPos(0);
        }
        //     
      Serial.print(" BufferSize="); Serial.print(pStream->getBufferSize());
      Serial.print(" Bufferlen=");  Serial.println(pStream->available());
      file.close();
    }
   //Serial.println(pStream->available()); 
  }  
 return valid; //  (len>0);
};//Set_loadFontToPSRAM



//------------------------------------------------------------------------------
void showFontSet() {
String str="",ss="",fntName="",dName="",aStr="",fntStr="",tStr=".";
boolean valid=true;
int i=0,j=0;
//uint16_t x=pVT_Net->TFT_HEIGHT, h=pVT_Net->TFT_KEYHEIGHT,w=pVT_Net->TFT_KEYWIDTH;
uint16_t x=pVT_Net->TFT_Height,w=pVT_Net->TFT_KeyWidth,h=pVT_Net->TFT_KeyHeight;
uint8_t  kNr=pVT_Net->TFT_KeyNr; 
 setSerialPrint(pVT_Net,"FontSet:"); 
  //max fonts
  //for (i=0;i<15;i++) {
  for (i=0;i<5;i++) {
    ss="arial" + String(i); str=""; aStr="/" + ss + "  \t"; 
    fntStr="";  str+="  \t("; 
      //
      if (i<kNr) {
       Set_fillRect(pVT_Net,x,i*h,w,h,cl_silver);
       Set_drawRect(pVT_Net,x,i*h,w,h,cl_blue);
       fntStr=String(i);
       Set_drawString(pVT_Net,fntStr,x+10,i*h+10);
      }
      //
      for (j=0;j<4;j++) {
        fntName=ss + "-" + String(j); fntStr="";
        dName="/" + fntName +  + ".vlw";
        pVT_Net->fontSet[i][j]=getFileExists(pVT_Net,2,dName.c_str());
          //           
          if (pVT_Net->fontSet[i][j]==1){
           dName="/" + fntName +  + ".vlw"; 
           valid=false;tStr=".";
            //Set font to PSRAM from SPIFFS
            if ((!valid) && (pVT_Net->sp_available)) {
              valid=(Set_loadFontToPSRAM(SPIFFS,dName.c_str(),&pVT_Net->stream_Font[i][j]));tStr=".";
            }
            //Set font to PSRAM from SD
            if ((!valid) && (pVT_Net->sd_available)) {
               //Serial.println(dName);
               valid=(Set_loadFontToPSRAM(SD,dName.c_str(),&pVT_Net->stream_Font[i][j])); tStr=":";
            }
            //
            //
            if (valid){
               if (i<kNr) {
                fntStr+="." + String(j);
                Set_drawString(pVT_Net,fntStr,x+20+12*j,i*h+10);
               }
               //TEST
               //Serial.print(String(i) + "_" + String(j) + " ");
               //Serial.println(pVT_Net->stream_Font[i][j].available());
            } else str+="!"; 
          } else str+="!";
          //
        str+="/" + fntName +  + ".vlw";
         if (j<3) str+=", "; 
      }//for j
      //
    str+=")"; 
      //
      for (j=0;j<7;j++) {
        aStr+=String(pVT_Net->fontSet[i][j]) + ",";
      }
    str=aStr + str;
    //
    setSerialPrint(pVT_Net,str); 
    setSerialPrint(pVT_Net,delm0); 
  } //for i
  //
 logMemory();
 setSerialPrint(pVT_Net,delm1);
};//showFontSet







//==============================================================================
//==============================================================================
