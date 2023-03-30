//==============================================================================
//Definition and Create Server handling
//==============================================================================
//unit UnitVTServer.h;
//==============================================================================
//WEB_MODE
#define WEB_MODE
#define ESP32_MODE
//#define IR_MODE

//==============================================================================
#ifdef WEB_MODE
//==============================================================================
 #include <ArduinoJson.h>
 //DynamicJsonDocument doc(1024);
 DynamicJsonBuffer jsonBuffer;
  //
   #include <WiFi.h>
   #include <WebServer.h>
   #include <ESPmDNS.h>
   
   #include <WebSocketsServer.h>
   WebServer server(80);

 //Websocket
 const int port=81;
 
WebSocketsServer webSocket = WebSocketsServer(port);
 //
 volatile uint8_t websock_num=0;
 int sample_period=50;
 //
 String cmdStr="", aPath="";
 boolean AccessPointMode=true;

 
#include "credentials.cred"
//------------------------------------------------------------------------------
 //Webserver this 3 lines for a fix IP-address
 IPAddress ip_w      (192, 168, 178, 41); 
 IPAddress gateway_w (192, 168, 178, 1); //FRITZ-BOX-7590
 IPAddress subnet_w  (255, 255, 255, 0);
 char* ssid_w     = MYSSID_W;
 char* password_w = MYPASSWORD_W;
 char* host_w     = MYHOSTNAME_W;
 boolean found_w  = false;
 //
 //AccessPoint this 3 lines for a fix default IP-address
 IPAddress ip_a      (192, 168, 4, 1);
 IPAddress gateway_a (192, 168, 4, 1);
 IPAddress subnet_a  (255, 255, 255, 0);
 char* ssid_a     = "M5StackVT41";
 //
 char* password_a = "";
 //char* password_a = MYPASSWORD_A;
 //
 char* host_a     = MYSSID_A;
 boolean found_a  = false;
 //boolean SD_Mode=true;
 //
 String entryName="";
 String actualPath="";
 String list_irStr="";

 //
 File fsUploadFile_SD;
 File fsUploadFile_SP;
 File datFile;
 File dirFile; 
 File logFile;
 File entry;

 
 
//------------------------------------------------------------------------------
//ArrayStringList
class MyClass {
private:
  int _arrSize;
  String *_myArr;
public:
  // Constructor
  MyClass(int arrSize) : _arrSize(arrSize)  {   _myArr = new String[arrSize]; }
  // Destructor
  ~MyClass(){ delete []_myArr;}
  void addElement(String *element, int position)  { _myArr[position] = *element;  }
  String& getElement(int position) const {return _myArr[position];}
};



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes) + "B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0) + "KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0) + "MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0) + "GB";
  }
};//formatBytes

//------------------------------------------------------------------------------
void setIR_enable(boolean setMode){
  #ifdef IR_MODE
    #ifdef ESP32_MODE
      if (setMode) irReceiver.enableIRIn(); else irReceiver.disableIRIn();
    #endif //ESP32_MODE
 #endif //IR_MODE
};//setIR_enable


//------------------------------------------------------------------------------
void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}; //returnFail


//------------------------------------------------------------------------------
void returnOK(String msg) {
  server.send(200, "text/plain", msg);
}; //returnOK

//------------------------------------------------------------------------------
String getPath_Mode(String path) {
  if ((path.indexOf("/SD")==0) || (path.indexOf("/SP")==0)) {
    SD_Mode=(path.indexOf("/SD")==0);
    path=path.substring(3);
  }
return path;  
};//getPath_Mode


//------------------------------------------------------------------------------
String getContentType(String filename){
 filename.toLowerCase();
  //
  if (server.hasArg("download"))    return "application/octet-stream";
  //
  if (filename.endsWith(".htm"))  return "text/html";
  if (filename.endsWith(".html")) return "text/html";
  //
  if (filename.endsWith(".txt"))  return "text/plain";
  if (filename.endsWith(".xml"))  return "text/xml";
  if (filename.endsWith(".ino"))  return "text/plain";
  //
  if (filename.endsWith(".css"))  return "text/css";
  if (filename.endsWith(".js"))   return "text/javascript";
  //
  if (filename.endsWith(".png"))  return "image/png";
  if (filename.endsWith(".gif"))  return "image/gif";
  if (filename.endsWith(".jpg"))  return "image/jpeg";
  if (filename.endsWith(".bmp"))  return "image/bmp";
  if (filename.endsWith(".ico"))  return "image/x-icon";
  //
  //C
  if (filename.endsWith(".c"))    return "text/plain";
  if (filename.endsWith(".h"))    return "text/plain";
  if (filename.endsWith(".cpp"))  return "text/plain";
  //Pascal
  if (filename.endsWith(".pas"))  return "text/plain";
  if (filename.endsWith(".dpr"))  return "text/plain";
  if (filename.endsWith(".dfm"))  return "text/plain";
  if (filename.endsWith(".cfg"))  return "text/plain";
  //bin_hex
  if (filename.endsWith(".vlw"))  return "bin/hex";
  if (filename.endsWith(".bin"))  return "bin/hex";
  if (filename.endsWith(".dll"))  return "bin/hex";
  if (filename.endsWith(".exe"))  return "bin/hex";
  if (filename.endsWith(".lnk"))  return "bin/hex";
  if (filename.endsWith(".iop"))  return "bin/hex";
  //application
  if (filename.endsWith(".pdf"))  return "application/x-pdf";
  // <embed src="filename.pdf#page=1&zoom=300" width="575" height="500">
  //
  if (filename.endsWith(".zip"))  return "application/x-zip";
  if (filename.endsWith(".gz"))   return "application/x-gzip";
  //
  if (filename.indexOf(".")<0)     return "add_text/htm";
  //
  return "text/plain";
};//getContentType



//--------------------------------------------------------
String getAccessPointMode(String path){
boolean SP_File=false, SD_File=false;
String ssOut="";
 int j=-1;
 cmdStr=""; aPath="";
 String uriStr=server.uri();
 String argStr="";
  if (server.args()>0) argStr=server.arg(0); 
 //Check AccessPoint or Webserver activ
 String uStr = server.client().remoteIP().toString();
 String str=WiFi.softAPIP().toString();
 String ss=str;
 int i=ss.lastIndexOf(".");
 ss=ss.substring(0,i);
 AccessPointMode=(uStr.indexOf(ss)>=0);
 //
 ssOut += delm0 + "\n";
 ssOut += "server.uri=" + uriStr + "\n";
 ssOut += "server.arg(0)=" + argStr + "\n";
 //
 ssOut += "server.client.remoteIP=" + uStr + "\n";
 ssOut += "softAPIP_server=" + str + "\n";
 ssOut += "softAPIP_client=" + ss + "\n";
 ssOut += "AccessPointMode=" + String(AccessPointMode) + "\n";
 ssOut += "0.handleFileRead=" + path + "\n";
 //
 setSerialPrint(pVT_Net,ssOut);
return ssOut;
};//getAccessPointMode


