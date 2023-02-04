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

template <typename T> 
class BaseWidget : public Widget {
  protected:
    T value;
    int (*drawer)(int, int, T);
  public:
    BaseWidget(int x, int y, uint8_t *widgetFont, u8g2_uint_t (U8G2::*drawer)(u8g2_uint_t, u8g2_uint_t, T)) : Widget(x, y, widgetFont) {
      this -> drawer = drawer;
    }
    void setValue(T value) {
      this -> value = value;
    }  
    void draw() override {
      u8g2.setFont(font);
      drawer(x, y, value);
    }
};

class ClockWidget : public BaseWidget<const char*> {
  public:
    ClockWidget() : BaseWidget(38, 32, u8g2_font_logisoso32_tf, &u8g2.drawStr) { };
};

class TemperatureWidget : public BaseWidget<const char*> {
  public:
    TemperatureWidget() : BaseWidget(0, 63, u8g2_font_logisoso20_tf, &u8g2.drawUTF8) { };
};

class HumidityWidget : public BaseWidget<const char*> {
  public:
    HumidityWidget() : BaseWidget(74, 63, u8g2_font_logisoso20_tf, &u8g2.drawStr) { };
};

class WeatherWidget : public BaseWidget<uint16_t> {
  public:
    WeatherWidget() : BaseWidget(0, 32, u8g2_font_open_iconic_weather_4x_t, &u8g2.drawGlyph) { };
};

class CarbonDioxideLevelWidget : public BaseWidget<uint16_t> {
  public:
    CarbonDioxideLevelWidget() : BaseWidget(52, 63, u8g2_font_emoticons21_tr, &u8g2.drawGlyph) { };
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

ClockWidget clockWidget = ClockWidget();
WeatherWidget weatherWidget = WeatherWidget();
CarbonDioxideLevelWidget carbonDioxideLevelWidget = CarbonDioxideLevelWidget();
HumidityWidget humidityWidget = HumidityWidget();
TemperatureWidget temperatureWidget = TemperatureWidget();
Widget* allWidgets[] = { &clockWidget, &weatherWidget, &carbonDioxideLevelWidget, &humidityWidget, &temperatureWidget };

void loop(void) {
  u8g2.clearBuffer();

  weatherWidget.setValue(64 +  weatherIcon);
  clockWidget.setValue(time);
  carbonDioxideLevelWidget.setValue(32 + currentEmotic);
  humidityWidget.setValue(humidity);
  temperatureWidget.setValue(degree);

  for (Widget* widget : allWidgets) {
    widget -> draw();
  }
  u8g2.drawLine(0, 33, 127, 33);

  weatherIcon = (weatherIcon + 1) % 6;
  currentEmotic = (currentEmotic + 1) % 10;
  // u8g2.drawLine(50, 32, 50, 63);
  // u8g2.drawLine(75, 32, 75, 63);

  u8g2.sendBuffer();
  delay(1000);  
}

