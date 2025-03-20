#include <cstdint>

#include "lfsr.h"

LFSR::LFSR(uint32_t seed) { this->_state = (uint64_t)seed + 0xACE10000; }

// fibonacci lfsr
uint8_t LFSR::_next_bit() {
  uint8_t bit = 0;

  uint8_t b0 = (this->_state & 0x1) >> 0;
  uint8_t b1 = (this->_state & 0x2) >> 1;
  uint8_t b2 = (this->_state & 0x4) >> 2;
  uint8_t b3 = (this->_state & 0x20) >> 3;
  bit = b0 ^ b1 ^ b2 ^ b3;
  this->_state = this->_state << 1;
  this->_state |= bit;

  return bit;
}

uint8_t LFSR::next_byte() {
  // implement next byte
  uint8_t bits = 0;
  bits |= this->_next_bit();
  bits |= 1 << (this->_next_bit());
  bits |= 2 << (this->_next_bit());
  bits |= 3 << (this->_next_bit());
  bits |= 4 << (this->_next_bit());
  bits |= 5 << (this->_next_bit());
  bits |= 6 << (this->_next_bit());
  bits |= 7 << (this->_next_bit());

  return bits;
}

uint32_t LFSR::next_int() {
  const uint8_t a = this->next_byte();  
  const uint8_t b = this->next_byte();  
  const uint8_t c = this->next_byte();  
  const uint8_t d = this->next_byte();  

  uint32_t x = 0;
  x |= a;
  x |= (b << 8);
  x |= (c << 16);
  x |= (d << 24);

  return x;
}
