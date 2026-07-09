#include "screens.h"
#include <raylib.h>
#include <string>

static int framesCounter = 0;
static bool bIsScreenFinished = false;

static int logoPositionX = 0;
static int logoPositionY = 0;

static Texture2D background = { 0 };
static Texture2D logo = { 0 };
static float logoAlpha = 0; // for fading amount
static float nameAlpha = 0;

static Sound fxLogo01 = { 0 };
static Sound fxLogo02 = { 0 };

static bool isReducingLogoAlpha = false;
static bool isReducingNameAlpha = false;


static int state = 0; // 0 : raylib logo   1 : "developer Name" text
 
void InitLogoScreen()
{
    framesCounter = 0;
    bIsScreenFinished = false;

    isReducingLogoAlpha = false;
    isReducingNameAlpha = false;

    logoPositionX = GetScreenWidth() / 2;
    logoPositionY = GetScreenHeight() / 2;

    logo = LoadTexture("resources/logo/raylib.png");
    background = LoadTexture("resources/logo/logo_background.png");
    logoAlpha = 0;

    fxLogo01 = LoadSound("resources/logo/logo01.wav");
    fxLogo02 = LoadSound("resources/logo/logo02.wav");
    PlaySound(fxLogo01);

    state = 0;
}

void UpdateLogoScreen()
{

    if (state == 0)
    {
        framesCounter++;

        if (!isReducingLogoAlpha)
            logoAlpha += .0075f;
        else
            logoAlpha -= .01f;

        if (logoAlpha >= 1 && framesCounter == 200)
        {
                isReducingLogoAlpha = true;     // startfading out logo
                logoAlpha = 1;      // incase if alpha is much higher than 1
        }
        
        if (logoAlpha <= 0)
        {
            logoAlpha = 0;  // hide logo
            state = 1;      // start showing name
            framesCounter = 0;
            PlaySound(fxLogo02);
        }

    }
    else if (state == 1)
    {
        framesCounter++;

        if (!isReducingNameAlpha)
            nameAlpha += .015f;
        else
            nameAlpha -= .025f;

        if (nameAlpha >= 1 && !isReducingNameAlpha && framesCounter == 150)
        {
            isReducingNameAlpha = true;
            nameAlpha = 1;
        }

        if (nameAlpha <= 0 && isReducingNameAlpha)
        {
            nameAlpha = 0;
            bIsScreenFinished = true;
        }
    }
    

}

void DrawLogoScreen()
{
    DrawTexture(background,
         (GetScreenWidth() / 2) - background.width/2, (GetScreenHeight() / 2) - background.height/2, WHITE);
    if (state == 0)
    {
        DrawText("Powered by", (GetScreenWidth()/2) - logo.width/2 , (GetScreenHeight() / 2) - logo.height/2 - 25,
         20, Fade(BLACK, logoAlpha));
        DrawTexture(logo, (GetScreenWidth() / 2) - logo.width/2, (GetScreenHeight() / 2) - logo.height/2,
         Fade(WHITE, logoAlpha));
    }
    else if (state == 1)
    {
        DrawText("A Game by", GetScreenWidth()/2 - 230, (GetScreenHeight() / 2) - 30,
         20, Fade(BLACK, nameAlpha));
        DrawText("Sobhan Azari Asl", GetScreenWidth()/2 - 200, GetScreenHeight()/2, 50, Fade(BLACK, nameAlpha));
    }
}

void UnloadLogoScreen()
{
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadSound(fxLogo01);
    UnloadSound(fxLogo02);
}

int FinishLogoScreen()
{
    return bIsScreenFinished;
}