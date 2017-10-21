/* GR-PEACH Sketch Template V1.02 */

#include <Arduino.h>
#include <SD.h>

#define INTERVAL 100


byte CmdData[48];
int ImageHeight = 0;
 
// Image Regster
// test.bmp -> AS-289R2
void ImageRegster() {
  File file = SD.open("test.bmp");  // Open Fiiename test.bmp
  if (file) {
    // Image Data Offset
    file.seek(10);
    int ImageOffset = (int)(file.read());
    //Serial.println(ImageOffset, HEX);
    // Image Height
    file.seek(22);
    ImageHeight = (int)(file.read());
    ImageHeight += (int)(file.read() * 256);
    //Serial.println(ImageHeight, HEX);
    // Command Output
    CmdData[0] = 0x1C;
    CmdData[1] = 0x2A;
    CmdData[2] = 0x62;
    CmdData[2] = 0x65;
    CmdData[3] = (byte)(ImageHeight / 256);
    CmdData[4] = (byte)(ImageHeight % 256);
    Serial1.write(CmdData, 5);
    Serial1.flush();
    // Image Data
    for (int i = 1; i <= ImageHeight; i ++) {
      int LeftPoint = ImageHeight * 48 + ImageOffset - (i * 48);
      file.seek(LeftPoint);
      for (int ii = 0; ii < 48; ii ++) {
        CmdData[ii] = (byte)(file.read() ^ 0xFF);
      }
      Serial1.write(CmdData, 48);
      Serial1.flush();
    }
    file.close();
  }
}

void setup()
{
	Serial.begin(9600);
	Serial1.begin(9600);
    pinMode(PIN_LED_RED   , OUTPUT);
    pinMode(PIN_LED_GREEN , OUTPUT);
    pinMode(PIN_LED_BLUE  , OUTPUT);
    pinMode(PIN_LED_USER  , OUTPUT);
    pinMode(PIN_SW        , INPUT);
    
  if (!SD.begin()) {
    Serial.println("Device error.");
    while (1);
  }
  ImageRegster(); // イメージ印字
}

void loop()
{
    digitalWrite(PIN_LED_RED, 1);
    delay(INTERVAL);
    digitalWrite(PIN_LED_RED, 0);
    digitalWrite(PIN_LED_GREEN, 1);
    delay(INTERVAL);
    digitalWrite(PIN_LED_GREEN, 0);
    digitalWrite(PIN_LED_BLUE, 1);
    delay(INTERVAL);
    digitalWrite(PIN_LED_BLUE, 0);
}

