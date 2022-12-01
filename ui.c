#include <assert.h>
#include <stdio.h>

#include <GL/glut.h>

#include "gl_canvas2d.h"

#include "def.h"
#include "expand.h"
#include "state.h"
#include "rgb.h"
#include "ui.h"

static char *state_text[] = { STATE_XS(EXPAND_AS_SECOND_FROM_TWO) };

void ui_render(struct ui ui[static 1], float fps, enum state state)
{
    assert(ui != NULL);

    cv_translate(0.0f, 0.0f);

    cv_color(RGB_BLACK);

    char buffer[DEF_STATUS_BAR_LENGHT] = {0};

    snprintf(
        buffer, DEF_STATUS_BAR_LENGHT, "FPS %8.3f %4dx%-4d %s",
        (double)fps, ui->canvas_width, ui->canvas_height, state_text[state]
    );

    cv_text(2.0f, 2.0f, buffer);
}

void ui_scale(struct ui ui[static 1], int area_width, int area_height)
{
    assert(ui != NULL);

    float const area_ratio   = (float)area_width       / area_height;
    float const canvas_ratio = (float)ui->canvas_width / ui->canvas_height;

    if (area_ratio > canvas_ratio)
    {
        ui->scale = ui->canvas_width / (float)area_width;
    }
    else
    {
        ui->scale = ui->canvas_height / (float)area_height;
    }
    // Choose which dimension to glue with which other.

    ui->offset_x = 0.5f * (ui->canvas_width  - ui->scale * area_width);
    ui->offset_y = 0.5f * (ui->canvas_height - ui->scale * area_height);
}