//------------------------------------------------------------------------------
bool handleFileRead(String path) {
boolean isDir=false;
String dataType = "text/plain",str="";
 getAccessPointMode(path); 
 setSerialPrint(pVT_Net,String(server.args()) + " URI->path=" + path);
 path=getPath_Mode(path);
  //
  if ((!VT_Net.sd_available) && (!VT_Net.sp_available)) return false;
  if ((!VT_Net.sd_available) && (SD_Mode))  SD_Mode=false; 
  if ((!VT_Net.sp_available) && (!SD_Mode)) SD_Mode=true; 
  //
 setSerialPrint(pVT_Net,"sd_available=" + String(VT_Net.sd_available));
 setSerialPrint(pVT_Net,"sp_available=" + String(VT_Net.sp_available));
 setSerialPrint(pVT_Net,"SD_Mode=" + String(SD_Mode));
 //
 dataType=getContentType(path);
  //
    if ((VT_Net.sd_available) && (SD_Mode)) {
     setSerialPrint(pVT_Net,"SD_INPUT->path=" + path);
      //
      if (SD.exists(path.c_str())) {
         setSerialPrint(pVT_Net,"SD_EXISTS->path=" + path);
         fsUploadFile_SD=SD.open(path.c_str()); 
         //
         if (fsUploadFile_SD) {
           if (fsUploadFile_SD.isDirectory()) {
            setSerialPrint(pVT_Net,"SD_DIRECTORY->path=" + path);
            return false; 
           }
          setSerialPrint(pVT_Net,"SD_START->path=" + path);
           if (server.hasArg("download")) dataType = "application/octet-stream";
           //           
           if (server.streamFile(fsUploadFile_SD, dataType) == fsUploadFile_SD.size()) {
            setSerialPrint(pVT_Net,"SD_END->path=" + path);
           } else setSerialPrint(pVT_Net,"SD_END->Sent less data than expected!");
           
          fsUploadFile_SD.close();
          return true;
         }//exists File
       }
    }//SD 
    //
    setSerialPrint(pVT_Net,"SP_INPUT->path=" + path);
    fsUploadFile_SP = SPIFFS.open(path.c_str());
      if (fsUploadFile_SP) isDir=fsUploadFile_SP.isDirectory();      
      if (isDir) return false;
      //
      if (fsUploadFile_SP) {
       setSerialPrint(pVT_Net,"SP_START->path=" + path);
        if (server.hasArg("download")) dataType = "application/octet-stream";
        //
        if (server.streamFile(fsUploadFile_SP, dataType) == fsUploadFile_SP.size()) {
         setSerialPrint(pVT_Net,"SP_END->path=" + path);
        } else setSerialPrint(pVT_Net,"SP_END->Sent less data than expected!");
       fsUploadFile_SP.close();
       return true;
      } 
return false;
};//handleFileRead



//------------------------------------------------------------------------------
void handleNotFound() {
String ss="NOT ", path="", sdStr="SPIFFS "; 
  path=server.uri();
  boolean valid=handleFileRead(path);
   //
   if ((!valid) && (path.lastIndexOf(".")<0)) {
     path += ".htm";
     valid=handleFileRead(path);
      if (!valid) {
       path="/index.htm";
       valid=handleFileRead(path); 
      }
   }
   //
   setSerialPrint(pVT_Net,"SD_mode=" + String(SD_Mode));
   //  
   if (SD_Mode) sdStr="SDCARD ";
   if (valid) ss="";
  //
  String message = "\n" + sdStr + ss  + "Detected\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
   //
   for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
      //
   }//for i
   //     
   if (!valid) server.send(404, "text/plain", message);
   //
   if (server.args()==0) message += "\n";
  setSerialPrint(pVT_Net,message);
};//handleNotFound


//------------------------------------------------------------------------------
void setHandleNotFound() {
 setIR_enable(false);
 SD_Mode=true;
 setSerialPrint(pVT_Net,"setHandleNotFound:SD_Mode=" + String(SD_Mode));
 handleNotFound();
 setIR_enable(true);
};//setHandleNotFound


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int list_get_indexOf(String str, String iStr) {
int i=0,j=-1;
String ss="";
boolean found=false;
   //            
   while (str.length()>0) { 
     j=str.indexOf("\n");
      if (j<0) {
       ss=str; str="";
      }
      else {    
       ss=str.substring(0,j);
       str.remove(0,j+1);
      }   
      //
     found=(ss.indexOf(iStr)>=0);
      if (found) break;
     i++;
    } //while
    //
    if (!found) i=-1;
    //
return i;  
};//list_get_indexOf



//------------------------------------------------------------------------------
String list_setString(String str){
 str.replace("\r","");
 return str; 
};//list_setString


//------------------------------------------------------------------------------
String list_get_item(String str,int index) {
int i=0,j=-1;
String ss="";
boolean found=false;
   //            
   while (str.length()>0) { 
     j=str.indexOf("\n");
      if (j<0) {
       ss=str; str="";
      }
      else {    
       ss=str.substring(0,j);
       str.remove(0,j+1);
      }   
      //
     found=(i==index);
      if (found) break;
     i++;
    } //while
    //
    if (!found) ss="";
    //
return ss;  
};//list_get_item

//------------------------------------------------------------------------------
int list_get_size(String str) {
int i=0,j=-1;
   //            
   while (str.length()>0) { 
     j=str.indexOf("\n");
      if (j<0) str=""; else str.remove(0,j+1);
     i++;
    } //while
    //
return i;  
};//list_get_size  


//------------------------------------------------------------------------------
//Bubble sort
String list_sortList(String str, int sortMode){
boolean setSort=false, setSortStart=false; 
String itemStr0, itemStr1,itemStrL0, itemStrL1;
int k=-1, j=0;
//
int dsize=list_get_size(str);
//
MyClass myObject(dsize);
  for (int i=0; i<dsize;i++) {
    String ss=list_get_item(str,i);
    //setSerialPrint(String(i) + ".TEST=" + ss);
    myObject.addElement(&ss, i);  j++;
    //setSerialPrint(pVT_Net,String(myObject.getElement(i)));
  }
  //
 //setSortStart=((j==dsize) && (dsize>1));
 setSortStart=(dsize>1);
  //
  if (setSortStart) {
   for(int j=1; j<dsize; j++){
    //
    for(int i=1; i<dsize; i++){
     itemStr1=myObject.getElement(i);  itemStrL1=itemStr1;
     itemStr0=myObject.getElement(i-1);itemStrL0=itemStr0;
     
      //
      if (sortMode%10==1) {
       itemStrL1.toLowerCase();
       itemStrL0.toLowerCase();
      } 
     setSort=(itemStrL1<itemStrL0);
      //
      if (setSort) {
         myObject.addElement(&itemStr0, i);
        myObject.addElement(&itemStr1, i-1);
      }  
    } //for i
   } //for j
  } //setSortStart
   //  
  str="";
  //
   for(int i=0; i<dsize; i++){
     if (str!="") str += "\n";
    str +=myObject.getElement(i);
   }  
   // 
   if (sortMode==11) {
    //k=str.lastIndexOf("\n"); 
    // if (k>0) str.remove(k,1);
    k=str.lastIndexOf(","); 
     if (k>0) str.remove(k,1);
   }  
  //
 return str;
};//list_sortList



