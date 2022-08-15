// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "pure3d.h"

pure3d_t::pure3d_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, pure3d_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_header = 0;
    m_chunk = 0;
    f_chunk = false;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void pure3d_t::_read() {
    m_header = new header_t(m__io, this, m__root);
}

pure3d_t::~pure3d_t() {
    _clean_up();
}

void pure3d_t::_clean_up() {
    if (m_header) {
        delete m_header; m_header = 0;
    }
    if (f_chunk) {
        if (m_chunk) {
            for (std::vector<chunk_t*>::iterator it = m_chunk->begin(); it != m_chunk->end(); ++it) {
                delete *it;
            }
            delete m_chunk; m_chunk = 0;
        }
    }
}

pure3d_t::header_t::header_t(kaitai::kstream* p__io, pure3d_t* p__parent, pure3d_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void pure3d_t::header_t::_read() {
    m_header = m__io->read_bytes(4);
    m_version = m__io->read_u4le();
    m_file_size = m__io->read_u4le();
}

pure3d_t::header_t::~header_t() {
    _clean_up();
}

void pure3d_t::header_t::_clean_up() {
}

pure3d_t::chunk_t::chunk_t(kaitai::kstream* p__io, pure3d_t* p__parent, pure3d_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_chunk_header = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void pure3d_t::chunk_t::_read() {
    m_chunk_header = new chunk_header_t(m__io, this, m__root);
    m_chunk_body = m__io->read_bytes((chunk_header()->data_size() - 12));
}

pure3d_t::chunk_t::~chunk_t() {
    _clean_up();
}

void pure3d_t::chunk_t::_clean_up() {
    if (m_chunk_header) {
        delete m_chunk_header; m_chunk_header = 0;
    }
}

pure3d_t::chunk_header_t::chunk_header_t(kaitai::kstream* p__io, pure3d_t::chunk_t* p__parent, pure3d_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void pure3d_t::chunk_header_t::_read() {
    m_chnk_id = m__io->read_u4le();
    m_data_size = m__io->read_u4le();
    m_size = m__io->read_u4le();
}

pure3d_t::chunk_header_t::~chunk_header_t() {
    _clean_up();
}

void pure3d_t::chunk_header_t::_clean_up() {
}

std::vector<pure3d_t::chunk_t*>* pure3d_t::chunk() {
    if (f_chunk)
        return m_chunk;
    std::streampos _pos = m__io->pos();
    m__io->seek(12);
    m_chunk = new std::vector<chunk_t*>();
    {
        int i = 0;
        while (!m__io->is_eof()) {
            m_chunk->push_back(new chunk_t(m__io, this, m__root));
            i++;
        }
    }
    m__io->seek(_pos);
    f_chunk = true;
    return m_chunk;
}
