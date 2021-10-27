#include "cuda_functions_1.h"

__global__ void mapEvenNums(int* d_input, int* d_output, const size_t dataSize);
__global__ void scan_upkernel(int* data, const size_t dataSize, int step);
__global__ void scan_downkernel(int* data, const size_t dataSize, int step);
__global__ void copy(int* d_input, int* d_output, const size_t dataSize);
__global__ void compact_final(int* d_input, int* d_output, const size_t dataSize, int* d_mapresult, int* d_scanresult);

__host__ void scan(int* d_input, int* d_output, const size_t dataSize, dim3 dimBlock, dim3 dimGrid);

//Host function
__host__ int* cudaCompactEven(int* data, const size_t dataSize, size_t* outSize, dim3 dimBlock, dim3 dimGrid)
{
	int* d_input = nullptr;
	int* d_mapresult = nullptr;
	int* d_scanresult = nullptr;
	int* d_finalresult = nullptr;

	cudaMalloc(&d_input, sizeof(int) * dataSize);
	cudaMalloc(&d_mapresult, sizeof(int) * dataSize);
	cudaMalloc(&d_scanresult, sizeof(int) * dataSize);
	cudaMalloc(&d_finalresult, sizeof(int) * dataSize);

	cudaMemcpy(d_input, data, sizeof(int) * dataSize, cudaMemcpyHostToDevice);


	mapEvenNums<<<dimGrid, dimBlock>>> (d_input, d_mapresult, dataSize);
	scan(d_mapresult, d_scanresult, dataSize, dimBlock, dimGrid);
	compact_final<<<dimGrid, dimBlock>>>(d_input, d_finalresult, dataSize, d_mapresult, d_scanresult);

	
	int* h_scanresult = (int*)malloc(sizeof(int) * dataSize);
	cudaMemcpy(h_scanresult, d_scanresult, sizeof(int) * dataSize, cudaMemcpyDeviceToHost);

	size_t resultSize = h_scanresult[dataSize -1];

	int* h_result = (int*)malloc(sizeof(int) * resultSize);
	cudaMemcpy(h_result, d_finalresult, sizeof(int) * resultSize, cudaMemcpyDeviceToHost);

	cudaFree(d_input);
	cudaFree(d_mapresult);
	cudaFree(d_scanresult);
	cudaFree(d_finalresult);
	free(h_scanresult);

	*outSize = resultSize;

	return h_result;
}

__host__ void scan(int* d_input, int* d_output, const size_t dataSize, dim3 dimBlock, dim3 dimGrid)
{
	copy<<<dimGrid, dimBlock>>>(d_input, d_output, dataSize);
	int step;
	for (step = 1; step < dataSize; step *= 2)
	{
		scan_upkernel<<<dimGrid, dimBlock>>>(d_output, dataSize, step);
	}
	step /= 2; //Biggest viable step
	for (step; step > 0; step /= 2)
	{
		scan_downkernel<<<dimGrid, dimBlock>>>(d_output, dataSize, step);
	}
}


//Device Map function
__global__ void mapEvenNums(int* d_input, int* d_output, const size_t dataSize)
{
	
	for (int idx = blockIdx.x * blockDim.x + threadIdx.x; idx < dataSize; idx += blockDim.x * gridDim.x)
	{
		if (idx < dataSize)
		{
			d_output[idx] = d_input[idx] * d_input[idx];

			if (d_input[idx] % 2 == 0)
				d_output[idx] = 1;
			else
				d_output[idx] = 0; 
		}
	}
	__syncthreads();
}

__global__ void copy(int* d_input, int* d_output, const size_t dataSize)
{
	for (int id = blockIdx.x * blockDim.x + threadIdx.x; id < dataSize; id += blockDim.x * gridDim.x)
	{
		d_output[id] = d_input[id];
	}
	__syncthreads();
}

__global__ void scan_upkernel(int* data, const size_t dataSize, int step)
{
	for (int id = blockIdx.x * blockDim.x + threadIdx.x; id < dataSize; id += blockDim.x * gridDim.x)
	{
		int sumIndex = dataSize - 1 - id * step * 2;

		if (sumIndex - step >= 0)
			data[sumIndex] += data[sumIndex - step];
	}
	__syncthreads();
}

__global__ void scan_downkernel(int* data, const size_t dataSize, int step)
{
	for (int id = blockIdx.x * blockDim.x + threadIdx.x; id < dataSize; id += blockDim.x * gridDim.x)
	{
		int sumIndex = dataSize - 1 - (id + 1) * step * 2 + step;

		if (sumIndex - step >= 0)
			data[sumIndex] += data[sumIndex - step];
	}
	__syncthreads();
}

__global__ void compact_final(int* d_input, int* d_output, const size_t dataSize, int* d_mapresult, int* d_scanresult)
{
	for (int id = blockIdx.x * blockDim.x + threadIdx.x; id < dataSize; id += blockDim.x * gridDim.x)
	{
		if (d_mapresult[id] == 1)
		{
			d_output[d_scanresult[id] - 1] = d_input[id];
		}
	}
	__syncthreads();
}