GLEW_LIBS=$(shell pkgconf glew --libs)
GLFW_LIBS=$(shell pkgconf glfw3 --libs)

cg1: Main.c
	gcc -o cg1 Main.c $(GLEW_LIBS) $(GLFW_LIBS) -lm

