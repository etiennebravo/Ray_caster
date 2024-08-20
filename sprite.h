#ifndef SPRITE_H
#define SPRITE_H

#include <cstdint>

struct Sprite
{
    float x, y;           // position
    size_t texture_id;    // ID
    float player_dist;    // Distance to the player

    float get_sprite_x() { return x; };
    float get_sprite_y() { return y; };
    size_t get_tex_id() { return texture_id; };

    /// @brief < Operator overload
    /// @param s Sprite struct 
    /// @return True if sprite is closer to player, false if its farther
    /// @note Compares float values
    inline bool operator < (const Sprite &s) const { return player_dist > s.player_dist;} ;
};


#endif  // SPRITE_H