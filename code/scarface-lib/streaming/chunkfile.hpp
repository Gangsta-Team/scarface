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

#endif // !_CHUNKFILE_HPP