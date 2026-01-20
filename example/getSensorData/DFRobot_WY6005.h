/*!
 * @file DFRobot_WY6005.h
 * @brief DFRobot_WY6005 class infrastructure
 * @copyright  Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author DFRobot
 * @version V1.0
 * @date 2025-04-10
 * @url https://github.com/DFRobot/DFRobot_WY6005
 */
#ifndef _DFROBOT_WY6005_H_
#define _DFROBOT_WY6005_H_

#include <Arduino.h>
#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...)                 \
  {                              \
    Serial.print("[");           \
    Serial.print(__FUNCTION__);  \
    Serial.print(":");        \
    Serial.print(__LINE__);      \
    Serial.print(" ] ");         \
    char _buf[128];              \
    sprintf(_buf, __VA_ARGS__);  \
    Serial.println(_buf);        \
  }
#else
#define DBG(...)
#endif

#define WY6005_RESPONSE_TIMEOUT     500   /*!< Response timeout in milliseconds */
#define WY6005_MAX_RETRY_COUNT      5     /*!< Maximum number of retries for commands */
#define WY6005_RESPONSE_OK_SEQ_LEN  4     /*!< Length of OK response sequence */
#define WY6005_SYNC_BYTE_0          0x0A
#define WY6005_SYNC_BYTE_1          0x4F
#define WY6005_SYNC_BYTE_2          0x4B
#define WY6005_SYNC_BYTE_3          0x0A
#define WY6005_FRAME_HEADER_SIZE    4
#define WY6005_POINT_DATA_SIZE      8
#define WY6005_MAX_POINTS           64

class DFRobot_WY6005 {
private:
  HardwareSerial* _serial;    /*!< Hardware serial port */
  uint32_t        _config;    /*!< Serial port configuration */
  int8_t          _rxPin;     /*!< RX pin number */
  int8_t          _txPin;     /*!< TX pin number */
  
  /**
   * @fn sendCommand
   * @brief Send AT command and wait for response
   * @param command AT command to send
   * @return true: Success false: Failure
   */
  bool sendCommand(const String& command);
  bool setStreamControl(bool enable);
  bool setFrameMode(bool continuousMode);
  bool setOutputLineData(uint8_t line, uint8_t startPoint, uint8_t pointCount);

  int _startPoint;
  int _endPoint;
  int _totalPoints;
  
public:
  typedef struct {
    int16_t xBuf[WY6005_MAX_POINTS];
    int16_t yBuf[WY6005_MAX_POINTS];
    int16_t zBuf[WY6005_MAX_POINTS];
    int16_t iBuf[WY6005_MAX_POINTS];
  } sPoint_t;
  /**
   * @fn DFRobot_WY6005
   * @brief Constructor, passing in serial port and configuration
   * @param serial Hardware serial port pointer
   * @param config Serial port configuration (e.g., SERIAL_8N1)
   * @param rxPin RX pin number
   * @param txPin TX pin number
   */
  DFRobot_WY6005(HardwareSerial &serial, uint32_t config, int8_t rxPin, int8_t txPin);
  
  /**
   * @fn begin
   * @brief Initialize the sensor
   * @param baudRate Serial communication baud rate
   */
  void begin(uint32_t baudRate);

  /** parsing helpers removed - use triggerGetRaw for raw reads */
  void parsePointData(const uint8_t* pointData, int16_t* x, int16_t* y, int16_t* z, int16_t* i);
  /**
   * @fn triggerGetRaw
   * @brief Trigger one frame and read raw x/y/z values (no filtering).
   * @param xBuf buffer for x values (may be NULL if not needed)
   * @param yBuf buffer for y values (may be NULL if not needed)
   * @param zBuf buffer for z values (may be NULL if not needed)
   * @param iBuf buffer for i values (may be NULL if not needed)
   * @param maxPoints maximum points to parse (caller buffer length)
   * @param timeoutMs timeout in milliseconds to wait for a complete frame
   * @return number of points parsed, or -1 on error/timeout
   */
  int triggerGetRaw(int16_t* xBuf, int16_t* yBuf, int16_t* zBuf, int16_t* iBuf, int maxPoints, uint32_t timeoutMs);

  /* getPointBuffer removed */
  
  /**
   * @fn setStreamControl
   * @brief Set stream control
   * @param enable true: Enable stream control false: Disable stream control
   * @return true: Success false: Failure
   */
  
  
  /**
   * @fn triggerOneFrame
   * @brief Trigger one frame data output
   * @return true: Success false: Failure
   */
  bool triggerOneFrame(void);
  
  /**
   * @fn saveConfig
   * @brief Save configuration to sensor
   * @return true: Success false: Failure
   */
  bool saveConfig(void);
  
  /**
   * @fn configSinglePointMode
   * @brief Configure single point mode
   * @param line Line number
   * @param point Point number
   * @return true: Success false: Failure
   */
  bool configSinglePointMode(uint8_t line, uint8_t point);
  
  /**
   * @fn configSingleLineMode
   * @brief Configure single line mode
   * @param line Line number
   * @param startPoint Start point number
   * @param endPoint End point number
   * @return true: Success false: Failure
   */
  bool configSingleLineMode(uint8_t line, uint8_t startPoint, uint8_t endPoint);
  
  /**
   * @fn configSingleFrameMode
   * @brief Configure single frame mode
   * @return true: Success false: Failure
   */
  bool configSingleFrameMode(void);
  
  /**
   * @fn configContinuousMode
   * @brief Configure continuous mode
   * @return true: Success false: Failure
   */
  bool configContinuousMode(void);

  sPoint_t point;
};
//连续输出模式 连续输出模式
#endif