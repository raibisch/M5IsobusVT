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
};//bitSet8

//------------------------------------------------------------------------------
void bitClear8(uint8_t & x, uint8_t n){
  if (n > 7) x &= ~(0x100000000 << (n - 8));
  else       x &= ~(0x1 << n);
}

//------------------------------------------------------------------------------
void bitToggle8(uint8_t & x, uint8_t n){
  if (n > 7) x ^= (0x100000000 << (n - 8));
  else       x ^= (0x1 << n);
};//bitToggle8

 
//------------------------------------------------------------------------------
uint8_t bitRead8(uint8_t & x, uint8_t bit){
  return ((x & (1ULL << bit)) > 0);
};//bitRead8

//------------------------------------------------------------------------------
void bitWrite8(uint8_t & x, uint8_t bit, uint8_t value){
  if (value) bitSet8(x, bit);
  else bitClear8(x, bit);
};//bitWrite8 



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
};//getNumericResult
   

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
};//sci

//==============================================================================
void sci(Stream &str, float f, uint8_t digits,boolean nTrunc){
  str.print(sci(f, digits,nTrunc));
};//sci


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
};//uint64ToString


//==============================================================================
//procedure UniCode 
//------------------------------------------------------------------------------
//get reference VTFontAttribute
uint16_t getUniCodeFontIndex(String str, uint16_t k, TVT_Net *pVT_Net) {
uint16_t un = 0,len=str.length();
uint8_t fntType=pVT_Net->fntType;
boolean uniCode=pVT_Net->uniCode;
  //check uniCode
    if (uniCode) {fntType=0;len=len/2;}
    //
    //check uniCode
    if (uniCode)  {
       if (2*k+1<2*len) {
         un=char(str[2*k]);
         un+=char(str[2*k+1])<<8;
         //Serial.println(String(un,HEX));
       }// else break;
    
    } else un=char(str[k]);
    //
    switch (fntType) {
      //Latin 9
      case 1:
        switch (un) {
          case 0x00A4: un = 0x20AC; break;
          case 0x00A6: un = 0x0160; break;
          case 0x00A8: un = 0x0161; break;
          case 0x00B4: un = 0x017D; break;
          case 0x00B8: un = 0x017E; break;
          case 0x00BC: un = 0x0152; break;
          case 0x00BD: un = 0x0153; break;
          case 0x00BE: un = 0x0178; break;
        }
        break;
      //MiddleEurope
      case 2:
        switch (un) {
          case 0x00A1: un = 0x0104; break;
          case 0x00A2: un = 0x02D8; break;
          case 0x00A3: un = 0x0141; break;
          case 0x00A5: un = 0x013D; break;
          case 0x00A6: un = 0x015A; break;
          case 0x00A9: un = 0x0160; break;
          case 0x00AA: un = 0x015E; break;
          case 0x00AB: un = 0x0164; break;
          case 0x00AC: un = 0x0179; break;
          case 0x00AD: un = 0x00A0; break;
          case 0x00AE: un = 0x017D; break;
          case 0x00AF: un = 0x017B; break;
          //
          case 0x00B1: un = 0x0105; break;
          case 0x00B2: un = 0x02DB; break;
          case 0x00B3: un = 0x0142; break;
          case 0x00B5: un = 0x013E; break;
          case 0x00B6: un = 0x015B; break;
          case 0x00B7: un = 0x02C7; break;
          case 0x00B9: un = 0x0161; break;
          case 0x00BA: un = 0x015F; break;
          case 0x00BB: un = 0x0165; break;
          case 0x00BC: un = 0x017A; break;
          case 0x00BD: un = 0x02DD; break;
          //
          case 0x00BE: un = 0x017E; break;
          case 0x00BF: un = 0x017C; break;
          //
          case 0x00C0: un = 0x0154; break;
          case 0x00C3: un = 0x0102; break;
          case 0x00C5: un = 0x0139; break;
          case 0x00C6: un = 0x0106; break;
          case 0x00C8: un = 0x010C; break;
          case 0x00CA: un = 0x0118; break;
          case 0x00CC: un = 0x011A; break;
          case 0x00CF: un = 0x010E; break;
          //
          case 0x00D0: un = 0x0110; break;
          case 0x00D1: un = 0x0143; break;
          case 0x00D2: un = 0x0147; break;
          case 0x00D5: un = 0x0150; break;
          case 0x00D8: un = 0x0158; break;
          case 0x00D9: un = 0x016E; break;
          case 0x00DB: un = 0x0170; break;
          case 0x00DE: un = 0x0162; break;
          //
          case 0x00E0: un = 0x0155; break;
          case 0x00E3: un = 0x0103; break;
          case 0x00E5: un = 0x013A; break;
          case 0x00E6: un = 0x0107; break;
          case 0x00E8: un = 0x010D; break;
          case 0x00EA: un = 0x0119; break;
          case 0x00EC: un = 0x011B; break;
          case 0x00EF: un = 0x010F; break;
          //
          case 0x00F0: un = 0x0111; break;
          case 0x00F1: un = 0x0144; break;
          case 0x00F2: un = 0x0148; break;
          case 0x00F5: un = 0x0151; break;
          case 0x00F8: un = 0x0159; break;
          case 0x00F9: un = 0x016F; break;
          case 0x00FB: un = 0x0171; break;
          case 0x00FE: un = 0x0163; break;
          case 0x00FF: un = 0x02D9; break;
        }
        break;

      //Baltic EastEurope
      case 4:
        switch (un) {
          case 0x00A1: un = 0x0104; break;
          case 0x00A2: un = 0x0138; break;
          case 0x00A3: un = 0x0156; break;
          case 0x00A5: un = 0x0128; break;
          case 0x00A6: un = 0x013B; break;
          case 0x00A9: un = 0x0160; break;
          case 0x00AA: un = 0x0114; break;
          case 0x00AB: un = 0x0122; break;
          case 0x00AC: un = 0x0166; break;
          case 0x00AD: un = 0x00A0; break;
          case 0x00AE: un = 0x017D; break;
          //
          case 0x00B1: un = 0x0105; break;
          case 0x00B2: un = 0x02DB; break;
          case 0x00B3: un = 0x0157; break;
          case 0x00B5: un = 0x0129; break;
          case 0x00B6: un = 0x013C; break;
          case 0x00B7: un = 0x02C7; break;
          case 0x00B9: un = 0x0161; break;
          case 0x00BA: un = 0x011B; break;
          case 0x00BB: un = 0x0123; break;
          case 0x00BC: un = 0x0167; break;
          case 0x00BD: un = 0x014A; break;
          case 0x00BE: un = 0x017E; break;
          case 0x00BF: un = 0x014B; break;
          //
          case 0x00C0: un = 0x0100; break;
          case 0x00C3: un = 0x0102; break;
          case 0x00C7: un = 0x012E; break;
          case 0x00C8: un = 0x010C; break;
          case 0x00CA: un = 0x0118; break;
          case 0x00CC: un = 0x011A; break;
          case 0x00CF: un = 0x0128; break;
          //
          case 0x00D0: un = 0x0110; break;
          case 0x00D1: un = 0x0145; break;
          case 0x00D2: un = 0x014C; break;
          case 0x00D3: un = 0x0136; break;
          case 0x00D5: un = 0x0150; break;
          case 0x00D9: un = 0x0172; break;
          case 0x00DD: un = 0x0168; break;
          case 0x00DE: un = 0x016A; break;
          //
          case 0x00E0: un = 0x0101; break;
          case 0x00E3: un = 0x0103; break;
          case 0x00E7: un = 0x012F; break;
          case 0x00E8: un = 0x010D; break;
          case 0x00EA: un = 0x0119; break;
          case 0x00EC: un = 0x011B; break;
          case 0x00EF: un = 0x0129; break;
          //
          case 0x00F0: un = 0x0111; break;
          case 0x00F1: un = 0x0146; break;
          case 0x00F2: un = 0x014D; break;
          case 0x00F3: un = 0x0137; break;
          case 0x00F5: un = 0x0151; break;
          case 0x00F9: un = 0x0173; break;
          case 0x00FD: un = 0x0169; break;
          case 0x00FE: un = 0x016B; break;
          case 0x00FF: un = 0x02D9; break;
        }
        break;
        //
      //Cyrillic
      case 5:
        switch (un) {
          case 0x00F0: un = 0x2116; break;
          case 0x00FD: un = 0x00A7; break;
          //
          default: if (un >= 0x00A0) un = 0x0400 + (un - 0x00A0);
            break;
        }
        break;
      //Greek
      case 7:
        switch (un) {
          case 0x00A1: un = 0x2018; break;
          case 0x00A2: un = 0x2019; break;
          case 0x00A4: un = 0x20AC; break;
          case 0x00A5: un = 0x007F; break;
          case 0x00AA: un = 0x007E; break;
          case 0x00AE: un = 0x0037; break;
          //
          default: if (un >= 0x00B4) un = 0x0384 + (un - 0x00B4);
            break;
        }
        break;
    }//switch fntType 
 return un;   
};//getUniCodeFontIndex



