all : main.o image.o 
	gcc -o lab04 main.o image.o -lm 

image.o : image.c
	gcc -c image.c -std=c11

aula04.o : main.c
	gcc -c main.c -std=c11 -lm 

clean :
	rm *.o
	rm lab04

run :
	./lab04