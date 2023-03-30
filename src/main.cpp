//==============================================================================
//Main program
//==============================================================================
/*
  Please add MCP_CAN_LIB to your library first........
  MCP_CAN_LIB file in M5stack lib examples -> modules -> COMMU -> MCP_CAN_lib.rar 
*/
//==============================================================================
#define STACK_MODE
//==============================================================================
//------------------------------------------------------------------------------

#if defined(ESP32) && defined(STACK_MODE)
/*
 #if !(USING_DEFAULT_ARDUINO_LOOP_STACK_SIZE)
  uint16_t USER_CONFIG_ARDUINO_LOOP_STACK_SIZE = 32768; //32768; //16384; //max 32768, default=8192
  //uint16_t CONFIG_ARDUINO_LOOP_STACK_SIZE=32768;
 #endif
 //
*/
 // ADC_MODE(ADC_VCC);
#endif //STACK_MODE


//==============================================================================
//create start pool data
#include "UnitVTObjCreatePool.h"
//
//create start server data
//#include "UnitVTServer.h"
//
#include <ESP32Time.h>

ESP32Time rtc(0);  // offset in seconds GMT+1,3600

//==============================================================================
void init_can();
void test_can_transmit();
void test_can_receive();
void CANMsgPGN(CANMsg msg);


//==============================================================================
void SetInput()
{
  String fntName = "arial3-0";
  // Clear input text area
  Set_fillRect(pVT_Net, 0, 0, pVT_Net->tft.width(), KEYBOARD_Y - 1, TFT_BLACK);
  Set_setTextDatum(pVT_Net, 0);
//
#ifdef LOAD_GFXFF
  if (Set_fontLoaded)
    Set_unloadFont(pVT_Net);
  Set_setFreeFont(pVT_Net, FF2);
#else
  Set_loadFont(pVT_Net, fntName);
#endif // LOAD_GFXFF
  //
  Set_drawString(pVT_Net, input_text, 0, 0);
}

//==============================================================================
void setup_VTDateTime()
{
  String fntName = "arial3-0";
  // DateTime screen
  if (pVT_Net->VTPushStop == 1)
  {
    pVT_Net->ImgMode = 0;
    if (Set_fontLoaded)
      Set_unloadFont(pVT_Net);
    //
    Set_fillScreen(pVT_Net, TFT_BLACK);
    Set_resetViewport(pVT_Net);
    Set_setTextColorBg(pVT_Net, TFT_YELLOW, TFT_BLACK);
    if (Set_fontLoaded)
      Set_unloadFont(pVT_Net);
//
#ifdef LOAD_GFXFF
    Set_setTextSize(pVT_Net, 1);
    Set_setFreeFont(pVT_Net, FF2);
#else
    Set_loadFont(pVT_Net, fntName);
#endif // LOAD_GFXFF
  }
  // normal screen
  if (pVT_Net->VTPushStop == 0)
  {
    if (pVT_Net->PSRam)
      pVT_Net->ImgMode = 2;
    Set_loadFont(pVT_Net, pVT_Net->fntName);
  }
  //
} // setup_VTDateTime

//==============================================================================
void SetDateAndTime(String str, boolean setCompile)
{
  boolean setDateTime = false;
  uint8_t k = 2;
  int i = -1, j = -1;
  String ss = "", cc = "";
  //
  uint8_t tt[2][3] = {{0, 0, 0}, {0, 0, 0}};
  uint16_t yy = 0;
  i = str.indexOf("TIME=");
  if (i == 0)
  {
    pVT_Net->VTPushStop = !pVT_Net->VTPushStop;
    setup_VTDateTime();
    VTFlushTime(pVT_Net);
    Serial.println("TIME=" + String(pVT_Net->timeStrbuff));
    pVT_Net->VTPoolRefresh = true;
    return;
  }
  //
  i = str.indexOf("HOUR=");
  if (i == 0)
  {
    str.replace("HOUR=", "");
    if (str.length() > 0)
      pVT_Net->hourOffset = str.toInt();
    Serial.println("hourOffset=" + String(pVT_Net->hourOffset));
    return;
  }
  //
  i = str.indexOf(".");
  j = str.lastIndexOf(".");
  if ((i > 0) && (j > 0) && (i < j))
  {
    k = 0;
    cc = ".";
    setDateTime = true;
  }
  //
  i = str.indexOf(":");
  j = str.lastIndexOf(":");
  if ((i > 0) && (j > 0) && (i < j))
  {
    k = 1;
    cc = ":";
    setDateTime = true;
  }
  j = 0;
  //
  if (setDateTime)
  {
    // get/set time or date "TIME="
    if (pVT_Net->RTC_active)
    {
      M5.Rtc.GetTime(&pVT_Net->RTCtime); // Gets the time in the real-time clock.
      M5.Rtc.GetDate(&pVT_Net->RTCDate); // Gets the date in the real-time clock.
    }
    else
    {
      VTFlushTime(pVT_Net, false);
    }
    //
    tt[0][0] = pVT_Net->RTCDate.Date;
    tt[0][1] = pVT_Net->RTCDate.Month;
    yy = pVT_Net->RTCDate.Year;
    tt[0][2] = yy % 2000;
    tt[1][0] = pVT_Net->RTCtime.Hours;
    tt[1][1] = pVT_Net->RTCtime.Minutes;
    tt[1][2] = pVT_Net->RTCtime.Seconds;
    //
    if (setCompile)
    {
      // TODO
    }
    //
    while ((str.length() > 0) && (j < 3) && (k < 2))
    {
      i = str.indexOf(cc);
      if (i > 0)
      {
        ss = str.substring(0, i);
        str.remove(0, i + 1);
        // Serial.println(ss);
        tt[k][j] = ss.toInt();
      }
      else
      {
        tt[k][j] = str.toInt();
        // Serial.println(str);
        // get year
        if ((k == 0) && (j == 2))
          yy = str.toInt();
        str = "";
      }
      j++;
    } // while
  }   // setDateTime
  //
  if (setDateTime)
  {
    // setSerialPrint("SetDateTime:");
    Serial.println("SetDate:" + String(tt[0][0]) + "." + String(tt[0][1]) + "." + String(yy));
    Serial.println("SetTime:" + String(tt[1][0]) + ":" + String(tt[1][1]) + ":" + String(tt[2][2]));
    // Set the time.
    pVT_Net->RTCtime.Hours = tt[1][0];
    pVT_Net->RTCtime.Minutes = tt[1][1];
    pVT_Net->RTCtime.Seconds = tt[1][2];
    pVT_Net->RTCDate.Date = tt[0][0];
    pVT_Net->RTCDate.Month = tt[0][1];
    pVT_Net->RTCDate.Year = yy; // tt[0][2];

    if (!M5.Rtc.SetTime(&pVT_Net->RTCtime))
      Serial.println("wrong time set!");
    if (!M5.Rtc.SetDate(&pVT_Net->RTCDate))
      Serial.println("wrong date set!");
  } // setDateTime
  //
} // SetDateAndTime

