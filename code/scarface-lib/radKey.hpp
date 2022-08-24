#ifndef _RADKEY_HPP
#define _RADKEY_HPP

namespace core
{

    typedef unsigned int Key;

    Key stxHashString(const char *txt, Key keyValue);
    Key stxHashStringLowerCase(const char* txt, Key keyValue);
    Key MakeKey(const char* txt, bool ignoreUpperCase = true);

}

#endif // !_RADKEY_HPP