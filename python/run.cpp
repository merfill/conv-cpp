
#include "conv.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <stdexcept>

namespace p = boost::python;
namespace np = boost::python::numpy;

np::ndarray gaussian_blur_gpu(np::ndarray& in_image, int kernel_size, float sigma) {
  intptr_t const * shape = in_image.get_shape();
  int dim = in_image.get_nd();
  if (dim != 3) {
    throw std::runtime_error("Incorrect dimension of input image, must be 3");
  }

  auto out_image = in_image.copy();
  uint8_t* in_image_data = reinterpret_cast<uint8_t*>(in_image.get_data());
  uint8_t* out_image_data = reinterpret_cast<uint8_t*>(out_image.get_data());

  conv::gaussian_blur_gpu(in_image_data, out_image_data, shape[0], shape[1], shape[2], sigma, kernel_size);

  return out_image;
}

np::ndarray gaussian_blur_cpu(np::ndarray& in_image, int kernel_size, float sigma) {
  intptr_t const * shape = in_image.get_shape();
  int dim = in_image.get_nd();
  if (dim != 3) {
    throw std::runtime_error("Incorrect dimension of input image, must be 3");
  }

  auto out_image = in_image.copy();
  uint8_t* in_image_data = reinterpret_cast<uint8_t*>(in_image.get_data());
  uint8_t* out_image_data = reinterpret_cast<uint8_t*>(out_image.get_data());

  conv::gaussian_blur_cpu(in_image_data, out_image_data, shape[0], shape[1], shape[2], sigma, kernel_size);

  return out_image;
}

BOOST_PYTHON_MODULE(conv) {
  np::initialize();

  p::def("gaussian_blur_gpu", gaussian_blur_gpu);
  p::def("gaussian_blur_cpu", gaussian_blur_cpu);
}

