
CC = g++
CCFLAGS = -std=c++11 
INCLUDEFLAGS = -I.
LINKFLAGS = -lm `sdl2-config --cflags --libs`

# name of the executable to produce
EXE = ivory-tower
# location for the executable
EXE_LOC = ./

# source files used in building
SRCS = main.cc init.cc render.cc state.cc style.cc mappings.cc

# object files
OBJS = $(SRCS:.cc=.o)

# dependency files
DEPS = $(SRCS:.cc=.d)
-include $(DEPS)


# decide executable extension
ifeq ($(OS),Windows_NT)
EXT = exe
else
EXT = out
endif


all: debug

$(EXE): $(OBJS)
	$(CC) -o $(EXE_LOC)$(EXE).$(EXT) $^ $(CCFLAGS) $(LINKFLAGS)



debug: CCFLAGS += -g -O0 -Wall
debug: $(EXE)

debug++: CCFLAGS += -Wpedantic -Wextra -Werror
debug++: debug


release: CCFLAGS += -Ofast
release: $(EXE)


demo: release
	./$(EXE_LOC)$(EXE).$(EXT)



.PHONY: clean
clean:
	rm -f $(OBJS) $(DEPS)
	
.PHONY: cleaner
cleaner: clean
	rm -f $(EXE_LOC)$(EXE).$(EXT)



%.o: %.cc
	$(CC) -MM $< -MF $*.d -MT $*.o -MG -MP
	$(CC) -c -o $*.o $< $(CCFLAGS) $(INCLUDEFLAGS)
