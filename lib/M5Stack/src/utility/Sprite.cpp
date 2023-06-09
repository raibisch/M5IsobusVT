/**************************************************************************************
// The following class creates Sprites in RAM, graphics can then be drawn in the Sprite
// and rendered quickly onto the TFT screen. The class inherits the graphics functions
// from the TFT_eSPI class. Some functions are overridden by this class so that the
// graphics are written to the Sprite rather than the TFT.
// Coded by Bodmer, see license file in root folder
***************************************************************************************/
/***************************************************************************************
// Color bytes are swapped when writing to RAM, this introduces a small overhead but
// there is a nett performance gain by using swapped bytes.
***************************************************************************************/
#include "Sprite.h"


// Clipping macro for pushImage
#define PI_CLIP                                        \
  if (_vpOoB) return;                                  \
  x+= _xDatum;                                         \
  y+= _yDatum;                                         \
                                                       \
  if ((x >= _vpW) || (y >= _vpH)) return;              \
                                                       \
  int32_t dx = 0;                                      \
  int32_t dy = 0;                                      \
  int32_t dw = w;                                      \
  int32_t dh = h;                                      \
                                                       \
  if (x < _vpX) { dx = _vpX - x; dw -= dx; x = _vpX; } \
  if (y < _vpY) { dy = _vpY - y; dh -= dy; y = _vpY; } \
                                                       \
  if ((x + dw) > _vpW ) dw = _vpW - x;                 \
  if ((y + dh) > _vpH ) dh = _vpH - y;                 \
                                                       \
  if (dw < 1 || dh < 1) return; 


/***************************************************************************************
** Function name:           TFT_eSprite
** Description:             Class constructor
*************************************************************************************x*/
TFT_eSprite::TFT_eSprite(TFT_eSPI *tft) {
  _tft = tft;  // Pointer to tft class so we can call member functions

  _iwidth = 0;  // Initialise width and height to 0 (it does not exist yet)
  _iheight = 0;
  _bpp = 16;
  _swapBytes = false;  // Do not swap pushImage colour bytes by default

  _created = false;
  _vpOoB   = true;

  _xs = 0;  // window bounds for pushColor
  _ys = 0;
  _xe = 0;
  _ye = 0;

  _xptr = 0;  // pushColor coordinate
  _yptr = 0;

  _colorMap = nullptr;

  _psram_enable = true; 

  _xpivot = 0;
  _ypivot = 0;

  this->cursor_y = this->cursor_x = 0;  // Text cursor position
}

/**************************************************************************************\
** Function name:           setPsram
** Description:             Enable/disable psram for the current sprite
\**************************************************************************************/

void TFT_eSprite::setPsram( bool enable )
{
  _usePsram = enable;
}



/***************************************************************************************
** Function name:           createSprite
** Description:             Create a sprite (bitmap) of defined width and height
*************************************************************************************x*/
// cast returned value to (uint8_t*) for 8 bit or (uint16_t*) for 16 bit colours
void *TFT_eSprite::createSprite(int16_t w, int16_t h, uint8_t frames) {
  if (_created) return _img8_1;

  if (w < 1 || h < 1) return NULL;

  _iwidth = _dwidth = _bitwidth = w;
  _iheight = _dheight = h;

  this->cursor_x = 0;
  this->cursor_y = 0;

  // Default scroll rectangle and gap fill colour
  _sx = 0;
  _sy = 0;
  _sw = w;
  _sh = h;
  _scolor = TFT_BLACK;

  _xpivot = w / 2;
  _ypivot = h / 2;

  _img8 = (uint8_t *)callocSprite(w, h, frames);
  _img8_1 = _img8;
  _img8_2 = _img8;
  _img = (uint16_t *)_img8;

  // This is to make it clear what pointer size is expected to be used
  // but casting in the user sketch is needed due to the use of void*
  if (_bpp == 1) {
    w = (w + 7) & 0xFFF8;
    _img8_2 = _img8 + ((w >> 3) * h + 1);
  }

  if (_img8) {
    _created = true;
    rotation = 0;
    setViewport(0, 0, _dwidth, _dheight);
    setPivot(_iwidth/2, _iheight/2); 
    return _img8;
  }

  return NULL;
}


/***************************************************************************************
** Function name:           getPointer
** Description:             Returns pointer to start of sprite memory area
***************************************************************************************/
void* TFT_eSprite::getPointer(void)
{
  if (!_created) return nullptr;
  return _img8_1;
} 


/***************************************************************************************
** Function name:           callocSprite
** Description:             Allocate a memory area for the Sprite and return pointer
*************************************************************************************x*/

void *TFT_eSprite::callocSprite(int16_t w, int16_t h, uint8_t frames) {
  // Add one extra "off screen" pixel to point out-of-bounds setWindow() coordinates
  // this means push/writeColor functions do not need additional bounds checks and
  // hence will run faster in normal circumstances.
  uint8_t *ptr8 = NULL;

  if (_bpp == 16) {
    if (psramFound() && _usePsram)
      ptr8 = (uint8_t *)ps_calloc(w * h + 1, sizeof(uint16_t));
    else
      // Since it's a M5Core2, disabled psram for this sprite **is** a user choice,
      // using heap_caps_calloc instead of calloc prevents auto assignation to psram
      ptr8 = (uint8_t *)heap_caps_calloc(w * h + 1, sizeof(uint16_t), MALLOC_CAP_8BIT);
  }
  else if (_bpp == 8) {
    if (psramFound() && _usePsram)
      ptr8 = (uint8_t *)ps_calloc(w * h + 1, sizeof(uint8_t));
    else
      // Since it's a M5Core2, disabled psram for this sprite **is** a user choice,
      // using heap_caps_calloc instead of calloc prevents auto assignation to psram
      ptr8 = (uint8_t *)heap_caps_calloc(w * h + 1, sizeof(uint8_t), MALLOC_CAP_8BIT);
  }
  else  // Must be 1 bpp
  {
    //_dwidth   Display width+height in pixels always in rotation 0 orientation
    //_dheight  Not swapped for sprite rotations
    // Note: for 1bpp _iwidth and _iheight are swapped during Sprite rotations

    w = (w + 7) &
        0xFFF8;  // width should be the multiple of 8 bits to be compatible with epdpaint
    _iwidth =
        w;  // _iwidth is rounded up to be multiple of 8, so might not be = _dwidth
    _bitwidth = w;

    if (frames > 2) frames = 2;  // Currently restricted to 2 frame buffers
    if (frames < 1) frames = 1;
    if (psramFound() && _usePsram)
      ptr8 =
          (uint8_t *)ps_calloc(frames * (w >> 3) * h + frames, sizeof(uint8_t));
    else
      // Since it's a M5Core2, disabled psram for this sprite **is** a user choice,
      // using heap_caps_calloc instead of calloc prevents auto assignation to psram
      ptr8 = (uint8_t *)heap_caps_calloc(frames * (w >> 3) * h + frames, sizeof(uint8_t), MALLOC_CAP_8BIT);
  }

  return ptr8;
}

/***************************************************************************************
** Function name:           frameBuffer
** Description:             For 1 bpp Sprites, select the frame used for graphics
*************************************************************************************x*/
// Frames are numbered 1 and 2
void *TFT_eSprite::frameBuffer(int8_t f) {
  if (!_created) return NULL;

  if (_bpp == 16) return _img;

  if (_bpp == 8) return _img8;

  if (f == 2)
    _img8 = _img8_2;
  else
    _img8 = _img8_1;

  return _img8;
}

/***************************************************************************************
** Function name:           setColorDepth
** Description:             Set bits per pixel for colour (1, 8 or 16)
*************************************************************************************x*/

void *TFT_eSprite::setColorDepth(int8_t b) {
  // Can't change an existing sprite's colour depth so delete it
  if (_created) free(_img8_1);

  // Now define the new colour depth
  if (b > 8)
    _bpp = 16;  // Bytes per pixel
  else if (b > 1)
    _bpp = 8;
  else
    _bpp = 1;

  // If it existed, re-create the sprite with the new colour depth
  if (_created) {
    _created = false;
    return createSprite(_iwidth, _iheight);
  }

  return NULL;
}

/***************************************************************************************
** Function name:           getColorDepth
** Description:             Get bits per pixel for colour (1, 8 or 16)
*************************************************************************************x*/

int8_t TFT_eSprite::getColorDepth(void) {
  if (_created)
    return _bpp;
  else
    return 0;
}

/***************************************************************************************
** Function name:           setBitmapColor
** Description:             Set the foreground foreground and background colour
***************************************************************************************/
void TFT_eSprite::setBitmapColor(uint16_t c, uint16_t b) {
  if (c == b) b = ~c;
  _tft->bitmap_fg = c;
  _tft->bitmap_bg = b;
}

/***************************************************************************************
** Function name:           deleteSprite
** Description:             Delete the sprite to free up memory (RAM)
*************************************************************************************x*/
void TFT_eSprite::deleteSprite(void) {
  if (!_created) return;

  free(_img8_1);

  _created = false;
}

/***************************************************************************************
** Function name:           setPivot
** Description:             Set the pivot point in this Sprite
*************************************************************************************x*/
void TFT_eSprite::setPivot(int16_t x, int16_t y) {
  _xpivot = x;
  _ypivot = y;
}

/***************************************************************************************
** Function name:           getPivotX
** Description:             Get the x pivot position
***************************************************************************************/
int16_t TFT_eSprite::getPivotX(void) { return _xpivot; }

/***************************************************************************************
** Function name:           getPivotY
** Description:             Get the y pivot position
***************************************************************************************/
int16_t TFT_eSprite::getPivotY(void) { return _ypivot; }

/***************************************************************************************
** Function name:           pushRotated
** Description:             Push a rotated copy of the Sprite to TFT screen
*************************************************************************************x*/
bool TFT_eSprite::pushRotated(int16_t angle, int32_t transp) {
  if (!_created) return false;

  // Trig values for the rotation
  float radAngle = -angle * 0.0174532925;  // Convert degrees to radians
  float sinra = sin(radAngle);
  float cosra = cos(radAngle);

  // Bounding box parameters
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Get the bounding box of this rotated source Sprite relative to Sprite pivot
  getRotatedBounds(sinra, cosra, width(), height(), _xpivot, _ypivot, &min_x,
                   &min_y, &max_x, &max_y);

  // Move bounding box so source Sprite pivot coincides with TFT pivot
  min_x += _tft->_xpivot;
  max_x += _tft->_xpivot;
  min_y += _tft->_ypivot;
  max_y += _tft->_ypivot;

  // Test only to show bounding box on TFT
  // _tft->drawRect(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, TFT_GREEN);

  // Return if bounding box is outside of TFT area
  if (min_x > _tft->width()) return true;
  if (min_y > _tft->height()) return true;
  if (max_x < 0) return true;
  if (max_y < 0) return true;

  // Clip bounding box to be within TFT area
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x > _tft->width()) max_x = _tft->width();
  if (max_y > _tft->height()) max_y = _tft->height();

  _tft->startWrite();  // ESP32: avoid transaction overhead for every tft pixel

  // Scan destination bounding box and fetch transformed pixels from source Sprite
  for (int32_t x = min_x; x <= max_x; x++) {
    int32_t xt = x - _tft->_xpivot;
    float cxt = cosra * xt + _xpivot;
    float sxt = sinra * xt + _ypivot;
    bool column_drawn = false;
    for (int32_t y = min_y; y <= max_y; y++) {
      int32_t yt = y - _tft->_ypivot;
      int32_t xs = (int32_t)round(cxt - sinra * yt);
      // Do not calculate ys unless xs is in bounds
      if (xs >= 0 && xs < width()) {
        int32_t ys = (int32_t)round(sxt + cosra * yt);
        // Check if ys is in bounds
        if (ys >= 0 && ys < height()) {
          int32_t rp = readPixel(xs, ys);
          if (rp != transp) _tft->drawPixel(x, y, rp);
          column_drawn = true;
        }
      } else if (column_drawn)
        y = max_y;  // Skip remaining column pixels
    }
  }

  _tft->endWrite();  // ESP32: end transaction

  return true;
}

