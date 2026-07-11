#include "grid.h"
#include "elements.h"
#include <algorithm>
#include <raymath.h>
#include <cstdio>
#include <raylib.h>
#include "assets.h"
#include "screens.h"

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
            if (CheckCollisionPointRec(mousePosition, tiles[i][j].rect))
            {
                    tiles[i][j].bIsHighlighted = true; 
                    m_pTile_Focused = &tiles[i][j];

                    /*if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
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
                    }*/
            }
            else
            {
                if (&tiles[i][j] != m_pTile_MatchingStart)
                    tiles[i][j].bIsHighlighted = false; 
            }
        
        }

        HighlightMatchingTileset();
    }

    // getting track of selected tiles
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m_pTile_Focused != nullptr)
    {

        if (m_pTile_MatchingStart == nullptr)
        {
            m_pTile_MatchingStart = m_pTile_Focused;
            m_pTile_MatchingStart->bIsHighlighted = true;
            PlaySound(clickSounds[rand() % 2]);
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


    HighlightMatchingTileset();
}

void TileGrid::DrawGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            Rectangle sourceRect = {0,0, gameplay_grid_tile_width_height, gameplay_grid_tile_width_height};

           
            if (tiles[i][j].bIsHighlighted)
                DrawTexturePro(tileTexture, sourceRect, tiles[i][j].rect,
             {0,0}, 0, {66,245,66, 255});
            else
                DrawTexturePro(tileTexture, sourceRect, tiles[i][j].rect,
             {0,0}, 0, GetHexValueColor(tiles[i][j].hexValue));

            DrawTextEx(font2, TextFormat("%c", GetHexSymbol(tiles[i][j].hexValue)),
             {tiles[i][j].position.x + tiles[i][j].rect.width/3, tiles[i][j].position.y},
              50, 1, BLACK);
        }
    }

}

void TileGrid::UnloadGrid()
{
    UnloadTexture(tileTexture);
}



void TileGrid::CheckSelections()
{
    if (m_pTile_MatchingStart == nullptr || m_pTile_MatchingEnd == nullptr)
        return;

    bool bCanMatch = true;
    int counter = 0; // this is just for cheking if the selected tiles have atleast one tile between them

    if (m_pTile_MatchingStart->row == m_pTile_MatchingEnd->row)
    {
        //printf("YES");

        int sharedRow = m_pTile_MatchingStart->row; // just for clarification; might remove later
        int minCol = std::min(m_pTile_MatchingStart->col, m_pTile_MatchingEnd->col);
        int maxCol = std::max(m_pTile_MatchingStart->col, m_pTile_MatchingEnd->col);

        for (int i = minCol; i <= maxCol; i++)
        {
            if (tiles[sharedRow][i].hexValue != m_pTile_MatchingStart->hexValue)
                {
                    bCanMatch = false;
                    break;
                }
            counter++;
        }

        if (bCanMatch && counter >= 3)
        {
            RemoveMatchingTilesFromRow(sharedRow, minCol, maxCol);
        }
    }
    else if (m_pTile_MatchingStart->col == m_pTile_MatchingEnd->col)
    {
        int sharedCol = m_pTile_MatchingStart->col; // just for clarification; might remove later
        int minRow = std::min(m_pTile_MatchingStart->row, m_pTile_MatchingEnd->row);
        int maxRow = std::max(m_pTile_MatchingStart->row, m_pTile_MatchingEnd->row);
        

        for (int i = minRow; i <= maxRow; i++)
        {
            if (tiles[i][sharedCol].hexValue != m_pTile_MatchingStart->hexValue)
                {
                    bCanMatch = false;
                    break;
                }
            counter++;
        }

        if (bCanMatch && counter >= 3)
        {
            RemoveMatchingTilesFromCol(sharedCol, minRow, maxRow);
        }
    }
        
    if (bCanMatch && counter >= 3)
        PlaySound(successSounds[rand() % 2]);
    else
        PlaySound(errorSounds[rand() % 2]);

    m_pTile_MatchingStart = nullptr;
    m_pTile_MatchingEnd = nullptr;
    return;
}

void TileGrid::RemoveMatchingTilesFromRow(const int& row, const int& colMin, const int& colMax)
{
    if (m_pTile_MatchingStart == nullptr || m_pTile_MatchingEnd == nullptr)
        return;

    for (int i = row; i >= 0; i--)
    {
        // im eliminating selected row and shifting the upperones one unit lower, but not row 0 because ill generate that
        for (int j = colMin; j <= colMax; j++)
        {
            if (i != 0 )
                tiles[i][j].hexValue = tiles[i - 1][j].hexValue;
            else
                tiles[i][j].hexValue = RandomHex();
        }
    }
}

void TileGrid::RemoveMatchingTilesFromCol(const int& col, const int& rowMin, const int& rowMax)
{
    if (m_pTile_MatchingStart == nullptr || m_pTile_MatchingEnd == nullptr)
        return;

    int iterator = 0;

    for (int i = rowMin ; i <= 0; i--)
    {
        if (i != 0)
            tiles[rowMax - iterator][col].hexValue = tiles[i - 1][col].hexValue;
        else
            break;

        iterator++;
    }

    for (int i = rowMax - iterator; i <= 0; i--)
    {
        tiles[i][col].hexValue = RandomHex();
    }

    printf("Col");
}

void TileGrid::HighlightMatchingTileset()
{
    if (m_pTile_MatchingStart == nullptr || m_pTile_Focused == nullptr)
        return;

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
