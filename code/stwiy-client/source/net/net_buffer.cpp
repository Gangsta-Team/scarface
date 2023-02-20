#include "net_buffer.hpp"

#include <cstdlib>
#include <cstring>

const size_t GROW_SIZE = 1024;

void gangsta::net::Buffer::write_data(void *p_src, size_t p_length) {
    if (p_length <= 0) {
        return;
    }
    if (position + p_length > real_length) {
        grow(p_length);
    }
    memcpy(&data[position], p_src, p_length);
    position += p_length;
    if (position > virtual_length) {
        virtual_length = position;
    }
}

uint8_t gangsta::net::Buffer::read_u8() {
    if (position >= virtual_length) {
        // end of file
        return 0;
    }
    uint8_t value = data[position];
    position++;
    return value;
}

uint16_t gangsta::net::Buffer::read_u16() {
    uint16_t value = (uint16_t) (read_u8() | read_u8() << 8);
    return value;
}

uint32_t gangsta::net::Buffer::read_u32() {
    uint32_t value = (uint32_t) (read_u8() | (read_u8() << 8) | (read_u8() << 16) | (read_u8() << 24));
    return value;
}

void gangsta::net::Buffer::read_data(void *p_dst, size_t p_length) {
    if (p_dst == nullptr) {
        return;
    }
    if (p_length > virtual_length) {
        return;
    }
    if (position + p_length > virtual_length) {
        return;
    }
    memcpy(p_dst, &data[position], p_length);
    position += p_length;
}

void gangsta::net::Buffer::get_data(size_t p_src_offset, size_t p_src_count, void *p_dst, size_t p_dst_length) const {
    if (p_dst == nullptr) {
        return;
    }
    if (p_src_count > virtual_length) {
        return;
    }
    if (p_src_offset + p_src_count > virtual_length) {
        return;
    }
    if (p_dst_length < p_src_count) {
        return;
    }
    memcpy(p_dst, &data[p_src_offset], p_src_count);
}

void gangsta::net::Buffer::get_all_data(void *p_dst, size_t p_dst_length) const {
    if (p_dst_length <= 0) {
        return;
    }
    if (p_dst_length < virtual_length) {
        return;
    }
    if (p_dst == nullptr) {
        return;
    }
    memcpy(p_dst, &data[0], virtual_length);
}

void gangsta::net::Buffer::seek(size_t p_position) {
    if (p_position > virtual_length) {
        position = virtual_length;
        return;
    }
    position = p_position;
}

size_t gangsta::net::Buffer::get_position() const {
    return position;
}

size_t gangsta::net::Buffer::get_length() const {
    return virtual_length;
}

void gangsta::net::Buffer::set_length(size_t p_length) {
    if (p_length > real_length) {
        grow(p_length - real_length);
    }
    virtual_length = p_length;
}

size_t gangsta::net::Buffer::get_available() const {
    return virtual_length - position;
}

gangsta::net::Buffer *gangsta::net::Buffer::slice(size_t p_length) {
    Buffer *buffer = new Buffer(&data[position], p_length);
    return buffer;
}

void gangsta::net::Buffer::grow(size_t p_length) {
    size_t new_real_length = real_length + p_length;
    void *new_loc = realloc(data, new_real_length);
    if (!new_loc) {
        return;
    }
    data = (uint8_t *) new_loc;
    real_length = new_real_length;
}

gangsta::net::Buffer::Buffer(void *p_data, size_t p_length) {
    real_length = p_length;
    data = (uint8_t *) malloc(real_length);
    position = 0;
    virtual_length = 0;
    write_data(p_data, p_length);
}

gangsta::net::Buffer::Buffer() {
    real_length = GROW_SIZE;
    data = (uint8_t *) malloc(real_length);
    position = 0;
    virtual_length = 0;
}

gangsta::net::Buffer::~Buffer() {
    free(data);
}

void gangsta::net::Buffer::write_u8(uint8_t p_val) {

}

uint8_t *gangsta::net::Buffer::get_ptr() {
    return data;
}
