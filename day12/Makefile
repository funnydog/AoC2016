CXXFLAGS=-Wall -std=c++17 -O2
LDFLAGS=-lfmt

.PHONY: all clean

all: day12

day12: day12.o
	$(CXX) $(LDLIBS) -o $@ $^ $(LDFLAGS)

clean:
	@rm -f *.o day12
