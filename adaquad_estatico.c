#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "adaquad_estatico.h"
#include "fila.h"
#include "utils.h"

fila * fila_res ;

//void debuga(char * val){ printf("%s\n", val );}
#define debuga(text) //printf("%s\n",text);




long double adaquad_admin(int qtd_threads, long double lim_esq, long double lim_dir, long double tolenrancia, long double (*funcao) (long double) )
{
	int i ;
	long double tam_intervalo = 0 , area_final = 0, trab_lim_esq = 0, trab_lim_dir = 0 ;
	intervalo * intervalo_terminado = NULL;
	omp_set_num_threads(qtd_threads);

	fila_res = cria_fila();

	tam_intervalo = (lim_dir - lim_esq ) / qtd_threads ;

	#pragma omp parallel for private(trab_lim_dir,trab_lim_esq)
	for (i = 0; i < qtd_threads; ++i)
	{
		trab_lim_esq = lim_esq  + i * tam_intervalo ;
		trab_lim_dir  = trab_lim_esq + tam_intervalo ;
		debuga("VAI criar trab");//printf("%Le %Le\n",trab_lim_esq,trab_lim_dir );
		adaquad_trab(trab_lim_esq,trab_lim_dir,tolenrancia,funcao);
		debuga("criou trab");

	}

	
	for ( intervalo_terminado = desenfileira(fila_res) ;  intervalo_terminado != NULL ; intervalo_terminado = desenfileira(fila_res) )
	{
		printf("intervalo: %Le\n",intervalo_terminado->area );
		area_final += intervalo_terminado->area;
		free(intervalo_terminado);
	}
	destroi_fila(fila_res);
	return area_final;

}

void adaquad_trab(long double lim_esq, long double lim_dir, long double tolenrancia, long double (*funcao) (long double))
{
	long double media_calc , lim_esq_calc , lim_dir_calc , total_area , area_esq  , area_dir  , media , dif_area , area_final ;

	media = (lim_dir+lim_esq) / 2;
	lim_dir_calc = funcao(lim_dir);
	lim_esq_calc = funcao(lim_esq);
	media_calc = funcao(media);

	total_area = area_trapezio(lim_esq_calc,lim_dir_calc,lim_dir-lim_esq);
	debuga("total area");
	printf(" chamada: %Le \n", area_trapezio(lim_esq_calc,media_calc,media-lim_esq) );
	area_dir = area_trapezio(lim_esq_calc,media_calc,media-lim_esq);
	area_esq = area_trapezio(media_calc,lim_dir_calc,lim_dir - media);
	dif_area = total_area - (area_dir + area_esq);

	printf("dir %Le , esq %Le \n", area_dir,area_esq );
	if(dif_area <= tolenrancia )
	 	area_final = area_dir + area_esq ;
	else{
		fila * fila_aux = cria_fila();
		enfileira(fila_aux,cria_intervalo(lim_esq,media,lim_esq_calc,media_calc,area_esq));
		
		intervalo * intervalo_corr = NULL;

		for(  intervalo_corr = cria_intervalo(media,lim_dir,	media_calc,lim_dir_calc, area_dir); 
			  intervalo_corr != NULL ;
			  intervalo_corr = desenfileira(fila_aux))
		{

			media = (intervalo_corr->lim_dir + intervalo_corr->lim_esq) / 2 ;
			media_calc = funcao(media);
			area_esq = area_trapezio(intervalo_corr->lim_esq_calc,media_calc,media - intervalo_corr->lim_esq);
			area_dir = area_trapezio(media_calc,intervalo_corr->lim_dir_calc,intervalo_corr->lim_dir - media);
			dif_area = intervalo_corr->area - (area_esq + area_dir);

			if (dif_area < tolenrancia)
				area_final += area_dir+area_esq;
			else{
				enfileira(fila_aux,cria_intervalo(intervalo_corr->lim_esq,media,intervalo_corr->lim_esq_calc,media_calc,area_esq));
				enfileira(fila_aux,cria_intervalo(media,intervalo_corr->lim_dir, media_calc ,intervalo_corr->lim_dir_calc,area_dir));

			}
			free(intervalo_corr);
		}

		destroi_fila(fila_aux);
	}
	debuga("vai enfileirar");
	printf("area final %Le\n",  area_final);
	mutex_enfileira(fila_res, cria_intervalo(0,0,0,0,area_final));
	debuga("enfileirar");

}
