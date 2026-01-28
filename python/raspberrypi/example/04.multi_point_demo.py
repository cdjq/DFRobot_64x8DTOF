# -*- coding: utf-8 -*-
'''
@file multi_point_demo.py
@brief Multi point mode demo - Read multiple points from a line (e.g. points 10-20 on line 4) from DFRobot_64x8DTOF.
@copyright   Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT license (MIT)
@author [PLELES] (https://github.com/PLELES)
@version  V1.0
@date  2026-1-26
'''

import sys
import time
import serial

sys.path.append("../")
from DFRobot_64x8DTOF import DFRobot_64x8DTOF

# Configuration macros
LINES = 4
START_POINT = 10
END_POINT = 20

# Initialize sensor with UART port
# If you are using a USB-to-serial converter, use '/dev/ttyUSB0'
# If you are using Raspberry Pi GPIO (TX/RX), use '/dev/serial0' (Recommended for GPIO)
# Note: Ensure Serial Hardware is enabled in 'sudo raspi-config' -> Interface Options
try:
  dtof64x8 = DFRobot_64x8DTOF(port="/dev/serial0", baudrate=921600)
except serial.SerialException as e:
  print(f"Error: Could not open serial port: {e}")
  print("Please allow permission to read/write the serial port or change the port name.")
  sys.exit(1)


def setup():
  print("DFRobot_64x8DTOF init...")

  if not dtof64x8.begin():
    print("Beginning sensor failed!")
    sys.exit(1)

  # Configure to single frame mode (retry until success)
  print("Configuring frame mode: Single Frame...")
  while not dtof64x8.config_frame_mode(dtof64x8.FRAME_MODE_SINGLE):
    print("Config Single Frame Mode failed, retrying...")
    time.sleep(0.2)
  print("Config Single Frame Mode: Success")

  # Configure to multi point mode using macros (retry until success)
  # Arguments: Line number, Start point, End point
  print(f"Configuring Multi Point Mode (Line {LINES}, Points {START_POINT}-{END_POINT})...")
  while not dtof64x8.config_measure_mode(LINES, START_POINT, END_POINT):
    print("Config Multi Point Mode failed, retrying...")
    time.sleep(0.2)
  print(f"Config Multi Point Mode (Line {LINES}, Points {START_POINT}-{END_POINT}): Success")

  time.sleep(0.5)


def loop():
  # Trigger acquisition of one frame
  # returns lists: x, y, z, intensity
  list_x, list_y, list_z, list_i = dtof64x8.get_data(timeout_ms=500)

  if len(list_x) > 0:
    print(f"Received {len(list_x)} points")
    # Output each point with Point[index] prefix and x,y,z values
    for i in range(len(list_x)):
      point_idx = START_POINT + i
      print(f"Point[{point_idx:02d}] X:{list_x[i]:04d} mm Y:{list_y[i]:04d} mm Z:{list_z[i]:04d} mm")
  else:
    print("No data received or timeout")

  time.sleep(2)


if __name__ == "__main__":
  setup()
  try:
    while True:
      loop()
  except KeyboardInterrupt:
    dtof64x8.close()
    print("Program stopped")
