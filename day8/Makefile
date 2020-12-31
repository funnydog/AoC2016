CXXFLAGS=-Wall -std=c++17 -O2
LDFLAGS=-lfmt

.PHONY: all clean

all: day8

day8: day8.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o day8
