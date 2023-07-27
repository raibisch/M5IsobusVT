#include <M5Core2.h>
#include <Free_Fonts.h>

#define KEYBOARD_X (2)
#define KEYBOARD_Y (26)

#define KEY_W (45)
#define KEY_H (50)

#define COLS (7)
#define ROWS (4)

#define MAX_SHIFT_MODE (4)

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

String input_text;
key_mode_t key_mode = KEY_MODE_LETTER;
bool shift_mode = false;
TouchButton *touch_button_list[COLS*ROWS];

void initKeyboard(void);
void drawKeyboard(void);
void buttonEvent(TouchEvent& e);

void initKeyboard()
{
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.setFreeFont(FF1);

  // Button A
  M5.Lcd.drawString("back", 40, 225, 2);
  // Button B
  M5.Lcd.drawString("done", 145, 225, 2);
  // Button C
  M5.Lcd.drawString("mode", 250, 225, 2);

  M5.Touch.addHandler(buttonEvent, TE_BTNONLY + TE_TOUCH);
  // Clear button list
  for(int i = 0; i < COLS*ROWS; i++)
  {
    touch_button_list[i] = NULL;
  }
  key_mode = KEY_MODE_LETTER;
  shift_mode = false;
  drawKeyboard();
}

void drawKeyboard()
{
  int i, x, y;

  i = 0;
  for(int r = 0; r < ROWS; r++)
  {
    for(int c = 0; c < COLS; c++)
    {
      x = (KEYBOARD_X + (c * KEY_W));
      y = (KEYBOARD_Y + (r * KEY_H));
      M5.Lcd.fillRect(x + 1, y + 1, KEY_W - 2, KEY_H - 2, TFT_BLACK);
      M5.Lcd.drawRoundRect(x, y, KEY_W, KEY_H, 10, TFT_DARKGREY);

      int key_page = 0;

      if(key_mode == KEY_MODE_NUMBER) key_page += 2;
      if(shift_mode == true) key_page += 1;

      String key;
      char ch = keymap[key_page][r][c];

      if(ch == '\002')  // Shift
      {
        M5.Lcd.setFreeFont(FF0);
        key = "shift";
        M5.Lcd.drawString(key, x + 9, y + 28);
      }
      else
      {
        M5.Lcd.setFreeFont(FF3);
        key = String(ch);
        M5.Lcd.drawString(key, x + 12, y + 12);
      }

      if(touch_button_list[i] != NULL)
      {
        delete(touch_button_list[i]);
      }
      touch_button_list[i++] = new TouchButton(x, y + 5, KEY_W, KEY_H, key.c_str());
    }
  }
}

void buttonEvent(TouchEvent& e)
{
  TouchButton& b = *e.button;

  // Backspace
  if(e.button == &M5.BtnA)
  {
    input_text = input_text.substring(0, input_text.length() - 1);
  }
  // Done
  else if(e.button == &M5.BtnB)
  {
    Serial.println("done");
    return;
  }
  // Key mode
  else if(e.button == &M5.BtnC)
  {
    switch(key_mode)
    {
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
    return;
  }
  else
  {
    if(String(b.name) == "shift")
    {
      shift_mode = !shift_mode;
      drawKeyboard();
      return;
    }
    input_text += b.name;
  }

  // Clear input text area
  M5.Lcd.fillRect(0, 0, M5.Lcd.width(), KEYBOARD_Y - 1, TFT_BLACK);
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString(input_text, 0, 0);
}

void setup()
{
  M5.begin(true, false, true, true);

  initKeyboard();
}

void loop()
{
  M5.update();
}
