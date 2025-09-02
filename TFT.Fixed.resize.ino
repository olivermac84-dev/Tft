#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <AnimatedGIF.h>

#define SD_CS 20

// Buttons
#define BTN_NEXT 21
#define BTN_PREV 22
#define BTN_MODE 26

// Brightness PWM
#define LED_PIN 15
int brightness = 200; // initial brightness 0-255

TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;

#define MAX_MEDIA 20
String mediaFiles[MAX_MEDIA];
int mediaCount = 0;
int currentMedia = 0;
bool gifMode = false;

// ========================
void setup() {
  Serial.begin(115200);

  // Buttons
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);

  // Brightness PWM
  analogWrite(LED_PIN, brightness);

  // TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // SD
  if(!SD.begin(SD_CS)){
    Serial.println("SD init failed!");
  } else {
    Serial.println("SD ready");
    loadMediaFiles();
  }
}

// Load media from SD
void loadMediaFiles(){
  File root = SD.open("/media");
  while(true){
    File entry = root.openNextFile();
    if(!entry) break;
    if(!entry.isDirectory()){
      String name = entry.name();
      if(name.endsWith(".bmp") || name.endsWith(".gif")){
        mediaFiles[mediaCount++] = name;
        if(mediaCount >= MAX_MEDIA) break;
      }
    }
    entry.close();
  }
  Serial.printf("Loaded %d media files\n", mediaCount);
}

// ========================
void loop() {
  handleButtons();

  String file = mediaFiles[currentMedia];
  
  // Auto adjust mode based on file type
  if(file.endsWith(".bmp")){
    if(gifMode) { // switch from GIF to BMP
      tft.fillScreen(TFT_BLACK);
      gif.close();
      gifMode = false;
    }
    displayBMP(file);
    delay(2000); // show BMP
  }
  else if(file.endsWith(".gif")){
    if(!gifMode){ // switch from BMP to GIF
      tft.fillScreen(TFT_BLACK);
      gifMode = true;
    }
    displayGIF(file);
  }
}

// ========================
// Button handling
// ========================
void handleButtons(){
  if(digitalRead(BTN_NEXT) == LOW){
    currentMedia = (currentMedia + 1) % mediaCount;
    delay(300);
  }
  if(digitalRead(BTN_PREV) == LOW){
    currentMedia = (currentMedia - 1 + mediaCount) % mediaCount;
    delay(300);
  }
  if(digitalRead(BTN_MODE) == LOW){
    // Adjust brightness
    brightness += 50;
    if(brightness > 255) brightness = 50;
    analogWrite(LED_PIN, brightness);
    delay(200);
  }
}

// ========================
// Display BMP
// ========================
void displayBMP(String filename){
  File bmpFile = SD.open("/media/" + filename);
  if(!bmpFile) return;
  tft.drawBmp(bmpFile, 0, 0);
  bmpFile.close();
}

// ========================
// Display GIF
// ========================
void displayGIF(String filename){
  File gifFile = SD.open("/media/" + filename);
  if(!gifFile) return;
  gif.open(gifFile, GIFDraw);
  while(gif.playFrame(false)){
    gif.update();
    yield();
  }
  gifFile.close();
}

// GIF draw callback
void GIFDraw(GIFDRAW *pDraw){
  uint16_t *bitmap = pDraw->pPixels;
  tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, bitmap);
}
