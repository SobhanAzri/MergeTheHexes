#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

class Player {

 public:

    Player() {};
    ~Player() {};

    void InitPlayer();
    void UpdatePlayer();
    void DrawPlayer();
    void UnloadPlayer();

    inline Vector2 GetPosition() const { return position; }
    inline float GetAttackInterval() const { return AttackInterval; }

    int maxLife = 5;
    int frameCounter = 0;
    int currentFrame = 0;
    float frameTimer = 0;
    int maxFrames = 6;
    float frameSpeed = 0.1f;


private:


    float scaledWidth;
    float scaledHeight;

    Vector2 position;
    Texture2D playerTexture;
    Texture2D playerLight;
    Rectangle playerRect = { 0 };
    int remainingLife;
    Vector2 aimDirection;

    float AttackInterval = .1f;
    
};

#endif /* PLAYER_H */
