#ifndef __UI_H__
#define __UI_H__

#include "state.h"

struct ui
{
    int screen_width;
    int screen_height;
    int mouse_x;
    int mouse_y;
    // User input.

    float scale;
    float offset_x;
    float offset_y;
    // User output.

    int zoom;
    // Internal.
};

void ui_render  (struct ui ui[static 1], float fps, enum state state);
void ui_scale   (struct ui ui[static 1]);
void ui_zoom_in (struct ui ui[static 1]);
void ui_zoom_out(struct ui ui[static 1]);

#endif
