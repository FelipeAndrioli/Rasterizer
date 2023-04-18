#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"
#include "vec2.h"
#include "algorithm.h"

bool is_running = false;

vec2_t vertices[5] = {
    {40, 40},
    {80, 40},
    {40, 80},
    {90, 90}
};

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
  
    float angle = SDL_GetTicks() / 1000.0f * 0.5f;
    vec2_t center = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

    vec2_t vec0 = vec2_rotate(vertices[0], center, angle);
    vec2_t vec1 = vec2_rotate(vertices[1], center, angle);
    vec2_t vec2 = vec2_rotate(vertices[2], center, angle);
    vec2_t vec3 = vec2_rotate(vertices[3], center, angle);

    fill_triangle(vec0, vec1, vec2);
    fill_triangle(vec3, vec2, vec1);

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