//------------------------------------------------------------------------------
//set back reference VTFontAttribute fntType
uint16_t setUniCodeFontIndex(String str, uint16_t k, TVT_Net *pVT_Net) {
uint16_t un = 0,len=str.length();
uint8_t fntType=pVT_Net->fntType;
boolean uniCode=pVT_Net->uniCode;
  //check uniCode
    if ((uniCode) && (fntType>0)) len=len/2;
    //
    //check uniCode
    if (uniCode)  {
       if (2*k+1<2*len) {
         un=char(str[2*k]);
         un+=char(str[2*k+1])<<8;
         //TEST
         //Serial.println(getStringHEX(un,4));
       }// else break;
    
    } else un=char(str[k]);
    //
    switch (fntType) {
      //Latin 9
      case 1:
        switch (un) {
          case 0x20AC: un = 0x00A4; break;
          case 0x0160: un = 0x00A6; break;
          case 0x0161: un = 0x00A8; break;
          case 0x017D: un = 0x00B4; break;
          case 0x017E: un = 0x00B8; break;
          case 0x0152: un = 0x00BC; break;
          case 0x0153: un = 0x00BD; break;
          case 0x0178: un = 0x00BE; break;
        }
        break;
      //MiddleEurope
      case 2:
        switch (un) {
          case 0x0104: un = 0x00A1 ; break;
          case 0x02D8: un = 0x00A2; break;
          case 0x0141: un = 0x00A3; break;
          case 0x013D: un = 0x00A5; break;
          case 0x015A: un = 0x00A6; break;
          case 0x0160: un = 0x00A9; break;
          case 0x015E: un = 0x00AA; break;
          case 0x0164: un = 0x00AB; break;
          case 0x0179: un = 0x00AC; break;
          case 0x00A0: un = 0x00AD; break;
          case 0x017D: un = 0x00AE; break;
          case 0x017B: un = 0x00AF; break;
          //
          case 0x0105: un = 0x00B1; break;
          case 0x02DB: un = 0x00B2; break;
          case 0x0142: un = 0x00B3; break;
          case 0x013E: un = 0x00B5; break;
          case 0x015B: un = 0x00B6; break;
          case 0x02C7: un = 0x00B7; break;
          case 0x0161: un = 0x00B9; break;
          case 0x015F: un = 0x00BA; break;
          case 0x0165: un = 0x00BB; break;
          case 0x017A: un = 0x00BC; break;
          case 0x02DD: un = 0x00BD; break;
          //
          case 0x017E: un = 0x00BE; break;
          case 0x017C: un = 0x00BF; break;
          //
          case 0x0154: un = 0x00C0; break;
          case 0x0102: un = 0x00C3; break;
          case 0x0139: un = 0x00C5; break;
          case 0x0106: un = 0x00C6; break;
          case 0x010C: un = 0x00C8; break;
          case 0x0118: un = 0x00CA; break;
          case 0x011A: un = 0x00CC; break;
          case 0x010E: un = 0x00CF; break;
          //
          case 0x0110: un = 0x00D0; break;
          case 0x0143: un = 0x00D1; break;
          case 0x0147: un = 0x00D2; break;
          case 0x0150: un = 0x00D5; break;
          case 0x0158: un = 0x00D8; break;
          case 0x016E: un = 0x00D9; break;
          case 0x0170: un = 0x00DB; break;
          case 0x0162: un = 0x00DE; break;
          //
          case 0x0155: un = 0x00E0; break;
          case 0x0103: un = 0x00E3; break;
          case 0x013A: un = 0x00E5; break;
          case 0x0107: un = 0x00E6; break;
          case 0x010D: un = 0x00E8; break;
          case 0x0119: un = 0x00EA; break;
          case 0x011B: un = 0x00EC; break;
          case 0x010F: un = 0x00EF; break;
          //
          case 0x0111: un = 0x00F0; break;
          case 0x0144: un = 0x00F1; break;
          case 0x0148: un = 0x00F2; break;
          case 0x0151: un = 0x00F5; break;
          case 0x0159: un = 0x00F8; break;
          case 0x016F: un = 0x00F9; break;
          case 0x0171: un = 0x00FB; break;
          case 0x0163: un = 0x00FE; break;
          case 0x02D9: un = 0x00FF; break;
        }
        break;

      //Baltic EastEurope
      case 4:
        switch (un) {
          case 0x0104: un = 0x00A1; break;
          case 0x0138: un = 0x00A2; break;
          case 0x0156: un = 0x00A3; break;
          case 0x0128: un = 0x00A5; break;
          case 0x013B: un = 0x00A6; break;
          case 0x0160: un = 0x00A9; break;
          case 0x0114: un = 0x00AA; break;
          case 0x0122: un = 0x00AB; break;
          case 0x0166: un = 0x00AC; break;
          case 0x00A0: un = 0x00AD; break;
          case 0x017D: un = 0x00AE; break;
          //
          case 0x0105: un = 0x00B1; break;
          case 0x02DB: un = 0x00B2; break;
          case 0x0157: un = 0x00B3; break;
          case 0x0129: un = 0x00B5; break;
          case 0x013C: un = 0x00B6; break;
          case 0x02C7: un = 0x00B7; break;
          case 0x0161: un = 0x00B9; break;
          case 0x011B: un = 0x00BA; break;
          case 0x0123: un = 0x00BB; break;
          case 0x0167: un = 0x00BC; break;
          case 0x014A: un = 0x00BD; break;
          case 0x017E: un = 0x00BE; break;
          case 0x014B: un = 0x00BF; break;
          //
          case 0x0100: un = 0x00C0; break;
          case 0x0102: un = 0x00C3; break;
          case 0x012E: un = 0x00C7; break;
          case 0x010C: un = 0x00C8; break;
          case 0x0118: un = 0x00CA; break;
          case 0x011A: un = 0x00CC; break;
          //case 0x0128: un = 0x00CF; break;
          //
          case 0x0110: un = 0x00D0; break;
          case 0x0145: un = 0x00D1; break;
          case 0x014C: un = 0x00D2; break;
          case 0x0136: un = 0x00D3; break;
          case 0x0150: un = 0x00D5; break;
          case 0x0172: un = 0x00D9; break;
          case 0x0168: un = 0x00DD; break;
          case 0x016A: un = 0x00DE; break;
          //
          case 0x0101: un = 0x00E0; break;
          case 0x0103: un = 0x00E3; break;
          case 0x012F: un = 0x00E7; break;
          case 0x010D: un = 0x00E8; break;
          case 0x0119: un = 0x00EA; break;
          //case 0x011B: un = 0x00EC; break;
          //case 0x0129: un = 0x00EF; break;
          //
          case 0x0111: un = 0x00F0; break;
          case 0x0146: un = 0x00F1; break;
          case 0x014D: un = 0x00F2; break;
          case 0x0137: un = 0x00F3; break;
          case 0x0151: un = 0x00F5; break;
          case 0x0173: un = 0x00F9; break;
          case 0x0169: un = 0x00FD; break;
          case 0x016B: un = 0x00FE; break;
          case 0x02D9: un = 0x00FF; break;
        }
        break;
        //
      //Cyrillic
      case 5:
        switch (un) {
          case 0x2116: un = 0x00F0; break;
          case 0x00A7: un = 0x00FD; break;
          //
          default: if (un >= 0x0400) {
                     //TEST
                     //Serial.println(getStringHEX(un,4));
                     un = 0x00A0 + (un - 0x0400);
                   }
            break;
        }
        break;
      //Greek
      case 7:
        switch (un) {
          case 0x2018: un = 0x00A1; break;
          case 0x2019: un = 0x00A2; break;
          case 0x20AC: un = 0x00A4; break;
          case 0x007F: un = 0x00A5; break;
          case 0x007E: un = 0x00AA; break;
          case 0x0037: un = 0x00AE; break;
          //
          default: if (un >= 0x0384) un = 0x00B4 + (un - 0x0384);
            break;
        }
        break;
    }//switch fntType 
 return un;   
};//setUniCodeFontIndex


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
};//getUniCodeInfo



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
};//getUniCodeIndexOf


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
};//getUniCodeReplace


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
};//getUniCodeSubstring

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
};//getUniCodeRemove


