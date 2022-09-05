#ifndef _CHUNKFILE_HPP
#define _CHUNKFILE_HPP

#include "../common.hpp"

struct Chunk
{
  uint32_t id;
  uint32_t dataLength;
  uint32_t chunkLength;
  uint32_t startPosition;
};

struct tChunkFile
{
  Chunk chunkStack[40];
  uint32_t stackTop;
  class radLoadBaseStream *realFile;
};

class radLoadBaseStream
{
public:
  unsigned char gap0[15];
  bool m_endianSwap;
  void *vtable_ptr;
  uint32_t m_size;
  uint32_t m_position;
};


#endif // !_CHUNKFILE_HPP