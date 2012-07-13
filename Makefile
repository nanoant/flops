CPPFLAGS += -fopenmp -O3 -g -march=native

ifneq ($(CXX),c++)
ifneq ($(CXX),g++)
SUFFIX := -$(CXX)
endif
endif

TESTS := sseflops$(SUFFIX) avxflops$(SUFFIX)

all: $(TESTS)

clean:; rm -f $(TESTS) $(addsuffix .o, $(TESTS))

.SECONDARY:

%$(SUFFIX).o:: %.cc
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(CPPFLAGS)

%$(SUFFIX): %$(SUFFIX).o
	$(CXX) -o $@ $< $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
