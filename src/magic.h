#ifndef MAGIC_H
#define MAGIC_H

#include <raylib.h>
#include "elements.h"

struct MagicBar
{
    char hexadecimalValue[7] = "000000";
    Color currentColor = {0, 0, 0, 255};

    void setHexValue(const int& index, const EHexValues& hexValue)
    {
        hexadecimalValue[index] = GetHexSymbol(hexValue);
    }
};

#endif /* MAGIC_H */
