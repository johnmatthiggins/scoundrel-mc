#include <cstdint>

#ifndef LFSR_H
#define LFSR_H

class LFSR {
  uint64_t _state;

  int _next_bit();

public:
  LFSR(uint32_t seed);
  ~LFSR(){};

  uint8_t next_byte();
  uint32_t next_int();
};

#endif
