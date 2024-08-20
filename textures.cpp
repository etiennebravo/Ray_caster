#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "utils.h"
#include "textures.h"

Texture::Texture(const std::string filename) : img_w(0), img_h(0), count(0), size(0), img()
{
    int nchannels = -1; // number of channels
    int w;              // width
    int h;              // height
    unsigned char *pixmap = stbi_load(filename.c_str(), &w, &h, &nchannels, 0);  // load image

    // image not found
    if (!pixmap)
    {
        std::cerr << "Error: can not load the textures" << std::endl;
        return;
    }

    // incorrect format
    if (nchannels != 4)
    {
        std::cerr << "Error: texture must be a 32 bit image" << std::endl;
        stbi_image_free(pixmap);
        return;
    }

    count = w / h;
    size = w / count;
    img_w = w;
    img_h = h;

    // incorrect size
    if (w != h * static_cast<int>(count))
    {
        std::cerr << "Error: texture file must contain N square textures packed horizontally" << std::endl;
        stbi_image_free(pixmap);
        return;
    }

    img = std::vector<uint32_t>(w*h);
    for (size_t i = 0; i < static_cast<size_t>(h); i++)
    {
        for (size_t j = 0; j < static_cast<size_t>(w); j++)
        {
            uint8_t r = pixmap[(j + i * w) * 4 + 0];
            uint8_t g = pixmap[(j + i * w) * 4 + 1];
            uint8_t b = pixmap[(j + i * w) * 4 + 2];
            uint8_t a = pixmap[(j + i * w) * 4 + 3];
            img[j + i * w] = pack_color(r, g, b, a);
        }
    }

    stbi_image_free(pixmap);
    return;
}

uint32_t &Texture::get(const size_t i, const size_t j, const size_t idx)
{
    assert(i < size && j < size && idx < count);
    return img[i + idx * size + j * img_w];
}

std::vector<uint32_t> Texture::get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height)
{
    img_w = size * count;
    img_h = size;

    assert(tex_coord < size && texture_id < count);

    std::vector<uint32_t> column(column_height);

    for (size_t y = 0; y < column_height; y++)
    {
        size_t pix_x = texture_id * size + tex_coord;
        size_t pix_y = (y * size) / column_height;
        column[y] = img[pix_x + pix_y * img_w];
    }
    return column;
}