#include "radkey.hpp"

core::Key core::stxHashString(const char *txt, Key keyValue)
{
    core::Key endKeyValue = 0;

    if(txt == nullptr)
    {
        return keyValue;
    }

    char currentChar = *txt;
    const char* currentStrPtr = txt;

    if(currentChar == 0x00)
    {
        return keyValue;
    }

    endKeyValue = keyValue & 0x7FFFFFFF;

    do
    {
        ++currentStrPtr;
        endKeyValue = currentChar ^ ((65599 * endKeyValue) & 0x7FFFFFFF);
        currentChar = *currentStrPtr;
    } while (*currentStrPtr);

    return endKeyValue | 0x80000000;
}

core::Key core::stxHashStringLowerCase(const char* txt, Key keyValue)
{
    core::Key endKeyValue = 0;

    if(txt == nullptr)
    {
        return keyValue;
    }

    char currentChar = *txt;
    const char* currentStrPtr = txt;

    if(currentChar == 0x00)
    {
        return keyValue;
    }

    endKeyValue = keyValue & 0x7FFFFFFF;

    do
    {
        ++currentStrPtr;

        if(currentChar < 'a')
        {
            currentChar += 0x20;
        }

        endKeyValue = currentChar ^ ((65599 * endKeyValue) & 0x7FFFFFFF);
        currentChar = *currentStrPtr;
    } while (*currentStrPtr);

    return endKeyValue | 0x80000000;
}

core::Key core::MakeKey(const char* txt, bool ignoreUpperCase)
{
    if(ignoreUpperCase)
    {
        return stxHashString(txt, 0);
    }

    return stxHashStringLowerCase(txt, 0);
}