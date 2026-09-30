#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

const int8_t BTN_DOWN = 0;
const int8_t BTN_LEFT = 1;
const int8_t BTN_UP = 2;
const int8_t BTN_RIGHT = 3;

const int8_t I2C_SDA = 8;
const int8_t I2C_SCL = 9;

int8_t current_item = 1;
int8_t items_draw = 1;

const char *EXTRAS_ITEMS[] = {
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW"};

enum AppState
{
  MainMenu
};

AppState APP_STATE = MainMenu;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

/*=============суперудобныйразделитель=============*/

void loading()
{
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_t0_14b_me);
  u8g2.drawStr(30, 35, "esp-board");
  u8g2.setFont(u8g2_font_tiny5_tf);
  u8g2.drawStr(50, 45, "loading...");

  u8g2.sendBuffer();
  delay(16);
}

void init()
{
  Wire.begin(I2C_SDA, I2C_SCL);

  u8g2.begin();
  u8g2.setPowerSave(0);
}

void input()
{
  int8_t max_items = 0;

  switch (APP_STATE)
  {
  case MainMenu:
    max_items = 3;
    break;

  default:
    break;
  }

  static int8_t prev_btn_up = HIGH;
  static int8_t prev_btn_down = HIGH;
  static int8_t prev_btn_left = HIGH;
  static int8_t prev_btn_right = HIGH;

  int8_t cur_btn_up = digitalRead(BTN_UP);
  int8_t cur_btn_down = digitalRead(BTN_DOWN);
  int8_t cur_btn_left = digitalRead(BTN_LEFT);
  int8_t cur_btn_right = digitalRead(BTN_RIGHT);

  if (cur_btn_down == LOW && prev_btn_down == HIGH)
  {
    if (current_item < max_items)
    {
      current_item++;
    }
  }
  else if (cur_btn_up == LOW && prev_btn_up == HIGH)
  {
    if (current_item > 1)
    {
      current_item--;
    }
  }
  else if (cur_btn_left == LOW && prev_btn_left == HIGH)
  {
    // code..
  }
  delay(20);

  switch (APP_STATE)
  {
  case MainMenu:
    switch (current_item)
    {
    case 1:
      menuDraw("Extras", EXTRAS_ITEMS);
      break;
    case 2:
      menuDraw("TEST", EXTRAS_ITEMS);
      break;
    case 3:
      menuDraw("TEST V2", EXTRAS_ITEMS);
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  prev_btn_up = cur_btn_up;
  prev_btn_down = cur_btn_down;
  prev_btn_left = cur_btn_left;
  prev_btn_right = cur_btn_right;
}

void menuDraw(const char *name, const char **itemsArray)
{
  u8g2.clearBuffer();
  u8g2.drawRFrame(7, 10, 46, 42, 5);

  u8g2.setFont(u8g2_font_t0_14b_me);
  u8g2.drawStr(60, 19, name);

  u8g2.setFont(u8g2_font_tiny5_tf);

  for (int i = 0; i < items_draw; i++)
  {
    u8g2.drawStr(62, 28 + (7 * i), itemsArray[i]);
  }

  u8g2.sendBuffer();
  delay(16);
}

/*=============суперудобныйразделитель=============*/

void setup()
{
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  init();
  loading();
  delay(1500);
}

void loop()
{
  input();
}
