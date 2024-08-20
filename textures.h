#ifndef TEXTURES_H
#define TEXTURES_H

#include <iostream>
#include <cstdint>
#include <vector>

struct Texture
{
    size_t img_w;                // Width
    size_t img_h;                // Height
    size_t count;                // Texture count
    size_t size;                 // Size of texture (Always a multiple of 64)
    std::vector<uint32_t> img;   // Vector container

    /// @brief Load texture
    /// @param filename File where the textures are located
    /// @note Texture format is 32bit images
    Texture(const std::string filename);
    
    /// @brief Retrieves a pixel from the desired texture
    /// @param i X coordinate
    /// @param j Y coordinate
    /// @param idx Texture ID
    /// @return pixel from texture pack
    uint32_t &get(const size_t i, const size_t j, const size_t idx);

    /// @brief Create a vector of 32-bit integers that contains rgb information of a column of pixels
    /// @param texture_id number of texture to be drawn, from left to right, 0 indexed
    /// @param tex_coord x coordinate of column
    /// @param column_height  height of column
    /// @return vector of pixels that make a column
    std::vector<uint32_t> get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height);

    /// @brief Validates if texture count is correct
    /// @return True if count is not 0
    inline bool validate() { return count ? true : false; };
};




#endif  // TEXTURES_H