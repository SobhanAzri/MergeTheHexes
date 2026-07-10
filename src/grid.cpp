#include "grid.h"
#include "elements.h"
#include <algorithm>
#include <raymath.h>
#include <cstdio>
#include <raylib.h>
#include "assets.h"

void TileGrid::InitGrid()
{
    gridOffsetX = GetScreenWidth()/2 - (GRID_SIZE/2 * (TILE_SIZE));
    gridOffsetY = 25 ;

    tileTexture = LoadTexture("resources/tileButton.png");

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            tiles[i][j].position = {gridOffsetX + (j * TILE_SIZE - 5), gridOffsetY + (i * TILE_SIZE - 5)};
            tiles[i][j].rect = {gridOffsetX + (j * TILE_SIZE - 5), gridOffsetY + (i * TILE_SIZE - 5), TILE_SIZE, TILE_SIZE};
            tiles[i][j].hexValue = RandomHex();
            tiles[i][j].bIsHighlighted = false;
            tiles[i][j].row = i;
            tiles[i][j].col = j;
        }
    }

    m_pTile_Focused = nullptr;
    m_pTile_MatchingStart = nullptr;
    m_pTile_MatchingEnd = nullptr;

    m_bIsHighlightingTileset = false;
}
    
void TileGrid::UpdateGrid()
{
    Vector2 mousePosition = GetMousePosition();


    m_pTile_Focused = nullptr; 

    // hovering on tiles
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (mousePosition.x > tiles[i][j].position.x && mousePosition.x < (tiles[i][j].position.x + TILE_SIZE))
            {
                if (mousePosition.y > tiles[i][j].position.y && mousePosition.y < (tiles[i][j].position.y + TILE_SIZE))
                {
                    tiles[i][j].bIsHighlighted = true; 
                    m_pTile_Focused = &tiles[i][j];

                }
                else
                {
                    if (&tiles[i][j] != m_pTile_MatchingStart)
                        tiles[i][j].bIsHighlighted = false; 
                }
            }
            else
            {
                if (&tiles[i][j] != m_pTile_MatchingStart)
                    tiles[i][j].bIsHighlighted = false;
            }
        
        }
    }

    // getting track of selected tiles
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m_pTile_Focused != nullptr)
    {
        if (m_pTile_MatchingStart == nullptr)
        {
            m_pTile_MatchingStart = m_pTile_Focused;
            m_pTile_MatchingStart->bIsHighlighted = true;
        }
        else if (m_pTile_MatchingEnd == nullptr)
        {
            m_pTile_MatchingEnd = m_pTile_Focused;
            CheckSelections();
        }
        else
        {
            m_pTile_MatchingStart = nullptr;
            m_pTile_MatchingEnd = nullptr;
        }

    }

    // i just put this here in case someone wants to reset their selection
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
            m_pTile_MatchingStart = nullptr;
            m_pTile_MatchingEnd = nullptr;
    }
}

void TileGrid::DrawGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            Rectangle sourceRect = {0,0, gameplay_grid_tile_width_height, gameplay_grid_tile_width_height};
            
            if (tiles[i][j].bIsHighlighted )
                DrawText("A", tiles[i][j].position.x, tiles[i][j].position.y, 10, WHITE);

           
            if (tiles[i][j].bIsHighlighted)
                DrawTexturePro(tileTexture, sourceRect, tiles[i][j].rect,
             {0,0}, 0, {120,255,120, 255});
            else
                DrawTexturePro(tileTexture, sourceRect, tiles[i][j].rect, {0,0}, 0, WHITE);
        }
    }

    HighlightMatchingTileset();
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

void TileGrid::CheckSelections()
{


    if (m_pTile_MatchingStart->row == m_pTile_MatchingEnd->row)
    {
        //printf("YES");

        int sharedRow = m_pTile_MatchingStart->row; // just for clarification; might remove later
        int minCol = std::min(m_pTile_MatchingStart->col, m_pTile_MatchingEnd->col);
        int maxCol = std::max(m_pTile_MatchingStart->col, m_pTile_MatchingEnd->col);

        for (int i = minCol; i <= maxCol; i++)
        {
            if (tiles[sharedRow][i].hexValue == m_pTile_MatchingStart->hexValue)
            {}
        }
    }
    else if (m_pTile_MatchingStart->col == m_pTile_MatchingEnd->col)
    {

    }

    m_pTile_MatchingStart = nullptr;
    m_pTile_MatchingEnd = nullptr;
    return;
}

void TileGrid::RemoveTileFromGrid(const int& row, const int& col)
{

}

void TileGrid::HighlightMatchingTileset()
{
    if (m_pTile_MatchingStart == nullptr || m_pTile_Focused == nullptr)
        return;

    m_bIsHighlightingTileset = true;

    if (m_pTile_MatchingStart->col == m_pTile_Focused->col)
    {
        int sharedCol = m_pTile_MatchingStart->col;
        int minRow = std::min(m_pTile_MatchingStart->row, m_pTile_Focused->row);
        int maxRow = std::max(m_pTile_MatchingStart->row, m_pTile_Focused->row);

        for (int i = minRow; i <= maxRow; i++)
        {
            tiles[i][sharedCol].bIsHighlighted = true;
        }

        return;
    }
    else if (m_pTile_MatchingStart->row == m_pTile_Focused->row)
    {
        int sharedRow = m_pTile_MatchingStart->row;
        int minCol = std::min(m_pTile_MatchingStart->col, m_pTile_Focused->col);
        int maxCol = std::max(m_pTile_MatchingStart->col, m_pTile_Focused->col);

        for (int i = minCol; i <= maxCol; i++)
        {
            tiles[sharedRow][i].bIsHighlighted = true;
        }

        return;
    }
}
