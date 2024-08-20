#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>
#include <string>

/// @brief Pack colors into one 32 bit integer using rgb values, rgb values go from 0 to 255
/// @param r red
/// @param g green
/// @param b blue
/// @param a alpha or transparency
/// @return 32 bit integer made from adding parameter values
uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255);

/// @brief Unpack colors from one 32-bit integer into four variables
/// @param color 32 bit integer that contains rgb data
/// @param r red
/// @param g green
/// @param b blue
/// @param a alpha or transparency
void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);

/// @brief Create a ppm image file of a certain size using the rbg data of each pixel
/// @param filename name of the file to be created
/// @param image vector of 32-bit integers that contain rbg data
/// @param w width of vector
/// @param h height of vector
/// @note w & h are the resolution of the desired image
void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h);

#endif // utils.h