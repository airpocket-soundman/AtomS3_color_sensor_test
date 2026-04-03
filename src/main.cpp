#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

namespace {

constexpr uint8_t I2C_SDA_PIN = 2;
constexpr uint8_t I2C_SCL_PIN = 1;

Adafruit_TCS34725 colorSensor(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_4X);

uint16_t toDisplayColor(uint8_t r, uint8_t g, uint8_t b) {
  return M5.Display.color565(r, g, b);
}

void showMessage(const char* line1, const char* line2, uint16_t bgColor) {
  M5.Display.fillScreen(bgColor);
  M5.Display.setTextColor(WHITE, bgColor);
  M5.Display.setTextDatum(middle_center);
  M5.Display.setTextSize(1);
  M5.Display.drawString(line1, M5.Display.width() / 2, 46);
  M5.Display.drawString(line2, M5.Display.width() / 2, 68);
}

void applyMeasuredColor() {
  uint16_t rawR = 0;
  uint16_t rawG = 0;
  uint16_t rawB = 0;
  uint16_t rawC = 0;
  colorSensor.getRawData(&rawR, &rawG, &rawB, &rawC);

  if (rawC == 0) {
    showMessage("No light", "Try again", TFT_BLACK);
    Serial.println("Color read failed: clear channel is zero.");
    return;
  }

  const float scale = 255.0f / static_cast<float>(rawC);
  const uint8_t r = static_cast<uint8_t>(min(255.0f, rawR * scale));
  const uint8_t g = static_cast<uint8_t>(min(255.0f, rawG * scale));
  const uint8_t b = static_cast<uint8_t>(min(255.0f, rawB * scale));
  const uint16_t screenColor = toDisplayColor(r, g, b);

  M5.Display.fillScreen(screenColor);
  M5.Display.setTextColor(TFT_WHITE, screenColor);
  M5.Display.setTextDatum(top_left);
  M5.Display.setTextSize(1);
  M5.Display.drawString("BtnA: read color", 6, 6);

  Serial.printf("R:%u G:%u B:%u C:%u -> screen RGB(%u,%u,%u)\n",
                rawR, rawG, rawB, rawC, r, g, b);
}

}  // namespace

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  M5.Display.setRotation(0);
  M5.Display.setFont(&fonts::Font2);

  if (!colorSensor.begin()) {
    showMessage("Color Unit", "not found", TFT_RED);
    Serial.println("Color Unit (TCS34725/TCS3472) not found on I2C.");
    while (true) {
      M5.update();
      delay(10);
    }
  }

  showMessage("Press BtnA", "to read color", TFT_BLACK);
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    applyMeasuredColor();
  }

  delay(10);
}
