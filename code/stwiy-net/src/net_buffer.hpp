#ifndef SF_MOD_NET_BUFFER_H
#define SF_MOD_NET_BUFFER_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace gangsta::sf_net {
    class Buffer {

    private:
        size_t position;
        size_t virtual_length;
        size_t real_length;
        uint8_t *data;

        void grow(size_t p_length);

    public:
        // write
        void write_data(void *p_src, size_t p_length);

        void write_u8(uint8_t p_val);

        void write_u32(uint32_t p_val);

        void write_string(const std::string &p_val);

        // read
        uint8_t read_u8();

        uint16_t read_u16();

        uint32_t read_u32();

        std::string read_string();

        void read_data(void *p_dst, size_t p_length);

        // other
        uint8_t* get_ptr();

        void get_data(size_t p_src_offset, size_t p_src_count, void *p_dst, size_t p_dst_length) const;

        void get_all_data(void *p_dst, size_t p_dst_length) const;

        void seek(size_t p_position);

        size_t get_position() const;

        size_t get_length() const;

        void set_length(size_t p_length);

        size_t get_available() const;

        Buffer *slice(size_t p_length);

        Buffer(void *p_data, size_t p_length);

        Buffer();

        ~Buffer();
    };
}

#endif