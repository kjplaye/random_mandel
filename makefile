all: _random_mandel.so

test: _random_mandel.so test_random_mandel.py
	python3 ./test_random_mandel.py

_random_mandel.so: _random_mandel.c
	gcc _random_mandel.c -o _random_mandel.so -fPIC -shared -lm
