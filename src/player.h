#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "magic.h"
#include <vector>



class Player {

 public:

    Player() {};
    ~Player() {};

    void InitPlayer();
    void UpdatePlayer();
    void DrawPlayer();
    void UnloadPlayer();

    void Shoot(const Vector2& targetPosition);
    void TakeDamage();
    bool bIsDead();

    inline Vector2 GetPosition() const { return position; }
    inline std::vector<MagicBall> GetBalls() const { return magicBalls; }    

    std::vector<MagicBall> magicBalls;

    int frameCounter = 0;
    int currentFrame = 0;
    float frameTimer = 0;
    int maxFrames = 6;
    float frameSpeed = 0.1f;

    float scaledWidth;
    float scaledHeight;

private:



    Vector2 position;
    Texture2D playerTexture;
    Texture2D playerLight;
    Texture2D heartTexture;

    Sound shootSound[2] = {0 };
    // i had different enemies in my mind that threw projectiles so i thought maybe they should have their own projectile textures but i was wrong and time was liimited
    Texture2D projectileTexture; 

    Rectangle playerRect = { 0 };
    int remainingLife = 3;
    Vector2 aimDirection;

    float AttackInterval = .1f;
    
};

#endif /* PLAYER_H */
