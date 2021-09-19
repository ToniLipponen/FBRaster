all:
	$(MAKE) -C src && ar -rcs libFBRaster.a src/*.o

example:
	$(MAKE) && cc example.c -L. libFBRaster.a -lm -O2 -msse3 -o example

clean:
	$(MAKE) -C src clean && rm libFBRaster.a && rm example
