
CC = gcc
CCFLAGS = -fms-extensions -Wall
INCLUDEFLAGS = -I.
LINKFLAGS = -lSDL2 -lm


all: debug


main: main.o state.o render.o init.o mappings.o style.o sl_arr.o
	$(CC) -o main.out main.o state.o render.o init.o mappings.o style.o sl_arr.o $(CCFLAGS) $(LINKFLAGS)


debug: CCFLAGS += -g -O0
debug: main


release: CCFLAGS += -Ofast
release: main


demo: release
	./main.out


clean:
	rm -f *.o


%.o: %.c
	$(CC) -c $(CCFLAGS) $(INCLUDEFLAGS) $*.c