all: main.o pilha.o matrioshka.o
	gcc -o lab02 main.o pilha.o matrioshka.o

pilha.o: pilha.c
	gcc -c pilha.c -std=c11

matrioshka.o: matrioshka.c
	gcc -c matrioshka.c -std=c11

main.o: main.c
	gcc -c main.c -std=c11

clean:
	rm *.o
	rm lab02

run:
	./lab02