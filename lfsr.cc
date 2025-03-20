#include <cstdint>

#include "lfsr.h"

LFSR::LFSR(uint32_t seed) { this->_state = (uint64_t)seed + 0xACE10000; }

uint8_t LFSR::next_byte() {
  // implement next byte
}

uint32_t LFSR::next_int() {
  // implement next int
}
