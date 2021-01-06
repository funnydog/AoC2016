CXXFLAGS=-Wall -std=c++17 -O2
LDLIBS=-lfmt

.PHONY: all clean

all: day20

day20: day20.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day20
