// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "cement.h"

cement_t::cement_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, cement_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_header = 0;
    m_directory = 0;
    m_filename_directory = 0;
    f_directory = false;
    f_filename_directory = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void cement_t::_read() {
    m_header = new header_t(m__io, this, m__root);
}

cement_t::~cement_t() {
    _clean_up();
}

void cement_t::_clean_up() {
    if (m_header) {
        delete m_header; m_header = 0;
    }
    if (f_directory) {
        if (m_directory) {
            for (std::vector<directory_t*>::iterator it = m_directory->begin(); it != m_directory->end(); ++it) {
                delete *it;
            }
            delete m_directory; m_directory = 0;
        }
    }
    if (f_filename_directory) {
        if (m_filename_directory) {
            for (std::vector<filename_directory_t*>::iterator it = m_filename_directory->begin(); it != m_filename_directory->end(); ++it) {
                delete *it;
            }
            delete m_filename_directory; m_filename_directory = 0;
        }
    }
}

cement_t::header_t::header_t(kaitai::kstream* p__io, cement_t* p__parent, cement_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void cement_t::header_t::_read() {
    m_file_id = kaitai::kstream::bytes_to_str(m__io->read_bytes(32), std::string("ASCII"));
    m_unk1 = m__io->read_u4le();
    m_dir_offset = m__io->read_u4le();
    m_dir_size = m__io->read_u4le();
    m_flnames_dir_offset = m__io->read_u4le();
    m_flnames_dir_size = m__io->read_u4le();
    m_unk2 = m__io->read_u4le();
    m_number_files = m__io->read_u4le();
}

cement_t::header_t::~header_t() {
    _clean_up();
}

void cement_t::header_t::_clean_up() {
}

cement_t::directory_t::directory_t(kaitai::kstream* p__io, cement_t* p__parent, cement_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    f_file = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void cement_t::directory_t::_read() {
    m_hash = m__io->read_u4le();
    m_fl_offset = m__io->read_u4le();
    m_fl_size = m__io->read_u4le();
}

cement_t::directory_t::~directory_t() {
    _clean_up();
}

void cement_t::directory_t::_clean_up() {
    if (f_file) {
    }
}

std::string cement_t::directory_t::file() {
    if (f_file)
        return m_file;
    std::streampos _pos = m__io->pos();
    m__io->seek(fl_offset());
    m_file = m__io->read_bytes(fl_size());
    m__io->seek(_pos);
    f_file = true;
    return m_file;
}

cement_t::filename_directory_t::filename_directory_t(kaitai::kstream* p__io, cement_t* p__parent, cement_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void cement_t::filename_directory_t::_read() {
    m_unk1 = m__io->read_u4le();
    m_unk2 = m__io->read_u4le();
    m_unk3 = m__io->read_u4le();
    m_path_len = m__io->read_u4le();
    m_path = kaitai::kstream::bytes_to_str(m__io->read_bytes((path_len() - 1)), std::string("ASCII"));
    m_padding = m__io->read_u4le();
}

cement_t::filename_directory_t::~filename_directory_t() {
    _clean_up();
}

void cement_t::filename_directory_t::_clean_up() {
}

std::vector<cement_t::directory_t*>* cement_t::directory() {
    if (f_directory)
        return m_directory;
    std::streampos _pos = m__io->pos();
    m__io->seek(header()->dir_offset());
    m_directory = new std::vector<directory_t*>();
    const int l_directory = header()->number_files();
    for (int i = 0; i < l_directory; i++) {
        m_directory->push_back(new directory_t(m__io, this, m__root));
    }
    m__io->seek(_pos);
    f_directory = true;
    return m_directory;
}

std::vector<cement_t::filename_directory_t*>* cement_t::filename_directory() {
    if (f_filename_directory)
        return m_filename_directory;
    std::streampos _pos = m__io->pos();
    m__io->seek((header()->flnames_dir_offset() + 8));
    m_filename_directory = new std::vector<filename_directory_t*>();
    const int l_filename_directory = header()->number_files();
    for (int i = 0; i < l_filename_directory; i++) {
        m_filename_directory->push_back(new filename_directory_t(m__io, this, m__root));
    }
    m__io->seek(_pos);
    f_filename_directory = true;
    return m_filename_directory;
}
