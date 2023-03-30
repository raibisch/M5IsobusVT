//==============================================================================
//All Definitions of Consts
//==============================================================================
//unit UnitVTObjConsts.cpp
//==============================================================================
#ifndef UnitVTObjConsts_h
  #define UnitVTObjConsts_h
  #include "UnitVTObjConsts.h"
#endif 



//==============================================================================
//global functions
//==============================================================================
void bitSet8(uint8_t & x, uint8_t n){
  if (n > 7)  x |= (0x100000000 << (n - 8));
  else        x |= (0x01 << n);
}

//------------------------------------------------------------------------------
void bitClear8(uint8_t & x, uint8_t n){
  if (n > 7) x &= ~(0x100000000 << (n - 8));
  else       x &= ~(0x1 << n);
}

//------------------------------------------------------------------------------
void bitToggle8(uint8_t & x, uint8_t n){
  if (n > 7) x ^= (0x100000000 << (n - 8));
  else       x ^= (0x1 << n);
}

 
//------------------------------------------------------------------------------
uint8_t bitRead8(uint8_t & x, uint8_t bit){
  return ((x & (1ULL << bit)) > 0);
}

//------------------------------------------------------------------------------
void bitWrite8(uint8_t & x, uint8_t bit, uint8_t value){
  if (value) bitSet8(x, bit);
  else bitClear8(x, bit);
} 



//==============================================================================
String getNumericResult(uint32_t nVal,uint32_t nOffs,uint32_t nScale,uint8_t nOpt,uint8_t nDecimals,boolean nFormat){
int32_t nnOffs=(int32_t) nOffs;
float rr=getFloatFromInt(nScale),erg=0.000;
boolean nTrunc=((nOpt & 0x08)>0);
String str="";
 nnOffs+=nVal; erg=rr*nnOffs;
 //TEST
 //Serial.println(nVal); 
 //Serial.println(nnOffs); 
 //Serial.println(rr); 
 //Serial.println("Result_erg=" + String(erg,3)); 
    //exponent view
    if (nFormat){
      if (nDecimals==0) nDecimals=1;     
     str=sci(erg,nDecimals,nTrunc);
    }else {
      //truncate mode
      if (nTrunc){
        uint32_t xp=pow(10,nDecimals);
        erg=trunc(erg*xp)/xp;  
      }
     //str=String(erg,2);
     str=String(erg,(unsigned int) nDecimals);
     //str=String((int)erg,nDecimals);
    }
    //LeadingBlanks
    if ((nOpt & 0x04)>0){
      if (erg==0) str="";
       //Leading Zeros
       if ((nOpt & 0x02)==0) {
         str.replace("0.","."); 
       }
    }
    //
   str.trim();
 return str;
}
   

//==============================================================================
// FLOAT REPRESENTATION HELPERS
//==============================================================================
char __mathHelperBuffer[17];

char * sci(double number, int digits,boolean nTrunc){
int exponent = 0,pos = 0;
  // Handling these costs 13 bytes RAM
  // shorten them with N, I, -I ?
  if (isnan(number)) {
    strcpy(__mathHelperBuffer, "nan");
    return __mathHelperBuffer;
  }
  if (isinf(number)) {
    if (number < 0) strcpy(__mathHelperBuffer, "-inf");
    strcpy(__mathHelperBuffer, "inf");
    return __mathHelperBuffer;
  }
  //
  // Handle negative numbers
  bool neg = (number < 0.0);
  if (neg) {
    __mathHelperBuffer[pos++] = '-';
    number = -number;
  }
  //
  while (number >= 10.0) {
    number /= 10;  exponent++;
  }
  while (number < 1 && number != 0.0) {
    number *= 10;  exponent--;
  }
  //
  if (!nTrunc){
  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i){
    rounding *= 0.1;
  }
  number += rounding;
  }
  
  if (number >= 10){
    exponent++;  number /= 10;
  }

  // Extract the integer part of the number and print it
  uint8_t d = (uint8_t)number;
  double remainder = number - d;
  __mathHelperBuffer[pos++] = d + '0';   // 1 digit before decimal point
  if (digits > 0){
    __mathHelperBuffer[pos++] = '.';  // decimal point TODO:rvdt CONFIG?
  }
  // Extract digits from the remainder one at a time to prevent missing leading zero's
  while (digits-- > 0){
    remainder *= 10.0;
    d = (uint8_t)remainder;
    __mathHelperBuffer[pos++] = d + '0';
    remainder -= d;
  }
  //
  // print exponent
  __mathHelperBuffer[pos++] = 'E';
  neg = exponent < 0;
  if (neg) {
    __mathHelperBuffer[pos++] = '-';
    exponent = -exponent;
  } else __mathHelperBuffer[pos++] = '+';

  // 3 digits for exponent;           // needed for double
  // d = exponent / 100;
  // __mathHelperBuffer[pos++] = d + '0';
  // exponent -= d * 100;

  // 2 digits for exponent
  d = exponent / 10;
  __mathHelperBuffer[pos++] = d + '0';
  d = exponent - d*10;
  __mathHelperBuffer[pos++] = d + '0';

  __mathHelperBuffer[pos] = '\0';
  //
  return __mathHelperBuffer;
}

