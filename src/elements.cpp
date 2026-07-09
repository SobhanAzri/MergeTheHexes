#include "elements.h"

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