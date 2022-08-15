MAKEFLAGS += --no-builtin-rules --no-builtin-variables --warn-undefined-variables

GCC = g++
LIBS = -lm -larmadillo -lmlpack -lfmt

CPPFLAGS = -g -flto -O2 -pedantic -std=c++17 -MMD
WARNINGFLAGS = -Wall -Wextra -Wconversion -fopenmp -Wshadow -Wdouble-promotion -Wfloat-equal -Wnon-virtual-dtor -Woverloaded-virtual -Wfatal-errors
CDEBUGFLAGS = -fno-omit-frame-pointer

INCLUDES = $(CURDIR)
HEADERS = $(wildcard */*.h) $(wildcard */*/*.h) $(wildcard */*/*/*.h)
CPPS = $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp)
TEMPLATES = $(wildcard */*.tpp) $(wildcard */*/*.tpp) $(wildcard */*/*/*.tpp)
OBJECTS = $(patsubst %.cpp,%.o,$(CPPS))
DEPENDS = $(patsubst %.cpp,%.d,$(CPPS))

all: vpkde test meteors angles

%.o: %.cpp $(TEMPLATES) $(HEADERS) Makefile
	$(GCC) $(CPPFLAGS) $(CDEBUGFLAGS) $(WARNINGFLAGS) -I $(INCLUDES) -c $< -o $@

bin/%: $(OBJECTS) %.o
	mkdir -p bin
	$(GCC) $(CPPFLAGS) $(CDEBUGFLAGS) $(WARNINGFLAGS) -I $(INCLUDES) $^ $(LIBS) -o $@

clean:
	rm -f bin/*
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)

-include $(DEPENDS)

%.png: %.bin
	./visualize2d.py $< $@ 1000 1000 --points data/data.tsv

.SECONDEXPANSION:

allpic: \
	$$(subst .bin,.png,$$(wildcard out/*.bin));

distclean: \
	clean