//------------------------------------------------------------------------------
String setReplaceJSON(String str) {
 str.replace("[",""); str.replace("]","");
 str.replace("{",""); str.replace("}","");
 str.replace("\"","");
 str.replace(","," ");
 return str;
}; //setReplaceJSON

//------------------------------------------------------------------------------
String getJSON_format(String ss) {
String json="";  
  if (ss.indexOf("[{")<0){ 
      //File list
      String str=list_setString(ss);
      ss=list_get_item(str,0);
      setSerialPrint(pVT_Net,ss);
      //
      json += "[{";
        for(int i=0; i<list_get_size(str); i++){
           if (i>0) json += ", \n";
         json += "\"line" + String(i) + "\":"  + "\"" + list_get_item(str,i) + "\"";
        }    
      json += "}]";
    } else json += ss;
     //
return json;
};//getJSON_format



//------------------------------------------------------------------------------
String getFileList_SD(String path, boolean response=true) {
boolean isDir=false;
String ssOut="" , str="", ss="";
 setIR_enable(false);
 SPIFFS.begin();
  setSerialPrint(pVT_Net,"SD:PATH=" + path);  
       fsUploadFile_SD=SD.open(path.c_str());
        if (!fsUploadFile_SD) return ssOut;
       isDir=fsUploadFile_SD.isDirectory();
         //
         if (!isDir) {
          fsUploadFile_SD.close();
          returnFail("NOT DIR");
          return ssOut;
         }
       fsUploadFile_SD.rewindDirectory();
         //
        if (response) {
         server.setContentLength(CONTENT_LENGTH_UNKNOWN);
         server.send(200, "text/json", "");
         WiFiClient client = server.client();
        }
     //   
     for (int cnt = 0; true; ++cnt) {
      entry = fsUploadFile_SD.openNextFile();
        if (!entry) break;
      entryName=entry.name();
      isDir=entry.isDirectory();
      ss=entryName;
        while (ss.indexOf("/")>=0) ss.remove(0,ss.indexOf("/")+1);
      //
      str="";
      str += "{\"type\":\"";
      str += (isDir) ? "dir" : "file";
      str += "\",\"name\":\"";
      str += ss;
      str += "\"";
        //ceck double folder name
        if (ssOut.indexOf(str)<0) { 
         //get file size
         str +=",\"size\":\"";
         ss=formatBytes(entry.size());
         str +=ss +"\"";
         //
         str += "},\n";
         ssOut +=str;
        }   
      
      entry.close();
     } //for i
     //
     if (fsUploadFile_SD) fsUploadFile_SD.close();
     //
     //sort ssOut
     if (ssOut.length()>0) ssOut=list_sortList(ssOut,11);
     //
  ssOut = "[" + ssOut + "]"; 
  ssOut.replace(",\n]","]\n");
  //  
  setIR_enable(true);
return ssOut;
};//getFileList_SD



//------------------------------------------------------------------------------
boolean getSPIFFS_IsDir(String path) {
boolean isDir=false;
  if (entryName=="/") {
   isDir=true;  
  }
  else {
     if (entryName.indexOf(path)==0) entryName.remove(0,path.length());
     if (entryName.indexOf("/")==0)  entryName.remove(0,1);
   int i=entryName.indexOf("/");
     //
     if (i>0) {
      isDir=true;
      entryName=entryName.substring(0,i);
     }
  }
 return isDir;       
};//getSPIFFS_IsDir        


//------------------------------------------------------------------------------
String getFileList_SP(String path, boolean response=true) {
boolean isDir=false;
String ssOut="" , str="", ss="";
 setIR_enable(false);
 SPIFFS.begin();
  setSerialPrint(pVT_Net,"SP:PATH=" + path);  
       fsUploadFile_SP=SPIFFS.open(path.c_str());
        if (!fsUploadFile_SP) return ssOut;
       isDir=fsUploadFile_SP.isDirectory();
         //
         if (!isDir) {
          fsUploadFile_SP.close();
          returnFail("NOT DIR");
          return ssOut;
         }
       fsUploadFile_SP.rewindDirectory();
         //
        if (response) {
         server.setContentLength(CONTENT_LENGTH_UNKNOWN);
         server.send(200, "text/json", "");
         WiFiClient client = server.client();
        }
     //   
     for (int cnt = 0; true; ++cnt) {
      entry = fsUploadFile_SP.openNextFile();
        if (!entry) break;
      entryName=entry.name();
      isDir=getSPIFFS_IsDir(path);  //entry.isDirectory();
      ss=entryName;
        while (ss.indexOf("/")==0) ss.remove(0,1);
      //
      str="";
      str += "{\"type\":\"";
      str += (isDir) ? "dir" : "file";
      str += "\",\"name\":\"";
      str += ss;
      str += "\"";
        //ceck double folder name
        if (ssOut.indexOf(str)<0) { 
         //get file size
         str +=",\"size\":\"";
         ss=formatBytes(entry.size());
         str +=ss + "\"";
         //
         str += "},\n";
         ssOut +=str;
        }   
        //
      entry.close();
     } //for i
     //
     if (fsUploadFile_SP) fsUploadFile_SP.close();
     //
     //sort ssOut
     if (ssOut.length()>0) ssOut=list_sortList(ssOut,11);
     //
  ssOut = "[" + ssOut + "]"; 
  ssOut.replace(",\n]","]\n");
  //  
  setIR_enable(true);
return ssOut;
};//getFileList_SP

//------------------------------------------------------------------------------
String setStringLengthHex(String str, byte len) {
 while (str.length()<len) str='0' + str;
return str;
} //setStringLengthHex  


//------------------------------------------------------------------------------
String getMacAddress(boolean aPoint) {
String ss="";
String str="";
byte mac[6]={0,0,0,0,0,0};
  if (aPoint) { 
   ss +="AP-MAC_address=";
   WiFi.softAPmacAddress(mac);
  }
  else {
   ss +="WS-MAC_address=";   
   WiFi.macAddress(mac);
  }
  for (int i=0; i<6; i++) {
     str=String(mac[i],HEX);
     str=setStringLengthHex(str,2);
     ss += str;
      if (i<5) ss += ":";
  }//for i
// 
return ss;  
};//getMacAddress



