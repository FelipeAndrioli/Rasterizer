#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vec2.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

bool is_running = false;

vec2_t vertices[5] = {
    {40, 40},
    {80, 40},
    {40, 80},
    {90, 90},
    {72, 20}
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

bool is_top_left(vec2_t *start, vec2_t *end) {
    vec2_t edge = {end->x - start->x, end->y - start->y};
    bool is_top_edge = edge.y == 0 && edge.x > 0;
    bool is_left_edge = edge.y < 0;

    return is_top_edge || is_left_edge;
}

int edge_cross(vec2_t *a, vec2_t *b, vec2_t *p) {
    vec2_t ab = {b->x - a->x, b->y - a->y};
    vec2_t ap = {p->x - a->x, p->y - a->y};

    return ab.x * ap.y - ab.y * ap.x;
}

void fill_triangle(vec2_t v0, vec2_t v1, vec2_t v2, uint32_t color) {
    int x_min = MIN(MIN(v0.x, v1.x), v2.x);
    int y_min = MIN(MIN(v0.y, v1.y), v2.y);
    int x_max = MAX(MAX(v0.x, v1.x), v2.x);
    int y_max = MAX(MAX(v0.y, v1.y), v2.y);

    int bias0 = is_top_left(&v1, &v2) ? 0 : -1;
    int bias1 = is_top_left(&v2, &v0) ? 0 : -1;
    int bias2 = is_top_left(&v0, &v1) ? 0 : -1;

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            vec2_t p = {x, y};

            int w0 = edge_cross(&v1, &v2, &p) + bias0;
            int w1 = edge_cross(&v2, &v0, &p) + bias1;
            int w2 = edge_cross(&v0, &v1, &p) + bias2;

            bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

            if (is_inside) {
                draw_pixel(x, y, color);
            }
        }
    }
}

void render(void) {
    clear_framebuffer(0xFF000000);
   
    vec2_t vec0 = vertices[0];
    vec2_t vec1 = vertices[1];
    vec2_t vec2 = vertices[2];
    vec2_t vec3 = vertices[3];
    vec2_t vec4 = vertices[4];

    fill_triangle(vec0, vec1, vec2, 0xFF00FF00);
    fill_triangle(vec3, vec2, vec1, 0xFFA74DE3);
    fill_triangle(vec4, vec1, vec0, 0xFF0390FC);

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
