PROGRAM = Pokemon
CC      = g++
CFLAGS  = -lm
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): main.o pokematf.o image.o
	$(CC) $(LDFLAGS) -o $(PROGRAM) image.o pokematf.o main.o $(LDLIBS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
