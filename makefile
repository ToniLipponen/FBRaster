all:
	$(MAKE) -C src && ar -rcs libFBRaster.a src/*.o

test:
	$(MAKE) && cc main.c -L. libFBRaster.a -lm -Ofast -march=native -mtune=native -flto -o test

clean:
	$(MAKE) -C src clean && rm libFBRaster.a && rm test