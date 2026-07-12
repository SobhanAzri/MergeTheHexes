#include "screens.h"
#include <raylib.h>
#include "assets.h"
#include "player.h"
#include "enemy.h"
#include "grid.h"
#include "magic.h"
#include <cstdio>

#define MAX_ENEMIES 24

//
// Type & Structs Definition
//

struct Torch 
{
    Rectangle rectTorch;
    Rectangle rectLight;
};

static Vector2 timerPosition = {0, 0};
static float secondsToEndScreen = 180;

static int frameCounter = 0;
static float frameTimer = 0;
static int thisFrame = 0;

static float enemySpeed = 13;

static bool bIsShowingTutorial = false;
static bool bIsShowingResult = false;
static bool bIsWinner = false;
static bool bIsScreenFinished = false;

static RenderTexture2D target = { 0 };
static Rectangle tutorialRect = { 0 };

static Texture2D levelBackground;
static Texture2D enemyTexture = {0 };

static Texture2D torchTexture;
static Texture2D torchLightTexture;
static Torch torches[3] = {0};
static int torchMaxFrames = 8;
static float torchFrameSpeed = .1f;
static int currentTorchFrame = 0;

static Music music = { 0 };

static Player player;
static  TileGrid tileGrid;
static std::vector<Enemy> enemies;
static float enemySpawnTimer = 0.0f;
static int enemyCount = 0;
 // i had some problems instead of fixing i redefined it to another problem,  i had this in title

void InitGameplayScreen()
{
    secondsToEndScreen = 150;
    timerPosition = {float(GetScreenWidth() - 200), float(GetScreenHeight()/2 - 150)};

    frameCounter = 0;
    thisFrame = 0;
    currentTorchFrame = 0;
    
    enemySpeed = 12;

    tutorialRect = { 25, 25, float(GetScreenWidth()) - 25*2, float(GetScreenHeight() - 25*2)};

    bIsShowingTutorial = true;
    bIsShowingResult = false;
    bIsWinner = false;
    bIsScreenFinished = false;

    enemyTexture = LoadTexture("resources/enemy.png");


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
    MagicBar::Get().InitMagic();
}

void UpdateGameplayScreen()
{
    if (!bIsShowingTutorial && !bIsShowingResult)
    {
        frameCounter++;
        frameTimer += GetFrameTime();

        if (enemySpeed <= 25)
            enemySpeed += 0.00055;

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
        MagicBar::Get().UpdateMagic();

        if (secondsToEndScreen <= 0)
            {
                bIsShowingResult = true;
                bIsWinner = true;
            }

        enemySpawnTimer += GetFrameTime();
        if (enemySpawnTimer >= 3.5f) 
        {
            enemySpawnTimer = 0.0f;
    
            int side = GetRandomValue(0, 1);
    
            float spawnX = (side == 0) ? -50.0f : (float)GetScreenWidth() + 50.0f;
    
            float spawnY = (float)GetRandomValue(GetScreenHeight() / 2, GetScreenHeight() - 50);
            Vector2 spawnPoint = {spawnX, spawnY};
            Enemy enemy = Enemy((Vector2){spawnX, spawnY}, player.GetPosition(), 5, enemyTexture);
            enemies.push_back(Enemy(spawnPoint, player.GetPosition(),enemySpeed, enemyTexture ));

        }

        printf("%f\n", enemySpeed);

        for (Enemy& enemy : enemies)
        {
            if (!enemy.bIsActive) continue;

            enemy.UpdateEnemy();

            // i couldve use player's dest rec from player.cpp but forget it i need to finish the game ASAP
            Rectangle playerHitbox = { 
            player.GetPosition().x - (player.scaledWidth / 2.0f), 
            player.GetPosition().y - (player.scaledHeight / 2.0f), 
            (float)player.scaledWidth, 
            (float)player.scaledHeight 
            };

            if (CheckCollisionRecs(enemy.GetRect(), playerHitbox))
            {
                enemy.bIsActive = false;
                player.TakeDamage();
                PlaySound(errorSounds[rand() % 2]);
                
                if (player.bIsDead())
                {
                    bIsWinner = false;
                    bIsShowingResult = true;
                }
            }

            for (MagicBall& ball : player.magicBalls) 
            {
                if (!ball.bIsActive) continue;

                Rectangle ballRect = { 
                ball.position.x, ball.position.y, 
                (float)gameplay_projectile_width_height / 2, 
                (float)gameplay_projectile_width_height / 2 
                };

                if (CheckCollisionRecs(ballRect, enemy.GetRect())) 
                {
                    ball.bIsActive = false; 
                    enemy.TakeDamage(MagicBar::Get().GetMagicColor());     
            
                    if (enemy.IsDead()) {
                        enemy.bIsActive = false;
                    };
                }
            }
        }


        
        
    }


    
}


