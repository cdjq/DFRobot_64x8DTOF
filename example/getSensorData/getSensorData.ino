#include "DFRobot_WY6005.h"
DFRobot_WY6005 wy6005(Serial1, SERIAL_8N1, 25, 26);
#define LINE_NUM    3
#define START_POINT 1
#define END_POINT   64

/**
 * Configure the demo for different modes:
 * 1: Single Point Mode
 * 2: Single Line Mode
 */
#define DEMO_MODE 2

// Calculate expected points based on mode
#if DEMO_MODE == 1
#define TOTAL_POINTS 1
#elif DEMO_MODE == 2
#define TOTAL_POINTS (END_POINT - START_POINT + 1)
#endif

void setup()
{
  Serial.begin(115200);
  wy6005.begin(921600);

  bool ok = false;

  // Ensure single frame mode for this demo
  if (!wy6005.configSingleFrameMode()) {
    Serial.println("configSingleFrameMode failed");
  }

#if DEMO_MODE == 1
  Serial.println("Configuring Single Point Mode...");
  // Example: Line 4, Point 32
  ok = wy6005.configSinglePointMode(4, 32);
#elif DEMO_MODE == 2
  Serial.println("Configuring Single Line Mode...");
  ok = wy6005.configSingleLineMode(LINE_NUM, START_POINT, END_POINT);
#endif

  if (!ok) {
    Serial.println("Configuration failed!");
  } else {
    Serial.println("Configuration successful.");
#if DEMO_MODE == 1
    Serial.println("Mode: Single Point (Line: 4, Point: 32)");
#elif DEMO_MODE == 2
    Serial.print("Mode: Single Line (Line: ");
    Serial.print(LINE_NUM);
    Serial.print(", Start: ");
    Serial.print(START_POINT);
    Serial.print(", End: ");
    Serial.print(END_POINT);
    Serial.println(")");
#endif
  }
}

void loop()
{
  int parsed = wy6005.triggerGetRaw(wy6005.point.xBuf, wy6005.point.yBuf, wy6005.point.zBuf, wy6005.point.iBuf, TOTAL_POINTS, 300);
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

  delay(1000);
}
