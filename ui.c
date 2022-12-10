#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#include "gl_canvas2d.h"

#include "def.h"
#include "expand.h"
#include "rgb.h"
#include "ui.h"
#include "util.h"

#define UI_STATUS_BAR_LENGHT 1000
#define UI_STATUS_BAR_HEIGHT   15
#define UI_MOUSE_CLAMP_MARGIN 0.05f
#define UI_ZOOM_FACTOR  4
#define UI_ZOOM_DOUBLES 6
#define UI_ZOOM_MAX     UI_ZOOM_FACTOR * UI_ZOOM_DOUBLES

static float ui_zoom(int level);

void ui_render(struct ui ui[static 1], float fps)
{
    assert(ui != NULL);

    cv_translate(0.0f, 0.0f);

    cv_color(RGB_WHITE);
    cv_rectFill(0.0f, 0.0f, ui->screen_width, UI_STATUS_BAR_HEIGHT);

    cv_color(RGB_BLACK);

    char buffer[UI_STATUS_BAR_LENGHT] = {0};

    float zoom = ui_zoom(ui->zoom);

    snprintf(
        buffer, UI_STATUS_BAR_LENGHT, "FPS %8.3f %4.1fx %4dx%-4d P to pause",
        (double)fps, zoom, ui->screen_width, ui->screen_height
    );

    cv_text(2.0f, 2.0f, buffer);
}

void ui_scale(struct ui ui[static 1])
{
    assert(ui != NULL);

    float const usable_height = ui->screen_height - UI_STATUS_BAR_HEIGHT;

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

    ui->scale *= ui_zoom(ui->zoom);

    float const clamp_min_x = ui->screen_width  *         UI_MOUSE_CLAMP_MARGIN;
    float const clamp_max_x = ui->screen_width  * (1.0f - UI_MOUSE_CLAMP_MARGIN);
    float const clamp_min_y = ui->screen_height *         UI_MOUSE_CLAMP_MARGIN;
    float const clamp_max_y = ui->screen_height * (1.0f - UI_MOUSE_CLAMP_MARGIN);

    float const mouse_clamp_x = CLAMP(clamp_min_x, ui->mouse_x, clamp_max_x) - clamp_min_x;
    float const mouse_clamp_y = CLAMP(clamp_min_y, ui->mouse_y, clamp_max_y) - clamp_min_y;

    float const relative_x = 1.0f - mouse_clamp_x / (clamp_max_x - clamp_min_x);
    float const relative_y = 1.0f - mouse_clamp_y / (clamp_max_y - clamp_min_y);

    float const view_width  = ui->scale * DEF_TEST_AREA_WIDTH;
    float const view_height = ui->scale * DEF_TEST_AREA_HEIGHT;

    float const pan_x = (ui->screen_width - view_width);
    float const pan_y = (usable_height    - view_height);

    ui->offset_x  = pan_x < 0.0f ? LERP(pan_x, 0.0f, relative_x) : pan_x * 0.5f;
    ui->offset_y  = pan_y < 0.0f ? LERP(pan_y, 0.0f, relative_y) : pan_y * 0.5f;
    ui->offset_y += UI_STATUS_BAR_HEIGHT;
}

void ui_zoom_in(struct ui ui[static 1])
{
    assert(ui != NULL);

    if (ui->zoom < UI_ZOOM_MAX)
    {
        ui->zoom++;
    }
}

void ui_zoom_out(struct ui ui[static 1])
{
    assert(ui != NULL);

    if (ui->zoom > 0)
    {
        ui->zoom--;
    }
}

static float ui_zoom(int level)
{
    return powf(2.0f, level / (float)UI_ZOOM_FACTOR);
}
