ifneq ($(findstring clang,$(CXX)),)
CLANG    := 1
LDFLAGS  += -mavx -g
CPPFLAGS += -mavx -g -O3 \
	-mllvm -pre-RA-sched=fast \
	# -mllvm -vectorize \
	# -mllvm -bb-vectorize-aligned-only \
	# -mllvm -bb-vectorize-vector-bits=256 \
	# -mllvm -bb-vectorize-req-chain-depth=4 \
	# -mllvm -bb-vectorize-search-limit=16 \
	# -mllvm -bb-vectorize-fast-dep \
	# -mllvm -unroll-allow-partial \
	# -mllvm -bb-vectorize-pow2-len-only \
	# -mllvm -vectorize \
	# -Xclang -load -Xclang /usr/local/clang-3.2/lib/LLVMPolly.dylib \
	# -mllvm -polly \
	# -mllvm -polly-vectorizer=polly
	# -Xclang -load -Xclang /usr/local/clang-3.2/lib/LLVMHello.dylib \
	# -mllvm -hello \

else
ifeq ($(CXX),icc)
LDFLAGS  += -march=native -fopenmp -g -fast
CPPFLAGS += -march=native -fopenmp -g -fast -vec-report
else
LDFLAGS  += -march=native -fopenmp -g
CPPFLAGS += -march=native -fopenmp -g -O3 \
	-fno-schedule-insns \
	-fno-schedule-insns2 \
	# -ftree-vectorizer-verbose=1 \
	# -fgcse-after-reload \
	# -finline-functions \
	# -fipa-cp-clone \
	# -fpredictive-commoning \
	# -ftree-loop-distribute-patterns \
	# -ftree-vectorize \
	# -funswitch-loops \
	# -ffast-math \

endif
endif

ifneq ($(CXX),c++)
ifneq ($(CXX),g++)
SPACE  := $(empty) $(empty)
SUFFIX := -$(subst $(SPACE),_,$(CXX))
endif
endif

TESTS := vecflops$(SUFFIX) sseflops$(SUFFIX) avxflops$(SUFFIX)

all: $(TESTS)

clean:; rm -f $(TESTS) $(addsuffix .o, $(TESTS))

.SECONDARY:

%$(SUFFIX).o:: %.cc Makefile
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(CPPFLAGS)
ifeq ($(CLANG),1)
	$(CXX) -o $(@:.o=.ll) -S -emit-llvm -c $< $(CXXFLAGS) $(CPPFLAGS)
endif

%$(SUFFIX): %$(SUFFIX).o
	$(CXX) -o $@ $< $(LDFLAGS)
