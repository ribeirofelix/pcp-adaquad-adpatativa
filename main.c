#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "adaquad_estatico.h"
#include "adaquad_din.h"
#include "utils.h"

#define TOLERANCIA 10e-20

int main(int argc , char ** argv)
{
	// Trata argumentos
	int qtd_threads = 1 ;
	int qtd_tarefas = 10 ;
	int dinamico = 0, estatico = 0 ;
	int o ;
	while( (o = getopt(argc,argv,"t:de") ) != -1 )
	{
		switch(o)
		{
			case 't': if (optarg != NULL) qtd_threads = atoi(optarg); break;
			case 'd': dinamico = 1; break;
			case 'e': estatico = 1 ; break;
			case '?': 
				if (optopt == 't')
				{
					fprintf (stderr, "Option -%c requires an argument: number of threads.\n", optopt);
				}
			return 1;
		}
	}

	printf("threads: %d \n", qtd_threads );
	printf("tarefas: %d \n", qtd_tarefas );
	printf("%d %d \n", estatico,dinamico );	


	if (dinamico)
	{
		adaquad_din_admin( qtd_tarefas , qtd_threads,0,1,TOLERANCIA,&quadratica);
	}

	if (estatico)
	{
		adaquad_admin(qtd_threads,0,1,TOLERANCIA,&quadratica);

	}

	// long double ret = adaquad_admin(qtd_threads,0,1,TOLERANCIA,&quadratica);
	// printf("%Le\n", ret );

	// ret = adaquad_din_admin( qtd_tarefas , qtd_threads,0,1,TOLERANCIA,&quadratica);
	// printf("%Le\n", ret );
	return 0;
}