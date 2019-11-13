/*************************************************************************
	> File Name: data.cpp
	> Author:
	> Mail: 
	> Created Time: Tue 06 Aug 2019 12:00:41 PM UTC
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
using namespace std;
#include "stdio.h"
#include <math.h>
#include "sys/time.h"
#define DATA_COUNT 256*20
int main()
{
	FILE* f_input_data = fopen("input2.txt", "wb+");
	srand((unsigned int)(time(NULL)));
	float* input_data = (float*)malloc(DATA_COUNT * sizeof(float));
	for (int i = 0; i < DATA_COUNT; i++) {
		input_data[i] =  (rand()%20)/1.0;
		fprintf(f_input_data, "%f\n", input_data[i]);
	}
			
	return 0;
}
