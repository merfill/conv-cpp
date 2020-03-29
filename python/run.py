
import numpy as np
from PIL import Image
import sys

import conv

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("usage: run.py path_to_image_file kernel_size sigma")
        sys.exit(1)

    img = Image.open(sys.argv[1])
    np_img = np.array(img)

    res = conv.gaussian_blur_gpu(np_img, int(sys.argv[2]), float(sys.argv[3]))
    img2 = Image.fromarray(res)
    img1 = Image.fromarray(np_img)
    img1.show()
    img2.show()
