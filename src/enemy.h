#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include <raymath.h>


// all of these codes are experimental
class Enemy
{
    
 public:

    Enemy(Vector2 startPoint, Vector2 targetPoint,float spawnSpeed,  Texture2D _texture) 
    {
         remainingHP = 750;
      target = targetPoint;
      start = startPoint; // just in case;
      position = start;
      speed = spawnSpeed;

      texture = _texture;
      bIsActive = true;


      InitializeEnemy();
    };
    ~Enemy() {};

    void SetMaxHP(const float& Amount);
    inline float GetHP() const { return remainingHP; }

    void SetTarget(const Vector2& playerPosition);

    void InitializeEnemy();
    void UpdateEnemy();
    void DrawEnemy();
    void UnloadEnemy();
   bool IsDead() { if (remainingHP <= 0) return true; return false;}

    void TakeDamage(Color magicalColor);

   Rectangle GetRect();


    bool bIsActive;
 private:

    bool bIsFrozen = false;
    bool bIsOnFire = false;
    bool bIsPoisoned = false;

    Vector2 position;
    Texture2D texture;
    float remainingHP;
    Vector2 target;
    Vector2 start;
    float speed;
    Vector2 direction;

    bool bIsFacingRight = true;

    float baseDamageToTake = 25;
    
};

#endif /* ENEMY_H */
