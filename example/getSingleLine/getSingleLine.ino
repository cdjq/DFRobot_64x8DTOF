/*!
 * @file getSingleLine.ino
 * @brief Example of getting single line data
 * @copyright  Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [PLELES] (https://github.com/PLELES)
 * @version V1.0
 * @date 2026-1-21
 * @url https://github.com/DFRobot/DFRobot_WY6005
 */
#include "DFRobot_WY6005.h"
DFRobot_WY6005 wy6005(Serial1, SERIAL_8N1, 25, 26);
#define LINE_NUM    3
#define START_POINT 1
#define END_POINT   64
#define TOTAL_POINTS (END_POINT - START_POINT + 1)

void setup()
{
  Serial.begin(115200);
  wy6005.begin(921600);

  if (!wy6005.configSingleFrameMode()) {
    Serial.println("configSingleFrameMode failed");
  }

  Serial.println("Configuring Single Line Mode...");
 
  if (!wy6005.configSingleLineMode(LINE_NUM, START_POINT, END_POINT)) {
    Serial.println("Configuration failed!");
  } else {
    Serial.println("Configuration successful.");
    Serial.print("Mode: Single Line (Line: ");
    Serial.print(LINE_NUM);
    Serial.print(", Start: ");
    Serial.print(START_POINT);
    Serial.print(", End: ");
    Serial.print(END_POINT);
    Serial.println(")");
  }
  while(Serial1.available()) Serial1.read();
  delay(500);
}

void loop()
{
  int parsed = wy6005.triggerGetRaw(wy6005.point.xBuf, wy6005.point.yBuf, wy6005.point.zBuf, wy6005.point.iBuf, TOTAL_POINTS, 600);
  Serial.println(parsed);
  if (parsed > 0) {
    Serial.print("Parsed ");
    Serial.print(parsed);
    Serial.println(" points:");
    for (int i = 0; i < parsed; i++) {
      char buf[80];
      sprintf(buf, "[%02d] x:%04dmm y:%04dmm z:%04dmm i:%04d", i, wy6005.point.xBuf[i], wy6005.point.yBuf[i], wy6005.point.zBuf[i], wy6005.point.iBuf[i]);
      Serial.println(buf);
    }
  } else if (parsed == 0) {
    Serial.println("No points parsed");
  } else {
    Serial.println("Error or timeout while reading frame");
  }
}
