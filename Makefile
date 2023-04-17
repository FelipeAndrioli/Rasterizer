build:
	gcc -Wall ./src/*.c `sdl2-config --libs --cflags` -lm -o triangle_rasterizer.exe
