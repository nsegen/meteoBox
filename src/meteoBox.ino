#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 8, /* data=*/ 9, /* cs=*/ 5, /* dc=*/ 7, /* reset=*/ 6);

void setup(void) {
  u8g2.begin();
  u8g2.setContrast(80);
  u8g2.enableUTF8Print();	
}

char weatherIcon = 0;
char* time = "23:09";
char* degree = "22.5";

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory

  //weatherIcon[0] = 64 + currentWeather; open_iconic_weather_2x2
  // u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
  u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
  u8g2.drawGlyph(0, 32, 64 + weatherIcon);
  weatherIcon = (weatherIcon + 1) % 6;

  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.drawStr(36, 32, time);

  // u8g2.setCursor(51, 42);
  // u8g2.print(degree);
  u8g2.drawStr(0, 64, "22.5°C");		// requires enableUTF8Print()

  // draw("What a beautiful day!", SUN, 27);
  // draw("The sun's come out!", SUN_CLOUD, 19);
  // draw("It's raining cats and dogs.", RAIN, 8);
  // draw("That sounds like thunder.", THUNDER, 12);
  // draw("It's stopped raining", CLOUD, 15);

  // u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  // u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  
}

