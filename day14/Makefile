CXXFLAGS=-Wall -std=c++17 -O2
LDLIBS=-lcrypto -lfmt

.PHONY: all clean

all: day14

day14: day14.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	@rm -f *.o day14