//==============================================================================
void sci(Stream &str, float f, uint8_t digits,boolean nTrunc){
  str.print(sci(f, digits,nTrunc));
}


//==============================================================================
String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c +='0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}
//==============================================================================

//procedure UniCode 
//------------------------------------------------------------------------------
String getUniCodeInfo(String str,boolean setLine) {
uint16_t len=str.length();  
String tStr="";
wchar_t wm=0x0000;
  if (len>1) {
    for (int k=0;k<len-1;k+=2){
     wm=char(str[k])+(char(str[k+1])<<8); 
     tStr+=getStringHEX(wm,4);
      if (setLine) tStr+="\n";
    } //for k
  }
 return tStr;
}



//------------------------------------------------------------------------------
int16_t getUniCodeIndexOf(String str,wchar_t wc0, wchar_t wc1) {
int16_t j=-1;  
uint16_t len=str.length();  
wchar_t wm=0x0000;
  if (len>1) {
    for (int k=0;k<len-1;k+=2){
     wm=char(str[k])+(char(str[k+1])<<8); 
      if (wm==wc0) {
        if ((wc1>0) && ((k+2)<(len-1))) {
          wm=char(str[k+2])+(char(str[k+3])<<8); 
           if (wm==wc1) {
            j=k;break;
           }
        } else {
          j=k;break;
        }
      }
    } //for k
  }
 return j;
}


//------------------------------------------------------------------------------
String getUniCodeReplace(String str,wchar_t wc0, wchar_t wc1) {
int16_t j=-1;  
uint16_t len=str.length();  
wchar_t wm=0x0000;
  if (len>1) {
    for (int k=0;k<len-1;k+=2){
     wm=char(str[k])+(char(str[k+1])<<8); 
      if (wm==wc0) {
        str[k]  =char((wc1>>0) & 0xFF);
        str[k+1]=char((wc1>>8) & 0xFF);
      }
    } //for k
  }
 return str;
}


//------------------------------------------------------------------------------
String getUniCodeSubstring(String str,uint16_t a,uint16_t cc) {
String tStr="";
uint16_t len=str.length();  
  if (len>1) {
    for (int k=0;k<len-1;k+=2){
      if ((a<len-1) &&  (k>=a) && (cc>0)){
       tStr+=str[k];
       tStr+=str[k+1];
       cc--; 
      }
      if (cc==0) break;
    } //for k
  }
 return tStr;
}

//------------------------------------------------------------------------------
String getUniCodeRemove(String str,uint16_t a,uint16_t cc) {
String tStr="";
uint16_t len=str.length();  
  if (len>1)  {
    for (int k=0;k<len-1;k+=2){
      if ((a<len-1) &&  (k>=a) && (cc>0)){
       cc--; 
      }else{
       tStr+=str[k];
       tStr+=str[k+1];
      }
    } //for k
  }
 return tStr;
}

