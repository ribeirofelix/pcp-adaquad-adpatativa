

typedef struct _intervalo intervalo;
struct _intervalo{
	// a , b
	long double lim_esq;
	long double lim_dir;

	// f(a) , f(b)
	long double lim_esq_calc;
	long double lim_dir_calc;

	// area(a,b,f(a),f(b))
	long double area ;

	intervalo * prox ;

};

// Cabeça da fila
typedef struct _fila {
	intervalo * prim ;
	intervalo * ult ;
}fila;


fila * cria_fila();

void * destroi_fila(fila*);

void enfileira(fila *, intervalo *);

intervalo * desenfileira(fila *);

void mutex_enfileira(fila *, intervalo *);

intervalo * mutex_desenfileira(fila *);

intervalo * cria_intervalo(long double lim_esq,long double lim_dir,long double lim_esq_calc,long double lim_dir_calc,long double area);
