#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <iomanip>

enum vty { FLT, DBL, BOL, CHR, HLF, WHL, LNG, PTR, INVALID };

typedef struct {
  uint64_t readSize;
  uint64_t readVal;
  uint64_t writeSize;
  uint64_t writeVal;
  vty t;
} address;
union value_store {
  uint32_t ptr;
  uint8_t i8;
  uint16_t i16;
  uint32_t i32;
  float f32;
  double d64;
  uint64_t i64;
  uint8_t m_bytes[sizeof(double)];

public:
  value_store() { this->i64 = 0; }
  value_store(void *ptr, vty typ) {
    this->i64 = 0;
    switch (typ) {
    case FLT:
      this->f32 = *((float *)ptr);
      break;
    case DBL:
      this->d64 = *((double *)ptr);
      break;
    case BOL:
      this->i8 = *((uint8_t *)ptr);
      break;
    case CHR:
      this->i8 = *((uint8_t *)ptr);
      break;
    case HLF:
      this->i16 = *((uint16_t *)ptr);
      break;
    case WHL:
      this->i32 = *((uint32_t *)ptr);
      break;
    case LNG:
      this->i64 = *((uint64_t *)ptr);
    case PTR:
      this->ptr = *((uint64_t *)ptr);
      break;
    default:
      this->i32 = 0;
    }
  }

  uint64_t vtytosize(vty typ) {
    switch (typ) {
    case FLT:
      return 4;
      break;
    case DBL:
      return 8;
      break;
    case BOL:
      return 1;
      break;
    case CHR:
      return 1;
      break;
    case HLF:
      return 2;
      break;
    case WHL:
      return 4;
      break;
    case LNG:
      return 8;
      break;
    case PTR:
      return sizeof(void *);
      break;
    default:
      return sizeof(void *);
    }
  }
  void dump(std::ostream &out, vty typ) {
    int size = vtytosize(typ);
    std::reverse_copy(m_bytes, m_bytes + size,
                      std::ostream_iterator<int>(out << std::hex, ""));
  }
};

struct entry {
  std::array<uint8_t, 4096> m_bytes;
  std::bitset<4096> flag;
  uint64_t address;
  entry() {
    std::fill(m_bytes.begin(), m_bytes.end(), 0);
    flag.reset();
  }

  void dump(std::ostream &outstream) {
    outstream << "@" << std::hex << /*"0x" <<*/ (address << 12) / 4 << std::endl;
//    outstream << "Old output : " << std::endl;

//        std::for_each(
//            m_bytes.cbegin(), m_bytes.cend(), [&outstream](const uint8_t c) {
//              outstream << std::hex << static_cast<int>(c) << "\n";
//            });

//    outstream << "New output : " << std::endl;
    for (uint32_t i = 0; i < m_bytes.size(); i+=4) {
        int32_t number = 0;
        char *number_ptr = (char*) &number;

        *(number_ptr + 0) = m_bytes[i + 0];
        *(number_ptr + 1) = m_bytes[i + 1];
        *(number_ptr + 2) = m_bytes[i + 2];
        *(number_ptr + 3) = m_bytes[i + 3];
        outstream << std::hex << std::setw(8)
                  << std::setfill('0')
                  << /*"0x" <<*/ number << "\n";
    }

    /*   int i = 0;
       for (auto &val : m_bytes) {
         outstream << std::hex << "0x" << static_cast<int>(val) << " " << (i++)
                   << "\n";
       }*/
  }

  void dumpwrites(std::ostream &outstream) {
    bool run = false;
    for (size_t i = 0; i < 4096; i++) {
      if (flag[i]) {
        if (!run) {
          outstream << std::hex << /*"0x" <<*/ (address << 12) + i << std::endl;
          run = true;
        }
        outstream << std::hex << /*"0x" <<*/ static_cast<int>(m_bytes[i]) << "\n";
      } else {
        run = false;
      }
    }
  }
};

#endif
