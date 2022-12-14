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

#define SEGMENT_LENGTH_MIN  1
#define SEGMENT_LENGTH_MAX 50

_Static_assert(SEGMENT_LENGTH_MAX >= SEGMENT_LENGTH_MIN, "Stupid!");

static struct segment segment_add(void);

void segments_generate(struct segments segments[static 1], int amount)
{
    assert(segments != NULL);
    assert(amount <= DEF_SEGMENT_MAX);

    for (int segment_i = 0; segment_i < amount; segment_i++)
    {
        segments->them[segment_i] = segment_add();
    }

    segments->amount = amount;
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

bool segment_dump(struct segment segment[static 1])
{
    printf(
        "(%.2f,%.2f)-(%.2f,%.2f)\n",
        segment->p1_x, segment->p1_y, segment->p2_x, segment->p2_y
    );

    fflush(stdout);

    return false;
}

static struct segment segment_add(void)
{
    float const max_x        = DEF_TEST_AREA_WIDTH;
    float const max_y        = DEF_TEST_AREA_HEIGHT;
    float const length_min   = SEGMENT_LENGTH_MIN;
    float const length_max   = SEGMENT_LENGTH_MAX;
    float const length_range = length_max - length_min;

    assert(max_x >= length_max);
    assert(max_y >= length_max);

    float const angle  = rand() * M_PI * 2.0f / RAND_MAX; 
    float const length = length_min + (rand() / (float)RAND_MAX) * length_range;

    float const change_x = length * cosf(angle);
    float const change_y = length * sinf(angle);

    float const actual_max_x = max_x - fabsf(change_x);
    float const actual_max_y = max_y - fabsf(change_y);
    float const offset_x     = -MIN(0.0f, change_x);
    float const offset_y     = -MIN(0.0f, change_y);

    float const p1_x = offset_x + (rand() / (float)RAND_MAX) * actual_max_x;
    float const p1_y = offset_y + (rand() / (float)RAND_MAX) * actual_max_y;
    float const p2_x = p1_x + change_x;
    float const p2_y = p1_y + change_y;

    assert(p1_x >= 0.0f);
    assert(p1_y >= 0.0f);
    assert(p2_x >= 0.0f);
    assert(p2_y >= 0.0f);

    assert(p1_x <= (float)DEF_TEST_AREA_WIDTH );
    assert(p1_y <= (float)DEF_TEST_AREA_HEIGHT);
    assert(p2_x <= (float)DEF_TEST_AREA_WIDTH );
    assert(p2_y <= (float)DEF_TEST_AREA_HEIGHT);

    return (struct segment) {p1_x, p1_y, p2_x, p2_y};
}
