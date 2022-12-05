#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    cv_translate(0.0f, 0.0f);
    ui_render(&ui, fps.fps, state);

    ui_scale(&ui);

    cv_translate(ui.offset_x, ui.offset_y + DEF_STATUS_BAR_HEIGHT);

    if (STATE_DISTRIBUTED == state || STATE_INTERSECTED == state)
    {
        segments_render(&segments, ui.scale);
    }

    if (STATE_INTERSECTED == state)
    {
        intersections_render(&intersections, ui.scale);
        sphash_render(&sphash, ui.scale);
    }
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
    (void)wheel;
    (void)direction;
    (void)x;
    (void)y;

    // Do nothing.
}

void resize(int width, int height)
{
    ui.canvas_width  = width;
    ui.canvas_height = height - DEF_STATUS_BAR_HEIGHT;

    // TODO Should reescale here, but canvas area is only set later...
}

int main(void)
{
    fps_init(&fps);
    srand(time(NULL));

    sphash.n_line   = 6;
    sphash.n_column = 8;

    printf("Using at least %zu MBs\n", CONSUMPTION / 1000 / 1000);
    printf("DEF_SEGMENT_VIEW_MAX = %zu\n", (size_t)DEF_SEGMENT_VIEW_MAX);
    printf("DEF_INTERSECTION_VIEW_MAX = %zu\n", (size_t)DEF_INTERSECTION_VIEW_MAX);

    cv_init(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, "Intersections and spatial hash!");
    cv_run();
}
