#ifndef __UI_H__
#define __UI_H__

#include "state.h"

struct ui
{
    int canvas_width;
    int canvas_height;

    float scale;
    float offset_x;
    float offset_y;
};

void ui_render(struct ui ui[static 1], float fps, enum state state);
void ui_scale (struct ui ui[static 1], int area_width, int area_height);

#endif
