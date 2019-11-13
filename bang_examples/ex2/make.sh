
cncc -c ex2Kernel.mlu -o ex2kernel.o
g++ -c main.cpp
g++ -c ex2_op.cpp -I/usr/local/neuware/include
g++ ex2kernel.o main.o ex2_op.o -o ex2_test -L /usr/local/neuware/lib64 -lcnrt
