PROGRAM = Pokemon.exe
CC	= g++
LDLIBS  = -g -lglut -lGLU -lGL -lm

$(PROGRAM): image.o pokematf.o main.o
	$(CC) -o $(PROGRAM) $^ $(LDLIBS)
image.o: image.cpp image.h
	$(CC) -c -o $@ $<
pokematf.o: pokematf.cpp pokematf.h
	$(CC) -c -o $@ $<
main.o: main.cpp image.h pokematf.h
	$(CC) -c -o $@ $<
