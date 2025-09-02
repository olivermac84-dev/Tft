#include <TFT_eSPI.h> // Or Adafruit_ILI9341 for Nano

TFT_eSPI tft = TFT_eSPI();  // Create TFT object

void setup() {
  Serial.begin(9600);
  
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Draw rectangles
  tft.fillRect(0, 0, 120, 120, TFT_RED);
  tft.fillRect(120, 0, 120, 120, TFT_GREEN);
  tft.fillRect(0, 120, 120, 120, TFT_BLUE);
  tft.fillRect(120, 120, 120, 120, TFT_YELLOW);

  // Draw text
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 10);
  tft.println("TFT Test!");

  tft.setCursor(10, 220);
  tft.println("Arduino Nano");
}

void loop() {
  // Color fill animation
  tft.fillScreen(TFT_BLACK);
  delay(500);
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_YELLOW);
  delay(500);
}