/***************************************************************************************
** Function name:           pushRotated
** Description:             Push a rotated copy of the Sprite to another Sprite
*************************************************************************************x*/
bool TFT_eSprite::pushRotated(TFT_eSprite *spr, int16_t angle, int32_t transp) {
  if (!_created) return false;       // Check this Sprite is created
  if (!spr->_created) return false;  // Ckeck destination Sprite is created

  // Trig values for the rotation
  float radAngle = -angle * 0.0174532925;  // Convert degrees to radians
  float sinra = sin(radAngle);
  float cosra = cos(radAngle);

  // Bounding box parameters
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Get the bounding box of this rotated source Sprite
  getRotatedBounds(sinra, cosra, width(), height(), _xpivot, _ypivot, &min_x,
                   &min_y, &max_x, &max_y);

  // Move bounding box so source Sprite pivot coincides with destination Sprite pivot
  min_x += spr->_xpivot;
  max_x += spr->_xpivot;
  min_y += spr->_ypivot;
  max_y += spr->_ypivot;

  // Test only to show bounding box
  // spr->fillSprite(TFT_BLACK);
  // spr->drawRect(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1, TFT_GREEN);

  // Return if bounding box is completely outside of destination Sprite
  if (min_x > spr->width()) return true;
  if (min_y > spr->height()) return true;
  if (max_x < 0) return true;
  if (max_y < 0) return true;

  // Clip bounding box if it is partially within destination Sprite
  if (min_x < 0) min_x = 0;
  if (min_y < 0) min_y = 0;
  if (max_x > spr->width()) max_x = spr->width();
  if (max_y > spr->height()) max_y = spr->height();

  // Scan destination bounding box and fetch transformed pixels from source Sprite
  for (int32_t x = min_x; x <= max_x; x++) {
    int32_t xt = x - spr->_xpivot;
    float cxt = cosra * xt + _xpivot;
    float sxt = sinra * xt + _ypivot;
    bool column_drawn = false;
    for (int32_t y = min_y; y <= max_y; y++) {
      int32_t yt = y - spr->_ypivot;
      int32_t xs = (int32_t)round(cxt - sinra * yt);
      // Do not calculate ys unless xs is in bounds
      if (xs >= 0 && xs < width()) {
        int32_t ys = (int32_t)round(sxt + cosra * yt);
        // Check if ys is in bounds
        if (ys >= 0 && ys < height()) {
          int32_t rp = readPixel(xs, ys);
          if (rp != transp) spr->drawPixel(x, y, rp);
          column_drawn = true;
        }
      } else if (column_drawn)
        y = max_y;  // Skip the remaining pixels below the Sprite
    }
  }

  return true;
}

/***************************************************************************************
** Function name:           rotatedBounds
** Description:             Get bounding box of a rotated Sprite wrt pivot
*************************************************************************************x*/
void TFT_eSprite::getRotatedBounds(float sina, float cosa, int16_t w, int16_t h,
                                   int16_t xp, int16_t yp, int16_t *min_x,
                                   int16_t *min_y, int16_t *max_x,
                                   int16_t *max_y) {
  w -= xp;  // w is now right edge coordinate relative to xp
  h -= yp;  // h is now bottom edge coordinate relative to yp

  // Calculate new corner coordinates
  int16_t x0 = -xp * cosa - yp * sina;
  int16_t y0 = xp * sina - yp * cosa;

  int16_t x1 = w * cosa - yp * sina;
  int16_t y1 = -w * sina - yp * cosa;

  int16_t x2 = h * sina + w * cosa;
  int16_t y2 = h * cosa - w * sina;

  int16_t x3 = h * sina - xp * cosa;
  int16_t y3 = h * cosa + xp * sina;

  // Find bounding box extremes, enlarge box to accomodate rounding errors
  *min_x = x0 - 2;
  if (x1 < *min_x) *min_x = x1 - 2;
  if (x2 < *min_x) *min_x = x2 - 2;
  if (x3 < *min_x) *min_x = x3 - 2;

  *max_x = x0 + 2;
  if (x1 > *max_x) *max_x = x1 + 2;
  if (x2 > *max_x) *max_x = x2 + 2;
  if (x3 > *max_x) *max_x = x3 + 2;

  *min_y = y0 - 2;
  if (y1 < *min_y) *min_y = y1 - 2;
  if (y2 < *min_y) *min_y = y2 - 2;
  if (y3 < *min_y) *min_y = y3 - 2;

  *max_y = y0 + 2;
  if (y1 > *max_y) *max_y = y1 + 2;
  if (y2 > *max_y) *max_y = y2 + 2;
  if (y3 > *max_y) *max_y = y3 + 2;
}

/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y
*************************************************************************************x*/
void TFT_eSprite::pushSprite(int32_t x, int32_t y) {
  if (!_created) return;

  if (_bpp == 16) {
    bool oldSwapBytes = _tft->getSwapBytes();
    _tft->setSwapBytes(false);
    _tft->pushImage(x, y, _iwidth, _iheight, _img);
    _tft->setSwapBytes(oldSwapBytes);
  }

  else
    _tft->pushImage(x, y, _dwidth, _dheight, _img8, (bool)(_bpp == 8));
}

/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y with transparent colour
*************************************************************************************x*/
void TFT_eSprite::pushSprite(int32_t x, int32_t y, uint16_t transp) {
  if (!_created) return;

  if (_bpp == 16) {
    bool oldSwapBytes = _tft->getSwapBytes();
    _tft->setSwapBytes(false);
    _tft->pushImage(x, y, _iwidth, _iheight, _img, transp);
    _tft->setSwapBytes(oldSwapBytes);
  } else if (_bpp == 8) {
    transp = (uint8_t)((transp & 0xE000) >> 8 | (transp & 0x0700) >> 6 |
                       (transp & 0x0018) >> 3);
    _tft->pushImage(x, y, _dwidth, _dheight, _img8, (uint8_t)transp,
                    (bool)true);
  } else
    _tft->pushImage(x, y, _dwidth, _dheight, _img8, 0, (bool)false);
}

/***************************************************************************************
** Function name:           readPixel
** Description:             Read 565 colour of a pixel at defined coordinates
*************************************************************************************x*/
uint16_t TFT_eSprite::readPixel(int32_t x, int32_t y) 
{
  if (_vpOoB  || !_created) return 0xFFFF;
  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return 0xFFFF;

  if (_bpp == 16)
  {
    uint16_t color = _img[x + y * _iwidth];
    return (color >> 8) | (color << 8);
  }

  if (_bpp == 8)
  {
    uint16_t color = _img8[x + y * _iwidth];
    
    if (color != 0){
    uint8_t  blue[] = {0, 11, 21, 31};
      color =   (color & 0xE0)<<8 | (color & 0xC0)<<5
              | (color & 0x1C)<<6 | (color & 0x1C)<<3
              | blue[color & 0x03];
    }
    //TEST
    //color=TFT_GREEN;
    return color;
  }

  if (_bpp == 4)
  {
    if (x >= _dwidth) return 0xFFFF;
    uint16_t color;
    if ((x & 0x01) == 0)
      color = _colorMap[_img4[((x+y*_iwidth)>>1)] >> 4];   // even index = bits 7 .. 4
    else
      color = _colorMap[_img4[((x+y*_iwidth)>>1)] & 0x0F]; // odd index = bits 3 .. 0.
    return color;
  }

  // Note: Must be 1bpp
  // _dwidth and _dheight bounds not checked (rounded up -iwidth and _iheight used)
  if (rotation == 1)
  {
    uint16_t tx = x;
    x = _dheight - y - 1;
    y = tx;
  }
  else if (rotation == 2)
  {
    x = _dwidth - x - 1;
    y = _dheight - y - 1;
  }
  else if (rotation == 3)
  {
    uint16_t tx = x;
    x = y;
    y = _dwidth - tx - 1;
  }

  uint16_t color = (_img8[(x + y * _bitwidth)>>3] << (x & 0x7)) & 0x80;

  if (color) return _tft->bitmap_fg;
  else       return _tft->bitmap_bg;
} 



