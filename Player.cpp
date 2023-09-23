#include "Player.h"

Player::Player(int x, int y) {
    position.x = x;
    position.y = y;
    rect.w = 50;
    rect.h = 50;
}

void Player::Move(int x, int y) {
    // Store the previous movement direction before updating the position
    previousDirection.x = x;
    previousDirection.y = y;

    position.x += x;
    position.y += y;
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
}

void Player::Power(int powerFactor) {
    // Apply a velocity in the previous direction
    position.x += previousDirection.x * powerFactor;
    position.y += previousDirection.y * powerFactor;
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
}
