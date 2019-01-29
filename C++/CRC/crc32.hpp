#pragma once

#include <cstdint>

namespace crc32 {

  const std::uint32_t IEEE = 0xedb88320;

  const std::uint32_t Castagnoli = 0x82f63b78;

  class Table {
  public:
    explicit Table(const std::uint32_t poly);
    inline std::uint32_t get(std::uint8_t index) const { return data[index]; }
  private:
    std::uint32_t data[256];
  };

  std::uint32_t CheckSum(const std::uint8_t *data, const std::size_t size, const Table& table);

  namespace _impl {

  }
}