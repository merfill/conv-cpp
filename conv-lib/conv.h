
#ifndef CONV_H
#define CONV_H

#include <stdint.h>
#include <vector>

namespace conv {

void gaussian_blur_gpu(const uint8_t* in_img, uint8_t* out_img, size_t height, size_t width, size_t depth, float sigma, size_t ksize);
void gaussian_blur_cpu(const uint8_t* in_img, uint8_t* out_img, size_t height, size_t width, size_t depth, float sigma, size_t ksize);

std::vector<float> get_gaussian_kernel(float sigma, size_t ksize);

} // namespace conv


#endif
