#include "elements.h"
#include <raylib.h>

char GetHexSymbol(const EHexValues& HexValue)
{
    switch (HexValue)
    {
        case EHexValues::Hex0:
            return '0';
        case EHexValues::Hex1:
            return '1';
        case EHexValues::Hex2:
            return '2';
        case EHexValues::Hex4:
            return '4';
        case EHexValues::Hex8:
            return '8';
        case EHexValues::HexF:
            return 'F';
        default:
            break;
    }

    return '0';
}

EHexValues RandomHex()
{
    EHexValues AllPossibleValues[5] = 
    {
        EHexValues::Hex1,
        EHexValues::Hex2,
        EHexValues::Hex4,
        EHexValues::Hex8,
        EHexValues::HexF
    };

    int randomIndex = GetRandomValue(0, 4);

    return AllPossibleValues[randomIndex];
}

Color GetHexValueColor(const EHexValues& hexValue)
{
    switch (hexValue)
    {
        case EHexValues::Hex1:
            return WHITE;
        case EHexValues::Hex2:
            return {210, 210, 240,255};
        case EHexValues::Hex4:
            return {180, 215, 240,255};
        case EHexValues::Hex8:
            return {145, 189, 223,255};
        case EHexValues::HexF:
            return {255, 235, 0, 255};
        default:
            break;
    }

    return WHITE;
}