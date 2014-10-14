#include <math.h>
#include "utils.h"
long double identidade(long double x){
	return x;
}

long double quadratica(long double x){
	return 20*pow(x,2) + x; 
}


long double area_trapezio(long double base1, long double base2, long double altura){
	//printf("b1:%Le , b2:%Le , a:%Le, TRAPEZOIDE: %Le\n",base1,base2,altura,((base1+base2)*altura)/2 );
	long double resp = ((base1+base2)*altura)/2; 
	//printf("resp : %Le\n", resp );
	return resp;
}