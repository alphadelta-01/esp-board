#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

const int8_t BTN_DOWN  = 0;
const int8_t BTN_LEFT  = 1;
const int8_t BTN_UP    = 2;
const int8_t BTN_RIGHT = 3;

const int8_t I2C_SDA = 8;
const int8_t I2C_SCL = 9;

int8_t current_item = 1;

const char* TESTMENU_ITEMS[] = {
	"TEST1",
	"TEST2",
	"TEST3",
	"TEST4",
	"TEST5",
	"TEST6",
};

const char* TESTMENU2_ITEMS[] = {
	"TEST1remake",
	"TEST2remake",
	"TEST3remake",
	"TEST4remake",
	"TEST5remake",
	"TEST6remake",
};
const char* TESTMENU3_ITEMS[] = {
	"732",
	"732",
	"732",
	"732",
	"732",
	"732",
};

enum AppState {
	TestMenu,
	TestMenu2
};

AppState APP_STATE = TestMenu;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

/*=============суперудобныйразделитель=============*/

void loading(){
	u8g2.clearBuffer();
	
	u8g2.setFont(u8g2_font_t0_14b_me); u8g2.drawStr(30, 35, "esp-board");
	u8g2.setFont(u8g2_font_tiny5_tf);  u8g2.drawStr(50, 45, "loading...");
	
	u8g2.sendBuffer();
	delay(16);
}

void init(){
	Wire.begin(I2C_SDA, I2C_SCL);
	
	u8g2.begin();
	u8g2.setPowerSave(0);
}

void input(){
	int8_t max_items = 0;

	switch (APP_STATE)
	{
	case TestMenu:
		max_items = 3;
		break;
	
	default:
		break;
	}

	if(digitalRead(BTN_DOWN) == LOW && current_item < max_items){
		current_item++;
		delay(200);
	}
	else if(digitalRead(BTN_UP) == LOW && current_item > 1){
		current_item--;
		delay(200);
	}
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

void testMenu(){
	menuDraw("TestMenu", TESTMENU_ITEMS);
}

void testMenu2(){
	menuDraw("TestMenu2", TESTMENU2_ITEMS);
}

void testMenu3(){
	menuDraw("TestMenu3", TESTMENU3_ITEMS);
}

/*=============суперудобныйразделитель=============*/

void setup() {
	pinMode(BTN_DOWN,  INPUT_PULLUP);
	pinMode(BTN_LEFT,  INPUT_PULLUP);
	pinMode(BTN_UP,    INPUT_PULLUP);
	pinMode(BTN_RIGHT, INPUT_PULLUP);
	
	init();
	loading();
	delay(1500);
}

void loop() {
	input();
	switch (current_item)
	{
	case 1:
		testMenu();
		break;
		
	case 2:
		testMenu2();
		break;	
	case 3:
		testMenu3();
		break;	
	}
}

/*
скомпилить - arduino-cli compile --fqbn esp32:esp32:esp32c3 . && arduino-cli upload --fqbn esp32:esp32:esp32c3 --port COM5 .
																вот тут введи свой ком порт от платы(arduino-cli board list - чтобы посмотреть платы) ↑
*/