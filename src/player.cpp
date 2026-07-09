#include "player.h"
#include <cstdio>
#include <raylib.h>
#include "assets.h"

void Player::InitPlayer()
{

    scaledWidth = gameplay_player_width * 1.5;
    scaledHeight = gameplay_player_height * 1.5;

    remainingLife = maxLife;

    frameCounter = 0;
    currentFrame = 0;
    frameTimer = 0;
    frameSpeed = 0.35f;
    
    playerTexture = LoadTexture("resources/player.png");
    playerLight = LoadTexture("resources/playerLight.png");

    position.x = (float(GetScreenWidth()) / 2) - scaledWidth/2 + 5; //i added offset by 5 , now its not centered but i f* upped the map texture so this will look better;
    position.y = (float(GetScreenHeight()) /2) + (scaledHeight * 2) - (float)scaledHeight/2 -20; // 20 is an offset;
}

void Player::UpdatePlayer()
{
    frameCounter++;
    frameTimer += GetFrameTime();

    if (frameTimer >= frameSpeed)
    {
        currentFrame++;
        frameTimer = 0;
    }

    if (currentFrame >= maxFrames)
        currentFrame = 0;

}

void Player::DrawPlayer()
{

    playerRect = (Rectangle){ float(currentFrame * gameplay_player_width), 0, gameplay_player_width, gameplay_player_height };
    Rectangle dest = (Rectangle){ position.x, position.y, scaledWidth, scaledHeight };
    //DrawTextureRec(playerTexture, playerRect, position, WHITE);
    DrawTexturePro(playerTexture, playerRect, dest, {0,0}, .0f, WHITE);

    // the png itself has some problems in itself so positioning is wierd
    DrawTexture(playerLight, position.x - playerLight.width/2 + 25,
     position.y - playerLight.height/2 + 25 , Fade(PURPLE, .11f));
}

void Player::UnloadPlayer()
{
    UnloadTexture(playerLight);
    UnloadTexture(playerTexture);
}