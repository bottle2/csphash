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

    cv_color(RGB_WHITE);
    cv_rectFill(0.0f, 0.0f, ui->screen_width, DEF_STATUS_BAR_HEIGHT);

    cv_color(RGB_BLACK);

    char buffer[DEF_STATUS_BAR_LENGHT] = {0};

    int zoom = 100.0f + ui->zoom * DEF_ZOOM_FACTOR * 100.0f;

    snprintf(
        buffer, DEF_STATUS_BAR_LENGHT, "FPS %8.3f %3d%% %4dx%-4d %s",
        (double)fps, zoom, ui->screen_width, ui->screen_height, state_text[state]
    );

    cv_text(2.0f, 2.0f, buffer);
}

void ui_scale(struct ui ui[static 1])
{
    assert(ui != NULL);

    float const usable_height = ui->screen_height - DEF_STATUS_BAR_HEIGHT;

    float const area_ratio   = DEF_TEST_AREA_RATIO;
    float const canvas_ratio = (float)ui->screen_width / usable_height;

    if (area_ratio > canvas_ratio)
    {
        ui->scale = ui->screen_width / (float)DEF_TEST_AREA_WIDTH;
    }
    else
    {
        ui->scale = usable_height / (float)DEF_TEST_AREA_HEIGHT;
    }
    // Choose which dimension to glue with which other.

    ui->scale *= 1.0f + ui->zoom * DEF_ZOOM_FACTOR;

    float const viewport_width  = ui->scale * DEF_TEST_AREA_WIDTH;
    float const viewport_height = ui->scale * DEF_TEST_AREA_HEIGHT;

#if 0
    float const relative_mouse_x = 1.0f - (ui->screen_width  - ui->mouse_x) / ui->screen_width;
    float const relative_mouse_y = 1.0f - (ui->screen_height - ui->mouse_y) / ui->screen_height;
#endif

    ui->offset_x = 0.5f * (ui->screen_width - viewport_width);
    ui->offset_y = 0.5f * (usable_height    - viewport_height) + DEF_STATUS_BAR_HEIGHT;
}

void ui_zoom_in(struct ui ui[static 1])
{
    assert(ui != NULL);

    if (ui->zoom < DEF_ZOOM_MAX)
    {
        ui->zoom++;
    }

    ui_scale(ui);
}

void ui_zoom_out(struct ui ui[static 1])
{
    assert(ui != NULL);

    if (ui->zoom > 0)
    {
        ui->zoom--;
    }

    ui_scale(ui);
}