/***************************************************************************************
** Function name:           pushImageAlpa with alpha and transparency huebner 2022
** Description:             push image into a defined area of a sprite 16bit to 8bit sprite
***************************************************************************************/
void  TFT_eSprite::pushImageAlpha(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint8_t *dataAlpha,uint16_t transp)
{
  if (!_created) return;
//  
 int32_t dxypw =w*h;
 uint16_t rColor[dxypw];
    for (int32_t ypp = 0; ypp < h; ypp++){
     dxypw=ypp*w;
      for (int32_t xpp = 0; xpp < w; xpp++){
       
        rColor[dxypw]=readPixel(x+xpp,y+ypp);
         if ((data!=nullptr) && (_tft->setScreenShot)) {
          data[dxypw]=rColor[dxypw];
         }
        dxypw++;
       }
    }
  //
  if (_tft->setScreenShot) return;
 
 // 
 PI_CLIP;
  //
  if (_bpp == 8) // Plot a 16 bpp image into a 8 bpp Sprite
  {
    uint16_t lastColor = 0;
    uint16_t pColor=0;
    uint16_t color=0;
    uint8_t  color8    = 0;
    //
    uint16_t dColor=transp<<8 | transp>>8; //_tft->alphaColor;
    uint8_t  ap=_tft->alpha;
    //
    for (int32_t yp = dy; yp < dy + dh; yp++)
    {
      int32_t xyw = x + y * _iwidth;
      dxypw = dx + yp * w;
      
      for (int32_t xp = dx; xp < dx + dw; xp++)
      {
        if (data!=nullptr) dColor = data[dxypw];
       //
       color=dColor;
       pColor=rColor[dxypw];
       //
       pColor=pColor<<8 | pColor>>8;
       //TEST
       //color=pColor;
        // 
        if (dataAlpha!=nullptr) ap=dataAlpha[dxypw];
        //
        if (ap!=0xFF) {
         color=color<<8 | color>>8;
         pColor=pColor<<8 | pColor>>8;
         color=alphaBlend(ap,color,pColor);  
         color=color<<8 | color>>8;
        }
        //
        if (color != lastColor) {
         //_swapBytes=true; 
          // When data source is a sprite, the bytes are already swapped
          if(!_swapBytes) color8 = (uint8_t)((color & 0xE0) | (color & 0x07)<<2 | (color & 0x1800)>>11);
          else color8 = (uint8_t)((color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3);
        
        }
        lastColor = color;
        //
        if ((_tft->setTransp) && (dColor==transp)){
          xyw++;
        }else{
          _img8[xyw++] = color8;
        } 
       dxypw++;
      }//for xp
      y++;
    }//for yp
  }
}





/***************************************************************************************
** Function name:           pushImage
** Description:             push 565 colour image into a defined area of a sprite
*************************************************************************************x*/
void TFT_eSprite::pushImage(int32_t x, int32_t y, int32_t w, int32_t h,uint16_t *data,uint16_t transp) {
boolean transpSet=false;
  if (data == nullptr || !_created) return;

  if ((x >= _iwidth) || (y >= _iheight) || (w == 0) || (h == 0) || !_created)
    return;
  if ((x + w < 0) || (y + h < 0)) return;

  int32_t xo = 0;
  int32_t yo = 0;

  int32_t xs = x;
  int32_t ys = y;

  int32_t ws = w;
  int32_t hs = h;

  if (x < 0) {
    xo = -x;
    ws += x;
    xs = 0;
  }
  if (y < 0) {
    yo = -y;
    hs += y;
    ys = 0;
  }

  if (xs + ws >= (int32_t)_iwidth) ws = _iwidth - xs;
  if (ys + hs >= (int32_t)_iheight) hs = _iheight - ys;

  if (_bpp == 16)  // Plot a 16 bpp image into a 16 bpp Sprite
  {
    for (int32_t yp = yo; yp < yo + hs; yp++) {
      x = xs;
      for (int32_t xp = xo; xp < xo + ws; xp++) {
        uint16_t color = data[xp + yp * w];
        if (!_swapBytes) color = color << 8 | color >> 8;
        _img[x + ys * _iwidth] = color;
        x++;
      }
      ys++;
    }
  } else if (_bpp == 8)  // Plot a 16 bpp image into a 8 bpp Sprite
  {
    for (int32_t yp = yo; yp < yo + hs; yp++) {
      x = xs;
      for (int32_t xp = xo; xp < xo + ws; xp++) {
       uint16_t color = data[xp + yp * w];
       transpSet=((_tft->setTransp) && (color==transp));
        //
        if (_swapBytes) color = color << 8 | color >> 8;
        //
        if (!transpSet) {
         _img8[x + ys * _iwidth] =
            (uint8_t)((color & 0xE000) >> 8 | (color & 0x0700) >> 6 |
                      (color & 0x0018) >> 3);
        }
       x++;
      } //for xp
      ys++;
    }//for yp
  }

  else  // 1bpp
  {
    // Move coordinate rotation to support fn
    if (_rotation == 1) {
      int32_t tx = x;
      x = _dwidth - y - 1;
      y = tx;
    } else if (_rotation == 2) {
      x = _dwidth - x - 1;
      y = _dheight - y - 1;
    } else if (_rotation == 3) {
      int32_t tx = x;
      x = y;
      y = _dheight - tx - 1;
    }
    // Plot a 1bpp image into a 1bpp Sprite
    uint8_t *pdata = (uint8_t *)data;
    uint32_t ww = (w + 7) & 0xFFF8;
    for (int32_t yp = 0; yp < h; yp++) {
      uint32_t yw = (yp * ww) >> 3;
      uint32_t yyp = y + yp;
      for (int32_t xp = 0; xp < w; xp++) {
        uint16_t readPixel = (pdata[(xp >> 3) + yw] & (0x80 >> (xp & 0x7)));
        drawPixel(x + xp, yyp, readPixel);
      }
    }
  }
}





/***************************************************************************************
** Function name:           pushImage
** Description:             push 565 colour FLASH (PROGMEM) image into a defined area
*************************************************************************************x*/
void TFT_eSprite::pushImage(int32_t x, int32_t y, int32_t w, int32_t h,const uint16_t *data) {
#ifdef ESP32
  pushImage(x, y, w, h, (uint16_t *)data);
#else
  // Partitioned memory FLASH processor
  if ((x >= _iwidth) || (y >= _iheight) || (w == 0) || (h == 0) || !_created)
    return;
  if ((x + w < 0) || (y + h < 0)) return;

  int32_t xo = 0;
  int32_t yo = 0;

  int32_t xs = x;
  int32_t ys = y;

  int32_t ws = w;
  int32_t hs = h;

  if (x < 0) {
    xo = -x;
    ws += x;
    xs = 0;
  }
  if (y < 0) {
    yo = -y;
    hs += y;
    ys = 0;
  }

  if (xs + ws >= (int32_t)_iwidth) ws = _iwidth - xs;
  if (ys + hs >= (int32_t)_iheight) hs = _iheight - ys;

  if (_bpp == 16)  // Plot a 16 bpp image into a 16 bpp Sprite
  {
    for (int32_t yp = yo; yp < yo + hs; yp++) {
      x = xs;
      for (int32_t xp = xo; xp < xo + ws; xp++) {
        uint16_t color = pgm_read_word(data + xp + yp * w);
        if (!_swapBytes) color = color << 8 | color >> 8;
        _img[x + ys * _iwidth] = color;
        x++;
      }
      ys++;
    }
  }

  else if (_bpp == 8)  // Plot a 16 bpp image into a 8 bpp Sprite
  {
    for (int32_t yp = yo; yp < yo + hs; yp++) {
      x = xs;
      for (int32_t xp = xo; xp < xo + ws; xp++) {
        uint16_t color = pgm_read_word(data + xp + yp * w);
        if (_swapBytes) color = color << 8 | color >> 8;
        _img8[x + ys * _iwidth] =
            (uint8_t)((color & 0xE000) >> 8 | (color & 0x0700) >> 6 |
                      (color & 0x0018) >> 3);
        x++;
      }
      ys++;
    }
  }

  else  // 1bpp
  {
    // Move coordinate rotation to support fn
    if (_rotation == 1) {
      int32_t tx = x;
      x = _dwidth - y - 1;
      y = tx;
    } else if (_rotation == 2) {
      x = _dwidth - x - 1;
      y = _dheight - y - 1;
    } else if (_rotation == 3) {
      int32_t tx = x;
      x = y;
      y = _dheight - tx - 1;
    }
    // Plot a 1bpp image into a 1bpp Sprite
    const uint8_t *pdata = (const uint8_t *)data;
    uint32_t ww = (w + 7) & 0xFFF8;
    for (int32_t yp = 0; yp < h; yp++) {
      for (uint32_t xp = 0; xp < ww; xp += 8) {
        uint8_t pbyte = pgm_read_byte(pdata++);
        for (uint8_t xc = 0; xc < 8; xc++) {
          if (xp + xc < (uint32_t)w)
            drawPixel(x + xp + xc, y + yp, (pbyte << xc) & 0x80);
        }
      }
    }
  }
#endif  // if ESP32 else ESP8266 check
}



/***************************************************************************************
void TFT_eSprite::pushInSprite(TFT_eSprite *spr, int32_t srcX, int32_t srcY,
                               int32_t srcW, int32_t srcH, int32_t desX,
                               int32_t desY) {
  for (int y = 0; y < srcH; y++) {
    for (int x = 0; x < srcW; x++) {
      drawPixel(desX + x, desY + y, spr->readPixel(srcX + x, srcY + y));
    }
  }
}


/***************************************************************************************
** Function name:           setWindow
** Description:             Set the bounds of a window for pushColor and writeColor
*************************************************************************************x*/
void TFT_eSprite::setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
  if (x0 > x1) swap_coord(x0, x1);
  if (y0 > y1) swap_coord(y0, y1);

  if ((x0 >= _iwidth) || (x1 < 0) || (y0 >= _iheight) ||
      (y1 < 0)) {  // Point to that extra "off screen" pixel
    _xs = 0;
    _ys = _iheight;
    _xe = 0;
    _ye = _iheight;
  } else {
    if (x0 < 0) x0 = 0;
    if (x1 >= _iwidth) x1 = _iwidth - 1;
    if (y0 < 0) y0 = 0;
    if (y1 >= _iheight) y1 = _iheight - 1;

    _xs = x0;
    _ys = y0;
    _xe = x1;
    _ye = y1;
  }

  _xptr = _xs;
  _yptr = _ys;
}

/***************************************************************************************
** Function name:           pushColor
** Description:             Send a new pixel to the set window
*************************************************************************************x*/
void TFT_eSprite::pushColor(uint32_t color) {
  if (!_created) return;

  // Write the colour to RAM in set window
  if (_bpp == 16)
    _img[_xptr + _yptr * _iwidth] = (uint16_t)(color >> 8) | (color << 8);

  else if (_bpp == 8)
    _img8[_xptr + _yptr * _iwidth] =
        (uint8_t)((color & 0xE000) >> 8 | (color & 0x0700) >> 6 |
                  (color & 0x0018) >> 3);

  else
    drawPixel(_xptr, _yptr, color);

  // Increment x
  _xptr++;

  // Wrap on x and y to start, increment y if needed
  if (_xptr > _xe) {
    _xptr = _xs;
    _yptr++;
    if (_yptr > _ye) _yptr = _ys;
  }
}

void TFT_eSprite::pushEmptyColor() {
  _xptr++;

  if (_xptr > _xe) {
    _xptr = _xs;
    _yptr++;
    if (_yptr > _ye) _yptr = _ys;
  }
}

/***************************************************************************************
** Function name:           pushColor
** Description:             Send a "len" new pixels to the set window
*************************************************************************************x*/
void TFT_eSprite::pushColor(uint32_t color, uint16_t len) {
  if (!_created) return;

  uint16_t pixelColor;

  if (_bpp == 16)
    pixelColor = (uint16_t)(color >> 8) | (color << 8);

  else if (_bpp == 8)
    pixelColor =
        (color & 0xE000) >> 8 | (color & 0x0700) >> 6 | (color & 0x0018) >> 3;

  else
    pixelColor = (uint16_t)color;  // for 1bpp

  while (len--) writeColor(pixelColor);
}

void TFT_eSprite::writeColors(uint16_t *color, uint16_t len) {
  uint16_t count = 0;
  uint16_t pixelColor;
  while (len--) {
    pixelColor = *(color + count);
    pixelColor = (uint16_t)(pixelColor >> 8) | (pixelColor << 8);
    writeColor(pixelColor);
    count++;
  }
}