//------------------------------------------------------------------------------
String setUniCodeStrToASCII(String uStr) {
String aStr="";  
  for (int i=0;i<uStr.length();i++){
    if (char(uStr[i])>0) aStr+=uStr[i];
  }//for i
return aStr;
};//setUniCodeStrToASCII


//------------------------------------------------------------------------------
String setASCIItoUniCodeStr(String aStr) {
String uStr="";  
  for (int i=0;i<aStr.length();i++){
    uStr+=aStr[i]; uStr+=char(0);
  }//for i
return uStr;
};//setASCIItoUniCodeStr


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
//TrimLeft blanks
String getStringLeftTrim(String str){
   while ((str.length()>0) && (str[0]=' ')) str.remove(0);
 return str;
}; //getStringRightTrim


//==============================================================================
//TrimRight blanks
String getStringRightTrim(String str){
   while ((str.length()>0) && (str.endsWith(" "))) str.remove(str.length()-1);
 return str;
}; //getStringRightTrim


//==============================================================================
String getStringHEX(uint32_t valHex,uint8_t len){
String ss=String(valHex,HEX); ss.toUpperCase();
   if (len>0) {
     while (ss.length()<len) ss="0" + ss;
   }  
 return ss;
}; //getStringHEX


//==============================================================================
boolean IsStringNumeric(String str){
boolean valid=true;
uint8_t nn=0;
  if (str.length()>0){
    for (int i=0; i<str.length();i++){
     nn=char(str[i]);
     valid=((nn>=0x30) && (nn<=0x39));
       if (!valid) break;
    }//for i
  } else valid=false;
  //  
 return valid;
}; //IsStringNumeric
         


