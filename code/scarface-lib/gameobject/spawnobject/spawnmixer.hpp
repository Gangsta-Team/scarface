#ifndef _SPAWNMIXER_HPP
#define _SPAWNMIXER_HPP

#include "../../common.hpp"

struct __declspec(align(1)) TrafficConfig
{
  char *m_Name;
  uint32_t m_Key;
  DWORD dword8;
  DWORD dwordC;
  DWORD dword10;
  BYTE gap14[44];
  DWORD dword40;
  BYTE gap44[36];
  uint8_t m_MaxVehicles;
  uint8_t m_MaxCharacters;
  uint8_t m_VehicleMix;
};

static_assert(sizeof(TrafficConfig) == 0x6C);

#endif // !_SPAWNMIXER_HPP