/***************************************************************************************
** Function name:           writeColor
** Description:             Write a pixel with pre-formatted colour to the set window
*************************************************************************************x*/
void TFT_eSprite::writeColor(uint16_t color) {
  if (!_created) return;

  // Write 16 bit RGB 565 encoded colour to RAM
  if (_bpp == 16) _img[_xptr + _yptr * _iwidth] = color;

  // Write 8 bit RGB 332 encoded colour to RAM
  else if (_bpp == 8)
    _img8[_xptr + _yptr * _iwidth] = (uint8_t)color;

  else
    drawPixel(_xptr, _yptr, color);

  // Increment x
  _xptr++;

  // Wrap on x and y to start, increment y if needed
  if (_xptr > _xe) {
    _xptr = _xs;
    _yptr++;
    if (_yptr > _ye) _yptr = _ys;
  }
}

/***************************************************************************************
** Function name:           setScrollRect
** Description:             Set scroll area within the sprite and the gap fill colour
*************************************************************************************x*/
void TFT_eSprite::setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h,
                                uint16_t color) {
  if ((x >= _iwidth) || (y >= _iheight) || !_created) return;

  if (x < 0) {
    w += x;
    x = 0;
  }
  if (y < 0) {
    h += y;
    y = 0;
  }

  if ((x + w) > _iwidth) w = _iwidth - x;
  if ((y + h) > _iheight) h = _iheight - y;

  if (w < 1 || h < 1) return;

  _sx = x;
  _sy = y;
  _sw = w;
  _sh = h;

  _scolor = color;
}

/***************************************************************************************
** Function name:           scroll
** Description:             Scroll dx,dy pixels, positive right,down, negative left,up
*************************************************************************************x*/
void TFT_eSprite::scroll(int16_t dx, int16_t dy) {
  if (abs(dx) >= _sw || abs(dy) >= _sh) {
    fillRect(_sx, _sy, _sw, _sh, _scolor);
    return;
  }

  // Fetch the scroll area width and height set by setScrollRect()
  uint32_t w = _sw - abs(dx);  // line width to copy
  uint32_t h = _sh - abs(dy);  // lines to copy
  int32_t iw = _iwidth;        // width of sprite

  // Fetch the x,y origin set by setScrollRect()
  uint32_t tx = _sx;  // to x
  uint32_t fx = _sx;  // from x
  uint32_t ty = _sy;  // to y
  uint32_t fy = _sy;  // from y

  // Adjust for x delta
  if (dx <= 0)
    fx -= dx;
  else
    tx += dx;

  // Adjust for y delta
  if (dy <= 0)
    fy -= dy;
  else {                // Scrolling down so start copy from bottom
    ty = ty + _sh - 1;  // "To" pointer
    iw = -iw;           // Pointer moves backwards
    fy = ty - dy;       // "From" pointer
  }

  // Calculate "from y" and "to y" pointers in RAM
  uint32_t fyp = fx + fy * _iwidth;
  uint32_t typ = tx + ty * _iwidth;

  // Now move the pixels in RAM
  if (_bpp == 16) {
    while (h--) {  // move pixel lines (to, from, byte count)
      memmove(_img + typ, _img + fyp, w << 1);
      typ += iw;
      fyp += iw;
    }
  } else if (_bpp == 8) {
    while (h--) {  // move pixel lines (to, from, byte count)
      memmove(_img8 + typ, _img8 + fyp, w);
      typ += iw;
      fyp += iw;
    }
  } else if (_bpp == 1) {
    if (dx > 0) {
      tx += w;
      fx += w;
    }              // Start from right edge
    while (h--) {  // move pixels one by one
      for (uint16_t xp = 0; xp < w; xp++) {
        if (dx <= 0) drawPixel(tx + xp, ty, readPixel(fx + xp, fy));
        if (dx > 0) drawPixel(tx - xp, ty, readPixel(fx - xp, fy));
      }
      if (dy <= 0) {
        ty++;
        fy++;
      } else {
        ty--;
        fy--;
      }
    }
  } else
    return;  // Not 1, 8 or 16 bpp

  // Fill the gap left by the scrolling
  if (dx > 0) fillRect(_sx, _sy, dx, _sh, _scolor);
  if (dx < 0) fillRect(_sx + _sw + dx, _sy, -dx, _sh, _scolor);
  if (dy > 0) fillRect(_sx, _sy, _sw, dy, _scolor);
  if (dy < 0) fillRect(_sx, _sy + _sh + dy, _sw, -dy, _scolor);
}

/***************************************************************************************
** Function name:           fillSprite
** Description:             Fill the whole sprite with defined colour
*************************************************************************************x*/
void TFT_eSprite::fillScreen(uint32_t color) { fillSprite(color); }

void TFT_eSprite::fillSprite(uint32_t color) {
  if (!_created) return;

  // Use memset if possible as it is super fast
  if (((uint8_t)color == (uint8_t)(color >> 8)) && _bpp == 16)
    memset(_img, (uint8_t)color, _iwidth * _iheight * 2);
  else if (_bpp == 8) {
    color =
        (color & 0xE000) >> 8 | (color & 0x0700) >> 6 | (color & 0x0018) >> 3;
    memset(_img8, (uint8_t)color, _iwidth * _iheight);
  } else if (_bpp == 1) {
    if (color)
      memset(_img8, 0xFF, (_iwidth >> 3) * _iheight + 1);
    else
      memset(_img8, 0x00, (_iwidth >> 3) * _iheight + 1);
  }

  else
    fillRect(0, 0, _iwidth, _iheight, color);
}

/***************************************************************************************
** Function name:           setCursor
** Description:             Set the sprite text cursor x,y position
*************************************************************************************x*/
// Not needed - using TFT_eSPI class function and this->cursor_x/y
//void TFT_eSprite::setCursor(int16_t x, int16_t y)
//{
//  this->cursor_x = x;
//  this->cursor_y = y;
//}

/***************************************************************************************
** Function name:           width
** Description:             Return the width of sprite
*************************************************************************************x*/
// Return the size of the display
int16_t TFT_eSprite::width(void) {
  if (!_created) return 0;

  if (_bpp > 1) return _iwidth;

  if (_rotation == 1 || _rotation == 3) return _dheight;

  return _dwidth;
}

/***************************************************************************************
** Function name:           height
** Description:             Return the height of sprite
*************************************************************************************x*/
int16_t TFT_eSprite::height(void) {
  if (!_created) return 0;

  if (_bpp > 1) return _iheight;

  if (_rotation == 1 || _rotation == 3) return _dwidth;

  return _dheight;
}

/***************************************************************************************
** Function name:           setRotation
** Description:             Rotate coordinate frame for 1bpp sprite
*************************************************************************************x*/
// Does nothing for 8 and 16 bpp sprites. TODO allow rotation of these sprites
void TFT_eSprite::setRotation(uint8_t rotation) {
  if (_bpp != 1) return;

  _rotation = rotation;
  if (rotation == 0 && _iwidth > _iheight) swap_coord(_iwidth, _iheight);
  if (rotation == 1 && _iwidth < _iheight) swap_coord(_iwidth, _iheight);
  if (rotation == 2 && _iwidth > _iheight) swap_coord(_iwidth, _iheight);
  if (rotation == 3 && _iwidth < _iheight) swap_coord(_iwidth, _iheight);
}

/***************************************************************************************
** Function name:           getRotation
** Description:             Get rotation for 1bpp sprite
*************************************************************************************x*/

uint8_t TFT_eSprite::getRotation(void) { return _rotation; }

void TFT_eSprite::getSprite2Buff(uint16_t *ptr, int32_t x, int32_t y, int32_t w,
                                 int32_t h) {
  for (int _y = 0; _y < h; _y++) {
    for (int _x = 0; _x < w; _x++) {
      ptr[_y * w + _x] = readPixel(x + _x, y + _y);
    }
  }
}

/***************************************************************************************
** Function name:           drawPixel
** Description:             push a single pixel at an arbitrary position
*************************************************************************************x*/
void TFT_eSprite::drawPixel(int32_t x, int32_t y, uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    _img[x+y*_iwidth] = (uint16_t) color;
  }
  else if (_bpp == 8)
  {
    _img8[x+y*_iwidth] = (uint8_t)((color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3);
  }
  else if (_bpp == 4)
  {
    uint8_t c = color & 0x0F;
    int index = (x+y*_iwidth)>>1;;
    if ((x & 0x01) == 0) {
      _img4[index] = (uint8_t)((c << 4) | (_img4[index] & 0x0F));
    }
    else {
      _img4[index] =  (uint8_t)(c | (_img4[index] & 0xF0));
    }
  }
  else // 1 bpp
  {
    if (rotation == 1)
    {
      uint16_t tx = x;
      x = _dwidth - y - 1;
      y = tx;
    }
    else if (rotation == 2)
    {
      x = _dwidth - x - 1;
      y = _dheight - y - 1;
    }
    else if (rotation == 3)
    {
      uint16_t tx = x;
      x = y;
      y = _dheight - tx - 1;
    }

    if (color) _img8[(x + y * _bitwidth)>>3] |=  (0x80 >> (x & 0x7));
    else       _img8[(x + y * _bitwidth)>>3] &= ~(0x80 >> (x & 0x7));
  }
} 



/***************************************************************************************
** Function name:           drawLine
** Description:             draw a line between 2 arbitrary points
*************************************************************************************x*/
void TFT_eSprite::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,uint32_t color)
{
  if (!_created || _vpOoB) return;

  //_xDatum and _yDatum Not added here, it is added by drawPixel & drawFastxLine

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap_coord(x0, y0);
    swap_coord(x1, y1);
  }

  if (x0 > x1) {
    swap_coord(x0, x1);
    swap_coord(y0, y1);
  }

  int32_t dx = x1 - x0, dy = abs(y1 - y0);;

  int32_t err = dx >> 1, ystep = -1, xs = x0, dlen = 0;

  if (y0 < y1) ystep = 1;

  // Split into steep and not steep for FastH/V separation
  if (steep) {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(y0, xs, color);
        else drawFastVLine(y0, xs, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastVLine(y0, xs, dlen, color);
  }
  else
  {
    for (; x0 <= x1; x0++) {
      dlen++;
      err -= dy;
      if (err < 0) {
        err += dx;
        if (dlen == 1) drawPixel(xs, y0, color);
        else drawFastHLine(xs, y0, dlen, color);
        dlen = 0; y0 += ystep; xs = x0 + 1;
      }
    }
    if (dlen) drawFastHLine(xs, y0, dlen, color);
  }
} 