//------------------------------------------------------------------------------
String getConnectParameter(boolean setPrint) {
String ss="";
  ss +=delm1 + "\n"; 
  ss += "AccessPoint IP address:";
  ss += WiFi.softAPIP().toString();
  ss += "\n";
  ss += getMacAddress(true);
  ss += "\n";
  //
  ss += "1.Open http://";
  ss += String(host_a);
  ss += ".local/edit to see the file browser\n";
  ss += "2.Open http://";
  ss += String(host_a);
  ss += ".local/index to see the index page\n";
  ss += "3.Open http://";
  ss += String(host_a);
  ss += ".local/pwm to see the pwm page\n";
  ss += "4.Open http://";
  ss += String(host_a);
  ss += ".local/value to see the value page\n";
  ss +=delm1 + "\n"; 
  //
  ss +=delm1 + "\n"; 
  ss += "Webserver IP address:";
  ss += WiFi.localIP().toString();
  ss += "\n";
  //
  ss += getMacAddress(false);
  ss += "\n";
  //
  ss += "1.Open http://";
  ss += String(host_w);
  ss += ".local/edit to see the file browser\n";
  ss += "2.Open http://";
  ss += String(host_w);
  ss += ".local/index to see the index page\n";
  ss += "3.Open http://";
  ss += String(host_w);
  ss += ".local/pwm to see the pwm page\n";
  ss += "4.Open http://";
  ss += String(host_w);
  ss += ".local/value to see the value page\n";
  ss +=delm1 + "\n"; 
  ss.replace("\n","\n\r");
//
//
 if (setPrint) setSerialPrint(pVT_Net,delm1 + "\n" + ss + "\n" + delm1);
//
return ss;
};//getConnectParameter 




//------------------------------------------------------------------------------
void setHandleDirectory(boolean allMode) {
String ss="";
 setIR_enable(false);
 ss="FILES";
    if (allMode) ss="DIRECTORY";
 setSerialPrint(pVT_Net,delm0 + "\nHTTP_GET=/" + ss + "\n" + delm0);
  // 
 ss="";
  if (allMode) ss += getConnectParameter(false);
 //
 ss += delm1 + "\nSPIFFS\n" + delm1 + "\n";
  if ((VT_Net.sp_available) && (!SD_Mode)) {
    ss += setReplaceJSON(getFileList_SP("/",false));
     if (allMode) ss += getSPIFFS_Info();
  }
 ss += "\n" + delm1 + "\nSD-CARD\n" + delm1 + "\n";
  //
  if ((VT_Net.sd_available) && (SD_Mode)) {
    ss += setReplaceJSON(getFileList_SD("/",false));
     if (allMode) ss += getSdFat_Info();
  }
 ss += "\n" + delm1 + "\n";
 //
 //setSerialPrint(ss);
 ss = getJSON_format(ss); //with Print
 setSerialPrint(pVT_Net,ss);
 //
 server.send(200, "text/json", ss);
 ss = String();
 setIR_enable(true);
};//setHandleDirectory


//------------------------------------------------------------------------------
//list?dir=/  or files?dir=/
//------------------------------------------------------------------------------
void handleFileList() {
String ssOut="", path ="", SD_modeStr="SP:"; 
size_t fileSize=0;
 setIR_enable(false);
  if (!server.hasArg("dir")) return returnFail("BAD ARGS");
 path = server.arg("dir");
 setSerialPrint(pVT_Net,"dir_arg=" + path);
 path=getPath_Mode(path); 
  if (SD_Mode) SD_modeStr="SD:"; 
  // 
 setSerialPrint(pVT_Net,SD_modeStr + "path=" + path);
  if ((VT_Net.sd_available) || (VT_Net.sp_available)) {
    if ((!VT_Net.sd_available) && (SD_Mode))  SD_Mode=false;
    if ((!VT_Net.sp_available) && (!SD_Mode)) SD_Mode=true; 
    //
    if ((VT_Net.sd_available) && (SD_Mode))  ssOut = getFileList_SD(path);
    //
    if ((VT_Net.sp_available) && (!SD_Mode)) ssOut = getFileList_SP(path);
  }
  //
  if (ssOut.length()>0) {
     if (SD_Mode) SD_modeStr="#SD#"; else SD_modeStr="#SP#"; 
   ssOut=SD_modeStr + ssOut;  
  }
  //
 setSerialPrint(pVT_Net,ssOut);
 server.sendContent(ssOut);
 setIR_enable(true);
};//handleFileList




//------------------------------------------------------------------------------
void setHandleFileList() {
 setIR_enable(false);
 handleFileList();
 setIR_enable(true);
};//setHandleFileList

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void handleFileRename() {
boolean isDir=false, setRename=false;
String dirStr="FILE_";
  if (server.args() < 2) return returnFail("BAD ARGS");
 //setSerialPrint(pVT_Net,String(server.args()));
 String path_src = server.arg(0);
 path_src=getPath_Mode(path_src);
  //
  if (server.hasArg("dir")) {
   isDir=true; dirStr="PATH_";
  }
 setSerialPrint(pVT_Net,dirStr + "SRC=" + server.arg(0));
 String path_dst = server.arg(1);
 path_dst.replace("Actual_File=","");
 int i=path_dst.indexOf(" [");
   if (i>0) path_dst=path_dst.substring(0,i);
 setSerialPrint(pVT_Net,dirStr + "SRC=" + path_src);
 setSerialPrint(pVT_Net,dirStr + "DST=" + path_dst);
 //
 path_src=getPath_Mode(path_src);
  if ((path_src == "/") || (path_src == "")) return returnFail(dirStr + "BAD=" + path_src);
  if ((path_dst == "/") || (path_dst == "")) return returnFail(dirStr + "BAD=" + path_dst);
 //
 //Check contentType
 String co_src=getContentType(path_src);
 String co_dst=getContentType(path_dst);
  //
  if ((!setRename) && (co_src.indexOf("text/")==0) && (co_dst.indexOf("text/")==0)) {
    setSerialPrint(pVT_Net,dirStr + "SRC=" + co_src);
    setSerialPrint(pVT_Net,dirStr + "DST=" + co_dst);
    setRename=true;
  }
  if ((!setRename) && (co_src==co_dst)) {
    setSerialPrint(pVT_Net,dirStr + "SRC=" + co_src);
    setSerialPrint(pVT_Net,dirStr + "DST=" + co_dst);
    setRename=true;
  }
  //Set path of path_dst
  if (setRename){
    String path = path_src.substring(0, path_src.lastIndexOf('/'));
    path_dst=path + path_dst;
    setSerialPrint(pVT_Net,dirStr + "DEFAULT_RENAME=" + path_src + " to " + path_dst);
  }
  //  
  if ((VT_Net.sd_available) && (SD_Mode)) {
      if (!SD.exists(path_src.c_str()))                  return returnFail("SD_BAD_PATH=" + path_src);
      if (!SD.rename(path_src.c_str(),path_dst.c_str())) return returnFail("RENAME_BAD_PATH=" + path_src);   
  }
  //  
  if ((VT_Net.sp_available) && (!SD_Mode)) {
     if (!SPIFFS.exists(path_src.c_str())) return returnFail("SP_BAD_PATH=" + path_src);
     if (SPIFFS.rename(path_src.c_str(),path_dst.c_str())) {
        setSerialPrint(pVT_Net,"File renamed");
     } else {
        setSerialPrint(pVT_Net,"Rename failed");
        return returnFail("RENAME_FAILED_PATH=" + path_src);
     }
  }
  //
 returnOK("");
};//handleFileRename


