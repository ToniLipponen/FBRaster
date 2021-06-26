all: Image.c Input.c Rasterizer.c
	cc Image.c Input.c Rasterizer.c -c -fPIC -msse3 -O2 -ffast-math -march=x86-64 -mtune=x86-64

clean:
	rm *.o
