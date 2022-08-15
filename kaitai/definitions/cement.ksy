meta:
  id: cement
  file-extension: rcf
  endian: le
doc: |
  This is crap. do not use it.
seq:
  - id: header
    type: header
instances:
  directory:
    type: directory
    pos: header.dir_offset
    repeat: expr
    repeat-expr: header.number_files
  filename_directory:
    type: filename_directory
    pos: header.flnames_dir_offset + 8
    repeat: expr
    repeat-expr: header.number_files
types:
  header:
    seq:
      - id: file_id
        size: 32
        type: str
        encoding: ASCII
      - id: unk1
        type: u4
      - id: dir_offset
        type: u4
      - id: dir_size
        type: u4
      - id: flnames_dir_offset
        type: u4
      - id: flnames_dir_size
        type: u4
      - id: unk2
        type: u4
      - id: number_files
        type: u4
  directory:
    seq:
      - id: hash
        type: u4
      - id: fl_offset
        type: u4
      - id: fl_size
        type: u4
    instances:
      file:
        pos: fl_offset
        size: fl_size
  filename_directory:
    seq:
      - id: unk1
        type: u4
      - id: unk2
        type: u4
      - id: unk3
        type: u4
      - id: path_len
        type: u4
      - id: path
        type: str
        encoding: ASCII
        size: path_len - 1
      - id: padding
        type: u4