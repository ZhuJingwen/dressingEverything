/*********************************************************************
Written by Kevin Townsend/KTOWN  for Adafruit Industries.
MIT license, check LICENSE for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include "Adafruit_BLE_UART.h"

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

// Neopixel
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(20, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(20, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(20, 12, NEO_GRB + NEO_KHZ800);

char inChar;
unsigned long interval = 20;
uint16_t count = 0, count2 = 0, count3 = 0, count4 = 0;
int currentBri;
int startBri;
int fullBri = 100;
int currPixel = 0;
boolean breatheSwitch = false;
boolean breatheSwitch2 = false;
boolean breatheSwitch3 = false;

float pos1 = 0, speed1 = 1;
float pos2 = -6, speed2 = 1;
float pos3 = -10, speed3 = 1;

uint32_t blueGradient[] = {0x0028C7, 0x0045CD, 0x0062D3, 0x007FD9, 0x009DE0, 0x00BAE6, 0x00D7EC, 0x00F5F3};
uint32_t greenToPurple[] = {0x2DD600, 0x39A827, 0x457A4F, 0x514C77, 0x5D1E9F};
uint32_t greenToOrange[] = {0x2DD600, 0x60BD03, 0x94A407, 0xC78B0A, 0xFB720E};
uint32_t greenPurple[] = {0x369C30, 0x4E864D, 0x67706A, 0x7F5A87, 0x9845A5};

#define NUM_PIXELS 20;

/**************************************************************************/
/*!
    This function is called whenever select ACI events happen
*/
/**************************************************************************/
void aciCallback(aci_evt_opcode_t event)
{
  switch (event)
  {
    case ACI_EVT_DEVICE_STARTED:
      Serial.println(F("Advertising started"));
      break;
    case ACI_EVT_CONNECTED:
      Serial.println(F("Connected!"));
      break;
    case ACI_EVT_DISCONNECTED:
      Serial.println(F("Disconnected or advertising timed out"));
      break;
    default:
      break;
  }
}

/**************************************************************************/
/*!
    This function is called whenever data arrives on the RX channel
*/
/**************************************************************************/
void rxCallback(uint8_t *buffer, uint8_t len)
{
  Serial.print(F("Received "));
    Serial.print(len);
    Serial.print(F(" bytes: "));
    for(int i=0; i<len; i++)
     Serial.print((char)buffer[i]);
  
    Serial.print(F(" ["));

    for(int i=0; i<len; i++)
    {
      Serial.print(" 0x"); Serial.print((char)buffer[i], HEX);
    }
    Serial.println(F(" ]"));

  count = 0;
  count2 = 0;
  startBri = currentBri;
  currPixel = 0;

  inChar = (char)buffer[0];

  /* Echo the same data back! */
  uart.write(buffer, len);
}

/**************************************************************************/
/*!
    Configure the Arduino and start advertising with the radio
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
//  while (!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Dressing Everything"));

  uart.setRXcallback(rxCallback);
  uart.setACIcallback(aciCallback);
  uart.setDeviceName("Dress"); /* 7 characters max! */
  uart.begin();

  strip1.begin();
  strip1.show();
  strip2.begin();
  strip2.show();
  strip3.begin();
  strip3.show();

}

