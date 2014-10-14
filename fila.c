#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "fila.h"

#define new(p) (p*)verificaMalloc(malloc(sizeof(p)))

static void * verificaMalloc(void * p)
{
	if(p == NULL)
	{
		fprintf(stderr,"Nao foi possivel efetuar alocacao de memoria\n");
		exit(1);
	}

	return p ;
}

fila * cria_fila()
{
	fila * f = new(fila);
	f->prim = f->ult = NULL;
	return f ;
}

void * destroi_fila(fila* f)
{
	intervalo * corr = NULL ;

	if (f != NULL)
	{
		while(f->prim != NULL)
		{
			corr = f->prim;
			f->prim = f->prim->prox;
			free(corr);
		}
		f->ult = NULL;
		free(f);
	}
}

void mutex_enfileira(fila *f, intervalo *novo)
{
	#pragma omp critical
	{
		if (f->prim == NULL)
		{
			f->prim = f->ult = novo;
			f->ult->prox = NULL;
		}else{
			f->ult->prox = novo;
			f->ult = novo;
			f->ult->prox = NULL;
		}
	}
}

intervalo * mutex_desenfileira(fila *f)
{
	intervalo * ret = NULL;

	#pragma omp critical
	{
		if (f != NULL && f->prim != NULL )
		{
			ret = f->prim;
			f->prim = f->prim->prox;
			if (f->prim == NULL)
				f->ult = NULL;
		}
	}
	return ret;
}

void enfileira(fila *f, intervalo * novo)
{
	if (f->prim == NULL)
	{
		f->prim = f->ult = novo;
		f->ult->prox = NULL;
	}else{
		f->ult->prox = novo;
		f->ult = novo;
		f->ult->prox = NULL;
	}


}

intervalo * desenfileira(fila * f)
{
	intervalo * ret = NULL;

	if (f != NULL && f->prim != NULL )
	{
		ret = f->prim;
		f->prim = f->prim->prox;
		if (f->prim == NULL)
			f->ult = NULL;
	}

	return ret;
}


intervalo * cria_intervalo(long double lim_esq,long double lim_dir,long double lim_esq_calc,long double lim_dir_calc,long double area){
	intervalo * i = new(intervalo);
	i->lim_dir = lim_dir;
	i->lim_esq = lim_esq;
	i->lim_dir_calc = lim_dir_calc;
	i->lim_esq_calc = lim_esq_calc;
	i->area = area;
	i->prox = NULL;
	return i ;
}