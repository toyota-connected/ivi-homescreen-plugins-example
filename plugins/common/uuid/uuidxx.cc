#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include "uuidxx.h"
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <random>

using namespace std;

namespace plugin_common::uuidxx {

bool uuid::operator==(const uuid& guid2) const {
  return memcmp(&guid2, this, sizeof(uuid)) == 0;
}

bool uuid::operator!=(const uuid& guid2) const {
  return !(*this == guid2);
}

bool uuid::operator<(const uuid& guid2) const {
  return memcmp(this, &guid2, sizeof(uuid)) < 0;
}

bool uuid::operator>(const uuid& guid2) const {
  return memcmp(this, &guid2, sizeof(uuid)) > 0;
}

uuid::uuid(const std::string& uuidString) : uuid(uuidString.c_str()) {}

uuid::uuid(const char* uuidString) {
  if (uuidString == nullptr) {
    // special case, and prevents random bugs
    memset(this, 0, sizeof(uuid));
    return;
  }

  const char* pos = uuidString;
  if (uuidString[0] == '{') {
    pos++;
  }

  Uuid.Data1 = static_cast<uint32_t>(strtoul(pos, nullptr, 16));
  pos = strchr(pos, '-') + 1;
  Uuid.Data2 = static_cast<uint16_t>(strtoul(pos, nullptr, 16));
  pos = strchr(pos, '-') + 1;
  Uuid.Data3 = static_cast<uint16_t>(strtoul(pos, nullptr, 16));
  pos = strchr(pos, '-') + 1;
  Uuid.Data4[0] = static_cast<uint8_t>(strtoul(pos, nullptr, 16));
  pos += 2;
  Uuid.Data4[1] = static_cast<uint8_t>(strtoul(pos, nullptr, 16));
  pos = strchr(pos, '-') + 1;
  for (int i = 2; i < 8; ++i) {
    Uuid.Data4[i] = static_cast<uint8_t>(strtoul(pos, nullptr, 16));
    pos += 2;
  }
}

string uuid::ToString(const bool withBraces) const {
  char buffer[39];
  sprintf(buffer, "%s%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X%s",
          withBraces ? "{" : "", Uuid.Data1, Uuid.Data2, Uuid.Data3,
          Uuid.Data4[0], Uuid.Data4[1], Uuid.Data4[2], Uuid.Data4[3],
          Uuid.Data4[4], Uuid.Data4[5], Uuid.Data4[6], Uuid.Data4[7],
          withBraces ? "}" : "");
  return buffer;
}

uuid uuid::FromString(const char* uuidString) {
  const uuid temp(uuidString);
  return temp;
}

uuid uuid::FromString(const std::string& uuidString) {
  const uuid temp(uuidString.c_str());
  return temp;
}

uuid uuid::Generatev4() {
  // mach-o does not support TLS and clang still has issues with thread_local
#if !defined(__APPLE__) && !defined(__clang__)
  thread_local std::random_device rd;
  thread_local auto gen = std::mt19937_64(rd());
#else
  std::random_device rd;
  std::mt19937_64 gen(rd());
#endif
  std::uniform_int_distribution<uint64_t> dis64;

  uuid newGuid;
  newGuid.WideIntegers[0] = dis64(gen);
  newGuid.WideIntegers[1] = dis64(gen);

  // RFC4122 defines (psuedo)random uuids (in big-endian notation):
  // MSB of DATA4[0] specifies the variant and should be 0b10 to indicate
  // standard uuid, and MSB of DATA3 should be 0b0100 to indicate version 4
  newGuid.Bytes.Data4[0] =
      (newGuid.Bytes.Data4[0] & 0x3F) | static_cast<uint8_t>(0x80);
  newGuid.Bytes.Data3[1] =
      (newGuid.Bytes.Data3[1] & 0x0F) | static_cast<uint8_t>(0x40);

  return newGuid;
}
}  // namespace plugin_common::uuidxx