/**************************************************************************/
/*!
    Constantly checks for new events on the nRF8001
*/
/**************************************************************************/
void loop()
{
  uart.pollACI();
    switch (inChar) {
    case 'a':
      art();
      break;
    case 'b':
      if ((unsigned long)(millis() % 5) == 0) {
        breathe();
      }
      break;
    case 'c':
      if ((unsigned long)(millis() % 4) == 0) {
        wind();
        count3 = count2 + fullBri;
        count4 = count3 + fullBri;
      }
      break;
    case 'd':
      if ((unsigned long)(millis() % interval) == 0) {
        fadeOut();
      }
      break;
    case 'f':
      turnOff();
      break;
    case 'h':
      if ((unsigned long)(millis() % 4) == 0) {
        heart();
      }
      break;
    case 'i':
      if ((unsigned long)(millis() % interval) == 0) {
        icecream();
      }
      break;
    case 'm':
      if ((unsigned long)(millis() % 30) == 0) {
        movie();
      }
      break;
    case 'n':
      if ((unsigned long)(millis() % interval) == 0) {
        fadeIn();
      }
      break;
    case 'o':
      turnOn();
      break;
    case 'r':
      if ((unsigned long)(millis() % 5) == 0) {
        rainbow(85, 170);
      }
      break;
    case 'w':
      if ((unsigned long)(millis() % 4) == 0) {
        wind();
        count3 = count2 + fullBri;
        count4 = count3 + fullBri;
      }
      break;
    case 'z':
      if ((unsigned long)(millis() % 30) == 0) {
        rain();
      }
      break;
  }
}

void turnOn() {
  for (int i = 0; i < 20; i++) {
    strip1.setPixelColor(i, 0, 0, 100);
    strip2.setPixelColor(i, 0, 0, 100);
    strip3.setPixelColor(i, 0, 0, 100);
    strip1.setBrightness(100);
    strip2.setBrightness(100);
    strip3.setBrightness(100);
    strip1.show();
    strip2.show();
    strip3.show();
    currentBri = 100;
  }
}

void turnOff() {
  for (int i = 0; i < 20; i++) {
    strip1.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip3.setPixelColor(i, 0, 0, 0);
    strip1.setBrightness(0);
    strip2.setBrightness(0);
    strip3.setBrightness(0);
    strip1.show();
    strip2.show();
    strip3.show();
    currentBri = 0;
  }
}

void fadeOut() {
  if (count >= startBri) {
    count = startBri;
  } else {
    count++;
  }
  strip1.setBrightness(startBri - count);
  strip2.setBrightness(startBri - count);
  strip3.setBrightness(startBri - count);
  strip1.show();
  strip2.show();
  strip3.show();
  currentBri = startBri - count;
  //Serial.println(currentBri);
}

void fadeIn() {
  for (int i = 0; i < 20; i++) {
    strip1.setPixelColor(i, 0, 0, 100);
    strip2.setPixelColor(i, 0, 0, 100);
    strip3.setPixelColor(i, 0, 0, 100);
  }
  strip1.setBrightness(startBri + count);
  strip2.setBrightness(startBri + count);
  strip3.setBrightness(startBri + count);
  strip1.show();
  strip2.show();
  strip3.show();
  currentBri = startBri + count;
  if (count >= (fullBri - startBri)) {
    count = (fullBri - startBri);
  } else {
    count++;
  }
  // Serial.println(currentBri);
}

void breathe() {
  for (int i = 0; i < 20; i++) {
    strip1.setPixelColor(i, 100, 100, 200);
    strip2.setPixelColor(i, 100, 100, 200);
    strip3.setPixelColor(i, 100, 100, 200);
  }
  strip1.setBrightness(count);
  strip2.setBrightness(count);
  strip3.setBrightness(count);
  strip1.show();
  strip2.show();
  strip3.show();

  if (count >= fullBri) {
    breatheSwitch = true;
  } else if (count <= 0) {
    breatheSwitch = false;
  }

  if (breatheSwitch == false) {
    count++;
  } else {
    count--;
  }
}

void heart() {
  for (int i = 0; i < 2; i++) {
    strip2.setPixelColor(i, 200, 30, 30);
    strip3.setPixelColor(i, 200, 30, 30);
  }

  strip1.setBrightness(count);
  strip2.setBrightness(count);
  strip3.setBrightness(count);
  strip1.show();
  strip2.show();
  strip3.show();

  for (int j = 0; j < 20; j++) {
    strip1.setPixelColor(j, 0, 0, 0);
    strip2.setPixelColor(j, 0, 0, 0);
    strip3.setPixelColor(j, 0, 0, 0);
  }

  if (count >= fullBri) {
    breatheSwitch = true;
  } else if (count <= 0) {
    breatheSwitch = false;
  }

  if (breatheSwitch == false) {
    count++;
  } else {
    count--;
  }
}


