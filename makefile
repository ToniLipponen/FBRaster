all:
	make -C src && ar -rcs libFBRaster.a src/*.o

test:
	make && cc main.c -L. libFBRaster.a -lm -O2 -flto -msse3 -o test

clean:
	make -C src clean && rm libFBRaster.a && rm test