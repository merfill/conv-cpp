
#define BOOST_TEST_MODULE convcpp
#define BOOST_AUTO_TEST_MAIN

#include "blur_impl.h"
#include "conv.h"

#include <iostream>

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(Conv_GaussianKernel_CheckConsistance) {
  for (size_t ksize = 1; ksize <= 42; ++ksize) {
    for (float sigma = .1; sigma <= 15.; sigma += .1) {
      std::vector<float> kernel = conv::get_gaussian_kernel(sigma, ksize);
      float sum = 0.;
      for (float k : kernel) {
        sum += k;
      }
      if (std::abs(sum - 1.) >= .01) {
        BOOST_ERROR("Consistance failed");
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(Conv_GaussianKernel_DotProductInside) {
  std::vector<float> kernel = conv::get_gaussian_kernel(3., 3);
  std::vector<float> check = { 0.106997, 0.11311, 0.106997, 0.11311, 0.119572, 0.11311, 0.106997, 0.11311, 0.106997 };
  for (size_t i = 0; i < check.size(); ++i) {
    if (std::abs(check[i] - kernel[i]) >= .0001) {
      BOOST_ERROR("Consistance failed on index: " << i);
    }
  }
}

BOOST_AUTO_TEST_CASE(Conv_GreyBlur_OddSizeKernel) {
  int width = 5, height = 4, ksize = 3;

  std::vector<uint8_t> in_img = {
      34 , 255, 22 , 15 , 11
    , 202, 21 , 42 , 67 , 128
    , 33 , 15 , 243, 123, 183
    , 22 , 90 , 234, 126, 231
  };

  std::vector<uint8_t> out_img(height * width);
  std::vector<float> kernel = conv::get_gaussian_kernel(3., ksize);

  int r = 2;
  int c = 1;

  GREY_BLUR();

  // Calculate dot product manually
  float dot_product = 0.;
  size_t k = 0;
  for (size_t i = 1; i < 4; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      dot_product += in_img[width * i + j] * kernel[k];
      ++k;
    }
  }

  BOOST_CHECK_EQUAL(out_img[width * r + c], static_cast<uint8_t>(dot_product));
}

BOOST_AUTO_TEST_CASE(Conv_GreyBlur_EvenSizeKernel) {
  int width = 5, height = 4, ksize = 4;

  std::vector<uint8_t> in_img = {
      34 , 255, 22 , 15 , 11
    , 202, 21 , 42 , 67 , 128
    , 33 , 15 , 243, 123, 183
    , 22 , 90 , 234, 126, 231
  };

  std::vector<uint8_t> out_img(height * width);
  std::vector<float> kernel = conv::get_gaussian_kernel(3., ksize);

  int r = 1;
  int c = 3;

  GREY_BLUR();

  // Calculate dot product manually
  float dot_product = 0.;
  size_t k = ksize;
  for (size_t i = 0; i < 3; ++i) {
    for (size_t j = 1; j < 5; ++j) {
      dot_product += in_img[width * i + j] * kernel[k];
      ++k;
    }
  }

  BOOST_CHECK_EQUAL(out_img[width * r + c], static_cast<uint8_t>(dot_product));
}

BOOST_AUTO_TEST_CASE(Conv_GreyBlur_Integral) {
  int width = 5, height = 4, ksize = 3;

  std::vector<uint8_t> in_img_orig = {
      34 , 255, 22 , 15 , 11
    , 202, 21 , 42 , 67 , 128
    , 33 , 15 , 243, 123, 183
    , 22 , 90 , 234, 126, 231
  };

  struct Checker {
    std::vector<uint8_t>& img_;
    Checker(std::vector<uint8_t>& img) : img_(img) {}

    uint8_t& operator[](int i) {
      if (i < 0 or i > static_cast<int>(img_.size())) {
        BOOST_ERROR("incorrect index: " << i);
      }
      return img_[i];
    }
  };
  Checker in_img(in_img_orig);

  std::vector<uint8_t> out_img(height * width);
  std::vector<float> kernel = conv::get_gaussian_kernel(3., ksize);

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      GREY_BLUR();
    }
  }

  std::vector<uint8_t> check_img  = {58, 65, 47, 31, 24, 62, 96, 88, 91, 59, 42, 99, 108, 152, 97, 18, 70, 94, 127, 75};
  for (size_t i = 0; i < check_img.size(); ++i) {
    BOOST_CHECK_EQUAL(out_img[i], check_img[i]);
  }
}

BOOST_AUTO_TEST_CASE(Conv_RgbBlur_OddSizeKernel) {
  int width = 5, height = 4, ksize = 3;

  std::vector<uint8_t> in_img = {
      34 , 14 , 21 ,  255, 205, 32 ,  22 , 125, 111,  15 , 31 , 112,  11 , 231, 97
    , 202, 212, 198,  67 , 12 , 253,  128, 128, 245,  21 , 0  , 214,  35 , 67 , 4
    , 33 , 15 , 243,  123, 183, 45 ,  7  , 8  , 5  ,  23 , 123, 57 ,  90 , 91 , 234
    , 22 , 90 , 234,  126, 231, 6  ,  121, 255, 243,  0  , 0  , 0  ,  56 , 78 , 125
  };

  std::vector<uint8_t> out_img(height * width * 3);
  std::vector<float> kernel = conv::get_gaussian_kernel(3., ksize);

  int r = 2;
  int c = 1;

  RGB_BLUR();

  // Calculate dot product manually
  float red_dot_product = 0., green_dot_product = 0., blue_dot_product = 0.;
  size_t k = 0;
  for (size_t i = 1; i < 4; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      red_dot_product += in_img[width * i * 3 + j * 3] * kernel[k];
      green_dot_product += in_img[width * i * 3 + j * 3 + 1] * kernel[k];
      blue_dot_product += in_img[width * i * 3 + j * 3 + 2] * kernel[k];
      ++k;
    }
  }

  BOOST_CHECK_EQUAL(out_img[width * r * 3 + c * 3], static_cast<uint8_t>(red_dot_product));
  BOOST_CHECK_EQUAL(out_img[width * r * 3 + c * 3 + 1], static_cast<uint8_t>(green_dot_product));
  BOOST_CHECK_EQUAL(out_img[width * r * 3 + c * 3 + 2], static_cast<uint8_t>(blue_dot_product));
}

