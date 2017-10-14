
CC = g++
CCFLAGS = -std=c++11 `sdl2-config --cflags --libs`
INCLUDEFLAGS = -I.
LINKFLAGS = -lm -lSDL2main -lSDL2


all: debug


main: main.o state.o render.o init.o mappings.o style.o
	$(CC) -o main.exe main.o state.o render.o init.o mappings.o style.o $(CCFLAGS) $(LINKFLAGS)


debug: CCFLAGS += -g -O0 -Wall
debug: main


release: CCFLAGS += -Ofast
release: main


demo: release
	./main.exe


clean:
	rm -f *.o


%.o: %.cc
	$(CC) -c $(CCFLAGS) $(INCLUDEFLAGS) $*.cc