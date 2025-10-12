#!/usr/bin/python3

# Debug CI
import sys
print("Running with:", sys.executable)
import numpy
print("numpy version:", numpy.__version__)


import os, sys
import ctypes
import numpy as np
from PIL import Image
from pathlib import Path


# Record the local path.
local_path = Path(os.path.dirname(os.path.abspath(__file__)))

# Parse arguments.
if len(sys.argv) < 3:
    print("Usage: ./random_mandel width height [write_path]")
    sys.exit(1)

width = int(sys.argv[1])
height = int(sys.argv[2])
if len(sys.argv) >= 4:
    write_path = Path(sys.argv[3])
else:
    write_path = local_path / 'random_mandel.jpg'


# Load library using ctypes.
_random_mandel = ctypes.cdll.LoadLibrary(local_path / \
                                         '_random_mandel.so')


# Draw fractal.
x_max = 0
while x_max == 0:
    x = np.zeros([height,width,4],dtype = np.uint8)
    _random_mandel.main_draw(x.ctypes._as_parameter_,
                             ctypes.c_int32(width),
                             ctypes.c_int32(height))
    x_max = x.max()

    
# Save.
X = Image.fromarray(x[:,:,:3])
X.save(write_path)
