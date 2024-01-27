# Random Mandel

Create random Mandelbrot zooms, useful for a daily wallpaper.

## Get random-mandel in Ubuntu

    sudo add-apt-repository ppa:kjplaye/ppa
    sudo apt update
    sudo apt install random-mandel
    man random-mandel
    random-mandel <out.jpg> <pixel_width> <pixel_height>

## Get random_mandel from source

Git clone this repo and run make.

## Basic usage

    random_mandel.py <x_resolution> <y_resolution>

## Gallery

![alt text](https://github.com/kjplaye/random_mandel/blob/master/example1.png?raw=true)

![alt text](https://github.com/kjplaye/random_mandel/blob/master/example2.png?raw=true)

![alt text](https://github.com/kjplaye/random_mandel/blob/master/example3.png?raw=true)

![alt text](https://github.com/kjplaye/random_mandel/blob/master/example4.png?raw=true)

## Typical steps to set up

1. make.
2. Run `random_mandel.py` as in `Basic usage` above to create `random_mandel.jpg`.
3. Point your desktop wallpaper to `random_mandel.jpg`.
4. Add step #2 to startup scripts and custom launchers such as cron.
