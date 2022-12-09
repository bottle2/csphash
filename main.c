#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cell.h"
#include "def.h"
#include "fps.h"
#include "gl_canvas2d.h"
#include "input.h"
#include "intersect.h"
#include "rgb.h"
#include "segment.h"
#include "sphash.h"
#include "state.h"
#include "ui.h"

static struct fps           fps           = {0};
static struct intersections intersections = {0};
static struct segments      segments      = {0};
static struct ui            ui            = {0};
static struct sphash        sphash        = {0};

#define CONSUMPTION (sizeof fps      + sizeof intersections + \
                     sizeof segments + sizeof ui            + \
                     sizeof sphash                          )
               
_Static_assert(CONSUMPTION < DEF_MAX_RAM_USAGE_ESTIMATE, "Use less memory!");

static enum state state = STATE_IDLE;

static bool is_paused = false;

void update(void)
{
    fps_update(&fps);

    cv_translate(ui.offset_x, ui.offset_y);

    if (!is_paused)
    {
        switch (state)
        {
            case STATE_IDLE:
                segments_generate(&segments, DEF_SEGMENT_AMOUNT);
                state = STATE_DISTRIBUTED;
            break;

            case STATE_DISTRIBUTED:
                sphash_update(&sphash, &segments);
                state = STATE_HASHED;
            break;

            case STATE_HASHED:
                intersections_test(&intersections, &segments, &sphash);
                state = STATE_INTERSECTED;
            break;

            case STATE_INTERSECTED:
                state = STATE_IDLE;
            break;

            default:
                assert(!"Untreated state!");
            break;
        }
    }

    if (STATE_DISTRIBUTED == state || STATE_INTERSECTED == state || STATE_HASHED == state)
    {
        segments_render(&segments, ui.scale);
    }

    if (STATE_INTERSECTED == state)
    {
        intersections_render(&intersections, ui.scale);
    }

    if (STATE_HASHED == state || STATE_INTERSECTED == state)
    {
	cells_render(&sphash.cells, ui.scale);
    }

    ui_render(&ui, fps.fps);
}

void keyboard(int key)
{
    switch (toupper(key))
    {
        case INPUT_KEY_ESC: // Fall through;
        case 'Q': exit(EXIT_SUCCESS); break;

        case 'P': is_paused = !is_paused; break;

        default: /* Do nothing. */ break;
    }
}

void keyboardUp(int key)
{
    (void)key;

    // Do nothing.
}

void mouse(int button, int _state, int wheel, int direction, int x, int y)
{
    (void)button;
    (void)_state;

    ui.mouse_x = x;
    ui.mouse_y = y;

    if (INPUT_SCROLL_YES == wheel)
    {
        if (INPUT_UPWARDS == direction)
        {
            ui_zoom_in(&ui);
        }
        else if (INPUT_DOWNWARDS == direction)
        {
            ui_zoom_out(&ui);
        }
    }

    ui_scale(&ui);
}

void resize(int width, int height)
{
    ui.screen_width  = width;
    ui.screen_height = height;

    ui_scale(&ui);
}

int main(void)
{
    fps_init(&fps);
    srand(time(NULL));

    sphash.cells.n_line      = 6;
    sphash.cells.n_column    = 8;
    sphash.cells.cell_width  = DEF_TEST_AREA_WIDTH  / sphash.cells.n_column;
    sphash.cells.cell_height = DEF_TEST_AREA_HEIGHT / sphash.cells.n_line;

#if 0
    printf("Using at least %zu MBs\n", CONSUMPTION / 1000 / 1000);
    printf("DEF_SEGMENT_VIEW_MAX = %zu\n", (size_t)DEF_SEGMENT_VIEW_MAX);
    printf("DEF_INTERSECTION_VIEW_MAX = %zu\n", (size_t)DEF_INTERSECTION_VIEW_MAX);
#endif

    cv_init(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, "Intersections and spatial hash!");
    cv_run();
}
