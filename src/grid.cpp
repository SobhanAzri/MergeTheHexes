#include "grid.h"
#include "elements.h"
#include <raylib.h>

void TileGrid::InitGrid()
{
    gridOffsetX = GetScreenWidth()/2 - ((GRID_SIZE/2) * (TILE_SIZE));
    gridOffsetY = 10;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j].hexValue = RandomHex();
        }
    }
}
    
void TileGrid::UpdateGrid()
{
}

void TileGrid::DrawGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            Rectangle rect = {gridOffsetX + float(i * TILE_SIZE),gridOffsetY + float(j * TILE_SIZE),
                 TILE_SIZE, TILE_SIZE};
            DrawRectangleLinesEx(rect, 1, GRAY);

            //Vector2 pos = {float(i * TILE_SIZE), float(j * TILE_SIZE)};

            DrawTextEx(GetFontDefault(),
            TextFormat("%c", GetHexSymbol(grid[i][j].hexValue)),
            (Vector2) {rect.x + 12, rect.y + 8},
            20,1,WHITE
            );
        }
    }
}

void TileGrid::UnloadGrid()
{

}

EHexValues TileGrid::RandomHex() const
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