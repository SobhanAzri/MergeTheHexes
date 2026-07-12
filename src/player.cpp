#include "player.h"
#include <cstdio>
#include <raylib.h>
#include "assets.h"
#include "magic.h"
#include <raymath.h>

void Player::InitPlayer()
{

    scaledWidth = gameplay_player_width * 2;
    scaledHeight = gameplay_player_height * 2;

    remainingLife = 3;
    AttackInterval = 0;

    frameCounter = 0;
    currentFrame = 0;
    frameTimer = 0;
    frameSpeed = 0.35f;

    shootSound[0] = LoadSound("resources/shoot01.wav");
    SetSoundVolume(shootSound[0], .65);
    shootSound[1] = LoadSound("resources/shoot02.wav");
    SetSoundVolume(shootSound[1], .65);

    heartTexture = LoadTexture("resources/heart.png");
    playerTexture = LoadTexture("resources/player.png");
    playerLight = LoadTexture("resources/playerLight.png");
    projectileTexture = LoadTexture("resources/projectile.png");

    position.x = (float(GetScreenWidth()) / 2); //i added offset by 5 , now its not centered but i f* upped the map texture so this will look better;
    position.y = (float(GetScreenHeight()) /2) + (scaledHeight * 2) - (float)scaledHeight/2 -20; // 20 is an offset;
}

void Player::UpdatePlayer()
{
    frameCounter++;
    frameTimer += GetFrameTime();
    //Vector2 mousePosition = GetMousePosition(); // i know i used this in couple of update functions so i will organize it later

    if (AttackInterval > 0)
        AttackInterval -= frameTimer;

    if (frameTimer >= frameSpeed)
    {
        currentFrame++;
        frameTimer = 0;
    }

    if (currentFrame >= maxFrames)
        currentFrame = 0;

    
    for (MagicBall& ball : magicBalls)
    {
        // i just update the location of balls and disable them when they get off screen

        if (ball.bIsActive)
        {
            ball.position = Vector2Add(ball.position, ball.velocity);
        }

        if (ball.position.x + gameplay_projectile_width_height/4 < 0 ||
             ball.position.x - gameplay_projectile_width_height/4 > GetScreenWidth() ||
              ball.position.y - gameplay_projectile_width_height/4> GetScreenHeight())
        {
            ball.bIsActive = false;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) //&& mousePosition.y > GetScreenHeight()/2)
    {
        Vector2 mousePosition = GetMousePosition();

        if (mousePosition.y > GetScreenHeight()/2 && AttackInterval <= 0)
            Shoot(mousePosition);
    }

}

void Player::DrawPlayer()
{

    playerRect = (Rectangle){ float(currentFrame * gameplay_player_width), 0, gameplay_player_width, gameplay_player_height };
    Rectangle dest = (Rectangle){ position.x, position.y, scaledWidth, scaledHeight };
    //DrawTextureRec(playerTexture, playerRect, position, WHITE);
    DrawTexturePro(playerTexture, playerRect, dest, {scaledWidth/2,scaledHeight/2}, .0f, WHITE);

    // the png itself has some problems in itself so positioning is wierd
    DrawTexture(playerLight, position.x - playerLight.width/2 + 25,
     position.y - playerLight.height/2 + 25 , Fade(PURPLE, .11f));

    for (MagicBall& ball : magicBalls)
    {
        // i just update the location of balls and disable them when they get off screen
        if (!ball.bIsActive) continue;;

        Rectangle sourceRect = {0,0, gameplay_projectile_width_height, gameplay_projectile_width_height};
        Rectangle destRect = {ball.position.x, ball.position.y, gameplay_projectile_width_height/2,
        gameplay_projectile_width_height/2};
        DrawTexturePro(projectileTexture, sourceRect, destRect,
             {gameplay_projectile_width_height/4,gameplay_projectile_width_height/4},
              0, MagicBar::Get().GetMagicColor());
    }

    Rectangle source = {0,0,16,16};
    for (int i = 0; i < remainingLife; i++)
        DrawTextureRec(heartTexture, source, {float(GetScreenWidth()/2 - 20 + (i * 17)), float(position.y - 40)}, WHITE);
}

void Player::UnloadPlayer()
{
    UnloadTexture(playerLight);
    UnloadTexture(playerTexture);
    UnloadTexture(projectileTexture);
    UnloadSound(shootSound[1]);
    UnloadSound(shootSound[0]);
    UnloadTexture(heartTexture);
    magicBalls.clear();
}

void Player::Shoot(const Vector2& targetPosition)
{
    MagicBall ball;

    ball.position = (Vector2){position.x, position.y + gameplay_player_height + 5}; // so it doesnt spawn from middle of the player
    ball.color = MagicBar::Get().GetMagicColor();
    ball.bIsActive = true;

    Vector2 direction = Vector2Normalize(Vector2Subtract(targetPosition, ball.position));
    ball.velocity = Vector2Scale(direction, 10.f);

    magicBalls.push_back(ball);
    AttackInterval = 2.5;

    PlaySound(shootSound[rand() % 2]);
}

void Player::TakeDamage()
{
    remainingLife -= 1;
}
bool Player::bIsDead()
{
    if (remainingLife <= 0)
        return true;
    else
        return false;
}