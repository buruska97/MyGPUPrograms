#pragma once
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>


__host__ int* cudaCompactEven(int* data, const size_t dataSize, size_t* outSize, dim3 dimBlock, dim3 dimGrid);
