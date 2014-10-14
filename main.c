#include <stdio.h>
#include "adaquad_estatico.h"
#include "utils.h"

#define TOLERANCIA 1e-100


int main()
{
	long double ret = adaquad_admin(6,0,1,TOLERANCIA,&identidade);
	printf("%Le\n", ret );
	return 0;
}