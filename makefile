all: _random_mandel.so

_random_mandel.so: _random_mandel.c
	gcc _random_mandel.c -o _random_mandel.so -fPIC -shared -lm
