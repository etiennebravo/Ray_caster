#ifndef MAP_H
#define MAP_H

#include <cstdlib>

class Map {
    private:
    size_t w;  // map width
    size_t h;  // map height

    public:
    /// @brief Creates an object of type map
    /// @note Dimentions = 16 x 16
    Map();

    inline size_t get_width() { return w; };  
    inline size_t get_height() { return h; };

    /// @brief Retrieve the value of a specified position on the map
    /// @param i X coordinate
    /// @param j Y coordinate
    /// @return Value of specified pos
    int get(const size_t i, const size_t j) const;

    /// @brief Determine is space on the map is empty
    /// @param i X coordinate
    /// @param j Y coordinate
    /// @return True if empty, otherwise false 
    bool is_empty(const size_t i, const size_t j) const;

    /// @brief Open a door (Change door id to empty space)
    void open_door();
};

#endif // MAP_H