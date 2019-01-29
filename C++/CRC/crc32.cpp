#include "crc32.hpp"

namespace crc32 {
  Table::Table(const std::uint32_t poly) {

    std::uint32_t crc;

    for (auto i = 0; i < 256; i++) {
      crc = std::uint32_t(i);

      for (auto j = 0; j < 8; j++) {
        if (crc & 1) {
          crc = (crc >> 1) ^ poly;
        } else {
          crc >>= 1;
        }
      }
      data[i] = crc;
    }
  }

  std::uint32_t CheckSum(const std::uint8_t *data, const std::size_t size, const Table &table) {
    std::uint32_t crc32 = 0xffffffff;
    std::uint8_t index;

    for (auto i = 0; i < size; i++) {
      index = static_cast<std::uint8_t>((crc32 ^ uint32_t(data[i])) & 0xff);
      crc32 = (crc32 >> 8) ^ table.get(index);
    }

    crc32 ^= 0xffffffff;
    return crc32;
  }
}