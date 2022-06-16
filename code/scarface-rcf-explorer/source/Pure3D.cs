// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using System.Collections.Generic;

namespace Kaitai
{

    /// <summary>
    /// This is crap. do not use it.
    /// </summary>
    public partial class Pure3d : KaitaiStruct
    {
        public static Pure3d FromFile(string fileName)
        {
            return new Pure3d(new KaitaiStream(fileName));
        }

        public Pure3d(KaitaiStream p__io, KaitaiStruct p__parent = null, Pure3d p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            f_chunk = false;
            _read();
        }
        private void _read()
        {
            _header = new Header(m_io, this, m_root);
        }
        public partial class Header : KaitaiStruct
        {
            public static Header FromFile(string fileName)
            {
                return new Header(new KaitaiStream(fileName));
            }

            public Header(KaitaiStream p__io, Pure3d p__parent = null, Pure3d p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _header = m_io.ReadBytes(4);
                _version = m_io.ReadU4le();
                _fileSize = m_io.ReadU4le();
            }
            private byte[] _header;
            private uint _version;
            private uint _fileSize;
            private Pure3d m_root;
            private Pure3d m_parent;
            public byte[] Header { get { return _header; } }
            public uint Version { get { return _version; } }
            public uint FileSize { get { return _fileSize; } }
            public Pure3d M_Root { get { return m_root; } }
            public Pure3d M_Parent { get { return m_parent; } }
        }
        public partial class Chunk : KaitaiStruct
        {
            public static Chunk FromFile(string fileName)
            {
                return new Chunk(new KaitaiStream(fileName));
            }

            public Chunk(KaitaiStream p__io, Pure3d p__parent = null, Pure3d p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _chunkHeader = new ChunkHeader(m_io, this, m_root);
                _chunkBody = m_io.ReadBytes((ChunkHeader.DataSize - 12));
            }
            private ChunkHeader _chunkHeader;
            private byte[] _chunkBody;
            private Pure3d m_root;
            private Pure3d m_parent;
            public ChunkHeader ChunkHeader { get { return _chunkHeader; } }
            public byte[] ChunkBody { get { return _chunkBody; } }
            public Pure3d M_Root { get { return m_root; } }
            public Pure3d M_Parent { get { return m_parent; } }
        }
        public partial class ChunkHeader : KaitaiStruct
        {
            public static ChunkHeader FromFile(string fileName)
            {
                return new ChunkHeader(new KaitaiStream(fileName));
            }

            public ChunkHeader(KaitaiStream p__io, Pure3d.Chunk p__parent = null, Pure3d p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _chnkId = m_io.ReadU4le();
                _dataSize = m_io.ReadU4le();
                _size = m_io.ReadU4le();
            }
            private uint _chnkId;
            private uint _dataSize;
            private uint _size;
            private Pure3d m_root;
            private Pure3d.Chunk m_parent;
            public uint ChnkId { get { return _chnkId; } }
            public uint DataSize { get { return _dataSize; } }
            public uint Size { get { return _size; } }
            public Pure3d M_Root { get { return m_root; } }
            public Pure3d.Chunk M_Parent { get { return m_parent; } }
        }
        private bool f_chunk;
        private List<Chunk> _chunk;
        public List<Chunk> Chunk
        {
            get
            {
                if (f_chunk)
                    return _chunk;
                long _pos = m_io.Pos;
                m_io.Seek(12);
                _chunk = new List<Chunk>();
                {
                    var i = 0;
                    while (!m_io.IsEof) {
                        _chunk.Add(new Chunk(m_io, this, m_root));
                        i++;
                    }
                }
                m_io.Seek(_pos);
                f_chunk = true;
                return _chunk;
            }
        }
        private Header _header;
        private Pure3d m_root;
        private KaitaiStruct m_parent;
        public Header Header { get { return _header; } }
        public Pure3d M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