//=============================================================================
int16_t hexCharacterToInt(String str) {
int16_t k=-1,i=0;
String sStr="",tStr="";
 str.toUpperCase();
  if (str.indexOf("0X")==0) str.replace("0X","");
  //
  while (str.length()>0) {
    sStr=str.substring(0,2); str.remove(0,2);
     for (i=0;i<256;i++) {
         tStr=getStringHEX(i,2);
           if (tStr==sStr){
             k=i;
             break; 
           }
     }//for i
  }//while
  //
 return k;  
};//hexCharacterToInt



//==============================================================================
int16_t getIntFromHEX(String str){
int16_t k=-1;
 str.toUpperCase();
  if (str.length()>0){
    if (str.indexOf("0X")==0) {
      k=hexCharacterToInt(str); 
    }else{
      if (IsStringNumeric(str)) k=str.toInt(); 
    }
  }
  //  
 return k;
}; //getIntFromHEX



//=============================================================================
uint16_t hexCharacterToObjID(String str) {
int16_t i=0,j=1;
uint16_t objID=0xFFFF;
String sStr="",tStr="";
 str.toUpperCase();
  if (str.indexOf("0X")==0) str.replace("0X","");
  //
  while ((j>=0) && (str.length()>0)) {
    sStr=str.substring(0,2); str.remove(0,2);
     for (i=0;i<256;i++) {
      tStr=getStringHEX(i,2);
        if (tStr==sStr){
           if (j==1) objID=0;
          objID+=i<<(8*j);
          break; 
        }
     }//for i
   j--;  
  }//while
  //
 return objID;  
};//hexCharacterToObjID


