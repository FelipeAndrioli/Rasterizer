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
    {90, 90}
};

color_t colors[3] = {
    {0xFF, 0x00, 0x00},
    {0x00, 0xFF, 0x00},
    {0x00, 0x00, 0xFF}
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

void fill_triangle(vec2_t v0, vec2_t v1, vec2_t v2) {
    int x_min = MIN(MIN(v0.x, v1.x), v2.x);
    int y_min = MIN(MIN(v0.y, v1.y), v2.y);
    int x_max = MAX(MAX(v0.x, v1.x), v2.x);
    int y_max = MAX(MAX(v0.y, v1.y), v2.y);

    int bias0 = is_top_left(&v1, &v2) ? 0 : -1;
    int bias1 = is_top_left(&v2, &v0) ? 0 : -1;
    int bias2 = is_top_left(&v0, &v1) ? 0 : -1;

    float parallelogram_area = edge_cross(&v0, &v1, &v2);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            vec2_t p = {x, y};

            int w0 = edge_cross(&v1, &v2, &p) + bias0;
            int w1 = edge_cross(&v2, &v0, &p) + bias1;
            int w2 = edge_cross(&v0, &v1, &p) + bias2;

            bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

            if (is_inside) {
                float alpha = w0 / parallelogram_area;
                float beta = w1 / parallelogram_area;
                float gamma = w2 / parallelogram_area;

                int a = 0xFF;
                int r = (alpha) * colors[0].r + (beta) * colors[1].r + (gamma) * colors[2].r;
                int g = (alpha) * colors[0].g + (beta) * colors[1].g + (gamma) * colors[2].g;
                int b = (alpha) * colors[0].b + (beta) * colors[1].b + (gamma) * colors[2].b;

                uint32_t interpolated_color = 0x000000;
                interpolated_color = (interpolated_color | a) << 8;
                interpolated_color = (interpolated_color | r) << 8;
                interpolated_color = (interpolated_color | g) << 8;
                interpolated_color = (interpolated_color | b);

                draw_pixel(x, y, interpolated_color);
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
