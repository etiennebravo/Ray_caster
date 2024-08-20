#include <iostream>
#include <cmath>
#include <cassert>
#include "player.h"
#include "map.h"

Player::Player() : position{3,2}, angle(0), speed(0) {}

bool Player::get_key_state(const size_t i) const 
{
    assert(i < keys_collected.size());
    return keys_collected[i];
}

void Player::set_key_state(const size_t i, const bool state)
{
    assert(i < keys_collected.size());
    keys_collected[i] = state;
}


void Player::input(bool &quit, const Uint8* state, SDL_Event &event, const Map map)
{
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    float cosPerpendicularAngle = cos(angle - (M_PI / 2));
    float sinPerpendicularAngle = sin(angle - (M_PI / 2));

    float newX = 0;
    float newY = 0;

    //////// Continuous response keys /////////
    
    // Movement forwards / backwards
    if (state[SDL_SCANCODE_W])
    {
        speed = 0.1;
    } 
    else if (state[SDL_SCANCODE_S])
    {
        speed = -0.1;
    } 
    else 
    {
        speed = 0;
    }

    // Calculate new position
    newX = position[0] + speed * cosAngle;
    newY = position[1] + speed * sinAngle;

    // Check boundaries
    if (newX > 1 && newX < 512 && newY > 1 && newY < 1024 && map.is_empty(static_cast<int>(newX), static_cast<int>(newY)))
    {
        position[0] = newX;
        position[1] = newY;
    }

    // Movement left / right
    if (state[SDL_SCANCODE_A])
    {
        speed = 0.1;
    }
    else if (state[SDL_SCANCODE_D])
    {
        speed = -0.1;
    }
    else
    {
        speed = 0;
    }

    // Calculate new position
    newX = position[0] + speed * cosPerpendicularAngle;
    newY = position[1] + speed * sinPerpendicularAngle;

    // Check boundaries
    if (newX > 1 && newX < 512 && newY > 1 && newY < 1024 && map.is_empty(static_cast<int>(newX), static_cast<int>(newY)))
    {
        position[0] = newX;
        position[1] = newY;
    }

    // Camera left / right
    if (state[SDL_SCANCODE_LEFT])
    {
        angle -= 0.07;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        angle += 0.07;
    }

    // Only values from -PI to PI radians
    if (angle > M_PI) { angle -= 2 * M_PI; }
    if (angle < M_PI) { angle += 2 * M_PI; }

    //////// Single key hit, mouse or other events /////////
    while (SDL_PollEvent(&event))
    {
        // Window close
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
    }
}