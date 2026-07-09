#include "screens.h"
#include <raylib.h>


static int frameCounter = 0;
static int frameTime = 0;


static bool bShowTutorial = false;
static bool bIsScreenFinished = false;

static Texture2D titleTexture;
static Texture2D fireTexture;
static Texture2D freezeTexture;
static Texture2D poisonTexture;
static Texture2D poisonSmokeTexture;



void InitTitleScreen()
{
    bShowTutorial = false;

    titleTexture = LoadTexture("resources/title/title.png");
}

void UpdateTitleScreen()
{

}

void DrawTitleScreen()
{

}

void UnloadTitleScreen()
{
    UnloadTexture(titleTexture);
    UnloadTexture(fireTexture);
    UnloadTexture(freezeTexture);
    UnloadTexture(poisonTexture);
    UnloadTexture(poisonSmokeTexture);
}

int FinishTitleScreen()
{
    return bIsScreenFinished;
}