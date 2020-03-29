
#ifndef BLUR_IMPL_H
#define BLUR_IMPL_H

#include <cstdint>
#include <cmath>


#define RGB_BLUR()\
  /* Do not process pixels that out of image */\
  if (c < width and r < height) {\
    float red_blur = 0.; /* result red pixel component value */\
    float green_blur = 0.; /* result green pixel component value */\
    float blue_blur = 0.; /* result blue pixel component value */\
\
    int k = 0; /* kernel iteration index */\
    for (int i = 0; i < ksize; ++i) {\
      for (int j = 0; j < ksize; ++j) {\
        /* Get current position in the image */\
        int w = r - ksize / 2 + i;\
        int h = c - ksize / 2 + j;\
\
        if (w >= 0 and w < height and h >= 0 and h < width) {\
          /* Calculate product of the pixel component to it's weight in gaussian distribution */\
          int im_idx = width * 3 * w + h * 3;\
          red_blur += static_cast<float>(in_img[im_idx]) * kernel[k];\
          green_blur += static_cast<float>(in_img[im_idx + 1]) * kernel[k];\
          blue_blur += static_cast<float>(in_img[im_idx + 2]) * kernel[k];\
        }\
        ++k;\
      }\
    }\
\
    int idx = width * r * 3 + c * 3;\
    out_img[idx] = static_cast<uint8_t>(red_blur);\
    out_img[idx + 1] = static_cast<uint8_t>(green_blur);\
    out_img[idx + 2] = static_cast<uint8_t>(blue_blur);\
  }\


#define GREY_BLUR()\
  /* Do not process pixels that out of image */\
  if (c < width and r < height) {\
    float blur = 0.; /* result pixel value */\
\
    int k = 0; /* kernel iteration index */\
    for (int i = 0; i < ksize; ++i) {\
      for (int j = 0; j < ksize; ++j) {\
        /* Get current position in the image */\
        int w = r - ksize / 2 + i;\
        int h = c - ksize / 2 + j;\
\
        /* Skip pixel if it's out of the image bounderies */\
        if (w >= 0 and w < height and h >= 0 and h < width) {\
          /* Calculate product of the pixel to it's weight in gaussian distribution */\
          blur += static_cast<float>(in_img[width * w + h]) * kernel[k];\
        }\
        ++k;\
      }\
    }\
\
    out_img[c + r * width]= static_cast<uint8_t>(blur);\
  }\

#endif
