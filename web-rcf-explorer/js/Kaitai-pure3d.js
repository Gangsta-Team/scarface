(function (root, factory) {
    if (typeof define === 'function' && define.amd) {
      define(['kaitai-struct/KaitaiStream'], factory);
    } else if (typeof module === 'object' && module.exports) {
      module.exports = factory(require('kaitai-struct/KaitaiStream'));
    } else {
      root.Pure3d = factory(root.KaitaiStream);
    }
  }(this, function (KaitaiStream) {
  /**
   * This is crap. do not use it.
   */
  
  var Pure3d = (function() {
    function Pure3d(_io, _parent, _root) {
      this._io = _io;
      this._parent = _parent;
      this._root = _root || this;
      this._debug = {};
  
    }
    Pure3d.prototype._read = function() {
      this._debug.header = { start: this._io.pos, ioOffset: this._io.byteOffset };
      this.header = new Header(this._io, this, this._root);
      this.header._read();
      this._debug.header.end = this._io.pos;
    }
  
    var Header = Pure3d.Header = (function() {
      function Header(_io, _parent, _root) {
        this._io = _io;
        this._parent = _parent;
        this._root = _root || this;
        this._debug = {};
  
      }
      Header.prototype._read = function() {
        this._debug.header = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.header = this._io.readBytes(4);
        this._debug.header.end = this._io.pos;
        this._debug.version = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.version = this._io.readU4le();
        this._debug.version.end = this._io.pos;
        this._debug.fileSize = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.fileSize = this._io.readU4le();
        this._debug.fileSize.end = this._io.pos;
      }
  
      return Header;
    })();
  
    var Chunk = Pure3d.Chunk = (function() {
      function Chunk(_io, _parent, _root) {
        this._io = _io;
        this._parent = _parent;
        this._root = _root || this;
        this._debug = {};
  
      }
      Chunk.prototype._read = function() {
        this._debug.chunkHeader = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.chunkHeader = new ChunkHeader(this._io, this, this._root);
        this.chunkHeader._read();
        this._debug.chunkHeader.end = this._io.pos;
        this._debug.chunkBody = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.chunkBody = this._io.readBytes((this.chunkHeader.dataSize - 12));
        this._debug.chunkBody.end = this._io.pos;
      }
  
      return Chunk;
    })();
  
    var ChunkHeader = Pure3d.ChunkHeader = (function() {
      function ChunkHeader(_io, _parent, _root) {
        this._io = _io;
        this._parent = _parent;
        this._root = _root || this;
        this._debug = {};
  
      }
      ChunkHeader.prototype._read = function() {
        this._debug.chnkId = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.chnkId = this._io.readU4le();
        this._debug.chnkId.end = this._io.pos;
        this._debug.dataSize = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.dataSize = this._io.readU4le();
        this._debug.dataSize.end = this._io.pos;
        this._debug.size = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this.size = this._io.readU4le();
        this._debug.size.end = this._io.pos;
      }
  
      return ChunkHeader;
    })();
    Object.defineProperty(Pure3d.prototype, 'chunk', {
      get: function() {
        if (this._m_chunk !== undefined)
          return this._m_chunk;
        var _pos = this._io.pos;
        this._io.seek(12);
        this._debug._m_chunk = { start: this._io.pos, ioOffset: this._io.byteOffset };
        this._m_chunk = [];
        this._debug._m_chunk.arr = [];
        var i = 0;
        while (!this._io.isEof()) {
          this._debug._m_chunk.arr[this._m_chunk.length] = { start: this._io.pos, ioOffset: this._io.byteOffset };
          var _t__m_chunk = new Chunk(this._io, this, this._root);
          _t__m_chunk._read();
          this._m_chunk.push(_t__m_chunk);
          this._debug._m_chunk.arr[this._m_chunk.length - 1].end = this._io.pos;
          i++;
        }
        this._debug._m_chunk.end = this._io.pos;
        this._io.seek(_pos);
        return this._m_chunk;
      }
    });
  
    return Pure3d;
  })();
  return Pure3d;
  }));
  