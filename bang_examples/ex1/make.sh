
cncc -c ex1Kernel.mlu -o ex1kernel.o
g++ -c main.cpp
g++ -c ex1_op.cpp -I/usr/local/neuware/include
g++ ex1kernel.o main.o ex1_op.o -o ex1_test -L /usr/local/neuware/lib64 -lcnrt
