#include <cstring>
#include <iostream>
#include "crc32.hpp"

using namespace std;

int main() {
  crc32::Table table(crc32::Castagnoli);
  const char *data = "Hello,World!";
  cout << crc32::CheckSum((const std::uint8_t *) data, strlen(data), table) << endl;
  data = "hello,World!";
  cout << crc32::CheckSum((const std::uint8_t *) data, strlen(data), table) << endl;
  return 0;
}