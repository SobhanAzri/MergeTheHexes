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


/*******************************************************************************************
*   This code is so messy and has so much problems that im kinda embarrassed to show it to
*   someone. but im fully aware of the problems and i will fix everything
*******************************************************************************************/



#include "assets.h"
#include "magic.h"
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
Font font = {0 };
Font font2 = { 0 };
static Texture2D cursorTexture;
static Texture2D cursorLightTexture;
static Vector2 cursorPosition = {0, 0 };


Sound clickSounds[] = { 0 };
Sound errorSounds[] = { 0 };
Sound successSounds[] = { 0 };
Music fireSound = { 0 };

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



    clickSounds[0] = LoadSound("resources/interfaceAudio/click01.ogg");
    clickSounds[1] = LoadSound("resources/interfaceAudio/click02.ogg");
    errorSounds[0] = LoadSound("resources/interfaceAudio/error01.ogg");
    errorSounds[1] = LoadSound("resources/interfaceAudio/error02.ogg");
    successSounds[0] = LoadSound("resources/interfaceAudio/success01.ogg");
    successSounds[1] = LoadSound("resources/interfaceAudio/success02.ogg");



    fireSound = LoadMusicStream("resources/title/title_fire.wav");

    cursorTexture = LoadTexture("resources/cursor.png");
    cursorLightTexture = LoadTexture("resources/cursor_light.png");
    currentScreen = EGameScreen::LOGO;

    font = LoadFont("resources/MountainKing.ttf");
    font2 = LoadFont("resources/font2.png");
    SetTextureFilter(font2.texture, TEXTURE_FILTER_BILINEAR);

    HideCursor();
    InitLogoScreen();

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

    UnloadFont(font);
    UnloadFont(font2);
    UnloadMusicStream(fireSound);
    for (int i = 0; i < 2; i++)
    {
        UnloadSound(clickSounds[i]);
        UnloadSound(errorSounds[i]);
        UnloadSound(successSounds[i]);
    }
    UnloadTexture(cursorTexture);
    UnloadTexture(cursorLightTexture);

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
                    TransitionToScreen(GAMEPLAY);
                }

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                if (FinishGameplayScreen() == 1) ChangeToScreen(TITLE);

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
             cursorPosition.y - global_cursor_width_height/4, Fade(RAYWHITE, .7f));
        DrawTexture(cursorLightTexture, cursorPosition.x - global_cursor_light_width_height/2 + 5,
             cursorPosition.y - global_cursor_light_width_height/2 + 5, Fade(RAYWHITE, .7f));
        

    EndDrawing();
    //----------------------------------------------------------------------------------  
}


bool Button(const Rectangle& bounds, const char* text, const float& fontSize, const Color& color)
{
    bool bIsPressed = false;
    bool bIsFocused = false;

    Vector2 textSize = MeasureTextEx(font, text, fontSize, 1);

    Vector2 mousePosition = GetMousePosition(); 

    if ( CheckCollisionPointRec(mousePosition, bounds))
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            PlaySound(clickSounds[rand() % 2]);
            return true;
        }
        else
            bIsFocused = true;
    }
    else
    {
        bIsFocused = false;
    }

    float buttonOpacity = 255;
    if (bIsFocused)
    {
            buttonOpacity = .75f;
    }
    else
    {
            buttonOpacity = 1;
    }

    DrawRectangleRoundedLinesEx(bounds, .5, 5,2, BLACK);
    DrawRectangleRounded(bounds, .5, 5, Fade(color, buttonOpacity));
    DrawTextEx(font, text, 
        {bounds.x + bounds.width/2 - textSize.x/2 - 4, bounds.y + bounds.height/2 - textSize.y/2 - 3},
             30, 1,Fade(BLACK, buttonOpacity));


    return bIsPressed;
}