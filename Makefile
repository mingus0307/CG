GLEW_LIBS=$(shell pkgconf glew --libs)
GLFW_LIBS=$(shell pkgconf glfw3 --libs)

cg1: ue3.c
	gcc -o cg1 ue3.c $(GLEW_LIBS) $(GLFW_LIBS) -lm

