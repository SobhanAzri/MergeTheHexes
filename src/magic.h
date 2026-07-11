#ifndef MAGIC_H
#define MAGIC_H

#include <raylib.h>
#include "elements.h"

struct MagicBar
{
 private:

    MagicBar() {};

 public:

    // i used this pattern just because i wanted to have only one MagicBar throughout the game
    // i learnt this from a SDL book and i dont know if its a good move right now :)

    static MagicBar& Get()
    {
        static MagicBar instance;
        return instance;
    }
    void InitMagic();
    void UpdateMagic();
    void DrawMagic();
    void UnloadMagic();


    float gridOffsetX;

    char hexadecimalValue[7] = "000000";
    Color currentColor = {255, 255, 255, 255};
    Texture2D tileTexture = { 0 };

    // there is only 24H left for the end of the jam so im just gonna rawdog it with messy codes

    void SetHexValue(const int& index, const EHexValues& hexValue);
    char GetHexValue(const int& index);
    int HexaToDec(const char& hexadecimal);
    void ResetMagic();
    void RefreshMagicColor();
    Color GetMagicColor();

};

#endif /* MAGIC_H */
