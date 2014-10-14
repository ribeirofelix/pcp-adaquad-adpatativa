#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "fila.h"
#include "utils.h"
#include "adaquad_din.h"


fila * fila_trabalhadoras , * fila_resultados ;

long double adaquad_din_admin(int num_inicial_tarefas,int qtd_threads, long double lim_esq, long double lim_dir, long double tolerancia, long double (*funcao) (long double) )
{
	int i;
	long double tam_intervalo , area_final ;
	intervalo * aux_interval ;

	tam_intervalo = (lim_dir + lim_esq)/num_inicial_tarefas;

	fila_trabalhadoras = cria_fila();

	for (i = 0; i < num_inicial_tarefas; ++i)
	{
		long double temp_esq = lim_esq+ i*tam_intervalo;
		long double temp_dir = temp_esq+tam_intervalo;
		long double calc_esq = funcao(temp_esq);
		long double calc_dir = funcao(temp_dir);
		enfileira(fila_trabalhadoras,cria_intervalo(temp_esq,temp_dir,calc_esq,calc_dir, area_trapezio(calc_esq,calc_dir,tam_intervalo)));
	}

	fila_resultados = cria_fila();

	omp_set_num_threads(qtd_threads);

	#pragma omp parallel for
	for (i = 0; i < qtd_threads; ++i)
		adaquad_din_trab(tolerancia,funcao);
	

	for(aux_interval = desenfileira(fila_resultados) ; aux_interval != NULL ; free(aux_interval), aux_interval = desenfileira(fila_resultados) )
		area_final += aux_interval->area ;

	destroi_fila(fila_trabalhadoras);
	destroi_fila(fila_resultados);
	return area_final;

}
void adaquad_din_trab(long double tolerancia, long double (*funcao) (long double) )
{
	intervalo * curr_intervalo ;
	for (curr_intervalo = mutex_desenfileira(fila_trabalhadoras); curr_intervalo != NULL ; free(curr_intervalo), curr_intervalo = mutex_desenfileira(fila_trabalhadoras))
	{
		long double media = (curr_intervalo->lim_dir + curr_intervalo->lim_esq) / 2 ;
		long double media_calc = funcao(media);
		long double area_esq = area_trapezio(curr_intervalo->lim_esq_calc, media_calc, media - curr_intervalo->lim_esq);
		long double area_dir = area_trapezio(media_calc, curr_intervalo->lim_dir_calc, curr_intervalo->lim_dir - media);
		
		if (curr_intervalo->area - (area_dir+area_esq) <= tolerancia )
			mutex_enfileira(fila_resultados,cria_intervalo(curr_intervalo->lim_esq,curr_intervalo->lim_dir,0,0,area_dir+area_esq));
		else{
			mutex_enfileira(fila_trabalhadoras,cria_intervalo(curr_intervalo->lim_esq,media,curr_intervalo->lim_esq_calc, media_calc,area_esq));
			mutex_enfileira(fila_trabalhadoras,cria_intervalo(media,curr_intervalo->lim_dir,media_calc,curr_intervalo->lim_dir_calc,area_dir));
		}
	}
}

