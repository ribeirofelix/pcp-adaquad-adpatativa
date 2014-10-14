adaquads: adaquad_estatico.o fila.o utils.o adaquad_din.o main.c
	gcc  -Wall -fopenmp -o adaquads adaquad_estatico.o adaquad_din.o fila.o utils.o main.c -lm
adaquad_estatico.o: adaquad_estatico.c 
	gcc -c -o adaquad_estatico.o adaquad_estatico.c
adaquad_din.o: adaquad_din.c
	gcc -c -o adaquad_din.o adaquad_din.c
fila.o: fila.c
	gcc -c -o fila.o fila.c
utils.o: utils.c
	gcc -c -o utils.o utils.c 

clean: 
	rm *.o adaquads
