# Default to 'python', fallback to 'python3' if not found
PYTHON ?= $(shell command -v python || command -v python3)

all: _random_mandel.so

test: _random_mandel.so test_random_mandel.py
	$(PYTHON) ./test_random_mandel.py

_random_mandel.so: _random_mandel.c
	gcc _random_mandel.c -o _random_mandel.so -fPIC -shared -lm
