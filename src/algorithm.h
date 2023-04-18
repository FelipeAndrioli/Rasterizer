#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdbool.h>

#include "./display.h"
#include "./vec2.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

bool is_top_left(vec2_t *start, vec2_t *end);
float edge_cross(vec2_t *a, vec2_t *b, vec2_t *p);
void fill_triangle(vec2_t v0, vec2_t v1, vec2_t v2);

#endif
