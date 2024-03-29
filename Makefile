PROGRAMS = patterns

# Important optimization options
CFLAGS = -O3 -ffast-math -fno-rtti

# Standard libraries
LFLAGS = -lm -lstdc++ -lpthread

# Debugging
CFLAGS += -g -Wall
LFLAGS += -g

CFLAGS += -std=c++11

# Annoying warnings on by default on Mac OS
CFLAGS += -Wno-tautological-constant-out-of-range-compare -Wno-gnu-static-float-init -Wno-unused-variable


all: $(PROGRAMS)

.cpp:
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)

.PHONY: clean all

clean:
	rm -f $(PROGRAMS)