/***************************************************************************************
** Function name:           drawFastVLine
** Description:             draw a vertical line
*************************************************************************************x*/
void TFT_eSprite::drawFastVLine(int32_t x, int32_t y, int32_t h,uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x < _vpX) || (x >= _vpW) || (y >= _vpH)) return;

  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((y + h) > _vpH) h = _vpH - y;

  if (h < 1) return;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    int32_t yp = x + _iwidth * y;
    while (h--) {_img[yp] = (uint16_t) color; yp += _iwidth;}
  }
  else if (_bpp == 8)
  {
    color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
    while (h--) _img8[x + _iwidth * y++] = (uint8_t) color;
  }
  else if (_bpp == 4)
  {
    if ((x & 0x01) == 0)
    {
      uint8_t c = (uint8_t) (color & 0xF) << 4;
      while (h--) {
        _img4[(x + _iwidth * y)>>1] = (uint8_t) (c | (_img4[(x + _iwidth * y)>>1] & 0x0F));
        y++;
      }
    }
    else {
      uint8_t c = (uint8_t)color & 0xF;
      while (h--) {
        _img4[(x + _iwidth * y)>>1] = (uint8_t) (c | (_img4[(x + _iwidth * y)>>1] & 0xF0)); // x is odd; new color goes into the low bits.
        y++;
      }
    }
  }
  else
  {
    x -= _xDatum; // Remove any offset as it will be added by drawPixel
    y -= _yDatum;
    while (h--)
    {
      drawPixel(x, y++, color);
    }
  }
} 



/***************************************************************************************
** Function name:           drawFastHLine
** Description:             draw a horizontal line
*************************************************************************************x*/
void TFT_eSprite::drawFastHLine(int32_t x, int32_t y, int32_t w,uint32_t color)
{
 //change huebner 2022 
 if (_tft->alpha<0xFF) {
   pushImageAlpha(x,y,w,1,NULL,NULL,color); 
   return;
 }

 //
 if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((y < _vpY) || (x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }

  if ((x + w) > _vpW) w = _vpW - x;

  if (w < 1) return;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    while (w--) _img[_iwidth * y + x++] = (uint16_t) color;
  }
  else if (_bpp == 8)
  {
    color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
    memset(_img8+_iwidth * y + x, (uint8_t)color, w);
  }
  else if (_bpp == 4)
  {
    uint8_t c = (uint8_t)color & 0x0F;
    uint8_t c2 = (c | ((c << 4) & 0xF0));
    if ((x & 0x01) == 1)
    {
      drawPixel(x - _xDatum, y - _yDatum, color);
      x++; w--;
      if (w < 1)
        return;
    }

    if (((w + x) & 0x01) == 1)
    {
      // handle the extra one at the other end
      drawPixel(x - _xDatum + w - 1, y - _yDatum, color);
      w--;
      if (w < 1) return;
    }
    memset(_img4 + ((_iwidth * y + x) >> 1), c2, (w >> 1));
  }
  else {
    x -= _xDatum; // Remove any offset as it will be added by drawPixel
    y -= _yDatum;

    while (w--)
    {
      drawPixel(x++, y, color);
    }
  }
}  


/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
*************************************************************************************x*/
void TFT_eSprite::fillRect(int32_t x, int32_t y, int32_t w, int32_t h,uint32_t color)
{
  if (!_created || _vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }
  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((x + w) > _vpW) w = _vpW - x;
  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  int32_t yp = _iwidth * y + x;

  if (_bpp == 16)
  {
    color = (color >> 8) | (color << 8);
    uint32_t iw = w;
    int32_t ys = yp;
    if(h--)  {while (iw--) _img[yp++] = (uint16_t) color;}
    yp = ys;
    while (h--)
    {
      yp += _iwidth;
      memcpy( _img+yp, _img+ys, w<<1);
    }
  }
  else if (_bpp == 8)
  {
    color = (color & 0xE000)>>8 | (color & 0x0700)>>6 | (color & 0x0018)>>3;
    while (h--)
    {
      memset(_img8 + yp, (uint8_t)color, w);
      yp += _iwidth;
    }
  }
  else if (_bpp == 4)
  {
    uint8_t c1 = (uint8_t)color & 0x0F;
    uint8_t c2 = c1 | ((c1 << 4) & 0xF0);
    if ((x & 0x01) == 0 && (w & 0x01) == 0)
    {
      yp = (yp >> 1);
      while (h--)
      {
        memset(_img4 + yp, c2, (w>>1));
        yp += (_iwidth >> 1);
      }
    }
    else if ((x & 0x01) == 0)
    {

      // same as above but you have a hangover on the right.
      yp = (yp >> 1);
      while (h--)
      {
        if (w > 1)
          memset(_img4 + yp, c2, (w-1)>>1);
        // handle the rightmost pixel by calling drawPixel
        drawPixel(x+w-1-_xDatum, y+h-_yDatum, c1);
        yp += (_iwidth >> 1);
      }
    }
    else if ((w & 0x01) == 1)
    {
      yp = (yp + 1) >> 1;
      while (h--) {
        drawPixel(x-_xDatum, y+h-_yDatum, color & 0x0F);
        if (w > 1)
          memset(_img4 + yp, c2, (w-1)>>1);
        // same as above but you have a hangover on the left instead
        yp += (_iwidth >> 1);
      }
    }
    else
    {
      yp = (yp + 1) >> 1;
      while (h--) {
        drawPixel(x-_xDatum, y+h-_yDatum, color & 0x0F);
        if (w > 1) drawPixel(x+w-1-_xDatum, y+h-_yDatum, color & 0x0F);
        if (w > 2)
          memset(_img4 + yp, c2, (w-2)>>1);
        // maximal hacking, single pixels on left and right.
        yp += (_iwidth >> 1);
      }
    }
  }
  else
  {
    x -= _xDatum;
    y -= _yDatum;
    while (h--)
    {
      int32_t ww = w;
      int32_t xx = x;
      while (ww--) drawPixel(xx++, y, color);
      y++;
    }
  }
}
 



/***************************************************************************************
** Function name:           write
** Description:             draw characters piped through serial stream
*************************************************************************************x*/
size_t TFT_eSprite::write(uint8_t utf8) {
  uint16_t uniCode = decodeUTF8(utf8);

  if (!uniCode) return 1;

  if (utf8 == '\r') return 1;

#ifdef SMOOTH_FONT
  if (this->fontLoaded) {
    if (uniCode < 32 && utf8 != '\n') return 1;

    //fontFile = SPIFFS.open( _gFontFilename, "r" );
    //fontFile = SPIFFS.open( this->_gFontFilename, "r" );

    //if(!fontFile)
    //{
    //  fontLoaded = false;
    //  return 1;
    //}
    //Serial.print("Decoded Unicode = 0x");Serial.println(unicode,HEX);

    drawGlyph(uniCode);

    //fontFile.close();
    return 1;
  }
#endif

  if (!_created) return 1;

  if (uniCode == '\n')
    uniCode += 22;  // Make it a valid space character to stop errors
  else if (uniCode < 32)
    return 1;

  uint16_t width = 0;
  uint16_t height = 0;

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv DEBUG vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//Serial.print((uint8_t) uniCode); // Debug line sends all printed TFT text to serial port
//Serial.println(uniCode, HEX); // Debug line sends all printed TFT text to serial port
//delay(5);                     // Debug optional wait for serial port to flush through
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ DEBUG ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#ifdef LOAD_GFXFF
  if (!gfxFont) {
#endif
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#ifdef LOAD_FONT2
    if (textfont == 2) {
      if (utf8 > 127) return 1;

      width = pgm_read_byte(widtbl_f16 + uniCode - 32);
      height = chr_hgt_f16;
      // Font 2 is rendered in whole byte widths so we must allow for this
      width =
          (width + 6) /
          8;  // Width in whole bytes for font 2, should be + 7 but must allow for font width change
      width = width * 8;  // Width converted back to pixles
    }
#ifdef LOAD_RLE
    else
#endif
#endif

#ifdef LOAD_RLE
    {
      if ((textfont > 2) && (textfont < 9)) {
        if (utf8 > 127) return 1;
        // Uses the fontinfo struct array to avoid lots of 'if' or 'switch' statements
        width = pgm_read_byte(
            (uint8_t *)pgm_read_dword(&(fontdata[textfont].widthtbl)) +
            uniCode - 32);
        height = pgm_read_byte(&fontdata[textfont].height);
      }
    }
#endif

#ifdef LOAD_GLCD
    if (textfont == 1) {
      width = 6;
      height = 8;
    }
#else
  if (textfont == 1) return 1;
#endif

    height = height * textsize;

    if (utf8 == '\n') {
      this->cursor_y += height;
      this->cursor_x = 0;
    } else {
      if (textwrapX && (this->cursor_x + width * textsize > _iwidth)) {
        this->cursor_y += height;
        this->cursor_x = 0;
      }
      if (textwrapY && (this->cursor_y >= _iheight)) this->cursor_y = 0;
      this->cursor_x +=
          drawChar(uniCode, this->cursor_x, this->cursor_y, textfont);
    }

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#ifdef LOAD_GFXFF
  }  // Custom GFX font
  else {
    if (utf8 == '\n') {
      this->cursor_x = 0;
      this->cursor_y +=
          (int16_t)textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    } else {
#ifdef USE_M5_FONT_CREATOR
      int32_t index = -1;
      index = getUnicodeFontIndex(uniCode);
      if (index == -1) {
        return 1;
      }
      uint16_t c2 = index;
#else
      if (uniCode > pgm_read_word(&gfxFont->last)) return 1;
      if (uniCode < pgm_read_word(&gfxFont->first)) return 1;

      uint8_t c2 = uniCode - pgm_read_word(&gfxFont->first);
#endif
      GFXglyph *glyph = &(((GFXglyph *)pgm_read_dword(&gfxFont->glyph))[c2]);
      uint8_t w = pgm_read_byte(&glyph->width),
              h = pgm_read_byte(&glyph->height);
      if ((w > 0) && (h > 0)) {  // Is there an associated bitmap?
        int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset);
        if (textwrapX && ((this->cursor_x + textsize * (xo + w)) > _iwidth)) {
          // Drawing character would go off right edge; wrap to new line
          this->cursor_x = 0;
          this->cursor_y +=
              (int16_t)textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }
        if (textwrapY && (this->cursor_y >= _iheight)) this->cursor_y = 0;
        drawChar(this->cursor_x, this->cursor_y, uniCode, textcolor,
                 textbgcolor, textsize);
      }
      this->cursor_x += pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
    }
  }
