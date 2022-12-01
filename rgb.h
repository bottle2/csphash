#ifndef __RGB__H__
#define __RGB__H__

#define RGB_UNSAFE(R, G, B, A) R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f

#define RGB_BLACK  RGB_UNSAFE(  0,   0,   0, 255)
#define RGB_RED    RGB_UNSAFE(255,   0,   0, 255)
#define RGB_GREEN  RGB_UNSAFE(  0, 255,   0, 255)
#define RGB_BLUE   RGB_UNSAFE(  0,   0, 255, 255)
#define RGB_YELLOW RGB_UNSAFE(255, 255,   0, 255)
#define RGB_CYAN   RGB_UNSAFE(  0, 255, 255, 255)
#define RGB_PINK   RGB_UNSAFE(255,   0, 255, 255)
#define RGB_MAROON RGB_UNSAFE(128,   0,   0, 255)
#define RGB_WHITE  RGB_UNSAFE(255, 255, 255, 255)
#define RGB_GRAY   RGB_UNSAFE(190, 190, 190, 255)
#define RGB_GREY   RGB_GRAY

#endif//__RGB__H__
