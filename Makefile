CC=cc -std=c18 -pipe

CFLAGS=                    \
-IC:/msys64/mingw64/include \
-Wpedantic -Wall -Wextra    \
-Wvla -Wshadow              \
-Wstrict-aliasing           \
-fstrict-aliasing

LDLIBS=-L/mingw64/lib -lfreeglut -lopengl32 -lglu32 -lm

OBJECT=gl_canvas2d.o intersect.o main.o segment.o sphash.o ui.o
TARGET=t2

all:$(TARGET)

$(TARGET):$(OBJECT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECT) $(LDLIBS)

clean:
	rm -f $(TARGET){,.exe} $(OBJECT)

gl_canvas2d.c:gl_canvas2d.h
	touch $@

intersect.c:def.h gl_canvas2d.h intersect.h rgb.h segment.h util.h
	touch $@

main.c:def.h input.h intersect.h gl_canvas2d.h fps.h rgb.h segment.h sphash.h state.h ui.h util.h
	touch $@

segment.c:def.h gl_canvas2d.h segment.h rgb.h util.h
	touch $@

sphash.c:def.h gl_canvas2d.h sphash.h rgb.h util.h
	touch $@

ui.c:def.h expand.h gl_canvas2d.h rgb.h ui.h
	touch $@