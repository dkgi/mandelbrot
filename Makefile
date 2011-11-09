all: 
	g++ -Wall -ansi -g -o Mandelbrot Sample.h Point.h Main.cpp Buffer.h Viewport.h Scene.h Sampler.h -lGL -lglut 