#endif  // LOAD_GFXFF
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  return 1;
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a single character in the Adafruit GLCD or freefont
*************************************************************************************x*/
void TFT_eSprite::drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color,
                           uint32_t bg, uint8_t size) {
  if (!_created) return;

  if ((x >= _iwidth) ||            // Clip right
      (y >= _iheight) ||           // Clip bottom
      ((x + 6 * size - 1) < 0) ||  // Clip left
      ((y + 8 * size - 1) < 0))    // Clip top
    return;

  if (c < 32) return;
#ifdef LOAD_GLCD
//>>>>>>>>>>>>>>>>>>
#ifdef LOAD_GFXFF
  if (!gfxFont) {  // 'Classic' built-in font
#endif
    //>>>>>>>>>>>>>>>>>>

    boolean fillbg = (bg != color);

    if ((size == 1) && fillbg) {
      uint8_t column[6];
      uint8_t mask = 0x1;

      for (int8_t i = 0; i < 5; i++)
        column[i] = pgm_read_byte(font + (c * 5) + i);
      column[5] = 0;

      int8_t j, k;
      for (j = 0; j < 8; j++) {
        for (k = 0; k < 5; k++) {
          if (column[k] & mask) {
            drawPixel(x + k, y + j, color);
          } else {
            drawPixel(x + k, y + j, bg);
          }
        }

        mask <<= 1;

        drawPixel(x + k, y + j, bg);
      }
    } else {
      for (int8_t i = 0; i < 6; i++) {
        uint8_t line;
        if (i == 5)
          line = 0x0;
        else
          line = pgm_read_byte(font + (c * 5) + i);

        if (size == 1)  // default size
        {
          for (int8_t j = 0; j < 8; j++) {
            if (line & 0x1) drawPixel(x + i, y + j, color);
            line >>= 1;
          }
        } else {  // big size
          for (int8_t j = 0; j < 8; j++) {
            if (line & 0x1)
              fillRect(x + (i * size), y + (j * size), size, size, color);
            else if (fillbg)
              fillRect(x + i * size, y + j * size, size, size, bg);
            line >>= 1;
          }
        }
      }
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifdef LOAD_GFXFF
  } else {  // Custom font
#endif
//>>>>>>>>>>>>>>>>>>>>>>>>>>>
#endif  // LOAD_GLCD

#ifdef LOAD_GFXFF
#ifdef USE_M5_FONT_CREATOR
    int32_t index = -1;
    index = getUnicodeFontIndex(c);
    if (index != -1) {
      c = index;
#else
  // Filter out bad characters not present in font
  if ((c >= pgm_read_word(&gfxFont->first)) &&
      (c <= pgm_read_word(&gfxFont->last))) {
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>

    c -= pgm_read_word(&gfxFont->first);
#endif
      GFXglyph *glyph = &(((GFXglyph *)pgm_read_dword(&gfxFont->glyph))[c]);
      uint8_t *bitmap = (uint8_t *)pgm_read_dword(&gfxFont->bitmap);

      uint32_t bo = pgm_read_dword(&glyph->bitmapOffset);
      uint8_t w = pgm_read_byte(&glyph->width),
              h = pgm_read_byte(&glyph->height);
      //xa = pgm_read_byte(&glyph->xAdvance);
      int8_t xo = pgm_read_byte(&glyph->xOffset),
             yo = pgm_read_byte(&glyph->yOffset);
      uint8_t xx, yy, bits = 0, bit = 0;
      int16_t xo16 = 0, yo16 = 0;

      if (size > 1) {
        xo16 = xo;
        yo16 = yo;
      }

      if (_smooth_bpp == 2) {
        // const uint8_t alphamap[4] = {255, 191, 63, 0};
        const uint8_t alphamap[4] = {0, 63, 191, 255};
        uint8_t c;
        uint16_t fg = textcolor;
        uint16_t bg = textbgcolor;
        bit = 0;
        for (yy = 0; yy < h; yy++) {
          for (xx = 0; xx < w; xx++) {
            if (!(bit++ & 3)) {
              bits = pgm_read_byte(&bitmap[bo++]);
            }
            c = bits >> 6;
            if (c) {
              if (getColor) bg = getColor(x + xo + xx, y + yo + yy);
              if (size == 1) {
                drawPixel(x + xo + xx, y + yo + yy,
                          alphaBlend(alphamap[c], fg, bg));
              } else {
                fillRect(x + (xo16 + xx) * size, y + (yo16 + yy) * size, size,
                         size, alphaBlend(alphamap[c], fg, bg));
              }
            }
            bits <<= 2;
          }
        }
        return;
      }

      uint16_t hpc = 0;  // Horizontal foreground pixel count
      for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
          if (bit == 0) {
            bits = pgm_read_byte(&bitmap[bo++]);
            bit = 0x80;
          }
          if (bits & bit)
            hpc++;
          else {
            if (hpc) {
              if (size == 1)
                drawFastHLine(x + xo + xx - hpc, y + yo + yy, hpc, color);
              else
                fillRect(x + (xo16 + xx - hpc) * size, y + (yo16 + yy) * size,
                         size * hpc, size, color);
              hpc = 0;
            }
          }
          bit >>= 1;
        }
        // Draw pixels for this line as we are about to increment yy
        if (hpc) {
          if (size == 1)
            drawFastHLine(x + xo + xx - hpc, y + yo + yy, hpc, color);
          else
            fillRect(x + (xo16 + xx - hpc) * size, y + (yo16 + yy) * size,
                     size * hpc, size, color);
          hpc = 0;
        }
      }
    }
#endif

#ifdef LOAD_GLCD
#ifdef LOAD_GFXFF
  }  // End classic vs custom font
#endif
#endif
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a unicode onto the screen
*************************************************************************************x*/
// Any UTF-8 decoding must be done before calling drawChar()
int16_t TFT_eSprite::drawChar(uint16_t uniCode, int32_t x, int32_t y) {
  return drawChar(uniCode, x, y, textfont);
}

// Any UTF-8 decoding must be done before calling drawChar()
int16_t TFT_eSprite::drawChar(uint16_t uniCode, int32_t x, int32_t y,
                              uint8_t font) {
  if (!_created) return 0;

  if (!uniCode) return 0;

  if (font == 1) {
#ifdef LOAD_GLCD
#ifndef LOAD_GFXFF
    drawChar(x, y, uniCode, textcolor, textbgcolor, textsize);
    return 6 * textsize;
#endif
#else
#ifndef LOAD_GFXFF
    return 0;
#endif
#endif

#ifdef LOAD_GFXFF
    drawChar(x, y, uniCode, textcolor, textbgcolor, textsize);
    if (!gfxFont) {  // 'Classic' built-in font
#ifdef LOAD_GLCD
      return 6 * textsize;
#else
      return 0;
#endif
    } else {
#ifdef USE_M5_FONT_CREATOR
      int32_t index = -1;
      index = getUnicodeFontIndex(uniCode);
      if (index != -1) {
        uint16_t c2 = index;
#else
      if ((uniCode >= pgm_read_word(&gfxFont->first)) &&
          (uniCode <= pgm_read_word(&gfxFont->last))) {
        uint16_t c2 = uniCode - pgm_read_word(&gfxFont->first);
#endif
        GFXglyph *glyph = &(((GFXglyph *)pgm_read_dword(&gfxFont->glyph))[c2]);
        return pgm_read_byte(&glyph->xAdvance) * textsize;
      } else {
        return 0;
      }
    }
#endif
  }

  if ((font > 1) && (font < 9) && ((uniCode < 32) || (uniCode > 127))) return 0;

  int32_t width = 0;
  int32_t height = 0;
  uint32_t flash_address = 0;
  uniCode -= 32;

#ifdef LOAD_FONT2
  if (font == 2) {
    // This is faster than using the fontdata structure
    flash_address = pgm_read_dword(&chrtbl_f16[uniCode]);
    width = pgm_read_byte(widtbl_f16 + uniCode);
    height = chr_hgt_f16;
  }
#ifdef LOAD_RLE
  else
#endif
#endif

#ifdef LOAD_RLE
  {
    if ((font > 2) && (font < 9)) {
      // This is slower than above but is more convenient for the RLE fonts
      flash_address = pgm_read_dword(
          (const void *)(pgm_read_dword(&(fontdata[font].chartbl)) +
                         uniCode * sizeof(void *)));
      width = pgm_read_byte(
          (uint8_t *)pgm_read_dword(&(fontdata[font].widthtbl)) + uniCode);
      height = pgm_read_byte(&fontdata[font].height);
    }
  }
#endif

  int32_t w = width;
  int32_t pX = 0;
  int32_t pY = y;
  uint8_t line = 0;

#ifdef LOAD_FONT2  // chop out code if we do not need it
  if (font == 2) {
    w = w + 6;  // Should be + 7 but we need to compensate for width increment
    w = w / 8;
    if (x + width * textsize >= _iwidth) return width * textsize;

    for (int32_t i = 0; i < height; i++) {
      if (textcolor != textbgcolor)
        fillRect(x, pY, width * textsize, textsize, textbgcolor);

      for (int32_t k = 0; k < w; k++) {
        line = pgm_read_byte((uint8_t *)flash_address + w * i + k);
        if (line) {
          if (textsize == 1) {
            pX = x + k * 8;
            if (line & 0x80) drawPixel(pX, pY, textcolor);
            if (line & 0x40) drawPixel(pX + 1, pY, textcolor);
            if (line & 0x20) drawPixel(pX + 2, pY, textcolor);
            if (line & 0x10) drawPixel(pX + 3, pY, textcolor);
            if (line & 0x08) drawPixel(pX + 4, pY, textcolor);
            if (line & 0x04) drawPixel(pX + 5, pY, textcolor);
            if (line & 0x02) drawPixel(pX + 6, pY, textcolor);
            if (line & 0x01) drawPixel(pX + 7, pY, textcolor);
          } else {
            pX = x + k * 8 * textsize;
            if (line & 0x80) fillRect(pX, pY, textsize, textsize, textcolor);
            if (line & 0x40)
              fillRect(pX + textsize, pY, textsize, textsize, textcolor);
            if (line & 0x20)
              fillRect(pX + 2 * textsize, pY, textsize, textsize, textcolor);
            if (line & 0x10)
              fillRect(pX + 3 * textsize, pY, textsize, textsize, textcolor);
            if (line & 0x08)
              fillRect(pX + 4 * textsize, pY, textsize, textsize, textcolor);
            if (line & 0x04)
              fillRect(pX + 5 * textsize, pY, textsize, textsize, textcolor);
            if (line & 0x02)
              fillRect(pX + 6 * textsize, pY, textsize, textsize, textcolor);
            if (line & 0x01)
              fillRect(pX + 7 * textsize, pY, textsize, textsize, textcolor);
          }
        }
      }
      pY += textsize;
    }
  }
#ifdef LOAD_RLE
  else
#endif
#endif  //FONT2

#ifdef LOAD_RLE  //674 bytes of code
  // Font is not 2 and hence is RLE encoded
  {
    w *= height;  // Now w is total number of pixels in the character

    if (textcolor != textbgcolor)
      fillRect(x, pY, width * textsize, textsize * height, textbgcolor);
    int16_t color = textcolor;
    if (_bpp == 16)
      color = (textcolor >> 8) | (textcolor << 8);
    else if (_bpp == 8)
      color = ((textcolor & 0xE000) >> 8 | (textcolor & 0x0700) >> 6 |
               (textcolor & 0x0018) >> 3);
    int32_t
        px = 0,
        py = pY;  // To hold character block start and end column and row values
    int32_t pc = 0;                    // Pixel count
    uint8_t np = textsize * textsize;  // Number of pixels in a drawn pixel
    uint8_t tnp = 0;                   // Temporary copy of np for while loop
    uint8_t ts = textsize - 1;         // Temporary copy of textsize
    // 16 bit pixel count so maximum font size is equivalent to 180x180 pixels in area
    // w is total number of pixels to plot to fill character block
    while (pc < w) {
      line = pgm_read_byte((uint8_t *)flash_address);
      flash_address++;  // 20 bytes smaller by incrementing here
      if (line & 0x80) {
        line &= 0x7F;
        line++;
        if (ts) {
          px =
              x +
              textsize *
                  (pc %
                   width);  // Keep these px and py calculations outside the loop as they are slow
          py = y + textsize * (pc / width);
        } else {
          px =
              x +
              pc %
                  width;  // Keep these px and py calculations outside the loop as they are slow
          py = y + pc / width;
        }
        while (line--) {
          pc++;
          setWindow(px, py, px + ts, py + ts);
          if (ts) {
            tnp = np;
            while (tnp--) writeColor(color);
          } else
            writeColor(color);

          px += textsize;

          if (px >= (x + width * textsize)) {
            px = x;
            py += textsize;
          }
        }
      } else {
        line++;
        pc += line;
      }
    }
  }
  // End of RLE font rendering
#endif
  return width * textsize;  // x +
}



#ifdef SMOOTH_FONT
/***************************************************************************************
** Function name:           drawGlyph
** Description:             Write a character to the sprite cursor position
***************************************************************************************/
//
void TFT_eSprite::drawGlyph(uint16_t code)
{

  //Serial.println("DRAWGLYPH");

  uint16_t fg = textcolor;
  uint16_t bg = textbgcolor;
  bool getBG  = false;
  if (fg == bg) getBG = true;

  // Check if cursor has moved
  if (last_cursor_x != cursor_x)
  {
    bg_cursor_x = cursor_x;
    last_cursor_x = cursor_x;
  }

  if (code < 0x21)
  {
    if (code == 0x20) {
      if (_fillbg) fillRect(bg_cursor_x, cursor_y, (cursor_x + gFont.spaceWidth) - bg_cursor_x, gFont.yAdvance, bg);
      cursor_x += gFont.spaceWidth;
      bg_cursor_x = cursor_x;
      last_cursor_x = cursor_x;
      return;
    }

    if (code == '\n') {
      cursor_x = 0;
      bg_cursor_x = 0;
      last_cursor_x = 0;
      cursor_y += gFont.yAdvance;
      if (textwrapY && (cursor_y >= height())) cursor_y = 0;
      return;
    }
  }

  uint16_t gNum = 0;
  bool found = this->getUnicodeIndex(code, &gNum);

  if (found)
  {
    bool newSprite = !_created;

    if (newSprite)
    {
      createSprite(gWidth[gNum], gFont.yAdvance);
      if(fg != bg) fillSprite(bg);
      cursor_x = -gdX[gNum];
      bg_cursor_x = cursor_x;
      last_cursor_x = cursor_x;
      cursor_y = 0;
    }
    else
    {
      if( textwrapX && ((cursor_x + gWidth[gNum] + gdX[gNum]) > width())) {
        cursor_y += gFont.yAdvance;
        cursor_x = 0;
        bg_cursor_x = 0;
        last_cursor_x = 0;
      }

      if( textwrapY && ((cursor_y + gFont.yAdvance) > height())) cursor_y = 0;
      if ( cursor_x == 0) cursor_x -= gdX[gNum];
    }

    uint8_t* pbuffer = nullptr;
    const uint8_t* gPtr = (const uint8_t*) gFont.gArray;

#ifdef FONT_FS_AVAILABLE
    if (fs_font) {
      fontFile.seek(gBitmap[gNum], fs::SeekSet); // This is slow for a significant position shift!
      pbuffer =  (uint8_t*)malloc(gWidth[gNum]);
    }
#endif

    int16_t cy = cursor_y + gFont.maxAscent - gdY[gNum];
    int16_t cx = cursor_x + gdX[gNum];

    //  if (cx > width() && bg_cursor_x > width()) return;
    //  if (cursor_y > height()) return;

    int16_t  fxs = cx;
    uint32_t fl = 0;
    int16_t  bxs = cx;
    uint32_t bl = 0;
    int16_t  bx = 0;
    uint8_t pixel = 0;

    int16_t fillwidth  = 0;
    int16_t fillheight = 0;

    // Fill area above glyph
    if (_fillbg) {
      fillwidth  = (cursor_x + gxAdvance[gNum]) - bg_cursor_x;
      if (fillwidth > 0) {
        fillheight = gFont.maxAscent - gdY[gNum];
        if (fillheight > 0) {
          fillRect(bg_cursor_x, cursor_y, fillwidth, fillheight, textbgcolor);
        }
      }
      else {
        // Could be negative
        fillwidth = 0;
      }

      // Fill any area to left of glyph                              
      if (bg_cursor_x < cx) fillRect(bg_cursor_x, cy, cx - bg_cursor_x, gHeight[gNum], textbgcolor);
      // Set x position in glyph area where background starts
      if (bg_cursor_x > cx) bx = bg_cursor_x - cx;
      // Fill any area to right of glyph
      if (cx + gWidth[gNum] < cursor_x + gxAdvance[gNum]) {
        fillRect(cx + gWidth[gNum], cy, (cursor_x + gxAdvance[gNum]) - (cx + gWidth[gNum]), gHeight[gNum], textbgcolor);
      }
    }

    for (int32_t y = 0; y < gHeight[gNum]; y++)
    {
#ifdef FONT_FS_AVAILABLE
      if (fs_font) {
        fontFile.read(pbuffer, gWidth[gNum]);
      }
#endif

      for (int32_t x = 0; x < gWidth[gNum]; x++)
      {
#ifdef FONT_FS_AVAILABLE
        if (fs_font) pixel = pbuffer[x];
        else
#endif
        pixel = pgm_read_byte(gPtr + gBitmap[gNum] + x + gWidth[gNum] * y);

        if (pixel)
        {
          if (bl) { drawFastHLine( bxs, y + cy, bl, bg); bl = 0; }
          if (pixel != 0xFF)
          {
            if (fl) {
              if (fl==1) drawPixel(fxs, y + cy, fg);
              else drawFastHLine( fxs, y + cy, fl, fg);
              fl = 0;
            }
            if (getBG) bg = readPixel(x + cx, y + cy);
            drawPixel(x + cx, y + cy, alphaBlend(pixel, fg, bg));
          }
          else
          {
            if (fl==0) fxs = x + cx;
            fl++;
          }
        }
        else
        {
          if (fl) { drawFastHLine( fxs, y + cy, fl, fg); fl = 0; }
          if (_fillbg) {
            if (x >= bx) {
              if (bl==0) bxs = x + cx;
              bl++;
            }
          }
        }
      }
      if (fl) { drawFastHLine( fxs, y + cy, fl, fg); fl = 0; }
      if (bl) { drawFastHLine( bxs, y + cy, bl, bg); bl = 0; }
    }

    // Fill area below glyph
    if (fillwidth > 0) {
      fillheight = (cursor_y + gFont.yAdvance) - (cy + gHeight[gNum]);
      if (fillheight > 0) {
        fillRect(bg_cursor_x, cy + gHeight[gNum], fillwidth, fillheight, textbgcolor);
      }
    }

    if (pbuffer) free(pbuffer);
    cursor_x += gxAdvance[gNum];

    if (newSprite)
    {
      pushSprite(cx, cursor_y);
      deleteSprite();
    }
  }
  else
  {
    // Not a Unicode in font so draw a rectangle and move on cursor
    drawRect(cursor_x, cursor_y + gFont.maxAscent - gFont.ascent, gFont.spaceWidth, gFont.ascent, fg);
    cursor_x += gFont.spaceWidth + 1;
  }
  bg_cursor_x = cursor_x;
  last_cursor_x = cursor_x;
}

 



void TFT_eSprite::drawColorBitmap(int32_t x0, int32_t y0, int32_t w, int32_t h,
                                  uint8_t *data, uint32_t color,
                                  uint32_t bkcolor) {
  setWindow(x0, y0, x0 + w - 1, y0 + h - 1);
  uint32_t len = w * h;

  //uint16_t *colorBuff = (uint16_t*)calloc(256,sizeof(uint16_t));
  uint16_t DataBuff[16];

  uint16_t ra = 0, ga = 0, ba = 0;
  uint16_t r1 = (color >> 16) & 0xff;
  uint16_t g1 = (color >> 8) & 0xff;
  uint16_t b1 = (color)&0xff;

  uint16_t r2 = (bkcolor >> 16) & 0xff;
  uint16_t g2 = (bkcolor >> 8) & 0xff;
  uint16_t b2 = (bkcolor)&0xff;

  uint8_t alpha = 0;

  for (size_t c = 0; c < 16; c++) {
    alpha = c;
    ra = (r2 * (16 - alpha) + r1 * alpha) / 16;
    ga = (g2 * (16 - alpha) + g1 * alpha) / 16;
    ba = (b2 * (16 - alpha) + b1 * alpha) / 16;
    DataBuff[c] = color565(ra, ga, ba);
  }

  for (int i = 0; i < len; i++) {
    alpha = *(data + (i / 2));
    if (i % 2 == 0) {
      alpha >>= 4;
    }
    alpha &= 0x0f;
    alpha = (alpha == 1) ? 0 : alpha;
    pushColor(DataBuff[alpha]);
  }
}
void TFT_eSprite::drawColorBitmapAlpha(int32_t x0, int32_t y0, int32_t w,
                                       int32_t h, uint8_t *data, uint32_t color,
                                       uint32_t bkcolor) {
  setWindow(x0, y0, x0 + w - 1, y0 + h - 1);
  uint32_t len = w * h;

  //uint16_t *colorBuff = (uint16_t*)calloc(256,sizeof(uint16_t));
  uint16_t DataBuff[16];

  uint16_t ra = 0, ga = 0, ba = 0;
  uint16_t r1 = (color >> 16) & 0xff;
  uint16_t g1 = (color >> 8) & 0xff;
  uint16_t b1 = (color)&0xff;

  uint16_t r2 = (bkcolor >> 16) & 0xff;
  uint16_t g2 = (bkcolor >> 8) & 0xff;
  uint16_t b2 = (bkcolor)&0xff;

  uint8_t alpha = 0;

  for (size_t c = 0; c < 16; c++) {
    alpha = c;
    ra = (r2 * (16 - alpha) + r1 * alpha) / 16;
    ga = (g2 * (16 - alpha) + g1 * alpha) / 16;
    ba = (b2 * (16 - alpha) + b1 * alpha) / 16;
    DataBuff[c] = color565(ra, ga, ba);
  }

  for (int i = 0; i < len; i++) {
    alpha = *(data + (i / 2));
    if (i % 2 == 0) {
      alpha >>= 4;
    }
    alpha &= 0x0f;
    if (alpha != 0) {
      pushColor(DataBuff[alpha]);
    } else {
      pushEmptyColor();
    }
  }
}

/***************************************************************************************
** Function name:           printToSprite
** Description:             Write a string to the sprite cursor position
*************************************************************************************x*/
void TFT_eSprite::printToSprite(String string) {
  if (!this->fontLoaded) return;
  uint16_t len = string.length();
  char cbuffer[len + 1];  // Add 1 for the null
  string.toCharArray(
      cbuffer,
      len + 1);  // Add 1 for the null, otherwise characters get dropped
  printToSprite(cbuffer, len);
  //printToSprite((char*)string.c_str(), string.length());
}

/***************************************************************************************
** Function name:           printToSprite
** Description:             Write a string to the sprite cursor position
*************************************************************************************x*/
void TFT_eSprite::printToSprite(char *cbuffer, uint16_t len)  //String string)
{
  if (!this->fontLoaded) return;

  //fontFile = SPIFFS.open( this->_gFontFilename, "r" );

  if (!this->fontFile) {
    this->fontLoaded = false;
    return;
  }

  uint16_t n = 0;
  bool newSprite = !_created;

  if (newSprite) {
    int16_t sWidth = 0;
    uint16_t index = 0;

    while (n < len) {
      uint16_t unicode = decodeUTF8((uint8_t *)cbuffer, &n, len - n);
      if (this->getUnicodeIndex(unicode, &index)) {
        if (n == 0) sWidth -= this->gdX[index];
        if (n == len - 1)
          sWidth += (this->gWidth[index] + this->gdX[index]);
        else
          sWidth += this->gxAdvance[index];
      } else
        sWidth += this->gFont.spaceWidth + 1;
    }

    createSprite(sWidth, this->gFont.yAdvance);

    if (this->textbgcolor != TFT_BLACK) fillSprite(this->textbgcolor);
  }

  n = 0;

  while (n < len) {
    uint16_t unicode = decodeUTF8((uint8_t *)cbuffer, &n, len - n);
    //Serial.print("Decoded Unicode = 0x");Serial.println(unicode,HEX);
    //Serial.print("n = ");Serial.println(n);
    drawGlyph(unicode);
  }

  if (newSprite) {  // The sprite had to be created so place at TFT cursor
    pushSprite(_tft->cursor_x, _tft->cursor_y);
    deleteSprite();
  }

  //fontFile.close();
}

/***************************************************************************************
** Function name:           printToSprite
** Description:             Print character in a Sprite, create sprite if needed
*************************************************************************************x*/
int16_t TFT_eSprite::printToSprite(int16_t x, int16_t y, uint16_t index) {
  bool newSprite = !_created;
  int16_t sWidth = this->gWidth[index];

  if (newSprite) {
    createSprite(sWidth, this->gFont.yAdvance);

    if (this->textbgcolor != TFT_BLACK) fillSprite(this->textbgcolor);

    drawGlyph(this->gUnicode[index]);

    pushSprite(x + this->gdX[index], y, this->textbgcolor);
    deleteSprite();
  }

  else
    drawGlyph(this->gUnicode[index]);

  return this->gxAdvance[index];
}

#include "rom/tjpgd.h"

#define jpgColor(c)                                \
  (((uint16_t)(((uint8_t *)(c))[0] & 0xF8) << 8) | \
   ((uint16_t)(((uint8_t *)(c))[1] & 0xFC) << 3) | \
   ((((uint8_t *)(c))[2] & 0xF8) >> 3))

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
const char *jd_errors_e[] = {
    "Succeeded",
    "Interrupted by output function",
    "Device error or wrong termination of input stream",
    "Insufficient memory pool for the image",
    "Insufficient stream input buffer",
    "Parameter error",
    "Data format error",
    "Right format but not supported",
    "Not supported JPEG standard"};
#endif

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t maxWidth;
  uint16_t maxHeight;
  uint16_t offX;
  uint16_t offY;
  jpeg_div_eSprite_t scale;
  const void *src;
  size_t len;
  size_t index;
  TFT_eSprite *tft;
  uint16_t outWidth;
  uint16_t outHeight;
} jpg_file_decoder_t;

static uint32_t jpgReadFile(JDEC *decoder, uint8_t *buf, uint32_t len) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  File *file = (File *)jpeg->src;
  if (buf) {
    return file->read(buf, len);
  } else {
    file->seek(len, SeekCur);
  }
  return len;
}

