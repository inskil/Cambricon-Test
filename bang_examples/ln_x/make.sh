
cncc -c lnKernel.mlu -o lnkernel.o
g++ -c main.cpp
g++ -c ln_op.cpp -I/usr/local/neuware/include
g++ lnkernel.o main.o ln_op.o -o ln_test -L /usr/local/neuware/lib64 -lcnrt
