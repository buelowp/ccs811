#ifndef __CCS811_H__
#define __CCS811_H__

#include <onion-i2c.h>
#include <cstdint>
#include <cstdio>
#include <unistd.h>
#include <time.h>

#include "LocalGpio.h"

#define nWAKE           4      // pin 4 is tied to CCS811 nWAKE pin in sensor node
#define nINT            7      // pin 7 is tied to CCS811 nINT pin in sensor node

//#define ADDR            0x5A   // when I2C_ADDR pin is LOW
#define ADDR            0x5B   // when I2C_ADDR pin is HIGH

// Registers for CCS811
#define STATUS          0x00
#define MEAS_MODE       0x01
#define ALG_RESULT_DATA 0x02
#define ENV_DATA        0x05
#define APP_START       0xF4
#define HW_ID           0x20
#define ERROR_ID        0xE0
#define SW_RESET        0xFF

class CCS811
{
  public:
    CCS811(int, uint8_t);     // constructor
    bool begin();
    uint8_t readStatus(void);
    uint8_t readHW_ID(void);
    uint8_t readErrorID(uint8_t _status);
    int readTVOC(void);
    int readCO2(void);
    void getData(void);
    void compensate(float t, float rh);
    void _digitalWrite(int);
    void reset(void);
    void sleep();
    int TVOC, CO2;

  private:
    void localSleep(int, int);

    uint8_t m_addr;
    LocalGpio *m_gpio;
};

#endif