//==============================================================================
void drawKeyboard()
{
  String fntName = "arial3-0";
  int i = 0, x = 0, y = 0, key_page = 0;
  Set_loadFont(pVT_Net, fntName);
  Set_setTextDatum(pVT_Net, 0);
  //
  for (int r = 0; r < ROWS; r++)
  {
    for (int c = 0; c < COLS; c++)
    {
      x = (KEYBOARD_X + (c * KEY_W));
      y = (KEYBOARD_Y + (r * KEY_H));
      Set_fillRect(pVT_Net, x + 1, y + 1, KEY_W - 2, KEY_H - 2, TFT_BLACK);
      Set_drawRoundRect(pVT_Net, x, y, KEY_W, KEY_H, 10, TFT_DARKGREY);
      key_page = 0;
      if (key_mode == KEY_MODE_NUMBER)
        key_page += 2;
      if (shift_mode == true)
        key_page += 1;

      String key;
      char ch = keymap[key_page][r][c];
      //
      if (ch == '\002')
      { // Shift
#ifdef LOAD_GFXFF
        if (Set_fontLoaded)
          Set_unloadFont(pVT_Net);
        Set_setTextSize(pVT_Net, 1);
        Set_setFreeFont(pVT_Net, FF0);
#else
        fntName = "arial2-0";
        Set_loadFont(pVT_Net, fntName);
#endif
        //
        key = "shift";
        Set_drawString(pVT_Net, key, x + 9, y + 28);
      }
      else
      {
        if ((r == 0) && (c == 0))
        {
#ifdef LOAD_GFXFF
          if (Set_fontLoaded)
            Set_unloadFont(pVT_Net);
          Set_setTextSize(pVT_Net, 1);
          Set_setFreeFont(pVT_Net, FF3);
#else
          fntName = "arial3-0";
          Set_loadFont(pVT_Net, fntName);
#endif // LOAD_GFXFF
        }
        //
        key = String(ch);
        Set_drawString(pVT_Net, key, x + 12, y + 12);
      }
//
#ifdef M5CORE2_MODE
      if (touch_button_list[i] != NULL)
      {
        touch_button_list[i]->delHandlers();
        delete (touch_button_list[i]);
        touch_button_list[i] = NULL;
      }
      //
      touch_button_list[i] = new Button(x, y + 5, KEY_W, KEY_H, false, key.c_str());
#endif
      //
      i++;
    } // for c
  }   // for r
} // drawKeyboard

//==============================================================================
// get input vs ScreenKeyboard
void setup_VTKeyboard()
{
  String fntName = "arial2-0";
  // M5.background.delHandlers();
  Serial.print("0.last_fntName=");
  Serial.println(pVT_Net->fntName);
  //
  if (pVT_Net->VTPushStop == 1)
  {
    Set_setTextDatum(pVT_Net, 0);
    pVT_Net->ImgMode = 0;
    if (Set_fontLoaded)
      Set_unloadFont(pVT_Net);
    //
    Set_fillScreen(pVT_Net, TFT_BLACK);
    Set_setTextColorBg(pVT_Net, TFT_GREEN, TFT_BLACK);
//
#ifdef LOAD_GFXFF
    Set_setTextSize(pVT_Net, 1);
    Set_setFreeFont(pVT_Net, FF1);
#else
    Set_loadFont(pVT_Net, fntName);
#endif // LOAD_GFXFF
    //
    // Button A
    Set_drawString(pVT_Net, "back", 40, 225);
    // Button B
    Set_drawString(pVT_Net, "done", 145, 225);
    // Button C
    Set_drawString(pVT_Net, "mode", 250, 225);
    //
    // TODO
    key_mode = KEY_MODE_LETTER;
    shift_mode = false;
    input_text = "Empty";
    //
    drawKeyboard();
    SetInput();
    //
    pVT_Net->VTPushStop = 2;
    pVT_Net->VTPoolRefresh = false;
    Serial.print("1.last_fntName=");
    Serial.println(pVT_Net->fntName);
  }
  //
  // load last font
  if (pVT_Net->VTPushStop == 0)
  {
    if (pVT_Net->PSRam)
      pVT_Net->ImgMode = 2;
    Set_setTextDatum(pVT_Net, 0);
    Serial.print("2.last_fntName=");
    Serial.println(pVT_Net->fntName);
    Set_loadFont(pVT_Net, pVT_Net->fntName);
  }
  //
} // setup_VTKeyBoard