//------------------------------------------------------------------------------
void setHandleFileRename(){
 setIR_enable(false);
 handleFileRename();
 setIR_enable(true);
};//setHandleFileRename


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void deleteRecursive(String path) {
String ss="";
boolean isDir=false, valid_SD=false, valid_SP=false;
    if  ((VT_Net.sd_available) && (SD_Mode)) {
     fsUploadFile_SD=SD.open(path.c_str());
     valid_SD=(fsUploadFile_SD);
    }
   //
   if  ((VT_Net.sp_available) && (!SD_Mode)) {
    fsUploadFile_SP = SPIFFS.open(path.c_str());
    valid_SP=(fsUploadFile_SP);
   }
   //  
   if ((!valid_SD) && (!valid_SP)) return;
   //
   if (valid_SD) isDir=fsUploadFile_SD.isDirectory();
   //
   if (valid_SP) {
    isDir=fsUploadFile_SP.isDirectory();
   }
   //
   if (!isDir) {
     //
       if ((valid_SD) && (fsUploadFile_SD))  fsUploadFile_SD.close();
       if  ((VT_Net.sd_available) && (SD_Mode))  SD.remove(path.c_str());
      setSerialPrint(pVT_Net,"SD_RemoveFile=" + path);
     //
       if ((valid_SP) && (fsUploadFile_SP)) fsUploadFile_SP.close();
       if  ((VT_Net.sp_available) && (!SD_Mode)) SPIFFS.remove(path.c_str());
      setSerialPrint(pVT_Net,"SP_RemoveFile=" + path);
    return;
   }
   //Remove directory recursivly
   if  ((VT_Net.sd_available) && (SD_Mode))  {
      datFile=SD.open(path.c_str());
        if (datFile){
         datFile.close();
         SD.rmdir(path.c_str());
         setSerialPrint(pVT_Net,"SD_RemoveDir=" + path);
        }
   }
   //
   if  ((VT_Net.sp_available) && (!SD_Mode)) {
       SPIFFS.rmdir(path.c_str());
       setSerialPrint(pVT_Net,"SP_RemoveDir=" + path);
   }
   //
   if (fsUploadFile_SD) fsUploadFile_SD.close();
  
   if (fsUploadFile_SP) fsUploadFile_SP.close();
  //
};//deleteRecursive

//------------------------------------------------------------------------------
void handleFileDelete() {
  if (server.args() == 0) return returnFail("BAD ARGS");
 String path = server.arg(0), str="";
 setSerialPrint(pVT_Net,"PATH_MODE=" + path);
 path=getPath_Mode(path);
 setSerialPrint(pVT_Net,"DELETE PATH=" + path);
  //   
  if (path == "/") str="BAD_PATH=" + path;
  //  
  if (str.length()==0) {
    if ((VT_Net.sd_available) && (SD_Mode)) {
     if (!SD.exists(path.c_str())) str="SD_BAD_PATH=" + path;
    }
   //  
    if ((VT_Net.sp_available) && (!SD_Mode)) {
      if (!SPIFFS.exists(path.c_str())) str="SP_BAD_PATH=" + path;
    }
  }
  //
  if (str.length()>0) {
   setSerialPrint(pVT_Net,str);
   return returnFail(str); 
  }
  //
 setSerialPrint(pVT_Net,"RECURSIVE DELETE PATH=" + path);
 deleteRecursive(path);
 returnOK("");
};//handleFileDelete


//------------------------------------------------------------------------------
void setHandleFileDelete(){
 setIR_enable(false);
 handleFileDelete();
 setIR_enable(true);
};//setHandleFileDelete


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void handleFileCreate() {
 String path="", str="";
  //
  if (server.args()>0) {
    path = server.arg(0);
    path=getPath_Mode(path); 
     //check exists path or file
       if ((VT_Net.sd_available) && (SD_Mode) && (SD.exists(path.c_str())))   str="BAD PATH";
       //
      if ((VT_Net.sp_available) && (!SD_Mode) && (SPIFFS.exists(path.c_str()))) str="BAD PATH";
  } else str="BAD ARGS"; 
  //
  if (str.length()>0) {
    setSerialPrint(pVT_Net,"handleFileCreate=" + str + "\n" + delm0);
    return returnFail(str);
  }
  //
 setSerialPrint(pVT_Net,"START_FileCreate=" + path + "\n" + delm0);
  //
  if (path.indexOf('.') > 0) {
      if ((VT_Net.sd_available) && (SD_Mode))  {
        setSerialPrint(pVT_Net,"SD_FileCreate=" + path + "\n" + delm0);
        fsUploadFile_SD=SD.open(path.c_str(),FILE_WRITE);
          if (fsUploadFile_SD) {
            fsUploadFile_SD.close();
            setSerialPrint(pVT_Net,"OK_SD_FileCreate=" + path + "\n" + delm0);
          }
      }
      // 
      if ((VT_Net.sp_available) && (!SD_Mode)) {
       setSerialPrint(pVT_Net,"SP_FileCreate=" + path + "\n" + delm0);
       fsUploadFile_SP = SPIFFS.open(path.c_str(), FILE_WRITE);
          if (fsUploadFile_SP) {
            fsUploadFile_SP.close();
            setSerialPrint(pVT_Net,"OK_SP_FileCreate=" + path + "\n" + delm0);
          }
      }
      if (fsUploadFile_SP) fsUploadFile_SP.close();
  } else {
      if ((VT_Net.sd_available) && (SD_Mode))  {
        setSerialPrint(pVT_Net,"DirCreateSD=" + path + "\n" + delm0);
        SD.mkdir(path.c_str());
      }
      //
      if ((VT_Net.sp_available) && (!SD_Mode)) {
         setSerialPrint(pVT_Net,"DirCreateSP=" + path + "\n" + delm0);
          //SPIFFS.mkdir(path.c_str());
         path=path + "/test.txt";
         fsUploadFile_SP = SPIFFS.open(path.c_str(), "w");
         fsUploadFile_SP.close();
      }
  }
  //
 returnOK("");
};//handleFileCreate


//------------------------------------------------------------------------------
void setHandleFileCreate(){
 setIR_enable(false);
 handleFileCreate();
 setIR_enable(true);
};//setHandleFileCreate

//--------------------------------------------------------
//--------------------------------------------------------
void handleSetCommand(){
  //String json=getJSON_format(ss);
  //server.send(200, "text/json", json);
  //server.send(200, "text/plain","TEST");
  String path=server.uri();
    //
  setSerialPrint(pVT_Net,"handleSetCommand_uri=" + path);
    if (server.args() == 0) return; 
  String ss = server.arg(0);
  setSerialPrint(pVT_Net,"handleSetCommand=" + ss);
  JsonObject& root = jsonBuffer.parseObject(ss);
  String cmd = root["cmd"];
  String cap = root["cap"];
  String dt = root["dt"];
  //
  cmdStr=cmd;
  setSerialPrint(pVT_Net,"cmd=" + cmdStr +  "\ncap=" + cap + "\ndt=" + dt);
  setSerialPrint(pVT_Net,delm0);
   //
   if (dt.length()>0) {
    setSerialPrint(pVT_Net,"SetDateTime=" + dt);
    setSerialPrint(pVT_Net,delm0);
    //set new Web DateTime
    inputSerialString.clear();
    inputSerialString+=dt;
    inputSerialComplete=true;
   }
   //
};//handleSetCommand


