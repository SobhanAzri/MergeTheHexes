/*******************************************************************************************
*
*   raylib gamejam template
*
*   Code licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2026 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "assets.h"
#include "raylib.h"
#include "screens.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>      // Emscripten library
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for:

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 720;
static const int screenHeight = 720;
static const int screenDivider = 360;

//static RenderTexture2D target = { 0 };  // Render texture to render our game
static int frameCounter = 0;


// some variables for game screens
static float transAlpha = 0;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

// TODO: Define global variables here, recommended to make them static

EGameScreen currentScreen = EGameScreen::LOGO;
static Texture2D cursorTexture;
static Vector2 cursorPosition = {0, 0};

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(const EGameScreen& screen);

static void TransitionToScreen(int screen);
static void UpdateTransition();
static void DrawTransition();

static void UpdateDrawFrame(void);      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Merge the Hexes");
    
    InitAudioDevice();

    // TODO: Load resources / Initialize variables at this point
    
    // Render texture to draw, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    //target = LoadRenderTexture(screenWidth, screenHeight);
    //SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);


    cursorTexture = LoadTexture("resources/cursor.png");
    currentScreen = EGameScreen::LOGO;

    HideCursor();
    //InitGameplayScreen();
    InitLogoScreen();
    //InitLogoScreen();
    //InitTitleScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------

    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        default: break;
    }
    
    // TODO: Unload all loaded resources at this point

    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------

static void ChangeToScreen(const EGameScreen& screen)
{
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        default: break;
    }

    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

static void TransitionToScreen(int screen)
{
    onTransition = true;
    transFromScreen = currentScreen;
    transToScreen = screen;
}

static void UpdateTransition()
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        if (transAlpha >= 1.0)
        {
            transAlpha = 1.0;

            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                default: break;
            }

            switch (transToScreen)
            {
                case LOGO:
                {
                    InitLogoScreen();
                    currentScreen = LOGO;
                } break;
                case TITLE:
                {
                    InitTitleScreen();
                    currentScreen = TITLE;
                } break;
                case GAMEPLAY:
                {
                    InitGameplayScreen();
                    currentScreen = GAMEPLAY;
                } break;
                default: break;
            }

            transFadeOut = true;
        }
    }
    else 
    {
        transAlpha -= 0.05f;

        if (transAlpha <= 0)
        {
            transAlpha = 0;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

static void DrawTransition()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point
   
    frameCounter++;
    cursorPosition = GetMousePosition();

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture,

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();

                if (FinishLogoScreen()) TransitionToScreen(TITLE);

            } break;
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1)
                {
                    //StopMusicStream(music);
                    //TransitionToScreen(GAMEPLAY);
                }

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                if (FinishGameplayScreen() == 1) ChangeToScreen(LOGO);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

            } break;
            default: break;
        }
    }
    else
        UpdateTransition();
    
    // Render to screen
    BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            default: break;
        }
        
        DrawTexture(cursorTexture, cursorPosition.x - global_cursor_width_height/4,
             cursorPosition.y - global_cursor_width_height/4, Fade({255, 200, 200}, .75f));

    EndDrawing();
    //----------------------------------------------------------------------------------  
}
