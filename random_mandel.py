#!/usr/bin/python

import os,sys
import ctypes
import numpy
from PIL import Image

my_path = os.path.dirname(os.path.abspath(__file__))

_random_mandel = ctypes.cdll.LoadLibrary(my_path + '/_random_mandel.so')

if len(sys.argv) < 3:
    raise ValueError("Usage: ./random_mandel width height")

width = int(sys.argv[1])
height = int(sys.argv[2])
    
x = numpy.zeros([height,width,4],dtype = numpy.uint8)
_random_mandel.main_draw(x.ctypes.get_as_parameter(),ctypes.c_int32(width),ctypes.c_int32(height))
X = Image.fromarray(x[:,:,:3])
X.save(my_path + '/random_mandel.jpg')
