CPPFLAGS += -fopenmp -O3 -g -march=native

all: sseflops avxflops

sseflops: sseflops.o
	$(CXX) -o $@ $< $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)

avxflops: avxflops.o
	$(CXX) -o $@ $< $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -f *.o