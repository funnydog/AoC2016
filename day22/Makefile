CXXFLAGS=-Wall -std=c++17 -O2
LDLIBS=-lfmt

.PHONY: all clean

all: day22

day22: day22.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day22
