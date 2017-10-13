
CC = g++
CCFLAGS = -std=c++1z
INCLUDEFLAGS = -I.
LINKFLAGS = -lSDL2 -lm


all: debug


main: main.o state.o render.o init.o mappings.o style.o
	$(CC) -o main.out main.o state.o render.o init.o mappings.o style.o $(CCFLAGS) $(LINKFLAGS)


debug: CCFLAGS += -g -O0 -Wall
debug: main


release: CCFLAGS += -Ofast
release: main


demo: release
	./main.out


clean:
	rm -f *.o


%.o: %.cc
	$(CC) -c $(CCFLAGS) $(INCLUDEFLAGS) $*.cc