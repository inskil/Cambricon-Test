#include <stdlib.h>
#include "cnrt.h"
#include "cnrt_data.h"
#include "stdio.h"
#ifdef __cplusplus
extern "C" {
#endif
void ex2Kernel(half* input,half* output, int32_t len);
#ifdef __cplusplus
}
#endif
void ex2Kernel(half* input, half* output, int32_t len);

int MLUEx2Op(float* input1, float*output, int dims_a) {
  
  cnrtInit(0);
  cnrtDev_t dev;
  cnrtGetDeviceHandle(&dev, 0);
  cnrtSetCurrentDevice(dev);
  cnrtQueue_t pQueue;
  cnrtCreateQueue(&pQueue);
  cnrtDim3_t dim;
  dim.x = 1;
  dim.y = 1;
  dim.z = 1;
  float hardware_time = 0.0;
  cnrtNotifier_t notifier_start;
  cnrtNotifier_t notifier_end;
  cnrtCreateNotifier(&notifier_start);
  cnrtCreateNotifier(&notifier_end);
  cnrtFunctionType_t c = CNRT_FUNC_TYPE_BLOCK;

  //prepare data
 // output[0] = 0;
  half* input1_half = (half*)malloc(dims_a * sizeof(half));
  half* output_half = (half*)malloc(dims_a * sizeof(half));
  cnrtConvertFloatToHalfArray(input1_half, input1, dims_a);
  cnrtConvertFloatToHalfArray(output_half, output,dims_a);
 
  half *mlu_input1,*mlu_input2, *mlu_output;
  if (CNRT_RET_SUCCESS != cnrtMalloc((void**)&mlu_input1, dims_a * sizeof(half))) {
    printf("cnrtMalloc Failed!\n");
    exit(-1);
  }
  if (CNRT_RET_SUCCESS != cnrtMalloc((void**)&mlu_output, dims_a * sizeof(half))) {
    printf("cnrtMalloc output Failed!\n");
    exit(-1);
  }
  if (CNRT_RET_SUCCESS != cnrtMemcpy(mlu_input1, input1_half,  dims_a * sizeof(half), CNRT_MEM_TRANS_DIR_HOST2DEV)) {
    printf("cnrtMemcpy Failed!\n");
    exit(-1);
  }
  //kernel parameters
  cnrtKernelParamsBuffer_t params;
  cnrtGetKernelParamsBuffer(&params);
  cnrtKernelParamsBufferAddParam(params, &mlu_input1, sizeof(half*)); 
  cnrtKernelParamsBufferAddParam(params, &mlu_output, sizeof(half*)); 
  cnrtKernelParamsBufferAddParam(params, &dims_a, sizeof(int)); 
  cnrtPlaceNotifier(notifier_start, pQueue);
  if ( CNRT_RET_SUCCESS != cnrtInvokeKernel_V2((void*)&ex2Kernel, dim, params, c, pQueue)) {
    printf("cnrtInvokeKernel Failed!\n");
    exit(-1);
  }
  if (CNRT_RET_SUCCESS != cnrtSyncQueue(pQueue))
  {
    printf("syncQueue Failed!\n");
    exit(-1);
    
  }
  cnrtPlaceNotifier(notifier_end, pQueue);
  cnrtNotifierDuration(notifier_start, notifier_end, &hardware_time);
  printf("Hardware Total Time: %.3f ms\n", hardware_time / 1000.0);
  //get output data
  if (CNRT_RET_SUCCESS != cnrtMemcpy(output_half, mlu_output, dims_a* sizeof(half),CNRT_MEM_TRANS_DIR_DEV2HOST)){
    printf("cnrtMemcpy Failed!\n");
    exit(-1);
  }
  
  cnrtConvertHalfToFloatArray(output, output_half,dims_a );

  //free data
  if (CNRT_RET_SUCCESS != cnrtFree(mlu_input1)) {
    printf("cnrtFree Failed!\n");
    exit(-1);
  }
  if (CNRT_RET_SUCCESS != cnrtFree(mlu_output)) {
    printf("cnrtFree output Failed!\n");
    exit(-1);
  }
  if (CNRT_RET_SUCCESS != cnrtDestroyQueue(pQueue)) {
    printf("cnrtDestroyQueue Failed!\n");
    exit(-1);
  }
  if (CNRT_RET_SUCCESS != cnrtDestroyKernelParamsBuffer(params)) {
    printf("cnrtDestroyKernelParamsBuffer Failed!\n");
    return -1;
  }
  cnrtDestroy();
  free(input1_half);
  free(output_half);
  return 0;
}
