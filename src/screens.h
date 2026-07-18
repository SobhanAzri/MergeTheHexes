/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2024 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef SCREENS_H
#define SCREENS_H

#include <raylib.h>

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
enum EGameScreen 
{   
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    ENDING
};

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern EGameScreen currentScreen;
extern Font font;
extern Font font2;
extern Music fireSound;
extern Sound clickSounds[2];
extern Sound errorSounds[2];
extern Sound successSounds[2];
extern Texture2D HexCurses[3];


bool Button(const Rectangle& bounds, const char* text, const float& fontSize, const Color& color);

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen();
void UpdateLogoScreen();
void DrawLogoScreen();
void UnloadLogoScreen();
int FinishLogoScreen();

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen();
void UpdateTitleScreen();
void DrawTitleScreen();
void UnloadTitleScreen();
int FinishTitleScreen();

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen();
void UpdateGameplayScreen();
void DrawGameplayScreen();
void UnloadGameplayScreen();
int FinishGameplayScreen();

#endif // SCREENS_H