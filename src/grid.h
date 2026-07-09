#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include <raymath.h>
#include "elements.h"

#define GRID_SIZE 6
#define TILE_SIZE 45
#define NUMBER_OF_ELEMENT_TYPES 5

struct FTile
{
  EHexValues hexValue;
  bool bIsHighlighted;
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

    EHexValues RandomHex() const;

 private:

    float gridOffsetX;
    float gridOffsetY;

    FTile grid[GRID_SIZE][GRID_SIZE];
};

#endif /* GRID_H */
