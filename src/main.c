#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vec2.h"

bool is_running = false;

void process_input(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    is_running = false;
                break;
        }
    }
}

void render(void) {
    clear_framebuffer(0xFF000000);

    render_framebuffer();
}

int main(int argc, char *argv[]) {
    is_running = create_window();

    while(is_running) {
        fix_framerate();
        process_input();
        render();
    }

    destroy_window();

    return 0;
}