//--------------------------------------------------------
// set serial manual Date or Time
void getInputSerialString()
{
  CANMsg msg;
  boolean setMSG = false, refresh = false, setFocus = false;
  String str = "", ss = "";
  uint8_t showAttr = 1, i = 0, j = 1, bMode = 0, nn = 0;
  uint16_t objID = 0xFFFF;
  uint32_t oValue = 0;
  int16_t objIdx = -1, k = -1;
  if (inputSerialComplete)
  {
    inputSerialComplete = false;
    str += inputSerialString;
    inputSerialString.clear();
    Serial.print("InputSerialString:\t");
    ss = str.substring(0, 3);
    ss.toUpperCase();
    // VTChangeValue
    if (ss.indexOf("A=V") == 0)
    {
      str.remove(0, 3);
      str = ss + str;
    }
    else
    {
      str.toUpperCase();
      str.trim();
    }
    //
    // set ScreenShot
    if (str.indexOf("SC=") == 0)
    {
      Serial.println(str);
      str.replace("SC=", "");
      // Clear screen
      if (str.length() == 0)
      {
        str = "Clear screen";
        VTPoolDataRefresh(pVT_Net, false, true);
        Serial.println(str);
        return;
      }
      else
      {
        if (str.indexOf("R") == 0)
        {
          str.remove(0, 1);
          bMode = true; // readMode
        }
        j = str.toInt();
      }
      //
      if (str.length() <= 2)
        str = getStringDEC(j, 4);
      //
      pVT_Net->x = 0;
      pVT_Net->y = 0;
      pVT_Net->w = pVT_Net->tft.width();
      pVT_Net->h = pVT_Net->tft.height();
      String path = "/img" + str + ".bmp";
      if (bMode)
      {
        str = "ScreenShot read from=" + path + "->NOT OK";
        if (readImageScreen(SD, path.c_str(), pVT_Net))
          str.replace("NOT", "");
      }
      else
      {
        str = "ScreenShot write to=" + path + "->NOT OK";
        if (writeImageScreen(SD, path.c_str(), pVT_Net))
          str.replace("NOT", "");
      }
      Serial.println(str);
      return;
    }
    // set/get CAN message
    if (str.indexOf("SETMSG=") == 0)
    {
      setMSG = getMsgFromCmd(pVT_Net, &msg, str);
      Serial.println(getMsgFrameStr(&msg));
      // read/send Messages simu
      if ((setMSG) && (pVT_Net->CAN_active))
      {
        if (msg.MSG_TX)
        {
          // Serial.println("CANMsgSend");
          VT_CAN_MsgSend(pVT_Net, &msg);
        }
        else
        {
          // Serial.println("CANMsgPGN");
          CANMsgPGN(msg);
        }
      }
      return;
    }
    //
    //"S=" TVT_VTSelectInput message, S=6000,1
    if (str.indexOf("S=") == 0)
    {
      Serial.println(str);
      str.replace("S=", "");
      str.trim();
      j = str.indexOf(",");
      if ((j > 0) && (pVT_Net->VT_ActiveMask < 0xFFFF))
      {
        ss = str.substring(0, j);
        str.remove(0, j + 1);
        objID = ss.toInt();
        oValue = str.toInt();
        objIdx = getVTDrawID(pVT_Net, objID);
        // getStreamDrawInfo(pVT_Net);
        //
        if (objIdx >= 0)
        {
          // key
          if (pVT_Net->VTObjType == 5)
          {
            pVT_Net->TFT_KeySelect == pVT_Net->TFT_SelectNr;
            setFocus = true;
          }
          // button
          if (pVT_Net->VTObjType == 6)
          {
            pVT_Net->TFT_ButtonSelect == pVT_Net->TFT_SelectNr;
            setFocus = true;
          }
          // input
          if (HasInArray(pVT_Net->VTObjType, inpAllObjSet))
          {
            pVT_Net->TFT_InputSelect = pVT_Net->TFT_SelectNr;
            setFocus = true;
          }
          //
          if (setFocus)
          {
            TVT_VTSelectInput msgObj;
            if (msgObj.setMsgToAttr(&msg, pVT_Net))
            {
              // check TVT_VTSelectInput message
              if (msg.MSG_TX)
                VT_CAN_MsgSend(pVT_Net, &msg);
            } // msg
          }   // setFocus
        }
        else
          oValue = 255; // objIdx>=0
        //
        if (oValue == 255)
        {
          pVT_Net->TFT_KeySelect = 0;
          pVT_Net->TFT_ButtonSelect = 0;
          pVT_Net->TFT_InputSelect = 0;
          pVT_Net->TFT_KeySelectObjID = 0xFFFF;
          pVT_Net->TFT_ButtonSelectObjID = 0xFFFF;
          pVT_Net->TFT_InputSelectObjID = 0xFFFF;
        }
        else
          setVTDrawListClearSelect(pVT_Net, objID);
        pVT_Net->VTPoolRefresh = true;
      } // j>0
      return;
    }
    //
    //"N=" TVT_VTChangeNumericValue message
    if (str.indexOf("N=") == 0)
    {
      Serial.println(str);
      str.replace("N=", "");
      str.replace("n=", "");
      str.trim();
      j = str.indexOf(",");
      if ((j > 0) && (pVT_Net->VT_ActiveMask < 0xFFFF))
      {
        ss = str.substring(0, j);
        str.remove(0, j + 1);
        objID = ss.toInt();
        oValue = str.toInt();
        objIdx = getVTObjID(pVT_Net, objID);
        if (objIdx >= 0)
        {
          // TEST
          Serial.println(objID);
          Serial.println(oValue);
          Serial.println(pVT_Net->VTObjID);
          Serial.println(pVT_Net->VTObjType);
          //
          TVT_VTChangeNumericValue msgObj;
          if (msgObj.setMsgToAttr(&msg, pVT_Net))
          {
            // check TVT_VTChangeNumericValue message
            if (msg.MSG_TX)
              VT_CAN_MsgSend(pVT_Net, &msg);
            pVT_Net->VTPoolRefresh = true;
          }
        }
      } // j>0
      return;
    }
    //
    //"P=" pointing event message
    if (str.indexOf("P") == 0)
    {
      Serial.println(str);
      j = str.indexOf("=");
      pVT_Net->VTsPos = 1;
      if (j == 2)
      {
        ss = str[1];
        pVT_Net->VTsPos = ss.toInt();
      }
      if (j > 0)
        str.remove(0, j + 1);
      str.trim();
      j = str.indexOf(",");
      if ((j > 0) && (pVT_Net->VT_ActiveMask < 0xFFFF))
      {
        ss = str.substring(0, j);
        str.remove(0, j + 1);
        pVT_Net->VTxPos = ss.toInt();
        pVT_Net->VTyPos = str.toInt();
        // Serial.println(pVT_Net->VTxPos);
        // Serial.println(pVT_Net->VTyPos);
        TVT_VTPointingEvent msgObj;
        msgObj.setMsgToAttr(&msg, pVT_Net);
        // check PointingEvent message
        if (msg.MSG_TX)
          VT_CAN_MsgSend(pVT_Net, &msg);
      } // j>0
      return;
    }
    //
    // with VTActivation VTAudio Terminierung
    if (str.indexOf("T=") == 0)
    {
      Serial.println(str);
      TVT_VTControlAudioDeviceTermination msgObj;
      setMSG = msgObj.setMsgToAttr(&msg, pVT_Net);
      if ((setMSG) && (pVT_Net->CAN_active))
      {
        if (msg.MSG_TX)
          VT_CAN_MsgSend(pVT_Net, &msg);
      }
      return;
    }
    //
    // with ACKN Key "C=", button "B=", Input "A=",
    if ((str.indexOf("C=") == 0) || (str.indexOf("B=") == 0) || (str.indexOf("A=") == 0))
    {
      VT_Net.VTSelectMode = str[0];
      Serial.println(str);
      bMode += 1 * (str.indexOf("B=") == 0);
      bMode += 3 * (str.indexOf("A=") == 0);
      //
      str.replace("C=", "");
      str.replace("B=", "");
      str.replace("A=", "");
      str.trim();
      j = str.length();
      if (j > 0)
      {
        msg.DATA[0] = bMode;
        refresh = (msg.DATA[0] > 0);
        // check is numeric
        nn = char(str[0]);
        // Serial.println(String(nn,HEX));
        if ((nn >= 0x30) && (nn <= 0x39))
        {
          msg.DATA[0] = 0xFF;
          // set deselect
          ss = "D";
          if (setKeyNumberSelect(pVT_Net, &msg, ss))
          {
            if (msg.MSG_TX)
              VT_CAN_MsgSend(pVT_Net, &msg);
          }
          j = 1;
        }
        //
        msg.DATA[0] = bMode;
        refresh = (msg.DATA[0] > 0);
        if (str[0] == 'V')
          j = 1;
        //
        for (i = 0; i < j; i++)
        {
          if (j == 1)
            ss = str;
          else
            ss = str[i];
          //
          if (setKeyNumberSelect(pVT_Net, &msg, ss))
          {
            // check KeyActivation message
            if (msg.MSG_TX)
            {
              if ((msg.ID & 0x00FF0000) == TP_PGN)
              {
                // Serial.println("TVTTransportProtocol");
                // getArray8Info(pVT_Net,(uint8_t*)pVT_Net->stream_TP[pVT_Net->listNr].getBuffer(),pVT_Net->stream_TP[pVT_Net->listNr].available());
                TVTTransportProtocol msgObj;
                msgObj.getTransportProtcol(&msg, pVT_Net, &pVT_Net->stream_TP[pVT_Net->listNr]);
              }
              VT_CAN_MsgSend(pVT_Net, &msg);
            }
          }
        } // for i
        //
      } // j>0
      return;
    }
    //
    if ((str.indexOf("IMAGEMODE=") == 0) || (str.indexOf("I=") == 0))
    {
      Serial.println(str);
      str.replace("IMAGEMODE=", "");
      str.replace("I=", "");
      str.trim();
      VT_Net.ImgMode = str.toInt();
      VTPoolDataRefresh(pVT_Net, false);
      return;
    }
    //
    if ((str.indexOf("INFOMODE=") == 0) || (str.indexOf("M=") == 0))
    {
      Serial.println(str);
      str.replace("INFOMODE=", "");
      str.replace("M=", "");
      str.trim();
      VT_Net.VT_InfoMode = str.toInt();
      return;
    }
    //
    // get info DrawList
    if (str.indexOf("D=") == 0)
    {
      Serial.println(str);
      Serial.println("DrawListSize=" + String(getVTDrawListSize(pVT_Net)));
      str.replace("D=", "");
      str.trim();
      if (str.length() > 0)
        objIdx = str.toInt();
      if (objIdx >= 0)
        showAttr = 3;
      if (objIdx < 0)
        showAttr = 0;
      //
      if (objIdx < -1)
      {
        getArray8Info(pVT_Net, pVT_Net->stream_Draw[pVT_Net->listNr].getBuffer(), pVT_Net->stream_Draw[pVT_Net->listNr].available());
      }
      else
        getVTDrawListInfo(pVT_Net, objIdx, showAttr);
      //
      getHeapStatus(pVT_Net, 110);
      return;
    }
    //
    // get info object list
    if (str.indexOf("O=") == 0)
    {
      Serial.println(str);
      Serial.println("ObjListSize=" + String(getVTObjectListSize(pVT_Net)));
      //
      str.replace("O=", "");
      str.trim();
      if (str.length() > 0)
        objIdx = str.toInt();
      if (objIdx >= 0)
      {
        pVT_Net->objNr = objIdx;
        showAttr = 7;
      }
      else
      {
        showAttr = abs(objIdx);
      }
      getVTObjectListInfo(pVT_Net, objIdx, showAttr);
      getHeapStatus(pVT_Net, 111);
      return;
    }
    //
    // get info object list
    if (str.indexOf("X=") == 0)
    {
      Serial.println(str);
      str.replace("X=", "");
      str.trim();
      pVT_Net->VTObjMax[pVT_Net->listNr] = VTObjMaxDef;
      if (str.length() > 0)
        objIdx = str.toInt();
      if (objIdx >= 16)
        pVT_Net->VTObjMax[pVT_Net->listNr] = objIdx;
      getHeapStatus(pVT_Net, 112);
      return;
    }
    //
    // SD filelist
    if (str.indexOf("SD=") == 0)
    {
      Serial.println(str);
      SD_Mode = true;
      str.replace("SD=", "");
      str.trim();
      // SP_SD ViewModus
      getSP_SD_List(str);
      getHeapStatus(pVT_Net, 112);
      return;
    }
    //
    // SPIFFS filelist
    if (str.indexOf("SP=") == 0)
    {
      Serial.println(str);
      SD_Mode = false;
      str.replace("SP=", "");
      str.trim();
      getSP_SD_List(str);
      getHeapStatus(pVT_Net, 113);
      return;
    }
    //
    // Delete file or folder
    if (str.indexOf("DEL=") == 0)
    {
      Serial.println(str);
      TVTVersion msgObj;
      str.replace("DEL=", "");
      str.trim();
      if (SD_Mode)
      {
        msgObj.deleteFile(SD, str.c_str(), NULL, pVT_Net);
        listDir(SD, "/", 1);
      }
      else
      {
        msgObj.deleteFile(SPIFFS, str.c_str(), NULL, pVT_Net);
        listDir(SPIFFS, "/", 1);
      }
      return;
    }
    //
    // make Dir or file
    if (str.indexOf("MKD=") == 0)
    {
      Serial.println(str);
      TVTVersion msgObj;
      str.replace("MKD=", "");
      str.trim();
      if (SD_Mode)
      {
        if (msgObj.createDir(SD, str.c_str(), pVT_Net))
        {
          listDir(SD, "/", 0);
        }
      }
      else
      {
        if (msgObj.createDir(SPIFFS, str.c_str(), pVT_Net))
        {
          listDir(SPIFFS, "/", 0);
        }
      }
      return;
    }
    //
    // get heap status
    if (str.indexOf("H=") == 0)
    {
      Serial.println(str);
      str.replace("H=", "");
      str.trim();
      getHeapStatus(pVT_Net, str.toInt());
      return;
    }
    // get Keyboard
    if (str.indexOf("KB=") == 0)
    {
      Serial.println(str);
      str.replace("KB=", "");
      str.trim();
      if (str.length() == 0)
      {
        if (pVT_Net->VTPushStop == 0)
          pVT_Net->VTPushStop = 1;
        else
          pVT_Net->VTPushStop = 0;
      }
      else
      {
        pVT_Net->VTPushStop = str.toInt();
      }
      //
      if (pVT_Net->VTPushStop > 0)
      {
        pVT_Net->VTKeyboardActive = pVT_Net->VTPushStop;
        pVT_Net->VTPoolRefresh = false;
        setup_VTKeyboard();
      }
      else
      {
        setup_VTKeyboard();
        pVT_Net->VTKeyboardActive = pVT_Net->VTPushStop;
        pVT_Net->VTPoolRefresh = true;
      }
      //
      return;
    }
    // not compiled time
    SetDateAndTime(str, false);
    //
  } // inputSerialComplete
} // getInputSerialString

