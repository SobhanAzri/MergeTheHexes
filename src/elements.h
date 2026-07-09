#ifndef ELEMENTS_H
#define ELEMENTS_H

enum class EElementTypes
{
    FireHex = 0,
    PoisonHex,
    FreezeHex
};

enum class EHexValues
{
    Hex0 = 0,
    Hex1,
    Hex2,
    Hex4,
    Hex8,
    HexF
};

char GetHexSymbol(const EHexValues& HexValue);

#endif /* ELEMENTS_H */
