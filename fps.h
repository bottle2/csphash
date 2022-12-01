//********************************************************
// Cesar Tadeu Pozzer
// 09/2009
// UFSM
// pozzer@inf.ufsm.br
//********************************************************

// Desclassificado.

#ifndef _FPS_H___
#define _FPS_H___

#include <time.h>

#define UPDATE_RATE 30 // Milisegundos

struct fps
{
    float fps;
    float delta;

    clock_t tick;
    clock_t last;
    int     count;
    // Internals.
};

static inline void fps_init(struct fps *fps)
{
    fps->tick  = clock();
    fps->count = 0;
    fps->fps   = 20;
}

static inline void fps_update(struct fps *fps)
{
    float elapsed = clock();
    float diff    = elapsed - fps->tick;

    fps->delta = elapsed - fps->last;
    fps->last  = elapsed;

    fps->count++;

    if (diff * 1000 / CLOCKS_PER_SEC > UPDATE_RATE)
    {
        fps->tick  = fps->last;
        fps->fps   = fps->count / (diff / CLOCKS_PER_SEC);
        fps->count = 0;
    }
}

#endif
