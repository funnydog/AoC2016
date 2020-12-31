CXXFLAGS=-Wall -std=c++17 -O2
LDFLAGS=-lfmt

.PHONY: all clean

all: day7

day7: day7.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o day7
