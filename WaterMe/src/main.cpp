#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define POWER_PIN  17
#define SIGNAL_PIN 34 
const int SCREEN_WIDTH = 128;

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); 

int waterstatus = 0; 

int roundnumber(int waterstatus) {
  return (waterstatus + 50) / 100 * 100;
} 
void setup() {
  Serial.begin(115200);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 24, "Hello World!");
  u8g2.sendBuffer();

  analogSetAttenuation(ADC_11db);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW); 
}

void loop() {
  digitalWrite(POWER_PIN, HIGH);
  waterstatus = analogRead(SIGNAL_PIN); 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  waterstatus = roundnumber(waterstatus);
  Serial.print("The water sensor value: ");
  Serial.println(waterstatus);
  u8g2.setFont(u8g2_font_ncenB08_tr);

  const char* line1 = "";
  const char* line2 = "";

  if (waterstatus < 1300){
    line1 = "Wet";
    line2 = "Soil: moist";
  }
  else if (waterstatus < 1800){
    line1 = "Very moist";
    line2 = "Soil: slightly wet";
  }
  else if (waterstatus < 2600){
    line1 = "Moist";
    line2 = "Soil: moderate";
  }
  else if (waterstatus < 3000 ){
    line1 = "Water me!";
    line2 = "Soil: dry";
  }
  else {
    line1 = "No water! Water me!";
    line2 = "Soil: very dry";
  }

  int x1 = (SCREEN_WIDTH - u8g2.getStrWidth(line1)) / 2;
  int x2 = (SCREEN_WIDTH - u8g2.getStrWidth(line2)) / 2;

  u8g2.drawStr(x1, 24, line1);
  u8g2.drawStr(x2, 40, line2);
  u8g2.sendBuffer();
  delay(1000);
}