FLOPs
=====

This is project for awesome code of [Mystical](http://stackoverflow.com/users/922184/mysticial) available at ["how to achieve 4 flops per cycle"](http://stackoverflow.com/questions/8389648/how-to-achieve-4-flops-per-cycle/8391601#8391601).

Usage
-----

Just `make` then run. If you do not have min. *Sandy Bridge / AVX* CPU `avxflops` build will fail, but you can still test with `sseflops`.

Use `make CXX=c++compiler` to build test with other compiler, the output files will be suffixed with `-c++compiler`.
