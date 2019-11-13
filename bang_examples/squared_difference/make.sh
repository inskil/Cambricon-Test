
cncc -c squaredDiffKernel.mlu -o squarediffkernel.o
g++ -c main.cpp
g++ -c squaredDiff.cpp -I/usr/local/neuware/include
g++ squarediffkernel.o main.o squaredDiff.o -o square_test -L /usr/local/neuware/lib64 -lcnrt