//==============================================================================
//Screen Keyboard
//==============================================================================
void buttonEventDirect(String str){
  inputSerialComplete=true;
  str=pVT_Net->VTSelectMode + "=" + str; inputSerialString.clear();
  inputSerialString+=str.c_str();
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
String str="",ss="";
int i=-1;
Button& b = *e.button;
 str=String(b.getName());
 Serial.println(e);
 Serial.println(str);
  eventDisplay(e);
 
  
  //no background events
  if(str.indexOf("background")==0) return;
  // 
  
  if ((pVT_Net->VTPushStop==0) || (pVT_Net->VTKeyboardActive==0)) {
    //
    if(e.button == &M5.BtnA){
      buttonEventDirect("B");
      return;
    }  
    if(e.button == &M5.BtnB){
      buttonEventDirect("PR");
      return;
    }  
    if(e.button == &M5.BtnC){
      buttonEventDirect("N");
      return;
    }  
    //
    if(str.indexOf("KEY")==0) {
      Serial.println(str);
      str.replace("KEY",""); i=str.toInt();
      i=i+6*pVT_Net->TFT_KeyPage;
      //Serial.println(i);
      //Serial.println(pVT_Net->TFT_KeyPage);
      pVT_Net->VTSelectMode="C";
      //set serial manual touch SoftKey
      buttonEventDirect(String(i));
      delay(100);
      buttonEventDirect("PR");
      return;
    }
    //
    if(str.indexOf("BUTTON")==0) {
      Serial.println(str);
      str.replace("BUTTON",""); i=str.toInt();
      //Serial.println(i);
      pVT_Net->VTSelectMode="B";
      //set serial manual touch button
      buttonEventDirect(String(i));
      delay(100);
      buttonEventDirect("PR");
      return;
    }
    //
    if(str.indexOf("INPUT")==0) {
      Serial.println(str);
      str.replace("INPUT",""); i=str.toInt();
      //Serial.println(i);
      pVT_Net->VTSelectMode="A";
      //set serial manual touch Input
      buttonEventDirect(String(i));
      delay(100);
      //buttonEventDirect("PR");
      return;
    }
   return;
  }//keyboard deactive
  //
  //Keyboard active
  if (pVT_Net->VTKeyboardActive>0) {
   // Backspace
   if(e.button == &M5.BtnA) {
    input_text = input_text.substring(0, input_text.length() - 1);
    SetInput();
    return;
   };
   // Done
   if(e.button == &M5.BtnB)  {
    Serial.println("done=" + input_text);
    pVT_Net->VTPushStop=0;
    pVT_Net->VTKeyboardActive=pVT_Net->VTPushStop;
    setup_VTKeyboard();
    pVT_Net->VTPoolRefresh=(pVT_Net->VTPushStop==0);
    return;
   }
   // Key mode
   if(e.button == &M5.BtnC){
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
     drawKeyboard();
     SetInput();
     return;
    }
    //
    if(String(b.getName()) == "shift"){
     shift_mode = !shift_mode; 
     drawKeyboard();
    } else {
     str=b.getName(); 
     input_text += b.getName();
    }
   SetInput();
  }
 //   
}//buttonEvent