//==============================================================================
boolean HasInArray(uint8_t eIdx, const int8_t objSet[]){
boolean valid=false;
 uint8_t nr=objSet[0];
   if (nr>0) {
     for (int i=1;i<=nr;i++) {
       if (objSet[i]<0) {
        if ((eIdx>=objSet[i-1]) && (eIdx<=abs(objSet[i]))) {
          valid=true;
          break;
        }
       } else {
        if (eIdx==objSet[i]){
          valid=true;
          break; 
        }
       }
     }//for i
   }
 //
 return valid;
};//HasInArray




//==============================================================================
String getStringHEX(uint32_t valHex,uint8_t len){
String ss=String(valHex,HEX); ss.toUpperCase();
   if (len>0) {
     while (ss.length()<len) ss="0" + ss;
   }  
 return ss;
}; //getStringHEX


//------------------------------------------------------------------------------
String getStringHEXInfo(String str,boolean info) {
uint16_t lCount=str.length(),k=0;
uint8_t cc=0;
String ss="",rStr="";
  if (lCount>0){
    if (info) Serial.println(delm0);
       for (k=0;k<str.length();k++){
         if ((k>0) && (k % 16==0)) {
            if (info) Serial.println(ss); 
          rStr+=ss;ss="";
         }     
         //
        cc=char(str[k]);
        ss+=getStringHEX(cc,2);ss+="|";
       }//for k
       if (ss.length()>0) {
          if (info) Serial.println(ss);
        rStr+=ss;
       }
       //     
       if (info) Serial.println(delm0);
  }//lCount>0
 return rStr; 
};


//==============================================================================
String getStringDEC(uint32_t valDEC,uint8_t len){
String ss=String(valDEC); ss.toUpperCase();
   if (len>0) {
     while (ss.length()<len) ss="0" + ss;
   }  
 return ss;
}; //getStringDEC



//==============================================================================
String getMsgFrameStr(CANMsg *pMsg){
String str="";
int i=0;
  str+=getStringHEX(pMsg->ID,8) + "  ";
  str+=getStringHEX(pMsg->LEN,2) + " ";
    for (i=0;i<pMsg->LEN;i++) str+=" " + getStringHEX(pMsg->DATA[i],2);
  return str;
};



//==============================================================================
float getFloatFromInt(uint32_t int4){
union
  {
    long yy;
    float zz;
  }data;
  data.yy = int4;
return data.zz;
};


//==============================================================================
double getDoubleFromInt(uint32_t int4){
union
  {
    long yy;
    double zz;
  }data;
  data.yy = int4;
return data.zz;
};


//==============================================================================
//==============================================================================
//LoopbackStream
//==============================================================================
uint32_t LoopbackStream::setNewBufferSize(uint32_t bSize,boolean psRAM) {
  if (this->buffer!=NULL) free(this->buffer);
  //
  if (bSize>0) {
    if (psRAM){
      this->buffer = (uint8_t*) ps_malloc(bSize);
    }else {
        if (bSize>16000) bSize=16000;
        //if (bSize>12000) bSize=12000;
        //if (bSize>8000) bSize=8000;
        //if (bSize>6000) bSize=6000;
        //if (bSize>1000) bSize=1000;
         
      this->buffer = (uint8_t*) malloc(bSize);
    }
   this->buffer_size=bSize;
  }
 this->pos=0;this->size=0;
 return this->buffer_size;
};


//==============================================================================
uint32_t LoopbackStream::clear(boolean psRAM) {
 this->pos=0;this->size=0;
 return this->buffer_size;
 // return setNewBufferSize(this->buffer_size,psRAM);
};


//==============================================================================
uint8_t* LoopbackStream::getBuffer() {
  return this->buffer;
};


//==============================================================================
uint32_t LoopbackStream::getBufferSize() {
  return this->buffer_size;
};


//==============================================================================
uint32_t LoopbackStream::available() {
 return this->size-this->pos;
};


//==============================================================================
uint32_t LoopbackStream::setPos(uint32_t bPos) {
 return this->pos=bPos;
};


//==============================================================================
uint32_t LoopbackStream::getPos() {
 return this->pos;
};


//==============================================================================
uint32_t LoopbackStream::setSize(uint32_t bSize) {
 return this->size=bSize;
};


//==============================================================================
uint32_t LoopbackStream::getSize() {
 return this->size;
};



