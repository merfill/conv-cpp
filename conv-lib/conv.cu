
#include "blur_impl.h"

#include <cuda_runtime.h>

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cmath>
#include <iostream>


__global__
void gaussian_blur_rgb(const uint8_t* in_img, uint8_t* out_img, int height, int width, int ksize, const float* kernel) {
  // Current pixel coordinates
  int r = blockIdx.x * blockDim.x + threadIdx.x;
  int c = blockIdx.y * blockDim.y + threadIdx.y;

  RGB_BLUR();
}

__global__
void gaussian_blur_grey(const uint8_t* in_img, uint8_t* out_img, int height, int width, int ksize, const float* kernel) {
  // Current pixel coordinates
  int c =  blockIdx.y * blockDim.y + threadIdx.y;
  int r =  blockIdx.x * blockDim.x + threadIdx.x;

  GREY_BLUR();
}

#define CUDA_CHECK(call) {\
  {\
    cudaError_t err = call;\
    if (cudaSuccess != err) {\
      throw std::runtime_error(cudaGetErrorString(err));\
    }\
  }\
}

namespace conv {

std::vector<float> get_gaussian_kernel(float sigma, size_t ksize) {
  // Variance inherited from sigma value
  float variance = 2. * sigma * sigma;

  // Sum of all kernel values for the normalization
  float sum = 0.;

  // Generate pure kernel values
  std::vector<float> kernel(ksize * ksize, 0.);
  size_t kindex = 0; // kernel index
  int mean = ksize / 2;
  for (int x = 0; x < ksize; ++x) {
    for (int y = 0; y < ksize; ++y) {
      float xr = x - mean, yr = y - mean;
      float r = sqrtf(xr * xr + yr * yr);
      kernel[kindex] = (std::exp(-(r * r) / variance) )/ (3.141592 * variance);
      sum += kernel[kindex];
      ++kindex;
    }
  }

  // Normalize kernel values
  if (sum < 0.e-6) {
    sum = 0.e-6;
  }
  float s = 0.;
  for (auto i = 0; i < ksize * ksize; ++i) {
    kernel[i] /= sum;
    s += kernel[i];
  }

  return kernel;
}

int ceiling(const int all_size, const int block_size) {
  return (all_size + block_size - 1) / block_size;
}

void gaussian_blur_gpu(const uint8_t* in_img, uint8_t* out_img, size_t height, size_t width, size_t depth, float sigma, size_t ksize) {
  // Device memory
  uint8_t *d_src = 0, *d_dst = 0;
  float *d_kernel = 0;
  size_t bytes = height * width * depth;

  typedef std::unique_ptr<uint8_t, cudaError_t(*)(void*)> cuda_uint8_ptr;
  typedef std::unique_ptr<float, cudaError_t(*)(void*)> cuda_float_ptr;

  // Allocate memory on GPU
  CUDA_CHECK(cudaMalloc(&d_src, bytes));
  cuda_uint8_ptr d_src_ptr(d_src, cudaFree);
  CUDA_CHECK(cudaMalloc(&d_dst, bytes));
  cuda_uint8_ptr d_dst_ptr(d_dst, cudaFree);
  CUDA_CHECK(cudaMalloc(&d_kernel, ksize * ksize * sizeof(float)));
  cuda_float_ptr d_kernel_ptr(d_kernel, cudaFree);

  // Generate kernel
  auto kernel = get_gaussian_kernel(sigma, ksize);

  // Copy host data to device memory
  CUDA_CHECK(cudaMemcpy(d_src, in_img, bytes, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemset(d_dst, 0, bytes));
  CUDA_CHECK(cudaMemcpy(d_kernel, (void*)&kernel[0], kernel.size() * sizeof(float), cudaMemcpyHostToDevice));

  // Calculate convolution
  const int block_size = 4;
  int grid_x = ceiling(height, block_size);
  int grid_y = ceiling(width, block_size);
  dim3 block(block_size, block_size);
  dim3 grid(grid_x, grid_y);
  switch (depth) {
    case 1:
      gaussian_blur_grey<<<grid, block>>>(d_src, d_dst, height, width, ksize, d_kernel);
      break;
    case 3:
      gaussian_blur_rgb<<<grid, block>>>(d_src, d_dst, height, width, ksize, d_kernel);
      break;
    default:
      throw std::runtime_error("incorrect color depth passed. Must be 1 or 3");
  }

  CUDA_CHECK(cudaGetLastError());
  CUDA_CHECK(cudaThreadSynchronize());

  // Copy data back to host
  CUDA_CHECK(cudaMemcpy(&out_img[0], d_dst, bytes, cudaMemcpyDeviceToHost));
}

void gaussian_blur_cpu(const uint8_t* in_img, uint8_t* out_img, size_t height, size_t width, size_t depth, float sigma, size_t ksize) {
  auto kernel = get_gaussian_kernel(sigma, ksize);
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      RGB_BLUR();
    }
  }
}

} // namespace conv