BOOST_AUTO_TEST_CASE(Conv_RgbBlur_EvenSizeKernel) {
  int width = 5, height = 4, ksize = 4;

  std::vector<uint8_t> in_img = {
      34 , 14 , 21 ,  255, 205, 32 ,  22 , 125, 111,  15 , 31 , 112,  11 , 231, 97
    , 202, 212, 198,  67 , 12 , 253,  128, 128, 245,  21 , 0  , 214,  35 , 67 , 4
    , 33 , 15 , 243,  123, 183, 45 ,  7  , 8  , 5  ,  23 , 123, 57 ,  90 , 91 , 234
    , 22 , 90 , 234,  126, 231, 6  ,  121, 255, 243,  0  , 0  , 0  ,  56 , 78 , 125
  };

  std::vector<uint8_t> out_img(height * width * 3);
  std::vector<float> kernel = conv::get_gaussian_kernel(3., ksize);

  int r = 1;
  int c = 3;

  RGB_BLUR();

  // Calculate dot product manually
  float red_dot_product = 0., green_dot_product = 0., blue_dot_product = 0.;
  size_t k = ksize;
  for (size_t i = 0; i < 3; ++i) {
    for (size_t j = 1; j < 5; ++j) {
      red_dot_product += in_img[width * i * 3 + j * 3] * kernel[k];
      green_dot_product += in_img[width * i * 3 + j * 3 + 1] * kernel[k];
      blue_dot_product += in_img[width * i * 3 + j * 3 + 2] * kernel[k];
      ++k;
    }
  }

  BOOST_CHECK_EQUAL(out_img[width * r * 3 + c * 3], static_cast<uint8_t>(red_dot_product));
  BOOST_CHECK_EQUAL(out_img[width * r * 3 + c * 3 + 1], static_cast<uint8_t>(green_dot_product));
  BOOST_CHECK_EQUAL(out_img[width * r * 3 + c * 3 + 2], static_cast<uint8_t>(blue_dot_product));
}

BOOST_AUTO_TEST_CASE(Conv_RgbBlur_Integral) {
  int width = 5, height = 4, ksize = 5;

  std::vector<uint8_t> in_img_orig = {
      34 , 14 , 21 ,  255, 205, 32 ,  22 , 125, 111,  15 , 31 , 112,  11 , 231, 97
    , 202, 212, 198,  67 , 12 , 253,  128, 128, 245,  21 , 0  , 214,  35 , 67 , 4
    , 33 , 15 , 243,  123, 183, 45 ,  7  , 8  , 5  ,  23 , 123, 57 ,  90 , 91 , 234
    , 22 , 90 , 234,  126, 231, 6  ,  121, 255, 243,  0  , 0  , 0  ,  56 , 78 , 125
  };

  struct Checker {
    std::vector<uint8_t>& img_;

    Checker(std::vector<uint8_t>& img) : img_(img) {}

    uint8_t& operator[](int i) {
      if (i < 0 or i > static_cast<int>(img_.size())) {
        BOOST_ERROR("incorrect index: " << i);
      }
      return img_[i];
    }
  };
  Checker in_img(in_img_orig);

  std::vector<uint8_t> out_img(height * width * 3);
  std::vector<float> kernel = conv::get_gaussian_kernel(3., ksize);

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      RGB_BLUR();
    }
  }

  std::vector<uint8_t> check_img  = {
    37, 38, 48,  41, 45, 64,  43, 59, 75,   31, 49, 59,  14, 34, 44,
    48, 60, 68,  53, 69, 86,  57, 85, 101,  43, 71, 76,  21, 46, 59,
    47, 61, 69,  52, 71, 86,  57, 86, 102,  43, 71, 76,  22, 46, 59,
    33, 47, 60,  37, 55, 71,  42, 62, 82,   32, 49, 58,  19, 30, 45
  };
  for (size_t i = 0; i < check_img.size(); ++i) {
    BOOST_CHECK_EQUAL(out_img[i], check_img[i]);
  }
}
