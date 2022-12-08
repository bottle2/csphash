#include <assert.h>
#include <ctype.h>
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

enum state state = STATE_IDLE;

void update()
{
    fps_update(&fps);

    cv_translate(ui.offset_x, ui.offset_y);

    if (STATE_DISTRIBUTED == state || STATE_INTERSECTED == state)
    {
        segments_render(&segments, ui.scale);
    }

    if (STATE_INTERSECTED == state)
    {
        intersections_render(&intersections, ui.scale);
	cells_render(&sphash.cells, ui.scale);
    }

    ui_render(&ui, fps.fps, state);
}

void keyboard(int key)
{
    switch (toupper(key))
    {
        case INPUT_KEY_ESC: // Fall through;
        case 'Q': exit(EXIT_SUCCESS); break;

        case 'D':
            segments.amount = DEF_SEGMENT_AMOUNT;

            segments_generate(&segments);

            state = STATE_DISTRIBUTED;
        break;

        case 'E':
            intersections_test(&intersections, &segments);
            sphash_update     (&sphash       , &segments);

            printf("mean %.2f intersections per segment.\n", (double)intersections.total / segments.amount);

            {
                int total = intersections_test2(segments.them, &sphash);

                printf("n to n has %d intersection vs %d got from sphash.\n", intersections.total, total);

                assert(total == intersections.total);
            }

            state = STATE_INTERSECTED;

            // TODO Separate kinds of execution.
        break;

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

    printf("Using at least %zu MBs\n", CONSUMPTION / 1000 / 1000);
    printf("DEF_SEGMENT_VIEW_MAX = %zu\n", (size_t)DEF_SEGMENT_VIEW_MAX);
    printf("DEF_INTERSECTION_VIEW_MAX = %zu\n", (size_t)DEF_INTERSECTION_VIEW_MAX);

    cv_init(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, "Intersections and spatial hash!");
    cv_run();
}
