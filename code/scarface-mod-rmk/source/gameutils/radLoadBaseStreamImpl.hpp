#ifndef _RADLOADBASESTREAMIMPL_HPP
#define _RADLOADBASESTREAMIMPL_HPP

class radLoadBaseStreamImpl : public radLoadBaseStream
{
public:
    inline bool Read(void* output, unsigned int size, unsigned int array_size)
    {
        typedef int (__thiscall* _Maphacks)(void* _this, void *output, unsigned int size, int array_size, int a5);

        return ((_Maphacks)0x006EB050)(this, output, size, 1, 1);
    }
};

#endif // !_RADLOADBASESTREAMIMPL_HPP