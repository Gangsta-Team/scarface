// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

using System.Collections.Generic;

namespace Kaitai
{

    /// <summary>
    /// This is crap. do not use it.
    /// </summary>
    public partial class Cement : KaitaiStruct
    {
        public static Cement FromFile(string fileName)
        {
            return new Cement(new KaitaiStream(fileName));
        }

        public Cement(KaitaiStream p__io, KaitaiStruct p__parent = null, Cement p__root = null) : base(p__io)
        {
            m_parent = p__parent;
            m_root = p__root ?? this;
            f_directory = false;
            f_filenameDirectory = false;
            _read();
        }
        private void _read()
        {
            _rcfHeader = new RcfHeader(m_io, this, m_root);
        }
        public partial class RcfHeader : KaitaiStruct
        {
            public static RcfHeader FromFile(string fileName)
            {
                return new RcfHeader(new KaitaiStream(fileName));
            }

            public RcfHeader(KaitaiStream p__io, Cement p__parent = null, Cement p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _fileId = System.Text.Encoding.GetEncoding("ASCII").GetString(m_io.ReadBytes(32));
                _unk1 = m_io.ReadU4le();
                _dirOffset = m_io.ReadU4le();
                _dirSize = m_io.ReadU4le();
                _flnamesDirOffset = m_io.ReadU4le();
                _flnamesDirSize = m_io.ReadU4le();
                _unk2 = m_io.ReadU4le();
                _numberFiles = m_io.ReadU4le();
            }
            private string _fileId;
            private uint _unk1;
            private uint _dirOffset;
            private uint _dirSize;
            private uint _flnamesDirOffset;
            private uint _flnamesDirSize;
            private uint _unk2;
            private uint _numberFiles;
            private Cement m_root;
            private Cement m_parent;
            public string FileId { get { return _fileId; } }
            public uint Unk1 { get { return _unk1; } }
            public uint DirOffset { get { return _dirOffset; } }
            public uint DirSize { get { return _dirSize; } }
            public uint FlnamesDirOffset { get { return _flnamesDirOffset; } }
            public uint FlnamesDirSize { get { return _flnamesDirSize; } }
            public uint Unk2 { get { return _unk2; } }
            public uint NumberFiles { get { return _numberFiles; } }
            public Cement M_Root { get { return m_root; } }
            public Cement M_Parent { get { return m_parent; } }
        }
        public partial class RcfDirectory : KaitaiStruct
        {
            public static RcfDirectory FromFile(string fileName)
            {
                return new RcfDirectory(new KaitaiStream(fileName));
            }

            public RcfDirectory(KaitaiStream p__io, Cement p__parent = null, Cement p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                f_file = false;
                _read();
            }
            private void _read()
            {
                _hash = m_io.ReadU4le();
                _flOffset = m_io.ReadU4le();
                _flSize = m_io.ReadU4le();
            }
            private bool f_file;
            private byte[] _file;
            public byte[] File
            {
                get
                {
                    if (f_file)
                        return _file;
                    long _pos = m_io.Pos;
                    m_io.Seek(FlOffset);
                    _file = m_io.ReadBytes(FlSize);
                    m_io.Seek(_pos);
                    f_file = true;
                    return _file;
                }
            }
            private uint _hash;
            private uint _flOffset;
            private uint _flSize;
            private Cement m_root;
            private Cement m_parent;
            public uint Hash { get { return _hash; } }
            public uint FlOffset { get { return _flOffset; } }
            public uint FlSize { get { return _flSize; } }
            public Cement M_Root { get { return m_root; } }
            public Cement M_Parent { get { return m_parent; } }
        }
        public partial class FilenameDirectory : KaitaiStruct
        {
            public static FilenameDirectory FromFile(string fileName)
            {
                return new FilenameDirectory(new KaitaiStream(fileName));
            }

            public FilenameDirectory(KaitaiStream p__io, Cement p__parent = null, Cement p__root = null) : base(p__io)
            {
                m_parent = p__parent;
                m_root = p__root;
                _read();
            }
            private void _read()
            {
                _unk1 = m_io.ReadU4le();
                _unk2 = m_io.ReadU4le();
                _unk3 = m_io.ReadU4le();
                _pathLen = m_io.ReadU4le();
                _path = System.Text.Encoding.GetEncoding("ASCII").GetString(m_io.ReadBytes((PathLen - 1)));
                _padding = m_io.ReadU4le();
            }
            private uint _unk1;
            private uint _unk2;
            private uint _unk3;
            private uint _pathLen;
            private string _path;
            private uint _padding;
            private Cement m_root;
            private Cement m_parent;
            public uint Unk1 { get { return _unk1; } }
            public uint Unk2 { get { return _unk2; } }
            public uint Unk3 { get { return _unk3; } }
            public uint PathLen { get { return _pathLen; } }
            public string Path { get { return _path; } }
            public uint Padding { get { return _padding; } }
            public Cement M_Root { get { return m_root; } }
            public Cement M_Parent { get { return m_parent; } }
        }
        private bool f_directory;
        private List<RcfDirectory> _directory;
        public List<RcfDirectory> Directory
        {
            get
            {
                if (f_directory)
                    return _directory;
                long _pos = m_io.Pos;
                m_io.Seek(RcfHeader_.DirOffset);
                _directory = new List<RcfDirectory>((int)(RcfHeader_.NumberFiles));
                for (var i = 0; i < RcfHeader_.NumberFiles; i++)
                {
                    _directory.Add(new RcfDirectory(m_io, this, m_root));
                }
                m_io.Seek(_pos);
                f_directory = true;
                return _directory;
            }
        }
        private bool f_filenameDirectory;
        private List<FilenameDirectory> _filenameDirectory;
        public List<FilenameDirectory> FilenameDirectory_
        {
            get
            {
                if (f_filenameDirectory)
                    return _filenameDirectory;
                long _pos = m_io.Pos;
                m_io.Seek(RcfHeader_.FlnamesDirOffset + 8);
                _filenameDirectory = new List<FilenameDirectory>((int)RcfHeader_.NumberFiles);
                for (var i = 0; i < RcfHeader_.NumberFiles; i++)
                {
                    _filenameDirectory.Add(new FilenameDirectory(m_io, this, m_root));
                }
                m_io.Seek(_pos);
                f_filenameDirectory = true;
                return _filenameDirectory;
            }
        }
        private RcfHeader _rcfHeader;
        private Cement m_root;
        private KaitaiStruct m_parent;
        public RcfHeader RcfHeader_ { get { return _rcfHeader; } }
        public Cement M_Root { get { return m_root; } }
        public KaitaiStruct M_Parent { get { return m_parent; } }
    }
}
