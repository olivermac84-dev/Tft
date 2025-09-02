#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <AnimatedGIF.h>

// Wi-Fi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // Sri Lanka +5:30

// TFT and SD
TFT_eSPI tft = TFT_eSPI(); 
#define SD_CS_PIN 4

// Buttons
#define BUTTON_NEXT 16  // D0
#define BUTTON_PREV 5   // D1
#define BUTTON_MODE A0

// Clock / Media mode
bool showClock = true;

// Media
#define MAX_MEDIA 20
String mediaFiles[MAX_MEDIA];
int mediaCount = 0;
int currentMedia = 0;

// GIF
AnimatedGIF gif;

// ========================
// Setup
// ========================
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PREV, INPUT_PULLUP);
  pinMode(BUTTON_MODE, INPUT_PULLUP);

  // Init TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Init SD
  if(!SD.begin(SD_CS_PIN)){
    Serial.println("SD init failed!");
  } else {
    Serial.println("SD ready!");
    loadMediaFiles();
  }

  // Wi-Fi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // NTP
  timeClient.begin();
}

// ========================
// Load media from SD
// ========================
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
// Loop
// ========================
void loop() {
  timeClient.update();

  // Read buttons
  if(digitalRead(BUTTON_NEXT) == LOW){
    currentMedia = (currentMedia + 1) % mediaCount;
    delay(300);
  }
  if(digitalRead(BUTTON_PREV) == LOW){
    currentMedia = (currentMedia - 1 + mediaCount) % mediaCount;
    delay(300);
  }
  if(digitalRead(BUTTON_MODE) == LOW){
    showClock = !showClock;
    tft.fillScreen(TFT_BLACK);
    delay(300);
  }

  if(showClock){
    displayClock();
  } else {
    displayMedia();
  }
}

// ========================
// Display Clock
// ========================
void displayClock(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);

  String timeStr = timeClient.getFormattedTime();
  tft.setCursor(20, 100);
  tft.print(timeStr);

  delay(1000);
}

// ========================
// Display Media
// ========================
void displayMedia(){
  if(mediaFiles[currentMedia].endsWith(".bmp")){
    displayBMP(mediaFiles[currentMedia]);
    delay(3000);
  } else if(mediaFiles[currentMedia].endsWith(".gif")){
    displayGIF(mediaFiles[currentMedia]);
  }
}

// BMP display
void displayBMP(String filename){
  File bmpFile = SD.open("/media/" + filename);
  if(!bmpFile) return;
  tft.fillScreen(TFT_BLACK);
  tft.drawBmp(bmpFile, 0, 0);
  bmpFile.close();
}

// GIF display
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

// GIF callback
void GIFDraw(GIFDRAW *pDraw){
  uint16_t *bitmap = pDraw->pPixels;
  tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, bitmap);
}