//=============================================================================
uint64_t hexCharacterToInt64(String str,int8_t len) {
uint16_t i=0;
uint64_t value=0;
String sStr="",tStr="";
 str.toUpperCase();
  if (str.indexOf("0X")==0) str.replace("0X","");
  //
  while ((len>=0) && (str.length()>0)) {
    sStr=str.substring(0,2); str.remove(0,2);
     for (i=0;i<256;i++) {
      tStr=getStringHEX(i,2);
        if (tStr==sStr){
          value+=i<<(8*len);
          break; 
        }
     }//for i
   len--; 
  }//while
  //
 return value;  
};//hexCharacterToInt64


//------------------------------------------------------------------------------
void getAddressInfo(TVT_Net *pVT_Net) {
 Serial.println(delm0);
 Serial.print("VT="   + getStringHEX(pVT_Net->VT_SRC,2) + "\t"); 
 Serial.println("VT_WS="   + pVT_Net->VT_SRC_WS); 
 //
 Serial.print("ECU0=" + getStringHEX(pVT_Net->VT_DST[0],2) + "\t"); 
 Serial.println("ECU0_WS="   + pVT_Net->VT_DST_WS[0]); 
 //             
 Serial.print("ECU1=" + getStringHEX(pVT_Net->VT_DST[1],2) + "\t"); 
 Serial.println("ECU1_WS="   + pVT_Net->VT_DST_WS[1]); 
 Serial.println(delm0);
};//getAddressInfo()



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
};//getStringHEXInfo


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
};//getMsgFrameStr



