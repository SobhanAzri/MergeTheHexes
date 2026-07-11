#include "screens.h"
#include <raylib.h>
#include "assets.h"
#include "player.h"
#include "enemy.h"
#include "grid.h"

#define MAX_ENEMIES 24

//
// Type & Structs Definition
//

struct Torch 
{
    Rectangle rectTorch;
    Rectangle rectLight;
};


static float secondsToEndScreen = 180;

static int frameCounter = 0;
static float frameTimer = 0;
static int thisFrame = 0;
static bool bIsScreenFinished = false;

static RenderTexture2D target = { 0 };


static Texture2D levelBackground;

static Texture2D torchTexture;
static Texture2D torchLightTexture;
static Torch torches[3] = {0};
static int torchMaxFrames = 8;
static float torchFrameSpeed = .1f;
static int currentTorchFrame = 0;

static Music music = { 0 };

static Player player;
static  TileGrid tileGrid;

void InitGameplayScreen()
{
    secondsToEndScreen = 180;

    frameCounter = 0;
    thisFrame = 0;
    currentTorchFrame = 0;
    bIsScreenFinished = false;


    torchTexture = LoadTexture("resources/torch.png");
    torchLightTexture = LoadTexture("resources/torchLight.png");
    for (int i = 0; i < 3; i++)
    {
        torches[i].rectTorch.x = 92 + (i * 247);
        torches[i].rectTorch.y = 370;
        torches[i].rectTorch.width = gameplay_torch_width;
        torches[i].rectTorch.height = gameplay_torch_height;

        torches[i].rectLight.x = 60 + (i * 247);
        torches[i].rectLight.y = 320;
        torches[i].rectLight.width = gameplay_torch_light_width_height;
        torches[i].rectLight.height = gameplay_torch_light_width_height; 
    }
    music = LoadMusicStream("resources/gameplay_music.wav");
    SetMusicVolume(music, .9f);
    PlayMusicStream(music);
    SetMusicVolume(fireSound, .55f);

    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    levelBackground = LoadTexture("resources/level_background.png");

    player.InitPlayer();
    tileGrid.InitGrid();
}

void UpdateGameplayScreen()
{
    frameCounter++;
    frameTimer += GetFrameTime();

    secondsToEndScreen -= GetFrameTime();

    UpdateMusicStream(music);

    if (frameTimer >= torchFrameSpeed)
    {
        currentTorchFrame++;
        frameTimer = 0;
    }

    if (currentTorchFrame >= torchMaxFrames)
    {
        currentTorchFrame = 0;
    }

    UpdateMusicStream(fireSound);
    
    player.UpdatePlayer();
    tileGrid.UpdateGrid();

    if (secondsToEndScreen <= 0)
        bIsScreenFinished = true;
}

void DrawGameplayScreen()
{
    // Draw Background
    DrawTexture(levelBackground,
         0, 0, WHITE);

    // Draw remaining seconds
    int minutes = int(secondsToEndScreen) / 60;
    int seconds = int(secondsToEndScreen) % 60;

    DrawTextEx(font, TextFormat("%d:%d",minutes,seconds), 
    {float(GetScreenWidth()/2), float(GetScreenHeight()/2)}, 30, 1, BLACK);
    

    // Draw Torches

    Rectangle torchSourceRec = {float(currentTorchFrame * gameplay_torch_width) , 0,
         gameplay_torch_width, gameplay_torch_height};
    Rectangle torchLightSourceRect = {float(currentTorchFrame * gameplay_torch_light_width_height) , 0,
         gameplay_torch_light_width_height, gameplay_torch_light_width_height};
    for (int i = 0; i < 3; i++)
    {
        Rectangle torchLightDestRect = {torches[i].rectLight.x - 30, torches[i].rectLight.y - 30,
        gameplay_torch_light_width_height * 1.5, gameplay_torch_light_width_height * 1.5};

        DrawTextureRec(torchTexture, torchSourceRec, (Vector2){torches[i].rectTorch.x, torches[i].rectTorch.y}, Fade(WHITE, .8f));
        //DrawTextureRec(torchLightTexture, torchLightSourceRect, (Vector2){torches[i].rectLight.x, torches[i].rectLight.y}, Fade(ORANGE, .45f));
        DrawTexturePro(torchLightTexture, torchLightSourceRect, torchLightDestRect,
             {0,0}, 0, Fade(ORANGE, .35f));
    }


    DrawText("TEXT", 20, 200, 20, RAYWHITE);

    player.DrawPlayer();
    tileGrid.DrawGrid();

}

void UnloadGameplayScreen()
{
    UnloadTexture(levelBackground);
    UnloadMusicStream(music);

    player.UnloadPlayer();
    tileGrid.UnloadGrid();
}

int FinishGameplayScreen()
{
    return bIsScreenFinished;
}