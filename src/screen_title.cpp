#include "assets.h"
#include "screens.h"
#include <raylib.h>
#include "raymath.h"


static int frameCounter = 0;


static bool bShowTutorial = false;
static bool bIsScreenFinished = false;

static Texture2D titleTexture;
static Texture2D titleBackground;
static Texture2D fireTexture;
static Texture2D snowflakeTexture;
static Texture2D poisonTexture;
static Texture2D poisonSmokeTexture;

static float frameTimer = 0;

static float fireFrameTimer = 0;
static int fireCurrentFrame = 0;
static float fireFrameSpeed = 0;
static int fireMaxFrames = 0;

static float poisonSmokeFrameTimer = 0;
static int poisonSmokeCurrentFrame = 0;
static float poisonSmokeFrameSpeed = 0;
static int poisonSmokeMaxFrames = 0;

static float snowflakeFrameTimer = 0;
static int snowflakeCurrentFrame = 0;
static float snowflakeFrameSpeed = 0;
static int snowflakeMaxframes = 0;


/*Vector2 fireTris[3];
Vector2 poisonTris[3];
Vector2 freezeTris[3];*/

void InitTitleScreen()
{
    bShowTutorial = false;


    titleTexture = LoadTexture("resources/title/title.png");
    titleBackground = LoadTexture("resources/title/title_background.png");
    fireTexture = LoadTexture("resources/title/fire.png");
    snowflakeTexture = LoadTexture("resources/title/snowflakes.png");
    poisonTexture = LoadTexture("resources/title/poison.png");
    poisonSmokeTexture = LoadTexture("resources/title/poisonSmoke.png");

    frameTimer = 0;

    fireCurrentFrame = 0;
    fireFrameTimer = 0;
    fireFrameSpeed = .075;
    fireMaxFrames = 8;

    poisonSmokeCurrentFrame = 0;
    poisonSmokeFrameTimer = 0;
    poisonSmokeFrameSpeed = .15;
    poisonSmokeMaxFrames = 24;

    snowflakeFrameSpeed = .3f;
    snowflakeCurrentFrame = 0;
    snowflakeFrameTimer = 0;
    snowflakeMaxframes = 8;


    // These look horrible because i couldnt be patient wit them

    /*fireTris[0] = {(float)GetScreenWidth() /2 -120 + 60, (float)GetScreenHeight()/2 + 120 + 100};
    fireTris[1] = {(float)GetScreenWidth() /2 -120 - 60, (float)GetScreenHeight()/2 + 120 + 100};
    fireTris[2] = {(float)GetScreenWidth() /2 -120, (float)GetScreenHeight()/2 + 120 - 100};


    poisonTris[0] = {(float)GetScreenWidth() /2 -120 + 60, (float)GetScreenHeight()/2 + 120 + 100};
    poisonTris[1] = {(float)GetScreenWidth() /2 -120 - 60, (float)GetScreenHeight()/2 + 120 + 100};
    poisonTris[2] = {(float)GetScreenWidth() /2, (float)GetScreenHeight()/2 + 120};


    freezeTris[0] = {(float)GetScreenWidth() /2 -120, (float)GetScreenHeight()/2 + 120 + 100};
    freezeTris[1] = {(float)GetScreenWidth() /2 -120 - 60, (float)GetScreenHeight()/2 + 120 - 100};
    freezeTris[2] = {(float)GetScreenWidth() /2 -120 + 60, (float)GetScreenHeight()/2 + 120 - 100};*/
}

void UpdateTitleScreen()
{
    frameTimer = GetFrameTime();

    fireFrameTimer += frameTimer;
    poisonSmokeFrameTimer += frameTimer;
    snowflakeFrameTimer += frameTimer;

    if (fireFrameTimer >= fireFrameSpeed)
    {
        fireCurrentFrame++;
        fireFrameTimer = 0;
    }
    if (fireCurrentFrame >= fireMaxFrames)
        fireCurrentFrame = 0;


    if (poisonSmokeFrameTimer >= poisonSmokeFrameSpeed)
    {
        poisonSmokeCurrentFrame++;
        poisonSmokeFrameTimer = 0;
    }
    if (poisonSmokeCurrentFrame >= poisonSmokeMaxFrames)
        poisonSmokeCurrentFrame = 0;

    
    if (snowflakeFrameTimer >= snowflakeFrameSpeed)
    {
        snowflakeCurrentFrame++;
        snowflakeFrameTimer = 0;
    }
    if (snowflakeCurrentFrame >= snowflakeMaxframes)
        snowflakeCurrentFrame = 0;
}

