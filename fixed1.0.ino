#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SD.h>
#include <AnimatedGIF.h>

// Global objects
TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;

// Example: Media file count
int mediaCount = 0;

void loadMediaFiles() {
    // Fixed serial output (NO printf)
    Serial.print("Loaded ");
    Serial.print(mediaCount);
    Serial.println(" media files");
}

void displayBMP(String bmpFile) {
    // TFT_eSPI does not have drawBmp. Use pushImage or your own BMP decoder.
    // For demonstration, we just fill the screen.
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("BMP display not implemented");
    // TODO: Implement BMP decoding or use available examples from TFT_eSPI.
}

void GIFDraw(GIFDRAW *pDraw) {
    // Correct pointer type and available members. Check AnimatedGIF library for struct details.
    // Example implementation for drawing a line of pixels
    tft.pushImage(pDraw->iX, pDraw->iY, pDraw->iWidth, 1, (uint16_t*)pDraw->pPixels);
    // NOTE: pPixels may be uint8_t*, so conversion may be needed
}

void displayGIF(String gifFileName) {
    // Open GIF from SD with proper callbacks
    File gifFile = SD.open(gifFileName.c_str());
    if (!gifFile) {
        Serial.println("GIF file not found!");
        return;
    }
    // Provide required callbacks (see AnimatedGIF examples for details)
    // This is pseudocode. You must define open, close, read, seek callbacks.
    gif.open(gifFileName.c_str(), GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw);

    int delay;
    while (gif.playFrame(false, &delay, NULL)) {
        delay(delay);
    }
    gif.close();
}

// You need to implement these callback functions for AnimatedGIF
void* GIFOpenFile(const char* fname, int32_t* pSize) {
    File* file = new File(SD.open(fname));
    if (file && *file) {
        *pSize = file->size();
        return file;
    }
    return NULL;
}

void GIFCloseFile(void* pHandle) {
    File* file = static_cast<File*>(pHandle);
    if (file) file->close();
    delete file;
}

int32_t GIFReadFile(GIFFILE* pFile, uint8_t* buf, int32_t len) {
    File* file = static_cast<File*>(pFile->fHandle);
    return file->read(buf, len);
}

int32_t GIFSeekFile(GIFFILE* pFile, int32_t position) {
    File* file = static_cast<File*>(pFile->fHandle);
    file->seek(position);
    return position;
}

void setup() {
    Serial.begin(115200);
    tft.init();
    if (!SD.begin()) {
        Serial.println("SD initialization failed!");
        return;
    }
    loadMediaFiles();
    displayBMP("/sample.bmp");
    displayGIF("/sample.gif");
}

void loop() {
    // Your main loop code
}