//==============================================================================
#endif //M5CORE2_MODE
//==============================================================================

//==============================================================================
void loop_VTDateTime() {
String fntName="arial4-0";
uint32_t tDiff=millis()-pVT_Net->TargetTime;
String str="";
int xpos=0,ypos=0;
 if ((pVT_Net->VTPushStop==0) || (pVT_Net->VTKeyboardActive>0)) {
  return;
 }
 //
 if (tDiff>=1000) {
   // Set next update for 1 second later
   pVT_Net->TargetTime = millis();
   VTFlushTime(pVT_Net);
     //
     #ifdef LOAD_GFXFF
      Set_setTextSize(pVT_Net,1);
     #else
      //fntName="arial3-0";
      Set_loadFont(pVT_Net,fntName);
     #endif //LOAD_GFXFF
    //
  // Update digital time
  xpos  = 160; ypos  = 10;  // Top left corner ot clock text, about half way down
  // Draw actual Date
  str=String(pVT_Net->timeStrbuff);
  String ss=str.substring(0,str.indexOf(" "));ss.trim(); 
  Set_drawCentreString(pVT_Net,ss, xpos, ypos);   
  //  
     #ifdef LOAD_GFXFF
      Set_setTextSize(pVT_Net,2);
     #else
      //fntName="arial4-0";
      //Set_loadFont(pVT_Net,fntName);
     #endif //LOAD_GFXFF
  // Draw actual Time
  str.remove(0,str.indexOf(" ")); str.trim(); ypos=45;  
  //
  str+=" ";
  Set_drawCentreString(pVT_Net,str, xpos, ypos);
  //
  //UTC-Time
     #ifdef LOAD_GFXFF
      Set_setTextSize(pVT_Net,1);
     #else
      fntName="arial3-0";
      Set_loadFont(pVT_Net,fntName);
     #endif //LOAD_GFXFF  
  
  xpos=0;ypos=100;
  //get offset Time UTC
  VTDateTime(pVT_Net,false,pVT_Net->hourOffset);
  str="UTC:" + String(pVT_Net->timeStrbuff);
  str+=" ";
  Set_drawString(pVT_Net,str, xpos, ypos);// Draw hour offset
    //
    if (pVT_Net->VTPushStop==1){
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
  pVT_Net->VTPushStop=2;  
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
 if ((pVT_Net->VTPushStop==0) && (pVT_Net->VTKeyboardActive==0)) {
   //
   if (tDiff>=2000) {
     
     // Set next update for 1 second later
     pVT_Net->TargetTime = millis();
     /*
     Sensor.UpdateData();
     Serial.print("Temperature: "); Serial.print(Sensor.GetTemperature());
     Serial.write("\xC2\xB0"); //The Degree symbol
     Serial.println("C");
     //
     Serial.print("Humidity: "); Serial.print(Sensor.GetRelHumidity());
     Serial.println("%");
     */
     //
     float tmp = dht12.readTemperature();
     float hum = dht12.readHumidity();
     Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%\r\n", tmp, hum);    
     Serial.println(delm0);
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
 Serial.println(delm0);
 inputSerialComplete=true; inputSerialString+=str.c_str(); 
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
              //Serial.println(objType);
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
 Serial.println("Objects=" + String(getVTObjectListSize(pVT_Net)));
 Serial.println(delm0);
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
   if (pVT_Net->stream_ETP[pVT_Net->listNr].available()) VTPoolDataRead(pVT_Net,dataMode,&pVT_Net->stream_ETP[pVT_Net->listNr]); 
   if (pVT_Net->stream_TP[pVT_Net->listNr].available())  VTPoolDataRead(pVT_Net,dataMode,&pVT_Net->stream_TP[pVT_Net->listNr]); 
 //getHeapStatus(pVT_Net,2);
}//VTPoolData


//==============================================================================
boolean clearPoolData(TVT_Net *pVT_Net){
 //UnitVTCreateObjPool
 return getPoolObjectClear(pVT_Net);
}


//==============================================================================
//==============================================================================
//UnitVTCommandTechData.h
void test_can_transmit(){
CANMsg msg;
 if (pVT_Net->CAN_active){
   VT_CAN_Transmit(pVT_Net,&msg);
 }
}//test_can_transmit



//==============================================================================
void test_can_receive(){
CANMsg msg;
 if (pVT_Net->CAN_active){
   if (VT_CAN_MsgReceive(pVT_Net,&msg)) CANMsgPGN(msg);
 }  
}//test_can_receive



//==============================================================================
void SetAudioCommand(){
uint32_t aTime=millis(),dTime=aTime-pVT_Net->VTActiveTime;
  if ((pVT_Net->VTRepetition>0) && (pVT_Net->VTFrequency>0) && (pVT_Net->VTOnTimeDuration>0)){
    if (pVT_Net->VTAudioActive==1) {
     Serial.print(pVT_Net->VTRepetition);
     Serial.print(".SetON="); Serial.println(dTime);
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
     Serial.print(pVT_Net->VTRepetition);
     Serial.print(".SetOFF="); Serial.println(dTime);
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
     Serial.print(pVT_Net->VTRepetition);
     Serial.print(".SetON=");  Serial.println(dTime);
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
  src=(msg.ID>>0) & 0xFF;
    if (src>=0xFE) src=0xFF;
  dst=(msg.ID>>8) & 0xFF;
  respMSG=((dst==pVT_Net->VT_SRC) || (dst>=0xFE)); 
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
        //Serial.println(String(src,HEX));
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
        if (sendMSG) VT_CAN_MsgSend(pVT_Net,&msg);
  }//TP_EOMA

 }//can_active 
}//CANMsgPGN


//==============================================================================
//SETUP
//==============================================================================
void init_can(){
 VT_CAN_Init(pVT_Net);
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

long timezone = 1; 
byte daysavetime = 1;


//==============================================================================
//==============================================================================
#ifdef M5CORE2_MODE
// Defines gestures
Gesture swipeRight("Swipe Right", 80, DIR_RIGHT, 30, true);
Gesture swipeDown( "Swipe Down",  60, DIR_DOWN,  30, true);
Gesture swipeLeft( "Swipe Left",  80, DIR_LEFT,  30, true);
Gesture swipeUp(   "Swipe Up",    60, DIR_UP,    30, true);


//==============================================================================
// Lazy output routine. Just enough to do the job.
// str should fit on the screen (320 pixels) and not contain a \n
void output_info(const char* name, const char* info) {
  Serial.printf("%-15s: %s\n", name, info);
  /*
  TEST
  if(disp.getCursorY() >= 220) {
    disp.scroll(0, first_scroll ? -3 : -16);
    first_scroll = false;
  }
  disp.printf("%s", name);
  disp.setCursor(110, disp.getCursorY());
  disp.printf("%s\n", info);
  if(disp.getCursorY() >= 220) {
    disp.setCursor(0, 220);
  }
  disp.pushSprite(0, 0);
  */
}


//==============================================================================
void gestureHandler(Event& e) {
uint8_t setPush=0;
String str=String(e.gesture->getName());
 output_info(e.typeName(), e.gesture->getName());
  if (str=="Swipe Right") {
    if (pVT_Net->VTPushStop>0){
     pVT_Net->VTPushStop=1;setPush=3; 
    }else {
     pVT_Net->VTPushStop=1;setPush=1;
    }
  }
  //
  if (str=="Swipe Left")  {
    if (pVT_Net->VTPushStop>0){
     pVT_Net->VTPushStop=1;setPush=4; 
    }else {
     pVT_Net->VTPushStop=1;setPush=2;
    }
  }
  //  
  if (str=="Swipe Down")  {pVT_Net->VTPushStop=1;setPush=2;}
  if (str=="Swipe Up")    {pVT_Net->VTPushStop=0;setPush=2;}
  //
  if (setPush>0){
     String pushStr="Gesture.event: VTPushStop=";
     setSerialPrint(pVT_Net,pushStr + String(pVT_Net->VTPushStop) + "/" + String(e.gesture->getName()));
       if (setPush==1) {
        pVT_Net->VTKeyboardActive=0;
        setup_VTDateTime();
       }
       if (setPush==2) {
         pVT_Net->VTKeyboardActive=pVT_Net->VTPushStop;
         setup_VTKeyboard();
       }
       //
       if (setPush==3){
         if (pVT_Net->VTKeyboardActive) {
           input_text=""; SetInput();
           pVT_Net->VTPushStop=0;
           pVT_Net->VTKeyboardActive=0;
           setup_VTDateTime();
         }else {
           pVT_Net->VTPushStop=1;
           pVT_Net->VTKeyboardActive=pVT_Net->VTPushStop;
           setup_VTKeyboard();
         }
       } 
       //
       if (setPush==4){
         if (pVT_Net->VTKeyboardActive) {
           input_text=""; SetInput();
           pVT_Net->VTPushStop=1;
           pVT_Net->VTKeyboardActive=0;
           setup_VTDateTime();
         }else {
           pVT_Net->VTPushStop=0;
           pVT_Net->VTKeyboardActive=pVT_Net->VTPushStop;
           setup_VTDateTime();
         }
       } 
       //   
     pVT_Net->VTPoolRefresh=(pVT_Net->VTPushStop==0);
  }
}//gestureHandler


//==============================================================================
// Gestues only need to be set up once. I'll protect against multiple initializations.
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
 Serial.println(delm1); 
 Serial.println("Scan I2C addresses[HEX]:"); 
  //  
  for (int i = 0; i < 120; i++) {
    Wire1.beginTransmission(i);
    if (Wire1.endTransmission() == 0) {
      Serial.printf("%02X   |FIND", i);
      Serial.println(".");
      cnt++;
       //check RTC address=0x51
       if (i==0x51) pVT_Net->RTC_active=true;   
    }
  }
 Serial.println(delm0); 
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
      Serial.println(str);   
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
//==============================================================================
void setup() {
String str="";
int i=0,j=0;
struct tm tmstruct ;
boolean setWiFi=false;
 pVT_Net->ImgMode=1;
  //ViewLog
  if (!pVT_Net->notSerial) {
     //#ifndef M5STACK_MODE
      Serial.begin(115200); 
       //
       while ((!Serial) && (i < 300)) {
        delay(10);  
        i++;
      }
     //#endif 
     //
    pVT_Net->serialOut=(Serial);
  } else pVT_Net->serialOut=false;
  //
 setupStack();
  //
  //
  #ifdef WEB_MODE
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid_w);
    WiFi.begin(ssid_w, password_w);
      //
      while ((i<6) && (WiFi.status() != WL_CONNECTED)) {
        delay(500);
        Serial.print(".");
        i++;
      }
      //
      if (WiFi.status()==WL_CONNECTED){
       Serial.println("WiFi connected");
       Serial.println("IP address: ");
       Serial.println(WiFi.localIP());
       Serial.println("Contacting Time Server");
       configTime(3600*timezone, daysavetime*3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
       delay(2000);
       pVT_Net->tmstruct.tm_year = 0;
       getLocalTime(&pVT_Net->tmstruct, 5000);
       printDateTimeStructure("WiFi",pVT_Net);
       setWiFi=true;
      }
   #endif //WEB_MODE
   //
   #ifdef STACK_MODE
    M5.begin();
      #ifdef M5STACK_MODE
        M5.Power.begin();
        pVT_Net->tft.setBrightness(255);
      #endif
      //
      #ifdef M5CORE2_MODE
       //M5.Buttons.addHandler(buttonEvent,E_TOUCH);
       setupGestures();   
      #endif
      //
   #endif
  //
 //M5Stack set/get PSRAM mode
 Set_M5Stack_PSRAM(pVT_Net);
 //Sketch info
 Serial.println(delm0);
 Serial.print("ESP_SketchSize="); Serial.println(ESP.getSketchSize());
 Serial.print("ESP_FreeSketchSpace="); Serial.println(ESP.getFreeSketchSpace());
 Serial.println(delm0);
 // 
 uint8_t oImgMode=pVT_Net->ImgMode;
 pVT_Net->ImgMode=0;
 Set_setTextColor(pVT_Net,0);
 setClearScreen(pVT_Net,true);
 setup_log();
 logMemory();
 //
 boolean Set_Stream[2]={true,pVT_Net->PSRam};
 
 pVT_Net->streamStr.setNewBufferSize(300000,pVT_Net->PSRam);
 pVT_Net->streamDraw.setNewBufferSize(16000,pVT_Net->PSRam);
   //
   for (i=0;i<2;i++) {
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
  Serial.println(delm1);
  Serial.print("streamStr=");  Serial.println(pVT_Net->streamStr.getBufferSize());
  Serial.print("streamDraw="); Serial.println(pVT_Net->streamDraw.getBufferSize());
  Serial.println(delm0);
   //
   for (i=0;i<2;i++) {
    Serial.print("streamObj" + String(i) + "="); Serial.println(pVT_Net->streamObj[i].getBufferSize());
    Serial.print("Draw" + String(i) + "=");     Serial.println(pVT_Net->stream_Draw[i].getBufferSize());
    Serial.print("Cmd"  + String(i) + "=");      Serial.println(pVT_Net->stream_Cmd[i].getBufferSize());
    Serial.print("Pool" + String(i) + "=");      Serial.println(pVT_Net->stream_Pool[i].getBufferSize());
    Serial.print("ETP"  + String(i) + "=");      Serial.println(pVT_Net->stream_ETP[i].getBufferSize());
    Serial.print("TP"   + String(i) + "=");      Serial.println(pVT_Net->stream_TP[i].getBufferSize());
    Serial.println(delm0);
   }
 Serial.println(delm1);
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
   if (!SPIFFS.begin()) {
     str="SPIFFS initialisation failed!";
     setSerialPrint(pVT_Net,str); 
     pVT_Net->sp_available=false;
   }
   
   #ifndef STACK_MODE
    if (!SD.begin(TFCARD_CS_PIN, SPI, 40000000)){
     str="SD initialisation failed!";
     setSerialPrint(pVT_Net,str); 
     pVT_Net->sd_available=false;
    }   
   #endif  
   //
   if (pVT_Net->sd_available){
     pVT_Net->fs_SD=&SD;
     //listDir(*pVT_Net->fs_SD, "/", 1,"SD:");
   } 
   if (pVT_Net->sp_available){
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
   if ((!pVT_Net->sp_available) && (pVT_Net->sd_available)) {
    str="/";
    //listDir(*pVT_Net->fs_SD, str.c_str(), 1,"SD:");
   }
   //
   if (pVT_Net->VT_InfoMode) getSdFat_Info();
   //
   //list fontfiles of SPIFFS
   if (pVT_Net->sp_available) {
    str="/";
    listDir(*pVT_Net->fs_SP, str.c_str(), 1,"SPIFFS:");
   }
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
  #if defined(ESP32) && defined(STACK_MODE)
     #if defined(WEB_MODE)
      setServerMode(pVT_Net);
      setServerInit(pVT_Net);
     #endif
  #endif //STACK_MODE
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
 str="\nSETUP END ImgMode=" + String(pVT_Net->ImgMode) +  "\n" + delm1;
 setSerialPrint(pVT_Net,str);
  //clear Serial
  while (Serial.available()) Serial.read();
 //
} //setup



//==============================================================================
//==============================================================================
void serialEventInput() {
  if (notSerialReceive) return;  
  //
  while (Serial.available()) {
   //get the new byte:
   uint8_t bb = Serial.read();
     if  (!inputSerialComplete) {
       if ((bb == 0x0D) || (bb == 0x0A)) {
        if (inputSerialString.length()>0) inputSerialComplete=true;
       }else {
         inputSerialString+=char(bb);
       }
     }
     //
     if ((bb == 0x0D) || (bb == 0x0A)) {
       if (inputSerialString.length()>0) inputSerialComplete=true;
     }
  } //while
}//serialEventInput





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
//LOOP
//==============================================================================
void loop() {
String str="";
CANMsg msg;
uint16_t pressTime=2000;
int16_t  i=0,dw=0,dh=0;
uint8_t  setPush=0;
String pushStr="M5.Buttons.event: VTPushStop=";
 loop_TempHumSensor();
 //
 loop_VTDateTime();
 // 
 #if defined(M5STACK_MODE) || defined(M5CORE2_MODE)
   //
  #ifdef M5STACK_MODE  
    //two button pressed
     //to normal screen
     if ((pVT_Net->VTPushStop==2) && (M5.BtnA.isPressed()) && (M5.BtnC.isPressed())) {
      pVT_Net->VTPressedABC=true; pushStr="M5.BtnB/C.isPressed: VTPushStop=";
      pVT_Net->VTPushStop=0; setPush=1; 
     }//A/C
     //to time screen
     if ((pVT_Net->VTPushStop==0) && (M5.BtnA.isPressed()) && (M5.BtnB.isPressed())) {
      pVT_Net->VTPressedABC=true; pushStr="M5.BtnA/B.isPressed: VTPushStop=";
      pVT_Net->VTPushStop=1;  setPush=1; 
     }//A/B
     //to keyboard screen
     if ((pVT_Net->VTPushStop==0) && (M5.BtnB.isPressed()) && (M5.BtnC.isPressed())) {
      pVT_Net->VTPressedABC=true; pushStr="M5.BtnA/C.isPressed: VTPushStop=";
          pVT_Net->VTPushStop=1;  setPush=2;
     }//B/C
     //  
     if (setPush>0){
      setSerialPrint(pVT_Net,pushStr + String(pVT_Net->VTPushStop) + "/" + String(dw) + "/" + String(dh));
       if (setPush==1) {
        pVT_Net->VTKeyboardActive=0;
        setup_VTDateTime();
       }
       if (setPush==2) {
         pVT_Net->VTKeyboardActive=pVT_Net->VTPushStop;
         setup_VTKeyboard();
       }
      pVT_Net->VTPoolRefresh=(pVT_Net->VTPushStop==0);
     }
  //
  //pressedFor
  if (pVT_Net->VTKeyboardActive==0){
   //
   if ((pVT_Net->VTPushStop==0) && (M5.BtnA.pressedFor(pressTime))) {
    setSerialPrint(pVT_Net,"M5.BtnA.pressedFor:" + String(pressTime));
    pVT_Net->VTPressedFor=true;
    pVT_Net->VTSelectMode="A";
    str=VT_Net.VTSelectMode + "=0";
    setSerialPrint(pVT_Net,"VTSelectMode:" + str);
    inputSerialComplete=true; inputSerialString+=str.c_str(); 
    getInputSerialString();
   }//A
   if ((pVT_Net->VTPushStop==0) && (M5.BtnB.pressedFor(pressTime))) {
    setSerialPrint(pVT_Net,"M5.BtnB.pressedFor:" + String(pressTime));
    pVT_Net->VTPressedFor=true;
    pVT_Net->VTSelectMode="B";
    str=VT_Net.VTSelectMode + "=0";
    setSerialPrint(pVT_Net,"VTSelectMode:" + str);
    inputSerialComplete=true; inputSerialString+=str.c_str(); 
    getInputSerialString();
   }//B
   if ((pVT_Net->VTPushStop==0) && (M5.BtnC.pressedFor(pressTime))) {
    setSerialPrint(pVT_Net,"M5.BtnC.pressedFor:" + String(pressTime));
    pVT_Net->VTPressedFor=true;
    ResetSelectMode();
   }//C
  
   //released
   if (M5.BtnA.wasPressed())  {
     if ((pVT_Net->VTPushStop==0) && (!pVT_Net->VTPressedFor) && (!pVT_Net->VTPressedABC)) buttonEventDirect("B");
    pVT_Net->VTPressedFor=false; pVT_Net->VTPressedABC=false;
   }//ButtonA
   //
   if (M5.BtnB.wasPressed())  {
     if ((pVT_Net->VTPushStop==0) && (!pVT_Net->VTPressedFor) && (!pVT_Net->VTPressedABC)) {
      str="";
       if ((pVT_Net->TFT_KeySelectObjID<0xFFFF) || (pVT_Net->TFT_ButtonSelectObjID<0xFFFF)) str="PR";
       if (pVT_Net->TFT_InputSelectObjID<0xFFFF)str="V";
       //
       if (str.length()>0) buttonEventDirect(str);
     }
    pVT_Net->VTPressedFor=false; pVT_Net->VTPressedABC=false;
   }//Button B
   //
   if (M5.BtnC.wasPressed())  {
      if ((pVT_Net->VTPushStop==0) && (!pVT_Net->VTPressedFor) && (!pVT_Net->VTPressedABC)) buttonEventDirect("N");
    pVT_Net->VTPressedFor=false; pVT_Net->VTPressedABC=false;
   }//Button C
   
   //
  }//pVT_Net->VTKeyboardActive==0
  //
  #endif //M5STACK_MODE
 
  M5.update();
  /*
  M5.Touch.update();
   if (M5.Touch.changed ) {
     if ((M5.Touch.point[0].x>=0) && (M5.Touch.point[0].y>=0)) {
      Serial.println(M5.Touch.point[0] );
        if (M5.Touch.point[0].x>240){
          for (i=0;i<6;i++){
            if ((M5.Touch.point[0].y>40*i) && (M5.Touch.point[0].y<40*(i+1))) {
              pVT_Net->VTSelectMode="C";
              buttonEventDirect(String(i));
              buttonEventDirect("PR");
              break;   
            }
          }
        }
     }   
   }
   */
 #endif //M5STACK_MODE || M5CORE2_MODE
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
   Serial.println(getMsgFrameStr(&msg));
   MacroCANMsgPGN(pVT_Net,&msg,true);
     if (pVT_Net->stream_Cmd[pVT_Net->listNr].available()<8) {
       pVT_Net->stream_Cmd[pVT_Net->listNr].clear();
       pVT_Net->VTPoolRefresh=true;
     }
  }
  //
  if (pVT_Net->VTPushStop>0) pVT_Net->VTPoolRefresh=false;
  //
  //Refresh paint pool data
  if (pVT_Net->VTPoolRefresh) {
    pVT_Net->VTPoolRefresh=false;
    str=setVTDrawListSoftKey(pVT_Net);
    //Serial.println(str);
    
    VTPoolDataRefreshDirect(pVT_Net);  
    Serial.println("VTPoolDataRefreshDirect:ImgMode=" + String(pVT_Net->ImgMode));
      if (!pVT_Net->VTChangeSelect) {
        pVT_Net->VTChangeSelect=(str!=setVTDrawListSoftKey(pVT_Net));
        //Serial.println(str);
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
