#include "exponentialEvent.h"

void defineSeed(unsigned int seed) {
	srand(seed);
}

ExponentialEvent createExponentialEvent(double lambda) {
	ExponentialEvent exponentialEvent;

	double y = (double)rand()/(double)RAND_MAX; //parametro pseudo-aleatorio da acumulativa da exponencial

	double x = log(1-y)/(-lambda); //funcao inversao da acumulativa da exponencial

	exponentialEvent.exponentialTime = x;
	return exponentialEvent;
}