//==============================================================================
uint32_t LoopbackStream::write(uint8_t b) {
 this->buffer[this->size]=b; 
   if (this->size<this->buffer_size) this->size++;
 this->pos=0;
 return this->size;
};

//==============================================================================
uint32_t LoopbackStream::writeBytes(uint8_t *buff,uint32_t bSize,int32_t bPos,uint32_t buffPos) {
boolean setSize=false;
uint8_t*  ptr= buff; 
 buff=0; 
 ptr+=buffPos;
   if (bPos<0) {
     bPos=this->size; setSize=true;   
   }
   //
   for (uint32_t i=0;i<bSize;i++){;
     if (bPos+i<this->buffer_size) { 
       this->buffer[bPos+i] = *ptr;
       ptr++;
         if (setSize) this->size++;
    }
   } //for i 
   //
 this->pos=0;
 //
 return bSize;
};


//==============================================================================
uint32_t LoopbackStream::writeBytesVal(uint32_t dVal,uint32_t bSize,int32_t bPos) {
boolean setSize=false;
   if (bPos<0) {
     bPos=this->size; setSize=true;   
   }
   for (uint32_t i=0;i<bSize;i++){
     if (bPos+i<this->buffer_size) { 
      this->buffer[bPos+i] = (dVal>>8*i) & 0xFF;
       if (setSize) this->size++;
     }
   }
 this->pos=0;  
 return bSize;
};


//==============================================================================
uint32_t LoopbackStream::removeBytes(uint32_t pPos0,uint32_t pPos1) {
  if (pPos1>pPos0) {
    uint32_t bSize=this->size;
    uint32_t len=bSize-pPos1;
    writeBytes((uint8_t*) this->buffer,len,pPos0,pPos1);
    this->size=pPos0+len;
    this->pos=0;
  }
 return this->size;
};


//==============================================================================
uint32_t LoopbackStream::readBytes(uint8_t *buff,uint32_t bSize,int32_t bPos) {
uint8_t*  ptr= buff; 
boolean   setPos=false;
 buff=0; 
   if (bPos<0) {
    bPos=this->pos;setPos=true;
   }
   //
   for (uint32_t i=0;i<bSize;i++){
     if (bPos+i<this->size) { 
       *ptr=this->buffer[bPos+i];
       ptr++;
        if (setPos) this->pos++; 
        if (this->pos>=this->size) {
          this->pos=this->size;
          break;
        }
     }
   } //for i 
 return bSize;
};

//==============================================================================
uint32_t LoopbackStream::readBytesVal(uint32_t bSize,int32_t bPos) {
uint32_t dVal=0;
boolean setPos=false;
   if (bPos<0){
    bPos=this->pos;setPos=true;
   }
   //
   for (uint32_t i=0;i<bSize;i++) {
     if (bPos+i<this->size) { 
      dVal+=this->buffer[bPos+i]<<8*i;
        if (setPos) this->pos++; 
        if (this->pos>=this->size) {
          this->pos=this->size;
          break;
        }
     }
   }
   //
 return dVal;
};


//==============================================================================
String LoopbackStream::readBytesString(uint32_t bSize,int32_t bPos) {
String sVal="";
boolean setPos=false;
   if (bPos<0){
    bPos=this->pos;setPos=true;
   }
   //
   for (uint32_t i=0;i<bSize;i++) {
     if (bPos+i<this->size) { 
      sVal+=char(this->buffer[bPos+i]);
        if (setPos) this->pos++; 
        if (this->pos>=this->size) {
          this->pos=this->size;
          break;
        }
     }
   }
   //
 return sVal;
};



//==============================================================================
uint8_t LoopbackStream::read() {
uint8_t b=0;
  if (this->pos<this->size) {
    b=this->buffer[this->pos++];
  }
  if (this->pos>=this->size) this->pos=this->size;
 return b;  
};

//==============================================================================
uint8_t LoopbackStream::peek() {
 return this->buffer[this->pos];
};


