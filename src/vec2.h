#ifndef VEC2_H
#define VEC2_H

#include <math.h>

typedef struct {
    float x;
    float y;
} vec2_t;

float vec2_length(vec2_t *v);
vec2_t vec2_new(float x, float y);
vec2_t vec2_add(vec2_t *a, vec2_t *b);
vec2_t vec2_sub(vec2_t *a, vec2_t *b);
vec2_t vec2_mult(vec2_t *v, float s);
vec2_t vec2_div(vec2_t *v, float s);
float vec2_dot(vec2_t *a, vec2_t *b);
float vec2_cross(vec2_t *a, vec2_t *b);
void vec2_normalize(vec2_t *v);
#endif
