CXXFLAGS=-Wall -std=c++17 -O2
LDLIBS=-lfmt

.PHONY: all clean

all: day3

day3: day3.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day3
