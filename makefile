all:
	$(MAKE) -C src && ar -rcs libFBRaster.a src/*.o

test:
	$(MAKE) && cc example.c -L. libFBRaster.a -lm -Ofast -march=native -mtune=native -flto -o example

clean:
	$(MAKE) -C src clean && rm libFBRaster.a && rm example
