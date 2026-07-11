#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include "elements.h"

#define GRID_SIZE 6
#define TILE_SIZE 45
#define NUMBER_OF_ELEMENT_TYPES 5

struct FTile
{
   Vector2 position;
   Rectangle rect;
   EHexValues hexValue;
   bool bIsHighlighted;

   int row;
   int col;
};

class TileGrid
{

 public:

    TileGrid() {};
    ~TileGrid() {};

    void InitGrid();
    void UpdateGrid();
    void DrawGrid();
    void UnloadGrid();

    void CheckSelections();
    void RemoveMatchingTilesFromRow(const int& row,  const int& colMin, const int& colMax);
    void RemoveMatchingTilesFromCol(const int& col, const int& rowMin, const int& rowMax);
    void HighlightMatchingTileset();

 private:

    float gridOffsetX;
    float gridOffsetY;

    FTile tiles[GRID_SIZE][GRID_SIZE];
    FTile* m_pTile_Focused;
    FTile* m_pTile_MatchingStart;
    FTile* m_pTile_MatchingEnd;
    Texture2D tileTexture;
};

#endif /* GRID_H */
