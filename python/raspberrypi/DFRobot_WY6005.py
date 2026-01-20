# -*- coding: utf-8 -*-
'''
@file DFRobot_FRN20.py
@brief Define the basic structure and methods of the DFRobot_FRN20 class.
@copyright   Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT license (MIT)
@author [fary](feng.yang@dfrobot.com)
@version  V1.0
@date  2025-12-15
@https://github.com/DFRobot/DFRobot_FRN20
'''

from smbus2 import SMBus, i2c_msg
from ctypes import Structure, c_uint16, c_ubyte


class DFRobot_FRN20:
  FRN20_IIC_ADDRESS = 0x40
  FRN20_COMMAND_READ_FLOW = 0x1000
  FRN20_COMMAND_READ_PARAMS = 0xCCDD
  FRN20_PARAM_FRAME_LEN = 41
  FRN20_FLOW_FRAME_LEN = 5
  FRN20_CRC_INITIAL_VALUE = 0x00
  FRN20_CRC_POLYNOMIAL = 0x131

  # Using ctypes to define Params structure
  class DFRobot_Params(Structure):
    _fields_ = [
      ('unit', c_uint16),  # Unit (0x15 mL/min, 0x16 L/min, etc.)
      ('range', c_uint16),  # Range
      ('offset', c_uint16),  # Offset
      ('medium_coeff', c_uint16),  # Conversion coefficient
      ('vout_min_mv', c_uint16),  # Output voltage lower limit (mV)
      ('vout_max_mv', c_uint16),  # Output voltage upper limit (mV)
      ('product_id', c_ubyte * 5),  # Product ID (5-byte ASCII, including terminator)
      ('crc', c_ubyte),  # CRC check
    ]

  def __init__(self, bus=1, addr=0x40):
    '''
    @fn __init__
    @brief Initialize the DFRobot_FRN20 class
    @param bus: I2C bus number
    @param addr: I2C address
    @return None
    '''
    self.bus = SMBus(bus)
    self.addr = addr
    self.params = self.DFRobot_Params()  # Initialize Params structure instance
    self.raw_flow_data = 0  # Raw flow data
    self.mass_flow_data = 0.0  # Mass flow data

  def _write_command(self, command):
    '''
    @fn _write_command
    @brief Write command to the sensor
    @param command: Sensor control command
    @return None
    '''
    write = i2c_msg.write(self.addr, [(command >> 8) & 0xFF, command & 0xFF])
    self.bus.i2c_rdwr(write)

  def _read_bytes(self, length):
    '''
    @fn _read_bytes
    @brief Read data from the sensor
    @param length: Length of data to be read
    @return Data read from the sensor
    '''
    read = i2c_msg.read(self.addr, length)
    self.bus.i2c_rdwr(read)
    return list(read)

  def _calc_crc(self, data):
    '''
    @fn _calc_crc
    @brief CRC check
    @param data: Data to be verified
    @return CRC check result
    '''
    crc = self.FRN20_CRC_INITIAL_VALUE
    for byte in data:
      crc ^= byte
      for _ in range(8, 0, -1):  # Use underscore for unused variable
        if crc & 0x80:
          crc = (crc << 1) ^ self.FRN20_CRC_POLYNOMIAL
        else:
          crc = crc << 1
        crc &= 0xFF  # Ensure CRC is always 8 bits
    return crc

  def begin(self):
    try:
      self._write_command(self.FRN20_IIC_ADDRESS)
      return 1
    except OSError as err:  # Use more descriptive variable name
      print("Init error: {}".format(err))
      return -1

  def read_params(self):
    '''
    @fn read_params
    @brief Read parameters from the sensor
    @return int
    @retval 1: Success
    @retval 0: Failed
    '''
    self._write_command(self.FRN20_COMMAND_READ_PARAMS)
    data = self._read_bytes(self.FRN20_PARAM_FRAME_LEN)
    if self._calc_crc(data[:-1]) == data[-1]:
      # Assign values using ctypes structure
      self.params.unit = (data[4] << 8) | data[5]
      self.params.range = (data[6] << 8) | data[7]
      self.params.offset = (data[8] << 8) | data[9]
      self.params.medium_coeff = (data[10] << 8) | data[11]
      self.params.vout_min_mv = (data[16] << 24) | (data[17] << 16) | (data[18] << 8) | data[19]
      self.params.vout_max_mv = (data[20] << 24) | (data[21] << 16) | (data[22] << 8) | data[23]

      # Process product_id (5 bytes)
      for i in range(4):
        self.params.product_id[i] = data[29 + i]
      self.params.product_id[4] = 0  # Add terminator

      self.params.crc = data[40]
      return 1
    return 0

  def read_raw_flow_data(self):
    '''
    @fn read_raw_flow_data
    @brief Read raw flow data from the sensor
    @return int
    @retval 1: Success
    @retval 0: Failed
    '''
    self._write_command(self.FRN20_COMMAND_READ_FLOW)
    data = self._read_bytes(self.FRN20_FLOW_FRAME_LEN)
    if self._calc_crc(data[:-1]) == data[-1]:
      self.raw_flow_data = (data[0] << 8) | data[1]
      return 1
    return 0

  def read_mass_flow_data(self):
    '''
    @fn read_mass_flow_data
    @brief Read mass flow data from the sensor
    @return int
    @retval 1: Success
    @retval 0: Failed
    '''
    if self.read_raw_flow_data():
      delta = self.raw_flow_data - self.params.offset
      self.mass_flow_data = float(delta) / float(self.params.medium_coeff)
      return 1
    return 0
