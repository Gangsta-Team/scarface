#ifndef _CHUNKFILE_HPP
#define _CHUNKFILE_HPP

#include "../common.hpp"

struct Chunk
{
  uint32_t m_id;
  uint32_t m_data_length;
  uint32_t m_chunk_length;
  uint32_t m_start_position;
};

struct tChunkFile
{
  Chunk m_chunk_stack[40];
  uint32_t m_stack_top;
  class radLoadBaseStream *m_real_file;
};

class radLoadBaseStream
{
public:
  unsigned char m_gap0[15];
  bool m_endianSwap;
  void *m_vtable_ptr;
  uint32_t m_size;
  uint32_t m_position;
};


#endif // !_CHUNKFILE_HPP