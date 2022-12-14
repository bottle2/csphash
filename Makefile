CC=cc -std=c18 -pipe

CFLAGS=                    \
-Wpedantic -Wall -Wextra    \
-Wvla -Wshadow              \
-Wstrict-aliasing           \
-fstrict-aliasing           \
-g -O0

#-fsanitize=undefined

LDLIBS=-lfreeglut -lopengl32 -lglu32 -lm

OBJECT=cell.o gl_canvas2d.o intersect.o main.o segment.o sphash.o ui.o
TARGET=t2

all:$(TARGET)

$(TARGET):$(OBJECT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECT) $(LDLIBS)

clean:
	rm -f $(TARGET){,.exe} $(OBJECT)

check:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction *.c

cell.c:cell.h def.h gl_canvas2d.h rgb.h
gl_canvas2d.c:gl_canvas2d.h
intersect.c:def.h gl_canvas2d.h intersect.h rgb.h segment.h sphash.h util.h
main.c:def.h input.h intersect.h gl_canvas2d.h fps.h rgb.h segment.h sphash.h state.h ui.h util.h
segment.c:def.h gl_canvas2d.h segment.h rgb.h util.h
sphash.c:cell.h def.h sphash.h util.h
ui.c:def.h expand.h gl_canvas2d.h rgb.h ui.h util.h

.SUFFIXES:.c.h
.h.c:
	touch $@
