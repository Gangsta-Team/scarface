#ifndef PURE3D_H_
#define PURE3D_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

/**
 * This is crap. do not use it.
 */

class pure3d_t : public kaitai::kstruct {

public:
    class header_t;
    class chunk_t;
    class chunk_header_t;

    pure3d_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, pure3d_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~pure3d_t();

    class header_t : public kaitai::kstruct {

    public:

        header_t(kaitai::kstream* p__io, pure3d_t* p__parent = 0, pure3d_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~header_t();

    private:
        std::string m_header;
        uint32_t m_version;
        uint32_t m_file_size;
        pure3d_t* m__root;
        pure3d_t* m__parent;

    public:
        std::string header() const { return m_header; }
        uint32_t version() const { return m_version; }
        uint32_t file_size() const { return m_file_size; }
        pure3d_t* _root() const { return m__root; }
        pure3d_t* _parent() const { return m__parent; }
    };

    class chunk_t : public kaitai::kstruct {

    public:

        chunk_t(kaitai::kstream* p__io, pure3d_t* p__parent = 0, pure3d_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~chunk_t();

    private:
        chunk_header_t* m_chunk_header;
        std::string m_chunk_body;
        pure3d_t* m__root;
        pure3d_t* m__parent;

    public:
        chunk_header_t* chunk_header() const { return m_chunk_header; }
        std::string chunk_body() const { return m_chunk_body; }
        pure3d_t* _root() const { return m__root; }
        pure3d_t* _parent() const { return m__parent; }
    };

    class chunk_header_t : public kaitai::kstruct {

    public:

        chunk_header_t(kaitai::kstream* p__io, pure3d_t::chunk_t* p__parent = 0, pure3d_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~chunk_header_t();

    private:
        uint32_t m_chnk_id;
        uint32_t m_data_size;
        uint32_t m_size;
        pure3d_t* m__root;
        pure3d_t::chunk_t* m__parent;

    public:
        uint32_t chnk_id() const { return m_chnk_id; }
        uint32_t data_size() const { return m_data_size; }
        uint32_t size() const { return m_size; }
        pure3d_t* _root() const { return m__root; }
        pure3d_t::chunk_t* _parent() const { return m__parent; }
    };

private:
    bool f_chunk;
    std::vector<chunk_t*>* m_chunk;

public:
    std::vector<chunk_t*>* chunk();

private:
    header_t* m_header;
    pure3d_t* m__root;
    kaitai::kstruct* m__parent;

public:
    header_t* header() const { return m_header; }
    pure3d_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // PURE3D_H_
