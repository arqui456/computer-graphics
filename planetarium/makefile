CC = gcc

OBJS = input.o planets.o planetary.o

#-mwindows ./freeglut.dll \
#-lglut32   \
#-lSDL_image \ 
# gcc -o main -Wall source/1-primeiro_triangulo.c -mwindows .\freeglut.dll -lopengl32 -lglu32


CC_FLAGS=-W         \
         -Wall      \
         -pedantic  \
		 -mwindows .\source\freeglut.dll \
		 -lopengl32 \
		 -lglu32    \
		 -lm

all: planetary clear

input.o: ./source/input.c ./source/input.h
	$(CC) $(CC_FLAGS) -c $^

planets.o: ./source/planets.c ./source/planets.h
	$(CC) $(CC_FLAGS) -c $^

planetary.o: ./source/planetary.c
	$(CC) $(CC_FLAGS) -c $^

planetary: $(OBJS)
	$(CC) $(CC_FLAGS) -o $@ $^

opengl:
	gcc .\source\input.c .\source\planets.c .\source\planetary.c -lm -lopengl32 -lglu32 -lSDL2_image -mwindows .\freeglut.dll -o main

clear:
	rm *.o