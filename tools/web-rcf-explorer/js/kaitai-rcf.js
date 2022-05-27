(function(root, factory) {
    if (typeof define === 'function' && define.amd) {
        define(['kaitai-struct/KaitaiStream'], factory);
    } else if (typeof module === 'object' && module.exports) {
        module.exports = factory(require('kaitai-struct/KaitaiStream'));
    } else {
        root.Cement = factory(root.KaitaiStream);
    }
}(this, function(KaitaiStream) {
    /**
     * This is crap. do not use it.
     */

    var Cement = (function() {
        function Cement(_io, _parent, _root) {
            this._io = _io;
            this._parent = _parent;
            this._root = _root || this;

            this._read();
        }
        Cement.prototype._read = function() {
            this.header = new Header(this._io, this, this._root);
        }

        var Header = Cement.Header = (function() {
            function Header(_io, _parent, _root) {
                this._io = _io;
                this._parent = _parent;
                this._root = _root || this;

                this._read();
            }
            Header.prototype._read = function() {
                this.fileId = KaitaiStream.bytesToStr(this._io.readBytes(32), "ASCII");
                this.unk1 = this._io.readU4le();
                this.dirOffset = this._io.readU4le();
                this.dirSize = this._io.readU4le();
                this.flnamesDirOffset = this._io.readU4le();
                this.flnamesDirSize = this._io.readU4le();
                this.unk2 = this._io.readU4le();
                this.numberFiles = this._io.readU4le();
            }

            return Header;
        })();

        var Directory = Cement.Directory = (function() {
            function Directory(_io, _parent, _root) {
                this._io = _io;
                this._parent = _parent;
                this._root = _root || this;

                this._read();
            }
            Directory.prototype._read = function() {
                this.hash = this._io.readU4le();
                this.flOffset = this._io.readU4le();
                this.flSize = this._io.readU4le();
            }
            Object.defineProperty(Directory.prototype, 'file', {
                get: function() {
                    if (this._m_file !== undefined)
                        return this._m_file;
                    var _pos = this._io.pos;
                    this._io.seek(this.flOffset);
                    this._m_file = this._io.readBytes(this.flSize);
                    this._io.seek(_pos);
                    return this._m_file;
                }
            });

            return Directory;
        })();

        var FilenameDirectory = Cement.FilenameDirectory = (function() {
            function FilenameDirectory(_io, _parent, _root) {
                this._io = _io;
                this._parent = _parent;
                this._root = _root || this;

                this._read();
            }
            FilenameDirectory.prototype._read = function() {
                this.unk1 = this._io.readU4le();
                this.unk2 = this._io.readU4le();
                this.unk3 = this._io.readU4le();
                this.pathLen = this._io.readU4le();
                this.path = KaitaiStream.bytesToStr(this._io.readBytes((this.pathLen - 1)), "ASCII");
                this.padding = this._io.readU4le();
            }

            return FilenameDirectory;
        })();
        Object.defineProperty(Cement.prototype, 'directory', {
            get: function() {
                if (this._m_directory !== undefined)
                    return this._m_directory;
                var _pos = this._io.pos;
                this._io.seek(this.header.dirOffset);
                this._m_directory = new Array(this.header.numberFiles);
                for (var i = 0; i < this.header.numberFiles; i++) {
                    this._m_directory[i] = new Directory(this._io, this, this._root);
                }
                this._io.seek(_pos);
                return this._m_directory;
            }
        });
        Object.defineProperty(Cement.prototype, 'filenameDirectory', {
            get: function() {
                if (this._m_filenameDirectory !== undefined)
                    return this._m_filenameDirectory;
                var _pos = this._io.pos;
                this._io.seek((this.header.flnamesDirOffset + 8));
                this._m_filenameDirectory = new Array(this.header.numberFiles);
                for (var i = 0; i < this.header.numberFiles; i++) {
                    this._m_filenameDirectory[i] = new FilenameDirectory(this._io, this, this._root);
                }
                this._io.seek(_pos);
                return this._m_filenameDirectory;
            }
        });

        return Cement;
    })();
    return Cement;
}));