static uint32_t jpgRead(JDEC *decoder, uint8_t *buf, uint32_t len) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  if (buf) {
    memcpy(buf, (const uint8_t *)jpeg->src + jpeg->index, len);
  }
  jpeg->index += len;
  return len;
}

static uint32_t jpgWrite(JDEC *decoder, void *bitmap, JRECT *rect) {
  jpg_file_decoder_t *jpeg = (jpg_file_decoder_t *)decoder->device;
  uint16_t x = rect->left;
  uint16_t y = rect->top;
  uint16_t w = rect->right + 1 - x;
  uint16_t h = rect->bottom + 1 - y;
  uint16_t oL = 0, oR = 0;
  uint8_t *data = (uint8_t *)bitmap;

  if (rect->right < jpeg->offX) {
    return 1;
  }
  if (rect->left >= (jpeg->offX + jpeg->outWidth)) {
    return 1;
  }
  if (rect->bottom < jpeg->offY) {
    return 1;
  }
  if (rect->top >= (jpeg->offY + jpeg->outHeight)) {
    return 1;
  }
  if (rect->top < jpeg->offY) {
    uint16_t linesToSkip = jpeg->offY - rect->top;
    data += linesToSkip * w * 3;
    h -= linesToSkip;
    y += linesToSkip;
  }
  if (rect->bottom >= (jpeg->offY + jpeg->outHeight)) {
    uint16_t linesToSkip = (rect->bottom + 1) - (jpeg->offY + jpeg->outHeight);
    h -= linesToSkip;
  }
  if (rect->left < jpeg->offX) {
    oL = jpeg->offX - rect->left;
  }
  if (rect->right >= (jpeg->offX + jpeg->outWidth)) {
    oR = (rect->right + 1) - (jpeg->offX + jpeg->outWidth);
  }

  uint16_t pixBuf[32];
  uint8_t pixIndex = 0;
  uint16_t line;

  //jpeg->tft->startWrite();
  // jpeg->tft->setAddrWindow(x - jpeg->offX + jpeg->x + oL, y - jpeg->offY +
  // jpeg->y, w - (oL + oR), h);
  jpeg->tft->setWindow(x - jpeg->offX + jpeg->x + oL, y - jpeg->offY + jpeg->y,
                       x - jpeg->offX + jpeg->x + oL + w - (oL + oR) - 1,
                       y - jpeg->offY + jpeg->y + h - 1);

  while (h--) {
    data += 3 * oL;
    line = w - (oL + oR);
    while (line--) {
      pixBuf[pixIndex++] = jpgColor(data);
      data += 3;
      if (pixIndex == 32) {
        jpeg->tft->writeColors(pixBuf, 32);
        // SPI.writePixels((uint8_t *)pixBuf, 64);
        pixIndex = 0;
      }
    }
    data += 3 * oR;
  }
  if (pixIndex) {
    jpeg->tft->writeColors(pixBuf, pixIndex);
    // SPI.writePixels((uint8_t *)pixBuf, pixIndex * 2);
  }
  //jpeg->tft->endWrite();
  return 1;
}