//==============================================================================
float getFloatFromInt(uint32_t int4){
union
  {
    long yy;
    float zz;
  }data;
  data.yy = int4;
return data.zz;
};//getFloatFromInt


//==============================================================================
double getDoubleFromInt(uint32_t int4){
union
  {
    long yy;
    double zz;
  }data;
  data.yy = int4;
return data.zz;
};//getDoubleFromInt


//==============================================================================
//reset all selected Inputs
void resetSelectInput(TVT_Net *pVT_Net){
  pVT_Net->TFT_KeySelect=0;
  pVT_Net->TFT_ButtonSelect=0;
  pVT_Net->TFT_InputSelect=0;
  //
  pVT_Net->TFT_KeySelectObjID=0xFFFF;
  pVT_Net->TFT_ButtonSelectObjID=0xFFFF;
  pVT_Net->TFT_InputSelectObjID=0xFFFF;
};//resetSelectInput

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
};//LoopbackStream::setNewBufferSize


//==============================================================================
uint32_t LoopbackStream::clear(boolean psRAM) {
 this->pos=0;this->size=0;
 return this->buffer_size;
 // return setNewBufferSize(this->buffer_size,psRAM);
};//LoopbackStream::clear


//==============================================================================
uint8_t* LoopbackStream::getBuffer() {
  return this->buffer;
};//LoopbackStream::getBuffer


//==============================================================================
uint32_t LoopbackStream::getBufferSize() {
  return this->buffer_size;
};//LoopbackStream::getBufferSize


//==============================================================================
uint32_t LoopbackStream::available() {
 return this->size-this->pos;
};//LoopbackStream::available


//==============================================================================
uint32_t LoopbackStream::setPos(uint32_t bPos) {
 return this->pos=bPos;
};//LoopbackStream::setPos


//==============================================================================
uint32_t LoopbackStream::getPos() {
 return this->pos;
};//LoopbackStream::getPos


//==============================================================================
uint32_t LoopbackStream::setSize(uint32_t bSize) {
 return this->size=bSize;
};//LoopbackStream::setSize


//==============================================================================
uint32_t LoopbackStream::getSize() {
 return this->size;
};//LoopbackStream::getSize



//==============================================================================
uint32_t LoopbackStream::write(uint8_t b) {
 this->buffer[this->size]=b; 
   if (this->size<this->buffer_size) this->size++;
 this->pos=0;
 return this->size;
};//LoopbackStream::write

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
};//LoopbackStream::writeBytes


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
};//LoopbackStream::writeBytesVal


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
};//LoopbackStream::removeBytes


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
};//LoopbackStream::readBytes

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
};//LoopbackStream::readBytesVal


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
};//LoopbackStream::readBytesString



//==============================================================================
uint8_t LoopbackStream::read() {
uint8_t b=0;
  if (this->pos<this->size) {
    b=this->buffer[this->pos++];
  }
  if (this->pos>=this->size) this->pos=this->size;
 return b;  
};//LoopbackStream::read

//==============================================================================
uint8_t LoopbackStream::peek() {
 return this->buffer[this->pos];
};//LoopbackStream::peek


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
  
