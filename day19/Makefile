CXXFLAGS=-Wall -std=c++17 -O2
LDLIBS=-lfmt

.PHONY: all clean

all: day19

day19: day19.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day19
