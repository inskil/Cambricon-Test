#include <cmath>
#include "stdio.h"
#include <stdlib.h>
#include <sys/time.h>

#define DATA_COUNT 32768
int MLUEx2Op(float* input1 ,float*output, int dims_a);

int main() {
  float* input_data = (float*)malloc(DATA_COUNT * sizeof(float));
  float* output_data = (float*)malloc(DATA_COUNT * sizeof(float));
  float* output_data_cpu = (float*)malloc(DATA_COUNT * sizeof(float));
  FILE* f_input_data = fopen("../data/in_x.txt", "r");
  struct timeval tpend, tpstart;
  float err = 0.0;
  float cpu_sum = 0.0;
  float time_use = 0.0;

  if (f_input_data == NULL) {
    printf("Open file fail!\n");
    return 0;
  }
  
  gettimeofday(&tpstart, NULL);
  for (int i = 0; i < DATA_COUNT; i++) {
    fscanf(f_input_data, "%f\n", &input_data[i]);
  }
  gettimeofday(&tpend, NULL);
  time_use = 1000000 * (tpend.tv_sec - tpstart.tv_sec)+ tpend.tv_usec - tpstart.tv_usec;
  printf("get data cost time %f ms\n", time_use/1000.0);

  gettimeofday(&tpstart, NULL);
  MLUEx2Op(input_data, output_data,DATA_COUNT);
  gettimeofday(&tpend, NULL);
  time_use = 1000000 * (tpend.tv_sec - tpstart.tv_sec)+ tpend.tv_usec - tpstart.tv_usec;
  printf("compute cost time %f ms\n", time_use/1000.0);
#if 1 
  for(int i = 0; i < DATA_COUNT;++i)
  {
     if (input_data[i] < 0.01 ) continue;
     output_data_cpu[i] = log(input_data[i])/(1+exp(-0.01* input_data[i]));
   //  printf("x[%d]= %f,mlu result %f\n cpu_result %f\n",i,input_data[i], output_data[i],output_data_cpu[i]);
     err +=fabs(output_data_cpu[i] - output_data[i]) ; 
     cpu_sum +=fabs(output_data_cpu[i]);
  }
     printf("err rate %f\n", err*100/cpu_sum);
#endif
  return 0;
}