void rainbow(int startColor, int endColor) {
  uint16_t i;

  if (count >= endColor - startColor) {
    breatheSwitch = true;
  } else if (count <= 0) {
    breatheSwitch = false;
  }

  if (breatheSwitch == false) {
    count++;
  } else {
    count--;
  }
  for (i = 0; i < 20; i++) {
    strip1.setPixelColor(i, Wheel((i + count + startColor)));
    strip2.setPixelColor(i, Wheel((i + count + startColor + 5)));
    strip3.setPixelColor(i, Wheel((i + count + startColor + 10)));
  }
  strip1.setBrightness(fullBri);
  strip2.setBrightness(fullBri);
  strip3.setBrightness(fullBri);
  strip1.show();
  strip2.show();
  strip3.show();
}

void cloud() {
  int j;
  for (int i = -5; i < 3; i++) {
    strip1.setPixelColor(pos1 + i, blueGradient[i + 4]);
  }
  for (int i = -4; i < 4; i++) {
    strip2.setPixelColor(pos2 + i, blueGradient[i + 4]);
  }
  for (int i = -3; i < 5; i++) {
    strip3.setPixelColor(pos3 + i, blueGradient[i + 4]);
  }
  strip1.setBrightness(fullBri / 2);
  strip2.setBrightness(fullBri / 2);
  strip3.setBrightness(fullBri / 2);
  strip1.show();
  strip2.show();
  strip3.show();


  for (j = -5; j < 3; j++) strip1.setPixelColor(pos1 + j, 0);
  for (j = -4; j < 4; j++) strip2.setPixelColor(pos2 + j, 0);
  for (j = -3; j < 5; j++) strip3.setPixelColor(pos3 + j, 0);

  if (pos1 >= 20) {
    pos1 -= 20;
  } else {
    pos1 += speed1;
  }

  if (pos2 >= 20) {
    pos2 -= 20;
  } else {
    pos2 += speed2;
  }

  if (pos3 >= 20) {
    pos3 -= 20;
  } else {
    pos3 += speed3;
  }

}

void wind() {
  uint16_t i;
  int startColor = 115;
  int endColor = 155;
  bool colorSwitch;

  if (count >= endColor - startColor) {
    colorSwitch = true;
  } else if (count <= 0) {
    colorSwitch = false;
  }

  if (colorSwitch == false) {
    count++;
  } else {
    count--;
  }
  for (i = 0; i < 20; i++) {
    strip1.setPixelColor(i, Wheel((i + count + startColor)));
    strip2.setPixelColor(i, Wheel((i + count + startColor + 5)));
    strip3.setPixelColor(i, Wheel((i + count + startColor + 10)));
  }

  strip1.setBrightness(count2);
  strip2.setBrightness(count2);
  strip3.setBrightness(count2);
  strip1.show();
  strip2.show();
  strip3.show();

  if (count2 >= fullBri) {
    breatheSwitch = true;
  } else if (count2 <= 0) {
    breatheSwitch = false;
  }
  if (breatheSwitch == false) {
    count2++;
  } else {
    count2--;
  }

  if (count3 >= fullBri) {
    breatheSwitch2 = true;
  } else if (count3 <= 0) {
    breatheSwitch2 = false;
  }
  if (breatheSwitch2 == false) {
    count3++;
  } else {
    count3--;
  }

  if (count4 >= fullBri) {
    breatheSwitch3 = true;
  } else if (count4 <= 0) {
    breatheSwitch3 = false;
  }
  if (breatheSwitch3 == false) {
    count4++;
  } else {
    count4--;
  }
}

void icecream() {

  int i = count % 20;
  count++;
  Serial.println(i);
  strip1.setPixelColor(i, 236 + i, 156 + i, 176);
  strip2.setPixelColor(i, 236 + i, 156 + i, 176);
  strip3.setPixelColor(i, 236 + i, 156 + i, 176);
  strip1.setBrightness(fullBri / 2);
  strip2.setBrightness(fullBri / 2);
  strip3.setBrightness(fullBri / 2);
  strip1.show();
  strip2.show();
  strip3.show();


}

