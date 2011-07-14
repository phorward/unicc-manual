
all: simple.xml simple.c

clean:
	rm -f simple.xml
	rm -f simple.c
	rm -f simple.h

simple.xml: simple.par
	unicc -s -x simple.par

simple.c: simple.par
	unicc -s simple.par

