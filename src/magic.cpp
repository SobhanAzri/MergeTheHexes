#include "magic.h"
#include "assets.h"
#include <raylib.h>
#include "screens.h"

void MagicBar::InitMagic()
{
    tileTexture = LoadTexture("resources/HexButton.png");
    currentColor = {0,0,0, 255};
    gridOffsetX = GetScreenWidth()/2 - (6/2 * (45)) - 5; // so many magic numbers but they are just GRID_SIZE and TILE_SIZE from grid.h
    
    colorText = "Magic Color Code :";
    textSize = MeasureTextEx(font, colorText, 30, 1.0f);
    textPos = {
        gridOffsetX - textSize.x - 20.0f, 
        (float(GetScreenHeight()) / 2.0f) - 65.0f + (45.0f - textSize.y) / 2.0f
    };
}

void MagicBar::UpdateMagic()
{

}

int MagicBar::HexaToDec(const char& hexadecimal)
{
    if (hexadecimal >= '0' && hexadecimal <= '9') return (hexadecimal - '0');
    if (hexadecimal >= 'A' && hexadecimal <= 'F') return (10 + (hexadecimal - 'A'));
    return 0;
}

void MagicBar::DrawMagic()
{
    Vector2 textPos = {
        gridOffsetX - textSize.x - 20.0f, 
        (float(GetScreenHeight()) / 2.0f) - 65.0f + (45.0f - textSize.y) / 2.0f
    };
    

    for (int i = 0; i < 6; i++)
    {
        Color tileColor = {0};

        if (i < 2)
            tileColor = {255, 120, 130,255};
        else if (i < 4)
            tileColor = {190, 255, 206,255};
        else if (i < 6)
            tileColor = {140, 180, 230,255};

        Rectangle sourceRect = {0,0, gameplay_magic_tile_width, gameplay_magic_tile_height};
        Rectangle destRect = {gridOffsetX + (i * 45), float(GetScreenHeight()/2) - 65, 45, 45 };

        DrawTexturePro(tileTexture, sourceRect, destRect, {0, 0}, 0, tileColor);
        DrawTextEx(font2, TextFormat("%c", GetHexValue(i)), {gridOffsetX + (i * 45) + 15,
             float(GetScreenHeight()/2) - 62}, 42, 1, BLACK);
    }

    DrawTextEx(font, colorText, {textPos.x + 10, textPos.y}, 30, 1.0f, Fade(RAYWHITE, 0.8f));

    DrawTextEx(font, TextFormat("Fire: %d", (int)currentColor.r), {textPos.x + 10, textPos.y - 150}, 25, 1, RED);
    DrawTextEx(font, TextFormat("Poison: %d", (int)currentColor.g), {textPos.x + 10, textPos.y - 120}, 25, 1, GREEN);
    DrawTextEx(font, TextFormat("Freeze: %d", (int)currentColor.b), {textPos.x + 10, textPos.y - 90}, 25, 1, BLUE);
}

void MagicBar::UnloadMagic()
{
    UnloadTexture(tileTexture);
}

void MagicBar::SetHexValue(const int& index, const EHexValues& hexValue)
{
    hexadecimalValue[index] = GetHexSymbol(hexValue);

    RefreshMagicColor();
}

char MagicBar::GetHexValue(const int& index)
{
    return hexadecimalValue[index];
}

void MagicBar::ResetMagic()
{
    for (int i = 0; i < 6; i++)
    {
        hexadecimalValue[i] = '0';
    }

    RefreshMagicColor();
}

void MagicBar::RefreshMagicColor()
{
    int red = (HexaToDec(hexadecimalValue[0]) * 16) + HexaToDec(hexadecimalValue[1]);
    int green = (HexaToDec(hexadecimalValue[2]) * 16) + HexaToDec(hexadecimalValue[3]);
    int blue = (HexaToDec(hexadecimalValue[4]) * 16) + HexaToDec(hexadecimalValue[5]);
    currentColor = (Color){ (unsigned char)red, (unsigned char)green, (unsigned char)blue, 255 };
}

Color MagicBar::GetMagicColor()
{
    return currentColor;   
}