//------------------------------------------------------------------------------
void setHandleSetCommand(){
 setIR_enable(false);
 handleSetCommand();
 setIR_enable(true);
};//setHandleSetCommand


//------------------------------------------------------------------------------
//holds the current upload
//------------------------------------------------------------------------------
void handleFileUpload() {
  if (server.uri() != "/edit")  return;
  //  
 HTTPUpload& upload = server.upload();
 String path=upload.filename;
 path=getPath_Mode(path); 
  //
  if (upload.status == UPLOAD_FILE_START) {
   setSerialPrint(pVT_Net,String(SD_Mode) + ".UPLOAD_START_fileName=" + path); 
      //
      if ((VT_Net.sd_available) && (SD_Mode)) {
         if (SD.exists(path.c_str())) {
            SD.remove(path.c_str());
            setSerialPrint(pVT_Net,"UPLOAD_SD_Upload: REMOVE, filename=" + path);
          }
          //
        fsUploadFile_SD = SD.open(path.c_str(), FILE_WRITE);
         if (fsUploadFile_SD) setSerialPrint(pVT_Net,"UPLOAD_SD_Upload: START, filename=" + path); 
      }//SD_Mode
      //   
      if ((VT_Net.sp_available) && (!SD_Mode)) {
          if (SPIFFS.exists(path.c_str())) {
            SPIFFS.remove(path.c_str());
            setSerialPrint(pVT_Net,"UPLOAD_SP_Upload: REMOVE, filename=" + path);
          }
          //
        fsUploadFile_SP = SPIFFS.open(path.c_str(), FILE_WRITE);
         if (fsUploadFile_SP) setSerialPrint(pVT_Net,"UPLOAD_SP_Upload: START, filename=" + path); 
      }
  } //status=UPLOAD_FILE_START
  //
  if (upload.status == UPLOAD_FILE_WRITE) {
    setSerialPrint(pVT_Net,"UPLOAD_FILE_WRITE");
        if ((VT_Net.sd_available) && (SD_Mode)  && (fsUploadFile_SD)) {
          fsUploadFile_SD.write(upload.buf, upload.currentSize);
          setSerialPrint(pVT_Net,"UPLOAD_SD_Upload: WRITE, Bytes=" + String(upload.currentSize)); 
        }
        //
        if ((VT_Net.sp_available) && (!SD_Mode) && (fsUploadFile_SP)) {
          fsUploadFile_SP.write(upload.buf, upload.currentSize);
          setSerialPrint(pVT_Net,"UPLOAD_SP_Upload: WRITE, Bytes=" + String(upload.currentSize)); 
        }
  }//status=UPLOAD_FILE_WRITE 
  //
  if (upload.status == UPLOAD_FILE_END) {
        if (fsUploadFile_SD) {
          fsUploadFile_SD.close();
          setSerialPrint(pVT_Net,"UPLOAD_SD_Upload: END_Size=" + formatBytes(upload.totalSize)); 
        }
        //
       if (fsUploadFile_SP) {
        fsUploadFile_SP.close();
        setSerialPrint(pVT_Net,"UPLOAD_SP_Upload: END_Size=" + formatBytes(upload.totalSize)); 
       }
  } //status=UPLOAD_FILE_END
  //
};//handleFileUpload


//------------------------------------------------------------------------------
void setHandleFileUpload(){
  setIR_enable(false);
  handleFileUpload();
  setIR_enable(true);
};//setHandleFileUpload



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void setServerInit(TVT_Net *pVT_Net) {
  //SERVER INIT
  //list directory 192.168.178.41/list?dir=/
  server.on("/list", HTTP_GET, setHandleFileList);
  //create file
  server.on("/edit", HTTP_PUT, setHandleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, setHandleFileDelete);
  //rename file or folder
  server.on("/rename", HTTP_GET, setHandleFileRename);
  //
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {returnOK("");}, setHandleFileUpload);
  //
  server.on("/index", HTTP_POST, setHandleSetCommand);
  //
  server.on("/files"    , HTTP_GET, [](){setHandleDirectory(false);});
  server.on("/directory", HTTP_GET, [](){setHandleDirectory(true);});
  //called when the url is not defined here
  //use it to load content from SD or SPIFFS
  //192.168.178.41/index.html
  server.onNotFound(setHandleNotFound);
  //
  //value.htm get heap status, analog input value and all GPIO statuses in one json call
  //server.on("/all", HTTP_GET, [](){setHandleAllValue();});
  //
  server.begin();
  //
  String str=""; 
   if (found_a) str += "AccesPoint started=" + WiFi.softAPIP().toString() + "\n";
   if (found_w) str += "WebServer started="  + WiFi.localIP().toString()  + "\n";
   //
  setSerialPrint(pVT_Net,delm1 + "\n" + str + "\n" + delm1 + "\n" + delm1);
};//setServerInit


//------------------------------------------------------------------------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//char bufStr[50];
String str="";
  setSerialPrint(pVT_Net,"NOT CAMERA_MODE");
  setSerialPrint(pVT_Net,"webSocketEvent: num=" + String(num) + " type=" + String(type));
  //printf(bufStr, "webSocketEvent(%d, %d, ...)\r\n", num, type);
  //setSerialPrint(pVT_Net,String(bufStr));
  websock_num=num;
  //
  str += "type=" + String(type);
  str += "\nWStype_DISCONNECTED=" + String(WStype_DISCONNECTED);
  str += "\nWStype_CONNECTED="    + String(WStype_CONNECTED);
  str += "\nWStype_TEXT=" + String(WStype_TEXT);
  setSerialPrint(pVT_Net,str);
  //
  switch(type) {
    case WStype_DISCONNECTED:
      setSerialPrint(pVT_Net,"num=" + String(num) + " Disconnected!");
      //printf(bufStr,"[%u] Disconnected!\r\n", num);
      //setSerialPrint(pVT_Net,bufStr);
      break;
    // 
    case WStype_CONNECTED:{
     IPAddress ip = webSocket.remoteIP(num);
     str="";
     str += "num=" + String(num) + " Connected from ";
     str += String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
     setSerialPrint(pVT_Net,str);
     //printf(bufStr, "[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
     //setSerialPrint(pVT_Net,bufStr);
     break;
    }
    
     //
    case WStype_TEXT: {
      setSerialPrint(pVT_Net,"num=" + String(num) + " get Text: payload=" + String(*payload));
      //printf(bufStr, "[%u] get Text: %s\r\n", num, payload);
      //setSerialPrint(pVT_Net,bufStr);
      //........................................parse JSON
      DynamicJsonBuffer json_input;
      JsonObject& root = json_input.parseObject((const char *)payload);
      const char *cmd = root["cmd"];
      const int val = root["val"];
      boolean setCmdStr=false;
      //
      if (!setCmdStr) {
        setSerialPrint(pVT_Net,"Unknown command");
        //sendMSG("INFO","Unknown command received");
      }
      break;
    }
    //
    default:
      setSerialPrint(pVT_Net,"Invalid WStype=" + String(type));
      //printf(bufStr,"Invalid WStype [%d]\r\n", type);
      //setSerialPrint(pVT_Net,bufStr);
      break;
  }
  //
 setSerialPrint(pVT_Net,"WebEvent=END");
}; //webSocketEvent