void DrawGameplayScreen()
{
    // Draw Background
    DrawTexture(levelBackground,
         0, 0, WHITE);

    // Draw remaining seconds
    

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


    player.DrawPlayer();

    if (!bIsShowingResult && !bIsShowingTutorial)
    {
        tileGrid.DrawGrid();
        MagicBar::Get().DrawMagic();

        for (Enemy& enemy : enemies)
        {
            enemy.DrawEnemy();
        }

        int minutes = int(secondsToEndScreen) / 60;
        int seconds = int(secondsToEndScreen) % 60;

        DrawTextEx(font, TextFormat("Remaining Time\n\t\t\t\t%d:%d",minutes,seconds), 
        timerPosition, 30, 1, Fade(RAYWHITE, .75f));
    }

    /*if (bIsShowingTutorial)
    {

        DrawRectangleRec(tutorialRect, Fade(GRAY, .95));

        Rectangle rect = {tutorialRect.width / 2 - 60, tutorialRect.height - 30, 120, 50};
        if (Button(rect, "OK", font.baseSize, {125,0,0, 255}))
            bIsShowingTutorial = false;

        char* Message1 = "every tile has a hexadecimal value\nmerge them to create a higher value hexadecimal";
        char* Message2 = "resulting hexa value will go to hexadecimal color field of your magic bar";
        char* Message3 = "magic bar is the colored in the center, the color defines your hex powers";
        char* Message4 = "the RR red field is fire power, also GG and BB are for poison and freeze powers";

        DrawTextEx(font2, Message1, {float(GetScreenWidth()/2), rect.y + 50}, font2.baseSize * 2, 1, BLACK);
    
    }*/

    if (bIsShowingTutorial)
    {
        DrawRectangleRec(tutorialRect, Fade(GRAY, 0.95f));
        DrawRectangleLinesEx(tutorialRect, 2.0f, WHITE);

        const char* lines[] = {
            "Every tile has a hexadecimal value.",
            "Merge similiar tiles to create a higher value.",
            "results is = value you selected * count",
            "min : 0   max : F",
            "The resulting value creates your",
            "magic bar's hexadecimal color.",
            "The color defines your hex powers:",
            "Red = Fire Power => 00-FF (0-255)",
            "Green = Poison Power => 00-FF (0-255)",
            "Blue = Freeze Power => 00-FF (0-255)",
            "LClick: Select/Shoot - RClick: ResetSelection",
            "Go and Merge your Hexes Together"
            };

        float fontSize = font2.baseSize * 0.7f; 
        float lineSpacing = fontSize + 5.0f;
        float startY = tutorialRect.y + 5.0f; 

        for (int i = 0; i < 12; i++)
        {
            Vector2 textSize = MeasureTextEx(font2, lines[i], fontSize, 1.0f);
            float centerX = tutorialRect.x + (tutorialRect.width - textSize.x) / 2.0f;

            DrawTextEx(font2, lines[i], {centerX, startY + (i * lineSpacing)}, fontSize, 1.0f, BLACK);
        }

        Rectangle rect = {tutorialRect.x + tutorialRect.width / 2 - 60, tutorialRect.y + tutorialRect.height - 70, 120, 50};
            if (Button(rect, "OK", font.baseSize, {125, 0, 0, 255}))
                bIsShowingTutorial = false;
            
    }

    if (bIsShowingResult)
    {
        DrawRectangleRec(tutorialRect, Fade(GRAY, .5));
        
        if (bIsWinner)
            DrawTextEx(font, "You Win!", {tutorialRect.width/2 - 150, tutorialRect.height/2 - 25}, 
        font.baseSize*3, 1, BLACK);
        else
            DrawTextEx(font, "You Died!", {tutorialRect.width/2 - 150, tutorialRect.height/2}, 
        80, 1, BLACK);


        Rectangle rect = {tutorialRect.width / 2 - 60, tutorialRect.height - 30, 120, 50};
        if (Button(rect, "OK", font.baseSize, {125,0,0, 255}))
            bIsScreenFinished = true;

    }

}

void UnloadGameplayScreen()
{
    UnloadTexture(levelBackground);
    UnloadMusicStream(music);
    UnloadTexture(enemyTexture);
    player.UnloadPlayer();
    tileGrid.UnloadGrid();
    MagicBar::Get().UnloadMagic();
    
    for (auto enemy : enemies)
        enemy.UnloadEnemy();



    enemies.clear();
}

int FinishGameplayScreen()
{
    return bIsScreenFinished;
}