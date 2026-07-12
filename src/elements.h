#ifndef ELEMENTS_H
#define ELEMENTS_H

struct Color;

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
    Hex3,
    Hex4,
    Hex5,
    Hex6,
    Hex7,
    Hex8,
    Hex9,
    HexA,
    HexB,
    HexC,
    HexD,
    HexE,
    HexF
};

char GetHexSymbol(const EHexValues& HexValue);
EHexValues RandomHex();
Color GetHexValueColor(const EHexValues& hexValue);


// some hexadecimaal and decimal converters
EHexValues DecToHex(const int& integer);
//int HexToDec(const char& hexadecimal);

#endif /* ELEMENTS_H */
