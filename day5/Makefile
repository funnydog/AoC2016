CXXFLAGS=-Wall -std=c++17 -O2
LDFLAGS=-lcrypto -lfmt

.PHONY: all clean

all: day5

day5: day5.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f *.o day5
