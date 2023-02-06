#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 8, /* data=*/ 9, /* cs=*/ 5, /* dc=*/ 7, /* reset=*/ 6);

class Widget {
  protected:
    const unsigned char *font;
    u8g2_uint_t x;
    u8g2_uint_t y;
  public:
    Widget(u8g2_uint_t x, u8g2_uint_t y, const unsigned char *widgetFont) {
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
    virtual void drawBody() = 0;
  public:
    using Widget::Widget;
    void setValue(T value) {
      this -> value = value;
    }
    void draw() override {
      u8g2.setFont(font);
      drawBody();
    }
};

class ClockWidget : public BaseWidget<char*> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      u8g2.drawStr(x, y, value);
    }
};

class TemperatureWidget : public BaseWidget<char*> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      u8g2.drawUTF8(x, y, value);
    }
};

class HumidityWidget : public BaseWidget<char*> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      u8g2.drawStr(x, y, value);
    }
};

class WeatherWidget : public BaseWidget<char> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      u8g2.drawGlyph(x, y, 64 + value);
    }
};

class CarbonDioxideLevelWidget : public BaseWidget<char> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      u8g2.drawGlyph(x, y, 32 + value);
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

ClockWidget clockWidget = ClockWidget(38, 32, u8g2_font_logisoso32_tf);
WeatherWidget weatherWidget = WeatherWidget(0, 32, u8g2_font_open_iconic_weather_4x_t);
CarbonDioxideLevelWidget carbonDioxideLevelWidget = CarbonDioxideLevelWidget(52, 63, u8g2_font_emoticons21_tr);
HumidityWidget humidityWidget = HumidityWidget(74, 63, u8g2_font_logisoso20_tf);
TemperatureWidget temperatureWidget = TemperatureWidget(0, 63, u8g2_font_logisoso20_tf);
Widget* allWidgets[] = { &clockWidget, &weatherWidget, &carbonDioxideLevelWidget, &humidityWidget, &temperatureWidget };

void loop(void) {
  u8g2.clearBuffer();

  weatherWidget.setValue(weatherIcon);
  clockWidget.setValue(time);
  carbonDioxideLevelWidget.setValue(currentEmotic);
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

