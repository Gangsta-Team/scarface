# Scarface The World Is Yours

<p align="center">
  <!--img width="460" height="300" src="https://user-images.githubusercontent.com/44430306/169044742-49f5a400-72fd-405e-8adf-aa647d9f0db8.png"-->
</p>
<a href="https://discord.gg/TA9zKUXQAc">Discord</a>
<!--hr-->

<h1>Project scaffolding:</h1>

<pre>
- ./
├── <a href="#code">code</a>/
├── <a href="#documentation">documentation</a>/
└── <a href="#tools">tools</a>/
</pre>

# Code

<h3>scarface-mod: WIP</h3>
<u>scarface-mod.ini</u>

```
[Scarface]
Log=0
DebugConsole=0
ReleaseMode=0
```

<h3>scarface-rcf-explorer: WIP</h3>

# Documentation
Game reference: https://en.wikipedia.org/wiki/Scarface:_The_World_Is_Yours

File formats using Kaitai Struct: https://kaitai.io/


ATG CORE CEMENT LIBRARY (.rcf)

```
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
        size: slen
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
```
Pure3D (.p3d)
```
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
```
# Tools
- <a href="https://1h3a3x7.github.io/scarface/tools/web-rcf-explorer/ui.html">Web RCF Explorer</a>
