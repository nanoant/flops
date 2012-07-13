CPPFLAGS += -fopenmp -O3 -g -march=native

all: avxflops sseflops

avxflops: avxflops.o
	$(CXX) -o $@ $< $(CPPFLAGS) $(LDFLAGS)

sseflops: sseflops.o
	$(CXX) -o $@ $< $(CPPFLAGS) $(LDFLAGS)
