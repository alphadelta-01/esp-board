#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include <time.h>

#define BTN_DOWN 0
#define BTN_LEFT 1
#define BTN_UP 2
#define BTN_RIGHT 3

#define I2C_SDA 8
#define I2C_SCL 9

#define WIFI_SSID     // enter your wifi ssid (name)
#define WIFI_PASSWORD // enter your wifi password

int8_t current_item = 1;

RTC_DATA_ATTR int8_t hours = 3;
RTC_DATA_ATTR int8_t minutes = 33;
RTC_DATA_ATTR int8_t seconds = 33;

RTC_DATA_ATTR int8_t utc = 3;

const char *EXTRAS_ITEMS[] = {
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
    "IDONTKNOW",
};

const int8_t EXTRAS_ITEMS_COUNT = sizeof(EXTRAS_ITEMS) / sizeof(EXTRAS_ITEMS[0]);

enum AppState
{
  MainMenu
};

AppState APP_STATE = MainMenu;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

struct tm timeInfo;

void syncTime()
{

  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  for (int8_t i = 0; i <= 100; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
    delay(500);
  }

  configTime(utc * 3600, 0, "pool.ntp.org", "time.google.com");

  while (!getLocalTime(&timeInfo))
  {
    delay(75);
  }
}

void updateClock()
{
  if (getLocalTime(&timeInfo))
  {
    hours   = timeInfo.tm_hour;
    minutes = timeInfo.tm_min;
    seconds = timeInfo.tm_sec;
  }
}



void init()
{
  Wire.begin(I2C_SDA, I2C_SCL);

  u8g2.begin();
  u8g2.setPowerSave(0);

  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.drawStr(4, 42, "esp-board");

  u8g2.setFont(u8g2_font_tiny5_tf);
  u8g2.drawStr(48, 62, "loading...");

  u8g2.sendBuffer();

  syncTime();
}

void statusBarDraw()
{
  char clockText[9];

  snprintf(clockText, sizeof(clockText), "%02d:%02d:%02d", hours, minutes, seconds);

  u8g2.setFont(u8g2_font_micro_tn);

  u8g2.drawStr(48, 64, clockText);
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

  prev_btn_up = cur_btn_up;
  prev_btn_down = cur_btn_down;
  prev_btn_left = cur_btn_left;
  prev_btn_right = cur_btn_right;
}

void menuDraw(const char *name, const char **itemsArray, int8_t itemsArrayCount)
{
  u8g2.clearBuffer();

  statusBarDraw();

  u8g2.drawRFrame(10, 10, 46, 42, 5);

  u8g2.setFont(u8g2_font_t0_14b_me);
  u8g2.drawStr(64, 19, name);

  u8g2.setFont(u8g2_font_tiny5_tf);

  int8_t items_draw = itemsArrayCount;

  for (int i = 0; i < items_draw; i++)
  {
    u8g2.drawStr(66, 28 + (7 * i), itemsArray[i]);
  }

  u8g2.sendBuffer();
  delay(16);
}

void drawAllMenu()
{
  switch (APP_STATE)
  {
  case MainMenu:
    switch (current_item)
    {
    case 1:
      menuDraw("Extras", EXTRAS_ITEMS, EXTRAS_ITEMS_COUNT);
      break;
    case 2:
      menuDraw("TEST", EXTRAS_ITEMS, EXTRAS_ITEMS_COUNT);
      break;
    case 3:
      menuDraw("TEST V2", EXTRAS_ITEMS, EXTRAS_ITEMS_COUNT);
      break;
    }
  }
}

void setup()
{
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  init();
}

void loop()
{
  updateClock();
  input();
  drawAllMenu();
}

// compile = arduino-cli compile -b esp32:esp32:esp32c3 -u -p (YOUR COM-PORT - arduino-cli board list)
