#ifndef PLAYER_H
#define PLAYER_H

#include "SDL2/SDL.h"
#include "holder.h" // Include any necessary headers
#include "Math.h"   // Include any necessary headers

class Player {
public:
    Player(int x, int y);
    void Move(int x, int y);
    void Power(int powerFactor);

public:
    Vector2 position;
    SDL_Rect rect;
    Vector2 previousDirection;
};

#endif // PLAYER_H
