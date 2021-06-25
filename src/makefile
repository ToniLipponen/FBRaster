all: Image.c Input.c Rasterizer.c
	cc Image.c Input.c Rasterizer.c -c -msse3 -O2 -march=native -Wall -fPIC

clean:
	rm *.o
