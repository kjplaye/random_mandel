prefix = /usr/local

all:
	$(CC) -g -o random-mandel random-mandel.c -ljpeg -lm -I/usr/include

install:
	install random-mandel $(DESTDIR)$(prefix)/bin
	install random-mandel.1 $(DESTDIR)$(prefix)/share/man/man1

clean:
	rm -f random-mandel
