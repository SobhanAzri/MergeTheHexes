#include "screens.h"


static int frameCounter = 0;
static int frameTime = 0;


static bool bShowTutorial = false;
static bool bIsScreenFinished = false;

void InitTitleScreen()
{
    bShowTutorial = false;
}

void UpdateTitleScreen()
{

}

void DrawTitleScreen()
{

}

void UnloadTitleScreen()
{

}

int FinishTitleScreen()
{
    return bIsScreenFinished;
}