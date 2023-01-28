#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 8, /* data=*/ 9, /* cs=*/ 5, /* dc=*/ 7, /* reset=*/ 6);

class A {

};

class B : public A {};

class Widget {
  protected:
    uint8_t *font;
    int x;
    int y;
  public:
    Widget(int x, int y, uint8_t *widgetFont) {
      font = widgetFont;
      this -> x = x;
      this -> y = y;
    }
    virtual void draw() = 0;
};

class ClockWidget : public Widget {
  public:
    ClockWidget(int x, int y, uint8_t *widgetFont) : Widget(x, y, widgetFont) { };
    void draw() override {

    }
};

void setup(void) {
  u8g2.begin();
  u8g2.setContrast(80);
  u8g2.enableUTF8Print();	
}

char weatherIcon = 0;
char* time = "23:09";
char* degree = "22°C";
char* humidity = "100%";
char currentEmotic = 0;

void loop(void) {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
  u8g2.drawGlyph(0, 32, 64 + weatherIcon);
  weatherIcon = (weatherIcon + 1) % 6;

  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.drawStr(38, 32, time);

  u8g2.drawLine(0, 33, 127, 33);

  u8g2.setFont(u8g2_font_logisoso20_tf);
  u8g2.drawUTF8(0, 63, degree);
  u8g2.drawStr(74, 63, humidity);

  u8g2.setFont(u8g2_font_emoticons21_tr);
  u8g2.drawGlyph(52, 63, 32 + currentEmotic);
  currentEmotic = (currentEmotic + 1) % 10;
  // u8g2.drawLine(50, 32, 50, 63);
  // u8g2.drawLine(75, 32, 75, 63);

  u8g2.sendBuffer();
  delay(1000);  
}

