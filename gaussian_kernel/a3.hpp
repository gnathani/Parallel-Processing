/*  Gaurav Laxmandas
 *  Nathani
 *  gauravla
 */

#ifndef A3_HPP
#define A3_HPP
#include <math.h>

/*
 * Constant Memory Variables (start with c_)
 */
__constant__ int c_n;
__constant__ float c_h;
__constant__ float c_common;

__global__ void process_gaussian_kde(const float *x, float *y, const int c_n, const float c_h, const float c_common) {
    /*
     * Shared Memory Variables (start with s_)
     */
    extern __shared__ float s_buf[];

    /*
     * Local Register Variables (start with r_)
     */
    float* r_buf = s_buf;
    float r_curr_x = x[blockDim.x * blockIdx.x + threadIdx.x];
    float r_val = 0.0;

    for (int i = 0; i < gridDim.x; i++) {
        /*
         * Copy one block of data to shared buffer
         */
        r_buf[threadIdx.x] = x[i * blockDim.x + threadIdx.x];
        __syncthreads();

        /*
         * Compute the sum for kernel summation per thread using shared block
         */
        for (int j = 0; j < blockDim.x && (i * blockDim.x + j < c_n); j++) {
            r_val += expf(-powf((r_curr_x - r_buf[j])/c_h, 2)/2);
        }
    }
    /*
     * Compute final result and store to resultant 
     */
    y[blockDim.x * blockIdx.x + threadIdx.x] = r_val * c_common;
}

void gaussian_kde(int n, float h, const std::vector<float>& x, std::vector<float>& y) {
    /*
     * Initializing device variables
     */
    float *d_x;
    float *d_y;
    int fl_sz = sizeof(float);
    long int size = n * fl_sz;
    cudaMalloc(&d_x, size);
    cudaMalloc(&d_y, size);
    cudaMemcpy(d_x, x.data(), size, cudaMemcpyHostToDevice);

    /*
     * Initializing constants for device
     */
    float first_term = 1.0 / (n * h * sqrtf(2 * M_PI));
    cudaMemcpyToSymbol(*(&c_common), &first_term, fl_sz, 0, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(*(&c_h), &h, fl_sz, 0, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(*(&c_n), &n, sizeof(int), 0, cudaMemcpyHostToDevice);

    /*
     * Initialzing dimensions
     */
    int thr_per_block = 1024;
    dim3 dimGrid((int)ceil(n/thr_per_block)+1);
    dim3 dimBlock(thr_per_block);

    /*
     * Call the CUDA device kernel
     */
    process_gaussian_kde<<<dimGrid, dimBlock, thr_per_block * fl_sz>>>(d_x, d_y, n, h, first_term);

    /*
     * Copy result to host and free device memory
     */
    cudaMemcpy(y.data(), d_y, size, cudaMemcpyDeviceToHost);
    cudaFree(d_x);
    cudaFree(d_y);
} // gaussian_kde

#endif // A3_HPP
