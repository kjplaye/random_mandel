# PYTHON must be provided; default fallback for local
PYTHON ?= python3

all: _random_mandel.so

test: _random_mandel.so test_random_mandel.py
	$(PYTHON) ./test_random_mandel.py
	clang-format --dry-run --Werror _random_mandel.c

_random_mandel.so: _random_mandel.c
	gcc _random_mandel.c -o _random_mandel.so -fPIC -shared -lm
