#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include <GL/glut.h>

#include "def.h"
#include "gl_canvas2d.h"
#include "intersect.h"
#include "rgb.h"
#include "segment.h"
#include "util.h"

static bool intersects(struct segment segments[static 2], float *x, float *y);

void intersections_test(struct intersections intersections[static 1],
                        struct segments      segments     [static 1])
{
    assert(intersections != NULL);
    assert(intersections != NULL);

    intersections->total  = 0;
    intersections->n_view = 0;

    int const n_segment = segments->amount;

    for (int segment_i = 0; segment_i < n_segment; segment_i++)
    {
        for (int segment_j = segment_i + 1; segment_j < n_segment; segment_j++)
        {
            float x = 0.0f / 0.0f;
            float y = 0.0f / 0.0f;

            struct segment *one   = segments->them + segment_i;
            struct segment *other = segments->them + segment_j;
            struct segment both[2] = {*one, *other};

            if (intersects(both, &x, &y))
            {
                intersections->total++;

                if (segment_i < DEF_SEGMENT_VIEW_MAX &&
                    segment_j < DEF_SEGMENT_VIEW_MAX
                ) {
                    assert(intersections->n_view < DEF_INTERSECTION_VIEW_MAX);

                    intersections->view_xs[intersections->n_view] = x;
                    intersections->view_ys[intersections->n_view] = y;
                    intersections->n_view++;
                }
            }
        }
    }
}

void intersections_render(struct intersections intersections[static 1], float scale)
{
    assert(intersections != NULL);

    int    n_view  = intersections->n_view;
    float *view_xs = intersections->view_xs;
    float *view_ys = intersections->view_ys;

    cv_color(RGB_RED);
    glPointSize(2.0f);
    for (int view_i = 0; view_i < n_view; view_i++)
    {
        cv_point(view_xs[view_i] * scale, view_ys[view_i] * scale);
    }
    glPointSize(1.0f);
}

static bool intersects(struct segment segments[static 2], float *x, float *y)
{
    // TODO Colinear intersection
    // Condition one: the length between extremes is <= than sum of both length.
    // Condition two: the angle between two point from different segment differ.

    assert(segments != NULL);

    float const p0_x = segments[0].p1_x;
    float const p0_y = segments[0].p1_y;
    float const p1_x = segments[0].p2_x;
    float const p1_y = segments[0].p2_y;
    float const p2_x = segments[1].p1_x;
    float const p2_y = segments[1].p1_y;
    float const p3_x = segments[1].p2_x;
    float const p3_y = segments[1].p2_y;

    float const s1_x = p1_x - p0_x;
    float const s2_x = p3_x - p2_x;

    float const s1_y = p1_y - p0_y;
    float const s2_y = p3_y - p2_y;

    float const divisor = -s2_x * s1_y + s1_x * s2_y;

    float const s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / divisor;
    float const t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / divisor;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected.

        if (x != NULL)
        {
            *x = p0_x + (t * s1_x);
        }

        if (y != NULL)
        {
            *y = p0_y + (t * s1_y);
        }

        return true;
    }

    return false; // No collision.
}
// See https://stackoverflow.com/a/1968345
