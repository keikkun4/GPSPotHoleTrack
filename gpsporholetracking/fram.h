#ifndef _FRAM_H_
#define _FRAM_H_

#include <stdint.h>

class FRAM {
  public:
    FRAM(uint8_t addr);
    void begin();
    void read(uint16_t addr, void *pData, uint16_t len);
    void write(uint16_t addr, const void *pData, uint16_t len);
  private:
    const uint8_t i2c_addr;
};

#endif //_FRAM_H_