//------------------------------------------------------------------------------
//File Write
//------------------------------------------------------------------------------
boolean fileWrite(String path, String str){
boolean resMode=false;
int len=str.length();
int idxNr=0, argNr=-1;
   if ((VT_Net.sd_available) && (SD_Mode)) {
      if (SD.exists(path.c_str())) SD.remove(path.c_str());

    fsUploadFile_SD=SD.open(path.c_str(),FILE_WRITE);
      //
      if (fsUploadFile_SD) {
        fsUploadFile_SD.write((uint8_t *)str.c_str(), len);
        setSerialPrint(pVT_Net,"FileWrite to SD-CARD=" + path);
        fsUploadFile_SD.close();
        return true;
      }
   }
   //
   if (VT_Net.sp_available) {  
     String mStr="w";
     fsUploadFile_SP = SPIFFS.open(path.c_str(), mStr.c_str());
       //verify the file opened:
       if (fsUploadFile_SP) {
         fsUploadFile_SP.write((uint8_t *)str.c_str(), len);
         setSerialPrint(pVT_Net,"FileWrite to SPIFFS=" + path);
         fsUploadFile_SP.close();
         return true;
       }
   }
  //
return false; 
}//fileWrite


//------------------------------------------------------------------------------
//Append to SPIFFS log.txt
//------------------------------------------------------------------------------
boolean fileAppend(String path, String str){
int len=str.length(), logSize=0;
   if (VT_Net.sp_available) {
    String mStr="w";
    String ss="FileWrite to SPIFFS=";
       if (SPIFFS.exists(path.c_str())) {
        
        fsUploadFile_SP = SPIFFS.open(path.c_str(), "r");
         if (fsUploadFile_SP) {
          logSize=fsUploadFile_SP.size();
          fsUploadFile_SP.close();
         }
         //set AppendMode
         if (logSize<10000) {
          mStr="a";
          ss="FileAppend to SPIFFS=";
         }
       }
     fsUploadFile_SP = SPIFFS.open(path.c_str(), mStr.c_str());
       //verify the file opened:
       if (fsUploadFile_SP) {
         fsUploadFile_SP.write((uint8_t *)str.c_str(), len);
         setSerialPrint(pVT_Net,ss + path + "\t" + str);
         fsUploadFile_SP.close();
         return true;
       }
   }
  //
return false; 
};//fileAppend



//------------------------------------------------------------------------------
//read file from SPIFFS and store it as a String variable
//------------------------------------------------------------------------------
String fileRead(String path){
String contents="";
  if ((VT_Net.sd_available) && (SD_Mode)) {
   if (SD.exists(path.c_str())) {
    fsUploadFile_SD=SD.open(path.c_str(),FILE_READ);
     if (fsUploadFile_SD) {
      contents += fsUploadFile_SD.readString();
      setSerialPrint(pVT_Net,"FileRead from SD-CARD=" + path);
      fsUploadFile_SD.close();
      return contents;
     }
   }
  }
  //  
  if (VT_Net.sp_available) {
   if (SPIFFS.exists(path.c_str())) {
    fsUploadFile_SP = SPIFFS.open(path.c_str(), "r");
     if (fsUploadFile_SP) {
      contents += fsUploadFile_SP.readString();
      setSerialPrint(pVT_Net,"FileRead from SPIFFS=" + path);
      fsUploadFile_SP.close();
      return contents;
     }
   }
  }
 return contents;
};//fileRead


//------------------------------------------------------------------------------
//Remove file
//------------------------------------------------------------------------------
boolean fileRemove(String path){
  if ((VT_Net.sd_available) && (SD_Mode)) {
   if (SD.exists(path.c_str())) {
      SD.remove(path.c_str());
      return true;
    }
  }
//
  if ((VT_Net.sp_available) && (SPIFFS.exists(path.c_str()))) {
   SPIFFS.remove(path.c_str());
   return true;
  }  
return false; 
};//fileRemove



//------------------------------------------------------------------------------
//get IRCode list or file data from SPIFFS
//------------------------------------------------------------------------------
String getFileListData(String path) {
 String str="", ss="(INTERNAL)", ssOut="";
 boolean irCodeFile=(path=="/ircode.txt");
 boolean autoFile=  (path=="/auto.txt");
 boolean writeFile= ((irCodeFile) || (autoFile));
  //
  if (VT_Net.sp_available) {
   setSerialPrint(pVT_Net,"SPIFFS.begin=" + path);
   //
   setIR_enable(false);
   SPIFFS.begin();
    //
    if ((!SPIFFS.exists(path)) && (writeFile)) {
      if (fileWrite(path,str)) 
      setSerialPrint(pVT_Net,"SP_fileWrite=" + path);
    }
    // 
    if (SPIFFS.exists(path)) {
      str=fileRead(path);
        if (str.length()>0) {
         ss="(SPIFFS=" + path + ")"; 
         ssOut=delm1 + "\n" + ss + "\tfileRead=" + path + "\n" + delm1;
         setSerialPrint(pVT_Net,ssOut);
           //
           if (irCodeFile) {
             ssOut=str + "\n" + delm1 + "\n" + ss + "\tSet ir_list\n" + delm1;
             setSerialPrint(pVT_Net,ssOut);
             //
             list_irStr=list_setString(str);  
             ssOut=delm1 + "\n" + ss + "\n" + list_irStr + "\n" + delm1;
             setSerialPrint(pVT_Net,ssOut);
           } //irCodeFile  
         //data_SP=true;  
        }//length  
    }
    //
   setIR_enable(true);
  }//sp_available
return str;
};//getFileListData


//------------------------------------------------------------------------------
String getParameterLine(String dStr, String ss, char** pp) {
String str="";
  if (dStr.length()>0) {
   int index=list_get_indexOf(dStr,ss);
    if (index>=0) {
     str=list_get_item(dStr,index);
     index=str.indexOf(ss);
     str.remove(0,index + ss.length());
     str.replace("\"",""); str.replace(",","");
     str.replace("[{",""); str.replace("]}","");
     str.trim();
     //
     *pp= new char[str.length()+1];
     str.toCharArray(*pp,str.length()+1);  
    }
  }
return str; 
};//getParameterLine



