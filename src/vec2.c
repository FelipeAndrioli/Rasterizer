#include "vec2.h"

float vec2_length(vec2_t *v) {
    return sqrt(v->x * v->x + v->y * v->y);
}

vec2_t vec2_new(float x, float y) {
    return (vec2_t){x, y};
}

vec2_t vec2_add(vec2_t *a, vec2_t *b) {
    vec2_t result = {a->x + b->x, a->y + b->y};
    return result;
}

vec2_t vec2_sub(vec2_t *a, vec2_t *b) {
    vec2_t result = {a->x - b->x, a->y - b->y};
    return result;
}

vec2_t vec2_mult(vec2_t *v, float s) {
    vec2_t result = {v->x * s, v->y * s};
    return result;
}

vec2_t vec2_div(vec2_t *v, float s) {
    vec2_t result = {v->x / s, v->y / s};
    return result;
}

float vec2_dot(vec2_t *a, vec2_t *b) {
    return a->x * b->x + a->y * b->y;
}

float vec2_cross(vec2_t *a, vec2_t *b) {
    return a->x * b->y - a->y * b->x;
}

void vec2_normalize(vec2_t *v) {
    float mag = vec2_length(v);
    v->x /= mag;
    v->y /= mag;
}
