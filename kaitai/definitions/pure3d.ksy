meta:
  id: pure3d
  file-extension: p3d
  endian: le
doc: |
  This is crap. do not use it.
seq:
  - id: header
    type: header
instances:
  chunk:
    type: chunk
    pos: 12
    repeat: eos
types:
  header:
    seq:
      - id: header
        size: 4
      - id: version
        type: u4
      - id: file_size
        type: u4
  chunk:
    seq:
      - id: chunk_header
        type: chunk_header
      - id: chunk_body
        size: chunk_header.data_size - 12
  chunk_header:
    seq:
      - id: chnk_id
        type: u4
      - id: data_size
        type: u4
      - id: size
        type: u4