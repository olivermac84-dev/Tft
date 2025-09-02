#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <AnimatedGIF.h>

#ifdef ESP8266
#define SD_CS 4
#define BUTTON_NEXT 16
#define BUTTON_PREV 5
#define BUTTON_MODE A0
#else
#define SD_CS 20
#define BUTTON_NEXT 21
#define BUTTON_PREV 22
#define BUTTON_MODE 26
#endif

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
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PREV, INPUT_PULLUP);
  pinMode(BUTTON_MODE, INPUT_PULLUP);

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
  // Buttons
  if(digitalRead(BUTTON_NEXT) == LOW){
    currentMedia = (currentMedia + 1) % mediaCount;
    delay(300);
  }
  if(digitalRead(BUTTON_PREV) == LOW){
    currentMedia = (currentMedia - 1 + mediaCount) % mediaCount;
    delay(300);
  }
  if(digitalRead(BUTTON_MODE) == LOW){
    gifMode = !gifMode;
    tft.fillScreen(TFT_BLACK);
    delay(300);
  }

  // Display
  String file = mediaFiles[currentMedia];
  if(file.endsWith(".bmp") && !gifMode){
    displayBMP(file);
    delay(2000); // show BMP
  }
  if(file.endsWith(".gif") && gifMode){
    displayGIF(file);
  }
}

// Display BMP
void displayBMP(String filename){
  File bmpFile = SD.open("/media/" + filename);
  if(!bmpFile) return;

  tft.fillScreen(TFT_BLACK);

  // Draw in chunks to save RAM
  tft.drawBmp(bmpFile, 0, 0);

  bmpFile.close();
}

// Display GIF
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
