CXXFLAGS=-Wall -std=c++17 -O2
LDFLAGS=-lfmt

.PHONY: all clean

all: day6

day6: day6.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o day6
