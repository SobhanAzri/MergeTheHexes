#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>

class Enemy
{
    
 public:

    Enemy() {};
    ~Enemy() {};

    void SetMaxHP(const float& Amount);
    inline float GetHP() const { return remainingHP; }

    void SetTarget(const Vector2& playerPosition);

    void InitializeEnemy();
    void UpdateEnemy();
    void DrawEnemy();
    void UnloadEnemy();


 private:


    float MaxHP = 100;
    int remainingHP;
    Vector2 target;
    Vector2 velocity;
    
};

#endif /* ENEMY_H */
