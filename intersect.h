#ifndef __INTERSECT_H__
#define __INTERSECT_H__

#include "def.h"
#include "segment.h"

struct intersections
{
    float view_xs[DEF_INTERSECTION_VIEW_MAX];
    float view_ys[DEF_INTERSECTION_VIEW_MAX];
    int   n_view;

    int total;
};

void intersections_test  (struct intersections intersections[static 1],
                          struct segments      segments     [static 1]);
void intersections_render(struct intersections intersections[static 1], float scale);

#endif
