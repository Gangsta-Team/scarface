#ifndef CEMENT_H_
#define CEMENT_H_

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

class cement_t : public kaitai::kstruct {

public:
    class header_t;
    class directory_t;
    class filename_directory_t;

    cement_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, cement_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~cement_t();

    class header_t : public kaitai::kstruct {

    public:

        header_t(kaitai::kstream* p__io, cement_t* p__parent = 0, cement_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~header_t();

    private:
        std::string m_file_id;
        uint32_t m_unk1;
        uint32_t m_dir_offset;
        uint32_t m_dir_size;
        uint32_t m_flnames_dir_offset;
        uint32_t m_flnames_dir_size;
        uint32_t m_unk2;
        uint32_t m_number_files;
        cement_t* m__root;
        cement_t* m__parent;

    public:
        std::string file_id() const { return m_file_id; }
        uint32_t unk1() const { return m_unk1; }
        uint32_t dir_offset() const { return m_dir_offset; }
        uint32_t dir_size() const { return m_dir_size; }
        uint32_t flnames_dir_offset() const { return m_flnames_dir_offset; }
        uint32_t flnames_dir_size() const { return m_flnames_dir_size; }
        uint32_t unk2() const { return m_unk2; }
        uint32_t number_files() const { return m_number_files; }
        cement_t* _root() const { return m__root; }
        cement_t* _parent() const { return m__parent; }
    };

    class directory_t : public kaitai::kstruct {

    public:

        directory_t(kaitai::kstream* p__io, cement_t* p__parent = 0, cement_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~directory_t();

    private:
        bool f_file;
        std::string m_file;

    public:
        std::string file();

    private:
        uint32_t m_hash;
        uint32_t m_fl_offset;
        uint32_t m_fl_size;
        cement_t* m__root;
        cement_t* m__parent;

    public:
        uint32_t hash() const { return m_hash; }
        uint32_t fl_offset() const { return m_fl_offset; }
        uint32_t fl_size() const { return m_fl_size; }
        cement_t* _root() const { return m__root; }
        cement_t* _parent() const { return m__parent; }
    };

    class filename_directory_t : public kaitai::kstruct {

    public:

        filename_directory_t(kaitai::kstream* p__io, cement_t* p__parent = 0, cement_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~filename_directory_t();

    private:
        uint32_t m_unk1;
        uint32_t m_unk2;
        uint32_t m_unk3;
        uint32_t m_path_len;
        std::string m_path;
        uint32_t m_padding;
        cement_t* m__root;
        cement_t* m__parent;

    public:
        uint32_t unk1() const { return m_unk1; }
        uint32_t unk2() const { return m_unk2; }
        uint32_t unk3() const { return m_unk3; }
        uint32_t path_len() const { return m_path_len; }
        std::string path() const { return m_path; }
        uint32_t padding() const { return m_padding; }
        cement_t* _root() const { return m__root; }
        cement_t* _parent() const { return m__parent; }
    };

private:
    bool f_directory;
    std::vector<directory_t*>* m_directory;

public:
    std::vector<directory_t*>* directory();

private:
    bool f_filename_directory;
    std::vector<filename_directory_t*>* m_filename_directory;

public:
    std::vector<filename_directory_t*>* filename_directory();

private:
    header_t* m_header;
    cement_t* m__root;
    kaitai::kstruct* m__parent;

public:
    header_t* header() const { return m_header; }
    cement_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // CEMENT_H_