void art() {
  for (int i = 0; i < 4; i++) {
    strip1.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 0, 255, 0);
    strip3.setPixelColor(i, 0, 255, 0);
  }

  for (int i = 4; i < 7 ; i++) {
    strip1.setPixelColor(i, 0, 0, 255);
    strip2.setPixelColor(i, 0, 0, 255);
    strip3.setPixelColor(i, 255, 255, 255);
  }

  for (int i = 7; i < 11 ; i++) {
    strip1.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 0);
    strip3.setPixelColor(i, 255, 255, 0);
  }

  for (int i = 11; i < 13 ; i++) {
    strip1.setPixelColor(i, 255, 0, 0);
    strip2.setPixelColor(i, 255, 255, 255);
    strip3.setPixelColor(i, 255, 255, 255);
  }

  for (int i = 13; i < 16 ; i++) {
    strip1.setPixelColor(i, 255, 0, 0);
    strip2.setPixelColor(i, 255, 0, 0);
    strip3.setPixelColor(i, 255, 255, 255);
  }

  for (int i = 16; i < 20 ; i++) {
    strip1.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip3.setPixelColor(i, 255, 0, 0);
  }

  strip1.setBrightness(fullBri / 2);
  strip2.setBrightness(fullBri / 2);
  strip3.setBrightness(fullBri / 2);
  strip1.show();
  strip2.show();
  strip3.show();
}


void movie() {
  int j;
  for (int i = -4; i < 1; i++) {
    strip1.setPixelColor(pos1 + i, greenPurple[i + 4]);
  }
  for (int i = -4; i < 1; i++) {
    strip2.setPixelColor(pos2 + i, greenPurple[i + 4]);
  }
  for (int i = -4; i < 1; i++) {
    strip3.setPixelColor(pos3 + i, greenPurple[i + 4]);
  }
  strip1.show();
  strip2.show();
  strip3.show();

  for (j = -4; j < 1; j++) strip1.setPixelColor(pos1 + j, 0);
  for (j = -4; j < 1; j++) strip2.setPixelColor(pos2 + j, 0);
  for (j = -4; j < 1; j++) strip3.setPixelColor(pos3 + j, 0);

  if (pos1 >= 20) {
    pos1 -= 20;
  } else {
    pos1 += speed1;
  }

  if (pos2 >= 20) {
    pos2 -= 25;
  } else {
    pos2 += speed2;
  }

  if (pos3 >= 20) {
    pos3 -= 30;
  } else {
    pos3 += speed3;
  }
}

void rain() {
  int j;
  for (int i = -4; i < 4; i++) {
    strip1.setPixelColor(pos1 + i, blueGradient[i + 4]);
  }
  for (int i = -4; i < 4; i++) {
    strip2.setPixelColor(pos2 + i, blueGradient[i + 4]);
  }
  for (int i = -4; i < 4; i++) {
    strip3.setPixelColor(pos3 + i, blueGradient[i + 4]);
  }
  strip1.setBrightness(fullBri / 2);
  strip2.setBrightness(fullBri / 2);
  strip3.setBrightness(fullBri / 2);
  strip1.show();
  strip2.show();
  strip3.show();


  for (j = -4; j < 4; j++) strip1.setPixelColor(pos1 + j, 0);
  for (j = -4; j < 4; j++) strip2.setPixelColor(pos2 + j, 0);
  for (j = -4; j < 4; j++) strip3.setPixelColor(pos3 + j, 0);

  if (pos1 >= 20) {
    pos1 -= 20;
  } else {
    pos1 += speed1;
  }

  if (pos2 >= 20) {
    pos2 -= 25;
  } else {
    pos2 += speed2;
  }

  if (pos3 >= 20) {
    pos3 -= 30;
  } else {
    pos3 += speed3;
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