//------------------------------------------------------------------------------
//Server mode from SP/auto.txt
boolean getServerMode(String path, boolean setNew, boolean setWeb) {
String dStr="",str=" PARAMETER-MODE_LIST:=", ss="",pStr="";
int index=-1, i=0;
char* pp;
  if (setWeb) str=" SERVER-MODE_LIST:=";
  
  setSerialPrint(pVT_Net,delm1 + "\nGET" + str  + path);
  //getfileData(path)
  dStr=getFileListData(path);
   //
   if (setNew) {
      if (dStr.length()>0) setSerialPrint(pVT_Net,"NEW" + str + path);
   } else dStr="";
   //
  ss="Mode="; str=getParameterLine(dStr,ss,&pp);
  setSerialPrint(pVT_Net,delm1 + "\n" + ss + str);
  //
  ss="SoundMode="; str=getParameterLine(dStr,ss,&pp);
  setSerialPrint(pVT_Net,ss + str);
   //if (str=="0") SoundON=false;
   //
  ss="WakeSeconds="; str=getParameterLine(dStr,ss,&pp);
  setSerialPrint(pVT_Net,ss + str);
   //if (str.length()>0) wakeSeconds=str.toInt();
  //
  ss="SleepSeconds="; str=getParameterLine(dStr,ss,&pp);
  setSerialPrint(pVT_Net,ss + str);
    //if (str.length()>0) sleepSeconds=str.toInt();
    //
   //
   if (setWeb) {
        setSerialPrint(pVT_Net,delm0);
        //
        ss="SSID_W="; getParameterLine(dStr,ss,&ssid_w);
        setSerialPrint(pVT_Net,ss + String(ssid_w));
        //    
        ss="PASSWORD_W="; getParameterLine(dStr,ss,&password_w);
        pStr=String(password_w);
          if (pStr.length()==0) pStr="No password";
        setSerialPrint(pVT_Net,ss + pStr);
        //
        ss="HOST_W=";  getParameterLine(dStr,ss,&host_w);
        setSerialPrint(pVT_Net,ss + String(host_w));
        //
        ss="IP_W=";  str=getParameterLine(dStr,ss,&pp);
        ip_w.fromString(str);
        setSerialPrint(pVT_Net,ss + ip_w.toString());
        //
        ss="GATEWAY_W=";  str=getParameterLine(dStr,ss,&pp);
        gateway_w.fromString(str);
        setSerialPrint(pVT_Net,ss + gateway_w.toString());
        //
        ss="SUBNET_W=";  str=getParameterLine(dStr,ss,&pp);
        subnet_w.fromString(str);
        setSerialPrint(pVT_Net,ss + subnet_w.toString() + "\n" + delm1);
         //
         //
        ss="SSID_A=";  getParameterLine(dStr,ss,&ssid_a);
        setSerialPrint(pVT_Net,ss + String(ssid_a));
        //    
        ss="PASSWORD_A="; getParameterLine(dStr,ss,&password_a);
        pStr=String(password_a);
         if (pStr.length()==0) pStr="No password";
        setSerialPrint(pVT_Net,ss + pStr);
        //
        ss="HOST_A=";  getParameterLine(dStr,ss,&host_a);
        setSerialPrint(pVT_Net,ss + String(host_a));
         //
        ss="IP_A=";  str=getParameterLine(dStr,ss,&pp);
        ip_a.fromString(str);
        setSerialPrint(pVT_Net,ss + ip_a.toString());
        //
        ss="GATEWAY_A=";  str=getParameterLine(dStr,ss,&pp);
        gateway_a.fromString(str);
        setSerialPrint(pVT_Net,ss + gateway_a.toString());
        //
        ss="SUBNET_A=";  str=getParameterLine(dStr,ss,&pp);
        subnet_a.fromString(str);
        setSerialPrint(pVT_Net,ss + subnet_a.toString() + "\n" + delm1);
   }//setWeb
 return true;
};//getServerMode


//------------------------------------------------------------------------------
boolean setServerMode(TVT_Net *pVT_Net) {
String str="", ss="";
getServerMode("/auto.txt",true,true); 
//
  //WIFI INIT AccessPoint
  setSerialPrint(pVT_Net,"AccessPoint to " + String(ssid_a));
  // before or after Wifi.softAP(ssid, password); 
  //WiFi.config(ip_a, gateway_a,subnet_a);       
   #ifdef IR_MODE
    setIR_enable(false);
   #endif
  setSerialPrint(pVT_Net,"AccessPoint to WiFi.softAPConfig START");
  //
  WiFi.softAPConfig(ip_a, gateway_a,subnet_a);       
  setSerialPrint(pVT_Net,"AccessPoint to WiFi.softAPConfig DONE");
  //
  WiFi.mode(WIFI_AP_STA);
   //
 ss=String(password_a);
   if (ss.length()>0) found_a=WiFi.softAP(ssid_a, password_a); else found_a=WiFi.softAP(ssid_a);
   // 
  delay(2000);
  MDNS.begin(host_a);
  //
  //WIFI INIT Webserver
  setSerialPrint(pVT_Net,"Try Webserver to " + String(ssid_w));
  // before or after Wifi.Begin(ssid, password); 
  WiFi.config(ip_w, gateway_w,subnet_w);       
  setSerialPrint(pVT_Net,"WebServer to WiFi.config DONE");
     //
     if (String(WiFi.SSID()) != String(ssid_w)) {
       WiFi.mode(WIFI_AP_STA);
        //
        if (password_w=="") WiFi.begin(ssid_w); else WiFi.begin(ssid_w, password_w);
     }
     //
   int counter=0;
   str=""; 
     while ((WiFi.status() != WL_CONNECTED)&&(counter<6)) {
      //delay(1000); 
      str += "." + String(counter);
      setSerialPrint(pVT_Net,"." + String(counter),true);
      delay(1000);
      counter++;
     } //while
     //
     if (counter<6) {
      MDNS.begin(host_w);
      found_w = true; 
     }
     //
   setSerialPrint(pVT_Net,""); 
    if ((found_a) || (found_w)) {
     setSerialPrint(pVT_Net,delm1 + "\nConnectParameter");
    }
    //
   str=delm1;
   pVT_Net->infoStr[1]="WebSocket:failed";
   pVT_Net->infoStr[2]="WebServer:failed";
   pVT_Net->infoStr[3]="AccessPoint:failed";
    //
    if ((found_a) || (found_w)) {
      str += "\nWebSocket Connected to " + String(ssid_w);
      str += " with IP address=" + WiFi.localIP().toString();
      str += " port=" + String(port);
      pVT_Net->infoStr[1]="WebSocket:" + String(ssid_w);
      //
      str += "\nWebserver Start with: " + WiFi.localIP().toString();
      str += "/index.htm?ip=" + WiFi.localIP().toString() + ":" + String(port);
      pVT_Net->infoStr[2]="Webserver:" + WiFi.localIP().toString();
      //
      str += "\nAccessPoint Start with: " + ip_a.toString();
      str += "/index.htm?ip=" + ip_a.toString() + ":" + String(port);
      pVT_Net->infoStr[3]="AccessPoint:" + ip_a.toString();
      //
      webSocket.begin();
      webSocket.onEvent(webSocketEvent);
    } else {
      str +="\nNOT WebSocket Connected to " + String(ssid_w);
    }
   setSerialPrint(pVT_Net,str); 
   //
   setIR_enable(true);
  //
 return ((found_a) || (found_w));
}; //setServerMode


//==============================================================================
#endif //WEB_MODE
//==============================================================================
//==============================================================================
