
CC = g++
CCFLAGS = -std=c++11 `sdl2-config --cflags --libs`
INCLUDEFLAGS = -I.
LINKFLAGS = -lm -lSDL2main -lSDL2


# decide executable extension
ifeq ($(OS),Windows_NT)
EXT = exe
else
EXT = out
endif


all: debug

main: main.o state.o render.o init.o mappings.o style.o
	$(CC) -o main.$(EXT) main.o state.o render.o init.o mappings.o style.o $(CCFLAGS) $(LINKFLAGS)


debug++: CCFLAGS += -Wpedantic -Wextra -Werror
debug++: debug

debug: CCFLAGS += -g -O0 -Wall
debug: main


release: CCFLAGS += -Ofast
release: main


demo: release
	./main.$(EXT)


cleaner: clean
	rm -f ./main.$(EXT)


clean:
	rm -f *.o


.cc.o:
	$(CC) -c -o $@ $< $(CCFLAGS) $(INCLUDEFLAGS)