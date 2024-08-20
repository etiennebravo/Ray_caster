#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "SDL.h"
#include "map.h"

class Player {
    public:
        /// @brief Constructor, initializes to specified values for now
        Player();

        inline float get_x() const { return position[0]; };
        inline float get_y() const { return position[1]; };
        inline float get_angle() const { return angle; };

        /// @brief Checks if key is collected
        /// @param i index
        /// @return true if key has been collected, false otherwise
        bool get_key_state(const size_t i) const;

        /// @brief Changes state of key array
        /// @param i key_collected index
        /// @param state true if collected, false otherwise
        void set_key_state(const size_t i, const bool state);

        /// @brief Input from the players keyboard
        /// @param quit pressing the 'x' on the window will close the game
        /// @param state Determines keyboards state
        /// @param event Determines if an event is happening
        /// @param map Map layout for collisions
        void input(bool &quit, const Uint8* state, SDL_Event &event, const Map map);

        // Destructor is created by the compiler
    private:
        float position[2];     // position, 0 = x, 1 = y
        float angle;           // view direction in radians
        float speed;           // Moving velocity
        std::vector<bool> keys_collected = {false};  // Key storage container
        
        
};

#endif // PLAYER_H