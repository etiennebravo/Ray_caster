#include <iostream>
#include <vector>
#include "SDL.h"

#include "map.h"
#include "utils.h"
#include "player.h"
#include "sprite.h"
#include "textures.h"
#include "raycaster.h"

int main(int argv, char **args)
{
    raycaster caster;
    Player player;
    Map map;

    Texture tex_walls("./walltext.png");
    if (!tex_walls.validate()) { return EXIT_FAILURE; };

    Texture tex_monst("./monsters.png");
    if (!tex_monst.validate()) { return EXIT_FAILURE; };

    Texture tex_keys("./keys.png");
    if (!tex_keys.validate()) { return EXIT_FAILURE; };

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer(caster.get_width(), caster.get_height(), SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer))
    {
        std::cerr << "Couldn't create window and renderer: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Texture *framebuffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, caster.get_width(), caster.get_height());
    SDL_UpdateTexture(framebuffer_texture, NULL, reinterpret_cast<void *>(caster.get_img().data()), caster.get_width() * 4);

    bool quit = false;
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState( nullptr );

    // Game loop
    while (!quit)
    {
        player.input(quit, state, event, map);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        caster.render(map, player, tex_walls, tex_monst, tex_keys);
        SDL_UpdateTexture(framebuffer_texture, NULL, reinterpret_cast<void *>(caster.get_img().data()), caster.get_width() * 4);

        SDL_Delay(32);
    }

    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}