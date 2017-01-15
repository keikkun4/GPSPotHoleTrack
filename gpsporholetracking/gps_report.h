#ifndef _GPS_REPORT_H_
#define _GPS_REPORT_H_

#include <stdint.h>
using namespace NeoGPS;

struct __attribute__((__packed__)) DEVICE_INFO {
  int16_t ID;
  int16_t numreg;
  int32_t res1;
  int32_t res2;
  int32_t res3;
  
};


struct __attribute__((__packed__)) GPS_REPORT {
  int32_t lat;
  int32_t lon;
  clock_t timestamp;
  uint16_t hdop;
  uint8_t numbutton;
  uint8_t reserved;
};

#endif //_GPS_REPORT_H_

