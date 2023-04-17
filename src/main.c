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

float edge_cross(vec2_t *a, vec2_t *b, vec2_t *p) {
    vec2_t ab = {b->x - a->x, b->y - a->y};
    vec2_t ap = {p->x - a->x, p->y - a->y};

    return ab.x * ap.y - ab.y * ap.x;
}

void fill_triangle(vec2_t v0, vec2_t v1, vec2_t v2) {
    int x_min = floor(MIN(MIN(v0.x, v1.x), v2.x));
    int y_min = floor(MIN(MIN(v0.y, v1.y), v2.y));
    int x_max = ceil(MAX(MAX(v0.x, v1.x), v2.x));
    int y_max = ceil(MAX(MAX(v0.y, v1.y), v2.y));

    float delta_w0_col = (v1.y - v2.y);
    float delta_w1_col = (v2.y - v0.y);
    float delta_w2_col = (v0.y - v1.y);

    float delta_w0_row = (v2.x - v1.x);
    float delta_w1_row = (v0.x - v2.x);
    float delta_w2_row = (v1.x - v0.x);

    float bias0 = is_top_left(&v1, &v2) ? 0 : -0.0001;
    float bias1 = is_top_left(&v2, &v0) ? 0 : -0.0001;
    float bias2 = is_top_left(&v0, &v1) ? 0 : -0.0001;

    float parallelogram_area = edge_cross(&v0, &v1, &v2);

    vec2_t p0 = {x_min + 0.5f, y_min + 0.5f};
    float w0_row = edge_cross(&v1, &v2, &p0) + bias0;
    float w1_row = edge_cross(&v2, &v0, &p0) + bias1;
    float w2_row = edge_cross(&v0, &v1, &p0) + bias2;

    for (int y = y_min; y <= y_max; y++) {
        float w0 = w0_row;
        float w1 = w1_row;
        float w2 = w2_row;
        for (int x = x_min; x <= x_max; x++) {
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

            w0 += delta_w0_col;
            w1 += delta_w1_col;
            w2 += delta_w2_col;
        }
        w0_row += delta_w0_row;
        w1_row += delta_w1_row;
        w2_row += delta_w2_row;
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
