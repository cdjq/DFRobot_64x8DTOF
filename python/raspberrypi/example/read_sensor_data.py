# -*- coding: utf-8 -*-
'''
@file read_sensor_data.py
@brief Read sensor data from DFRobot_WY6005.
@copyright   Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT license (MIT)
@author [PLELES] (https://github.com/PLELES)
@version  V1.0
@date  2026-1-21
@https://github.com/DFRobot/DFRobot_WY6005
'''

import sys
import time

sys.path.append("../")
from DFRobot_WY6005 import DFRobot_WY6005

# Initialize sensor with UART port
# Please change '/dev/ttyUSB0' to your actual serial port
wy6005 = DFRobot_WY6005(port="/dev/ttyUSB0", baudrate=921600)

def setup():
  print("WY6005 init...")

  # Retry configuration until success
  while not wy6005.config_single_frame_mode():
    print("failed, Connection error or device busy!")
    time.sleep(1)
  
  print("successed")

  # Configure to full output mode (get all 64*8 points)
  if wy6005.config_full_output_mode():
    print("Config Full Output Mode: Success")
  else:
    print("Config Full Output Mode: Failed")

def loop():
  # Trigger acquisition of one frame
  # returns lists: x, y, z, intensity
  x, y, z, i = wy6005.trigger_get_raw(timeout_ms=1000)
  
  if len(x) > 0:
    print(f"Received {len(x)} points")
    # Print the middle point data as example
    idx = len(x) // 2
    print(f"Sample Point[{idx}]: X:{x[idx]} Y:{y[idx]} Z:{z[idx]} I:{i[idx]}")
  else:
    print("No data received or timeout")
  
  time.sleep(1)

if __name__ == "__main__":
  setup()
  try:
    while True:
      loop()
  except KeyboardInterrupt:
    wy6005.close()
    print("Program stopped")
