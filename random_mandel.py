#!/usr/bin/python

import os
import ctypes
import numpy
from PIL import Image

my_path = os.path.dirname(os.path.abspath(__file__))

_random_mandel = ctypes.cdll.LoadLibrary(my_path + '/_random_mandel.so')

x = numpy.zeros([800,1400,4],dtype = numpy.uint8)
_random_mandel.main_draw(x.ctypes.get_as_parameter())
X = Image.fromarray(x[:,:,:3])
X.save(my_path + '/random_mandel.jpg')