void DrawTitleScreen()
{
    DrawTexture(titleBackground, 0, 0, WHITE);
    DrawTexture(titleTexture, (GetScreenWidth()/2) - title_title_width/2, 60, WHITE);

    Vector2 hexCenter = {(float)GetScreenWidth() /2 -150, (float)GetScreenHeight()/2 + 60};

   // DrawPolyLinesEx(hexCenter,
    // 6, 125, 0, 5 , {125,0,0,255});
    DrawPolyLinesEx(hexCenter,
     3 , 100, -90, 5, {125,0,0,255});

    DrawPolyLinesEx(hexCenter,
     6 , 25, 0, 4, {125,0,0,255});
    DrawPolyLinesEx(hexCenter,
     6 , 19, 0, 4, {1,93,1,255});
    DrawPolyLinesEx(hexCenter,
     6 , 13, 0, 4, {105,190,210,255});
    DrawCircle(hexCenter.x,hexCenter.y, 5, {125,0,0,255});
     //DrawTriangleLines(fireTris[0], fireTris[1], fireTris[2], RED);
    //DrawTriangleLines(poisonTris[0], poisonTris[1], poisonTris[2], GREEN);
    //DrawTriangleLines(freezeTris[0], freezeTris[1], freezeTris[2], BLUE);


    // these title drawing parts are horrible because i did these in the last days 
    // but i will comeback and fix these later

    Rectangle fireSourceRect = {float(fireCurrentFrame * title_fire_width),0, title_fire_width, title_fire_height};
    Rectangle fireDestRect = {(float)GetScreenWidth() /2 -150 - float(title_fire_width * 2.5) / 2,
         (float)GetScreenHeight()/2 - float(title_fire_height * 2.5),title_fire_width * 2.5, title_fire_height * 2.5};
    // there is no enough time so im not gonna experiment with locations of these textures on screen
    DrawTexturePro(fireTexture, fireSourceRect, fireDestRect, {0,0}, 0.f, WHITE);

    Rectangle poisonSmokeSourceRect = {float(poisonSmokeCurrentFrame * title_poison_smoke_width),0,
    title_poison_smoke_width, title_poison_smoke_height};
    DrawTexture(poisonTexture, hexCenter.x - int(title_poison_width_height/2) - 60,
    hexCenter.y - int(title_poison_width_height/2) + 30, WHITE);
    DrawTextureRec(poisonSmokeTexture, poisonSmokeSourceRect,
        {hexCenter.x - int(title_poison_smoke_width/2) - 60, hexCenter.y - int(title_poison_smoke_height/2) - 15}, GREEN);

    
    Rectangle snowflakeRect = { float(snowflakeCurrentFrame * title_snowflake_width), 0,
        title_snowflake_width, title_snowflake_height};
    DrawTextureRec(snowflakeTexture, snowflakeRect, 
        {hexCenter.x + int(title_snowflake_width/2) ,
            hexCenter.y + int(title_snowflake_height/2) - 30},
             {60,180,255,255});


    DrawText("Developed by Sobhan Azari Asl", 15, GetScreenHeight() - 20, 14, GRAY);
}

void UnloadTitleScreen()
{
    UnloadTexture(titleTexture);
    UnloadTexture(titleBackground);
    UnloadTexture(fireTexture);
    UnloadTexture(snowflakeTexture);
    UnloadTexture(poisonTexture);
    UnloadTexture(poisonSmokeTexture);
}

int FinishTitleScreen()
{
    return bIsScreenFinished;
}