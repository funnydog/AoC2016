CXXFLAGS=-Wall -std=c++17 -O2
LDLIBS=-lfmt

.PHONY: all clean

all: day11

day11: day11.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day11
