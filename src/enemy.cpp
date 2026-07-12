#include "enemy.h"
#include "assets.h"
#include <raylib.h>
#include <raymath.h>

void Enemy::SetMaxHP(const float& Amount)
{
    remainingHP = Amount;
}

void Enemy::SetTarget(const Vector2& playerPosition)
{
    target = playerPosition;
}

void Enemy::InitializeEnemy()
{

    direction = Vector2Normalize(Vector2Subtract(target, position));
    baseDamageToTake = 25;

    if (position.x > GetScreenWidth()/2)
    {
        bIsFacingRight = false;
    }
    else
        bIsFacingRight = true;

}

void Enemy::UpdateEnemy()
{
    if ((!bIsActive)) return;

    baseDamageToTake += 0.005;


    position.x += direction.x * speed * GetFrameTime();
    position.y += direction.y * speed * GetFrameTime();

    if (bIsOnFire)
        remainingHP -= 10.f * GetFrameTime();
    if (bIsPoisoned)
        remainingHP -=  5.f * GetFrameTime() ;
}

void Enemy::DrawEnemy()
{
    if (!bIsActive) return;

    Rectangle sourceRect = { 0.0f, 0.0f, gameplay_enemy_width, gameplay_enemy_height };
    if (!bIsFacingRight)
        sourceRect.width *= -1;

    Rectangle destRect = { position.x, position.y, gameplay_enemy_width, gameplay_enemy_height };
    Vector2 origin = { gameplay_enemy_width / 2.0f, gameplay_enemy_height / 2.0f };

    DrawTexturePro(texture, sourceRect, destRect, origin, 0.0f, WHITE);

    DrawRectangle(position.x - (gameplay_enemy_width / 2.0f), position.y - (gameplay_enemy_height / 2.0f) - 10,
     (remainingHP / 750.0f) * gameplay_enemy_width, 5, GREEN);
}

void Enemy::UnloadEnemy()
{

}

Rectangle Enemy::GetRect() {
    return (Rectangle){ position.x - (gameplay_enemy_width / 2.0f), position.y - (gameplay_enemy_height / 2.0f),
         gameplay_enemy_width, gameplay_enemy_height };
}

void Enemy::TakeDamage(Color magicalColor)
{
    float fireDamage = magicalColor.r / 255.f;
    float poisonDamage = magicalColor.g / 255.f;
    float freezeDamage = magicalColor.b  / 255.f;

    int damage = baseDamageToTake + int(fireDamage * 130) + int(poisonDamage * 110) + int(freezeDamage * 100);

    // i multiplied them so it can become much fair to play
    remainingHP -= damage;

    if (fireDamage > 0)
        bIsOnFire = true;
    if (poisonDamage > 0)
        bIsPoisoned = true;
    if (freezeDamage > 0)
    {
        speed = speed * .75;
        bIsFrozen = true;
    }
}