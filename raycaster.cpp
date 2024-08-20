#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <iomanip>

#include "raycaster.h"
#include "map.h"
#include "utils.h"
#include "player.h"
#include "sprite.h"
#include "textures.h"

std::vector<Sprite> monster_sprites{{3.523, 3.812, 2, 0}, {1.834, 8.765, 0, 0}, {5, 5, 1, 0}};
std::vector<Sprite> key_sprites{{14, 3, 2, 0}};
raycaster::raycaster() : w(1024), h (512), img (std::vector<uint32_t>(1024 * 512, pack_color(255, 255, 255, 255))) {}

int raycaster::wall_x_texcoord(const float hitx, const float hity, Texture &tex_walls)
{
    float x = hitx - floor(hitx + 0.5);  // x and y contain (signed) fractional parts of hitx and hity.
    float y = hity - floor(hity + 0.5);  // they vary between -0.5 and +0.5, and one of them is supposed to be very close to 0
    int tex = 0;
    
    if (std::abs(y) > std::abs(x))  // we need to determine whether we hit a "vertical" or a "horizontal" wall in the map
    {
        tex = y * tex_walls.size;
    } 
    else 
    {
        tex = x * tex_walls.size;
    }

    if (tex < 0)  // x_texcoord can be negative
    {
        tex += tex_walls.size;
    }
    assert(tex >= 0 && tex < (int)tex_walls.size);

    return tex;
}

void raycaster::set_pixel(const size_t x, const size_t y, const uint32_t color)
{
    assert(img.size() == w * h && x < w && y < h);
    img[x + y * w] = color;
}

void raycaster::draw_rectangle(const size_t rect_x, const size_t rect_y, const size_t rect_w, const size_t rect_h, const uint32_t color)
{
    assert(img.size() == w * h);  // assert correct image size
    for (size_t i = 0; i < rect_w; i++)
    {
        for (size_t j = 0; j < rect_h; j++)
        {
            size_t coordx = rect_x + i;
            size_t coordy = rect_y + j;
            if (coordx < w && coordy < h)
            {
                set_pixel(coordx, coordy, color);  // prevent object from going out of bounds
            }
            
        }
    }
}

void raycaster::clear(const uint32_t color)
{
    img = std::vector<uint32_t>(w * h, color);
}

void raycaster::map_show_entity_sprite(Sprite &sprite, Map &map, size_t size)
{
    const size_t rect_w = get_width() / (map.get_width() * 2);  // size of one map cell on the screen
    const size_t rect_h = get_height() / map.get_height();
    draw_rectangle(sprite.x * rect_w - (size / 2), sprite.y * rect_h - (size / 2), size, size, pack_color(255, 0, 0));
}

void raycaster::map_show_player_sprite(Player &player, Map &map, size_t size)
{
    const size_t rect_w = get_width() / (map.get_width() * 2);  // size of one map cell on the screen
    const size_t rect_h = get_height() / map.get_height();
    draw_rectangle(player.get_x() * rect_w - (size / 2), player.get_y() * rect_h - (size / 2), size, size, pack_color(0, 0, 255));
}

void raycaster::draw_sprite(Sprite &sprite, std::vector<float> depth, Player &player, Texture &tex_sprites)
{
    // Absolute direction from player to sprite in radians
    float sprite_direction = atan2(sprite.y - player.get_y(), sprite.x - player.get_x());

    // remove unncesessary periods from the relative direction
    while (sprite_direction - player.get_angle() >  M_PI) sprite_direction -= 2*M_PI; 
    while (sprite_direction - player.get_angle() < -M_PI) sprite_direction += 2*M_PI;
    
    // Distance from player to sprite
    float sprite_distance = sqrt(pow(player.get_x() - sprite.x, 2) + pow(player.get_y() - sprite.y, 2));

    // Sprite size
    size_t sprite_size = std::min(1000, static_cast<int>(get_height() / sprite_distance));  // (Max sprite size = 1000)

    // Find top left corner of sprite
    int horizontal_offset = (sprite_direction - player.get_angle()) * (get_width() / 2) + (get_width() / 2) / 2 - tex_sprites.size / 2;
    int vertical_offset = get_height() / 2 - sprite_size / 2;

    // Draw sprite
    for (size_t i = 0; i < sprite_size; i++)
    {
        if (horizontal_offset + static_cast<int>(i) < 0 || horizontal_offset + i >= get_width() / 2) { continue; }  // horizontal bounds
        if (depth[horizontal_offset + i] < sprite.player_dist) { continue; }  // Check is sprite is behind something
        for (size_t j = 0; j < sprite_size; j++)
        {
            if (vertical_offset + static_cast<int>(j) < 0 || vertical_offset + j >= get_height()) { continue; }  // vertical bounds

            // Get colors from png while ignoring transparent pixels
            uint32_t color = tex_sprites.get(i * tex_sprites.size / sprite_size, j * tex_sprites.size / sprite_size, sprite.texture_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a > 128)
            {
                set_pixel(get_width() / 2 + horizontal_offset + i, vertical_offset + j, color);
            }
        }
    }
}

