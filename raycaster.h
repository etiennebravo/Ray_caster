#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "utils.h"
#include "map.h"
#include "player.h"
#include "sprite.h"
#include "textures.h"

constexpr float fov = 60 * M_PI / 180.0;  // field of view

class raycaster
{
    private:
        /////////////////////////////// Variables /////////////////////////////////////////

        size_t w;                   // Width of screen
        size_t h;                   // Height of screen
        std::vector<uint32_t> img;  // Vector container

        //////////////////////////// Private Functions ////////////////////////////////////

        /// @brief Finds the wall texture hit by a ray cast
        /// @param hitx x coordinate
        /// @param hity y coordinate
        /// @param tex_walls texture of the walls
        /// @return texture x coordinate
        int wall_x_texcoord(const float hitx, const float hity, Texture &tex_walls);

        /// @brief shows enemies as red dots in the map
        /// @param sprite sprite struct
        /// @param map where the dots are going to be drawn
        /// @param size size of the red dot
        void map_show_entity_sprite(Sprite &sprite, Map &map, size_t size);

        /// @brief Shows the player as blue dot in the map
        /// @param player player object
        /// @param map where the dot is going to be drawn
        /// @param size size of the blue dot
        void map_show_player_sprite(Player &player, Map &map, size_t size);

        /// @brief Shows sprite in the 3d view
        /// @param sprite sprite struct
        /// @param depth array that holds depth, needs to be sorted
        /// @param player player object
        /// @param tex_sprites texture to be used
        void draw_sprite(Sprite &sprite, std::vector<float> depth, Player &player, Texture &tex_sprites);

        /// @brief Shows entitites (e.g. monsters) in game and map
        /// @param player player object
        /// @param map map object for layout
        /// @param sprites sprite structs
        /// @param depth_buffer Provides distances to prevent overlapping of sprites
        /// @param tex Texture object containing sprites
        void show_entities_game_and_map(Player &player, Map &map, std::vector<Sprite> &sprites, std::vector<float> &depth_buffer, Texture &tex);

        /// @brief Shows entities only in game
        /// @param sprites Sprites to be shown
        /// @param depth_buffer Provides distances to prevent overlapping of sprites
        /// @param player player object
        /// @param tex_keys Texture object containing key sprites
        void show_keys_game(std::vector<Sprite> &sprites, std::vector<float> &depth_buffer, Player &player, Texture &tex_keys);

        /// @brief Check if a key was collected
        /// @param player player object
        /// @param map map layout
        void check_key_collect(Player &player, Map &map);

        /// @brief Draw the map in the screen
        /// @param map map layout
        /// @param rect_w width of one map cell on screen
        /// @param rect_h height of one map cell on screen
        /// @param tex_walls Textures for walls
        void draw_map(Map &map, const size_t rect_w, const size_t rect_h, Texture &tex_walls);

        /// @brief Sets image to a solid color
        /// @param color Packed color e.g. pack_color(255,255,255)
        void clear(const uint32_t color);

        /// @brief Sets pixel to a certain color
        /// @param x X coordinate
        /// @param y Y coordinate
        /// @param color Packed color e.g. pack_color(255,255,255)
        void set_pixel(const size_t x, const size_t y, const uint32_t color);

        /// @brief Draws a flat color or texture in a specified area of the screen
        /// @param x X coordinate of the texture
        /// @param y Y coordinate of the texture
        /// @param w Width of the texture
        /// @param h Height of the texture
        /// @param color Color that is going to be drawn
        void draw_rectangle(const size_t x, const size_t y, const size_t w, const size_t h, const uint32_t color);

    public:
        /// @brief constructor, creates object with default values
        raycaster();

        ///////////////////////////////////////////////// Accessors //////////////////////////////////////////////////

        inline size_t get_width() const { return w; };

        inline size_t get_height() const { return h; };

        inline std::vector<uint32_t> get_img() const { return img; };

        ///////////////////////////////////////////// Public Functions ///////////////////////////////////////////////

        /// @brief Creates the image that is going to be shown on the screen
        /// @param map Gives the layout for the map
        /// @param player This determines the position and angle for the raycaster
        /// @param sprites Sprite objects shown
        /// @param tex_walls Textures for walls
        /// @param tex_monst Textures for monsters
        void render(Map &map, Player &player, Texture &tex_walls, Texture &tex_monst, Texture &tex_keys);
};

#endif // TINYRAYCASTER_H