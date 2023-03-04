#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

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

class TemperatureWidget : public BaseWidget<uint8_t> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      char widgetValue[3];

      snprintf(widgetValue, 3, "%d%", value);
      u8g2.drawUTF8(x, y, value);
    }
};

class HumidityWidget : public BaseWidget<uint8_t> {
  public:
    using BaseWidget::BaseWidget;
  protected:
    void drawBody() override {
      char widgetValue[3];

      snprintf(widgetValue, 3, "%d%", value);
      u8g2.drawStr(x, y, widgetValue);
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

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme = Adafruit_BME280(); // I2C
char weatherIcon = 0;
char* time = "23:09";
uint8_t degree; //"22°C";
uint8_t humidity; //"100%";
char currentEmotic = 0;
bool status;


ClockWidget clockWidget = ClockWidget(38, 32, u8g2_font_logisoso32_tf);
WeatherWidget weatherWidget = WeatherWidget(0, 32, u8g2_font_open_iconic_weather_4x_t);
CarbonDioxideLevelWidget carbonDioxideLevelWidget = CarbonDioxideLevelWidget(52, 63, u8g2_font_emoticons21_tr);
HumidityWidget humidityWidget = HumidityWidget(74, 63, u8g2_font_logisoso20_tf);
TemperatureWidget temperatureWidget = TemperatureWidget(0, 63, u8g2_font_logisoso20_tf);
Widget* allWidgets[] = { &clockWidget, &weatherWidget, &carbonDioxideLevelWidget, &humidityWidget, &temperatureWidget };

void setup(void) {
  Serial.begin(9600);
  
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  Serial.println(F("Before sensor"));
  status = bme.begin(BME280_ADDRESS_ALTERNATE);
  Serial.println(F("After sensor"));  
  u8g2.begin();
  u8g2.setContrast(80);
  u8g2.enableUTF8Print();
}

void readSensors() {
  bme.takeForcedMeasurement();
  degree = bme.readTemperature();
  humidity = bme.readHumidity();
  // dispTemp = bme.readTemperature();
  // dispHum = bme.readHumidity();
  // dispPres = (float)bme.readPressure() * 0.00750062;
  // dispCO2 = mhz19.getPPM();

  // if (dispCO2 < 800) setLED(2);
  // else if (dispCO2 < 1200) setLED(3);
  // else if (dispCO2 >= 1200) setLED(1);
}

void loop(void) {
  u8g2.clearBuffer();

  if (!status) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
  }
  readSensors();

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