void raycaster::render(Map &map, Player &player, Texture &tex_walls, Texture &tex_monst, Texture &tex_keys)
{
    clear(pack_color(255, 255, 255));  // clear the screen
    draw_rectangle(get_width() / 2, get_height() / 2, get_width(), get_height() / 2, pack_color(128,128,128));  // Floor

    // red sky
    size_t red = 32;
    for (size_t i = 0; i < get_height() / 2; i += get_height() / 16)
    {
        draw_rectangle(get_width() / 2, i, get_width(), get_height() / 16, pack_color(red,0,0));
        red += 8;
    }

    // size of one map cell on the screen
    const size_t rect_w = get_width() / (map.get_width() * 2); 
    const size_t rect_h = get_height() / map.get_height();

    // draw the map
    draw_map(map, rect_w, rect_h, tex_walls);

    std::vector<float> depth_buffer(get_width() / 2, 1000);
    // draw field of view
    for (size_t i = 0; i < get_width() / 2; i++)
    {
        float angle = player.get_angle() - (fov / 2) + fov * (i / float(get_width() / 2));
        // ray marching loop
        for (float t = 0; t < 20; t += .01)
        { 
            float x = player.get_x() + t * cos(angle);
            float y = player.get_y() + t * sin(angle);

            set_pixel(x * rect_w, y * rect_h, pack_color(160, 160, 160));  // This draws the visibility cone

            if (map.is_empty(x, y))
            {
                continue;  // Check for collisions 
            }

            float dist = t * cos(angle - player.get_angle());                            // Determine distance between collision and player
            depth_buffer[i] = dist;                                                      // Record depth of collision
            
            int x_texcoord = wall_x_texcoord(x, y, tex_walls);                           // Find x coordinate of collision in wall
            size_t texid = map.get(x, y);                                                // Identify what kind of wall was hit
            size_t column_height = std::min(2000, static_cast<int>(get_height()/dist));  // column with cosine correction
            std::vector<uint32_t> column = tex_walls.get_scaled_column(texid, x_texcoord, column_height);

            int pix_x = i + get_width() / 2; // we are drawing at the right half of the screen, thus +fb.w/2

            // copy the texture column to the frame
            for (size_t j = 0; j < column_height; j++) 
            { 
                int pix_y = j + get_height() / 2 - column_height / 2;
                if (pix_y >= 0 && pix_y < (int)get_height())
                {
                    set_pixel(pix_x, pix_y, column[j]);
                }
            }
            break;
        }  // ray marching loop
    }  // field of view ray sweeping

    show_entities_game_and_map(player, map, monster_sprites, depth_buffer, tex_monst);

    check_key_collect(player, map);

    show_keys_game(key_sprites, depth_buffer, player, tex_keys);

} // end of render()

void raycaster::check_key_collect(Player &player, Map &map)
{
    float key_size = 0.5;

    for (size_t i = 0; i < key_sprites.size(); i++)
    {
        if (!player.get_key_state(i) &&
            player.get_x() < key_sprites[i].get_sprite_x() + key_size &&
            player.get_x() + key_size > key_sprites[i].get_sprite_x() &&
            player.get_y() < key_sprites[i].get_sprite_y() + key_size &&
            player.get_y() + key_size > key_sprites[i].get_sprite_y()) 
        {
            player.set_key_state(i, true);
            map.open_door();
        }
    }
}

void raycaster::show_keys_game(std::vector<Sprite> &sprites, std::vector<float> &depth_buffer, Player &player, Texture &tex)
{
    // calculate distance
    for (size_t i = 0; i < sprites.size(); i++)
    {
        sprites[i].player_dist = sqrt(pow(player.get_x() - sprites[i].x, 2) + pow(player.get_y() - sprites[i].y, 2));
    }

    // draw keys that are not collected
    for (size_t i = 0; i < sprites.size(); i++)
    {
        if (player.get_key_state(i) == false)
        {
          draw_sprite(sprites[i], depth_buffer, player, tex);  
        }
    }
}

void raycaster::show_entities_game_and_map(Player &player, Map &map, std::vector<Sprite> &sprites, std::vector<float> &depth_buffer, Texture &tex)
{
    // show squares in map
    int dot_size = 6;

    map_show_player_sprite(player, map, dot_size);

    for (size_t i = 0; i < sprites.size(); i++)
    {
        sprites[i].player_dist = sqrt(pow(player.get_x() - sprites[i].x, 2) + pow(player.get_y() - sprites[i].y, 2));
    }

    std::sort(sprites.begin(), sprites.end());

    for (size_t i = 0; i < sprites.size(); i++)
    {
        map_show_entity_sprite(sprites[i], map, dot_size);
        draw_sprite(sprites[i], depth_buffer, player, tex);
    }
}

void raycaster::draw_map(Map &map, const size_t rect_w, const size_t rect_h, Texture &tex_walls)
{
    for (size_t j = 0; j < map.get_height(); j++)
    {
        for (size_t i = 0; i < map.get_width(); i++)
        {
            if (map.is_empty(i, j))
            {
                continue; // skip empty spaces
            }
            size_t rect_x = i * rect_w;
            size_t rect_y = j * rect_h;
            size_t texid = map.get(i, j);
            draw_rectangle(rect_x, rect_y, rect_w, rect_h, tex_walls.get(0, 0, texid)); // the color is taken from the upper left pixel of the texture
        }
    }
}
