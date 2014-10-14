adaquads: adaquad_estatico.o fila.o utils.o main.c
	gcc -g3 -Wall -fopenmp -o adaquads adaquad_estatico.o fila.o utils.o main.c
adaquad_estatico.o: adaquad_estatico.c 
	gcc -c -o adaquad_estatico.o adaquad_estatico.c
fila.o: fila.c
	gcc -c -o fila.o fila.c
utils.o: utils.c
	gcc -c -o utils.o utils.c

clean: 
	rm *.o adaquads
