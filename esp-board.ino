#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

const int8_t BTN_DOWN  = 0;
const int8_t BTN_LEFT  = 1;
const int8_t BTN_UP    = 2;
const int8_t BTN_RIGHT = 3;

const int8_t I2C_SDA = 8;
const int8_t I2C_SCL = 9;

const char* TESTMENU_ITEMS[] = {
	"TEST1",
	"TEST2",
	"TEST3",
	"TEST4",
	"TEST5",
	"TEST6",
};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

/*=============суперудобныйразделитель=============*/

void init(){
	Wire.begin(I2C_SDA, I2C_SCL);
	
	u8g2.begin();
	u8g2.setPowerSave(0);
}

void menuDraw(const char* name, const char** itemsArray){
	u8g2.clearBuffer();
	u8g2.drawRFrame(7, 10, 46, 42, 5);
	
	u8g2.setFont(u8g2_font_t0_14b_me);
	u8g2.drawStr(60 ,19, name);
	
	u8g2.setFont(u8g2_font_tiny5_tf);
	
	u8g2.drawStr(62, 28, itemsArray[0]);	
	u8g2.drawStr(62, 35, itemsArray[1]);
	u8g2.drawStr(62, 42, itemsArray[2]);
	u8g2.drawStr(62, 49, itemsArray[3]);
	
	u8g2.sendBuffer();
	delay(16);
}

/*=============суперудобныйразделитель=============*/

void setup() {
	pinMode(BTN_DOWN,  INPUT_PULLUP);
	pinMode(BTN_LEFT,  INPUT_PULLUP);
	pinMode(BTN_UP,    INPUT_PULLUP);
	pinMode(BTN_RIGHT, INPUT_PULLUP);
	
	init();
}

void loop() {
	menuDraw("TestMenu", TESTMENU_ITEMS);
}

/*
скомпилить - arduino-cli compile --fqbn esp32:esp32:esp32c3 . && arduino-cli upload --fqbn esp32:esp32:esp32c3 --port COM5 .
								вот тут введи свой ком порт от платы(arduino-cli board list - чтобы посмотреть платы) ↑
*/