static bool jpgDecode(jpg_file_decoder_t *jpeg,
                      uint32_t (*reader)(JDEC *, uint8_t *, uint32_t)) {
  static uint8_t work[3100];
  JDEC decoder;

  JRESULT jres = jd_prepare(&decoder, reader, work, 3100, jpeg);
  if (jres != JDR_OK) {
    log_e("jd_prepare failed! %s", jd_errors_e[jres]);
    return false;
  }

  uint16_t jpgWidth = decoder.width / (1 << (uint8_t)(jpeg->scale));
  uint16_t jpgHeight = decoder.height / (1 << (uint8_t)(jpeg->scale));

  if (jpeg->offX >= jpgWidth || jpeg->offY >= jpgHeight) {
    log_e("Offset Outside of JPEG size");
    return false;
  }

  size_t jpgMaxWidth = jpgWidth - jpeg->offX;
  size_t jpgMaxHeight = jpgHeight - jpeg->offY;

  jpeg->outWidth =
      (jpgMaxWidth > jpeg->maxWidth) ? jpeg->maxWidth : jpgMaxWidth;
  jpeg->outHeight =
      (jpgMaxHeight > jpeg->maxHeight) ? jpeg->maxHeight : jpgMaxHeight;

  jres = jd_decomp(&decoder, jpgWrite, (uint8_t)jpeg->scale);
  if (jres != JDR_OK) {
    log_e("jd_decomp failed! %s", jd_errors_e[jres]);
    return false;
  }

  return true;
}

void TFT_eSprite::drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x,
                          uint16_t y, uint16_t maxWidth, uint16_t maxHeight,
                          uint16_t offX, uint16_t offY,
                          jpeg_div_eSprite_t scale) {
  if ((x + maxWidth) > width() || (y + maxHeight) > height()) {
    log_e("Bad dimensions given");
    return;
  }

  jpg_file_decoder_t jpeg;

  if (!maxWidth) {
    maxWidth = width() - x;
  }
  if (!maxHeight) {
    maxHeight = height() - y;
  }

  jpeg.src = jpg_data;
  jpeg.len = jpg_len;
  jpeg.index = 0;
  jpeg.x = x;
  jpeg.y = y;
  jpeg.maxWidth = maxWidth;
  jpeg.maxHeight = maxHeight;
  jpeg.offX = offX;
  jpeg.offY = offY;
  jpeg.scale = scale;
  jpeg.tft = this;

  jpgDecode(&jpeg, jpgRead);
}

void TFT_eSprite::drawJpgFile(fs::FS &fs, const char *path, uint16_t x,
                              uint16_t y, uint16_t maxWidth, uint16_t maxHeight,
                              uint16_t offX, uint16_t offY,
                              jpeg_div_eSprite_t scale) {
  if ((x + maxWidth) > width() || (y + maxHeight) > height()) {
    log_e("Bad dimensions given");
    return;
  }

  File file = fs.open(path);
  if (!file) {
    log_e("Failed to open file for reading");
    return;
  }

  jpg_file_decoder_t jpeg;

  if (!maxWidth) {
    maxWidth = width() - x;
  }
  if (!maxHeight) {
    maxHeight = height() - y;
  }

  jpeg.src = &file;
  jpeg.len = file.size();
  jpeg.index = 0;
  jpeg.x = x;
  jpeg.y = y;
  jpeg.maxWidth = maxWidth;
  jpeg.maxHeight = maxHeight;
  jpeg.offX = offX;
  jpeg.offY = offY;
  jpeg.scale = scale;
  jpeg.tft = this;

  jpgDecode(&jpeg, jpgReadFile);

  file.close();
}

#endif
