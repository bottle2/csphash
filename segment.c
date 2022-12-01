#define _USE_MATH_DEFINES

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "gl_canvas2d.h"

#include "segment.h"
#include "rgb.h"
#include "util.h"

static struct segment segment_add(struct segments segments[static 1]);

void segments_generate(struct segments segments[static 1])
{
    assert(segments != NULL);
    assert(segments->amount < DEF_SEGMENT_MAX);

    for (int segment_i = 0; segment_i < (int)segments->amount; segment_i++)
    {
        segments->them[segment_i] = segment_add(segments);
    }
}

void segments_render(struct segments segments[static 1], float scale)
{
    assert(segments != NULL);

    cv_color(RGB_BLACK);

    int n_segment = MIN(segments->amount, DEF_SEGMENT_VIEW_MAX);

    for (int segment_i = 0; segment_i < n_segment; segment_i++)
    {
        struct segment *it = segments->them + segment_i;

        cv_line(it->p1_x * scale, it->p1_y * scale,
                it->p2_x * scale, it->p2_y * scale);
    }
}

static struct segment segment_add(struct segments segments[static 1])
{
    // TODO Add and use min_length.

    assert(segments != NULL);

    float const max_x      = segments->max_x;
    float const max_y      = segments->max_y;
    float const max_length = segments->max_length;

    assert(max_x >= max_length);
    assert(max_y >= max_length);

    float const angle    = rand() * M_PI * 2.0f / RAND_MAX; 
    float const length   = rand() * max_length  / RAND_MAX;
    float const change_x = length * cosf(angle);
    float const change_y = length * sinf(angle);

    float const actual_max_x = segments->max_x - fabsf(change_x);
    float const actual_max_y = segments->max_y - fabsf(change_y);
    float const offset_x     = -MIN(0.0f, change_x);
    float const offset_y     = -MIN(0.0f, change_y);

    float const p1_x = offset_x + rand() * actual_max_x / RAND_MAX;
    float const p1_y = offset_y + rand() * actual_max_y / RAND_MAX;
    float const p2_x = p1_x + change_x;
    float const p2_y = p1_y + change_y;

    assert(p1_x >= 0.0f);
    assert(p1_y >= 0.0f);
    assert(p2_x >= 0.0f);
    assert(p2_y >= 0.0f);

    assert(p1_x <= segments->max_x);
    assert(p1_y <= segments->max_y);
    assert(p2_x <= segments->max_x);
    assert(p2_y <= segments->max_y);

    return (struct segment) {
        .p1_x   = p1_x,
        .p1_y   = p1_y,
        .p2_x   = p2_x,
        .p2_y   = p2_y,
        .length = length,
        .angle  = angle,
    };
}
