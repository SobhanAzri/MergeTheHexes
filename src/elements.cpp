#include "elements.h"
#include <raylib.h>

char GetHexSymbol(const EHexValues& HexValue)
{
    /*switch (HexValue)
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
    }*/

    int val = static_cast<int>(HexValue);

    if (val >= 0 && val <= 9) {
        return '0' + val; 
    } 
    else if (val >= 10 && val <= 15) {
        return 'A' + (val - 10); 
    }

    return '0';
}

EHexValues RandomHex()
{
    // i tried so many ways to not hit dead end but i noticed for a 6x6 grid its nearly impossible
    // to have 5 different tile hexadecimal values so i decreased the number of elements to 4
    // also i thought about checking for deadends and reshuffling if there is no possible moves
    // but imlementing that would take a full day for me so i will just add a button for reshuffling

    int dice = GetRandomValue(1, 100);

    if (dice <= 35)
        return EHexValues::Hex2;
    else if (dice <= 75)
        return EHexValues::Hex3;
    else if (dice <= 100)
        return EHexValues::Hex4;
}



//
//  After playing the game myself , i noticed that it hurts my eyes to stare at a bunch of tiles with same color
//  and just cheking the characters written on it.
//  so i made F golden and the other hexadecimal values are just slightly different in color so player can find matching
//  tiles easily, i could use these colors for font colors instead of the tile itself but i think i need to test it first
//
//  ---nvm i tested out font colors and it was horrible!

Color GetHexValueColor(const EHexValues& hexValue)
{
    switch (hexValue)
    {
        case EHexValues::Hex2:
            return WHITE;
        case EHexValues::Hex3:
            return {210, 210, 240,255};
        case EHexValues::Hex4:
            return {180, 215, 240,255};
        default:
            break;
    }

    return WHITE;
}