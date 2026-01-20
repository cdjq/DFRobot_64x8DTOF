#include "DFRobot_WY6005.h"
DFRobot_WY6005 wy6005(Serial1, SERIAL_8N1, 25, 26);
#define LINE_NUM    4
#define START_POINT 1
#define END_POINT   64
#define TOTAL_POINTS (END_POINT - START_POINT + 1)
void setup() {
  Serial.begin(115200);
  wy6005.begin(921600);
  bool ok = wy6005.configSingleFrameMode();
  if (!ok) {
      Serial.println("configContinuousFrameMode failed");
    }
   ok = wy6005.configSingleLineMode(LINE_NUM, START_POINT, END_POINT);
  if (!ok) {
    Serial.println("configSingleLineMode failed");
  } else {
    Serial.print("Configured single line mode: line=");
    Serial.print(LINE_NUM);
    Serial.print(", points=");
    Serial.print(START_POINT);
    Serial.print("..");
    Serial.println(END_POINT);
  }
}

void loop() {
  int parsed = wy6005.triggerGetRaw(wy6005.point.xBuf, wy6005.point.yBuf, wy6005.point.zBuf, wy6005.point.iBuf, TOTAL_POINTS, 300);
  if (parsed > 0) {
    Serial.print("Parsed ");
    Serial.print(parsed);
    Serial.println(" points:");
    for (int i = 0; i < parsed; i++) {
      char buf[80];
      sprintf(buf, "[%02d] x:%04dmm y:%04dmm z:%04dmm i:%04d", i, 
              wy6005.point.xBuf[i], wy6005.point.yBuf[i], wy6005.point.zBuf[i], wy6005.point.iBuf[i]);
      Serial.println(buf);
    }
  } else if (parsed == 0) {
    Serial.println("No points parsed");
  } else {
    Serial.println("Error or timeout while reading frame");
  }

  delay(1000);
}