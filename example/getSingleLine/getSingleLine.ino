/*!
 * @file getSingleLine.ino
 * @brief Example of getting single line data
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (https://github.com/PLELES)
 * @version V1.0
 * @date 2026-1-21
 * @url https://github.com/DFRobot/DFRobot_64x8DTOF
 */
#include "DFRobot_64x8DTOF.h"
DFRobot_64x8DTOF dtof64x8(Serial1, SERIAL_8N1, 25, 26);
#define LINE_NUM    4

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  while(!dtof64x8.begin());
  Serial.println("Configuring frame mode: Single Frame...");
  while (!dtof64x8.configFrameMode(DFRobot_64x8DTOF::eFrameSingle)) {
    Serial.println("configFrameMode failed, retrying...");
    delay(200);
  }
  Serial.println("Configuration successful.");
  delay(300);
  // Configure single line mode (retry until success)
  Serial.println("Configuring Single Line Mode...");
  while (!dtof64x8.configMeasureMode(LINE_NUM)) {
    Serial.println("Configuration failed, retrying...");
    delay(200);
  }
  Serial.println("Configuration successful.");
  Serial.print("Mode: Single Line (Line: ");
  Serial.print(LINE_NUM);
  Serial.println(")");
  delay(300);
}

void loop()
{
  int cnt = dtof64x8.getData(300);
  Serial.print("Got points: ");
  Serial.println(cnt);
  if (cnt > 0) {
    for (int i = 0; i < cnt; i++) {
      char numbuf[16];
      Serial.print(1 + i);
      Serial.print(": X="); sprintf(numbuf, "%04d", dtof64x8.point.xBuf[i]); Serial.print(numbuf); Serial.print(" mm ");
      Serial.print("Y="); sprintf(numbuf, "%04d", dtof64x8.point.yBuf[i]); Serial.print(numbuf); Serial.print(" mm ");
      Serial.print("Z="); sprintf(numbuf, "%04d", dtof64x8.point.zBuf[i]); Serial.print(numbuf); Serial.print(" mm ");
      Serial.print("I="); Serial.println(dtof64x8.point.iBuf[i]);
    }
  }else {
    Serial.println("getData timeout or error");
  }
  delay(500);
}