//==============================================================================
//class TVTDateTime
//==============================================================================
void TVTDateTime::Str2Time(void) {
  Second = (asc[0] - 0x30) * 10 + asc[1] - 0x30;
  Minute = (asc[2] - 0x30) * 10 + asc[3] - 0x30;
  Hour = (asc[4] - 0x30) * 10 + asc[5] - 0x30;
  /*
  uint8_t Hour;
  uint8_t Week;
  uint8_t Day;
  uint8_t Month;
  uint8_t  Year;
  */
};//TVTDateTime::Str2Time

//==============================================================================
void TVTDateTime::DataMask() {
  trdata[0] = trdata[0] & 0x7f; 
  trdata[1] = trdata[1] & 0x7f; 
  trdata[2] = trdata[2] & 0x3f;  

  trdata[3] = trdata[3] & 0x3f;  
  trdata[4] = trdata[4] & 0x07;  
  trdata[5] = trdata[5] & 0x1f;  

  trdata[6] = trdata[6] & 0xff;  
};//TVTDateTime::DataMask



//==============================================================================
void TVTDateTime::Bcd2asc(void) {
  uint8_t i, j;
  for (j = 0, i = 0; i < 7; i++) {
    asc[j++] =
        (trdata[i] & 0xf0) >> 4 | 0x30; 
    asc[j++] = (trdata[i] & 0x0f) | 0x30;
  }
};//TVTDateTime::Bcd2asc

//==============================================================================
uint8_t TVTDateTime::Bcd2ToByte(uint8_t Value) {
  uint8_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
};//TVTDateTime::Bcd2ToByte

//==============================================================================
uint8_t TVTDateTime::ByteToBcd2(uint8_t Value) {
  uint8_t bcdhigh = 0;

  while (Value >= 10) {
    bcdhigh++;
    Value -= 10;
  }
  return ((uint8_t)(bcdhigh << 4) | Value);
};//TVTDateTime::ByteToBcd2


//==============================================================================
void TVTDateTime::GetTime(VT_TimeTypeDef *VT_TimeStruct) {
  uint8_t buf[3] = {0};
  VT_TimeStruct->Seconds = Bcd2ToByte(buf[0] & 0x7f);  //秒
  VT_TimeStruct->Minutes = Bcd2ToByte(buf[1] & 0x7f);  //分
  VT_TimeStruct->Hours = Bcd2ToByte(buf[2] & 0x3f);    //时
};//TVTDateTime::GetTime


//==============================================================================
int TVTDateTime::SetTime(VT_TimeTypeDef *VT_TimeStruct) {
  if (VT_TimeStruct == NULL || VT_TimeStruct->Hours > 24 ||
      VT_TimeStruct->Minutes > 60 || VT_TimeStruct->Seconds > 60)
    return 0;
  /*
  Wire1.beginTransmission(0x51);
  Wire1.write(0x02);
  Wire1.write(ByteToBcd2(RTC_TimeStruct->Seconds));
  Wire1.write(ByteToBcd2(RTC_TimeStruct->Minutes));
  Wire1.write(ByteToBcd2(RTC_TimeStruct->Hours));
  Wire1.endTransmission();
  */
  return 1;
};//TVTDateTime::SetTime

//==============================================================================
void TVTDateTime::GetDate(VT_DateTypeDef *VT_DateStruct) {
  uint8_t buf[4] = {0};

  VT_DateStruct->Date = Bcd2ToByte(buf[0] & 0x3f);
  VT_DateStruct->WeekDay = Bcd2ToByte(buf[1] & 0x07);
  VT_DateStruct->Month = Bcd2ToByte(buf[2] & 0x1f);

  if (buf[2] & 0x80) {
    VT_DateStruct->Year = 1900 + Bcd2ToByte(buf[3] & 0xff);
  } else {
    VT_DateStruct->Year = 2000 + Bcd2ToByte(buf[3] & 0xff);
  }
};//TVTDateTime::GetDate

//==============================================================================
int TVTDateTime::SetDate(VT_DateTypeDef *VT_DateStruct) {
  if (VT_DateStruct == NULL || VT_DateStruct->WeekDay > 7 ||
      VT_DateStruct->Date > 31 || VT_DateStruct->Month > 12)
    return 0;
  return 1;
};//TVTDateTime::SetDate



//
//==============================================================================
//==============================================================================
  
