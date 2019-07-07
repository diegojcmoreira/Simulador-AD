#include "event.h"

void defineSeed(unsigned int seed) {
	srand(seed);
}

double generateExponentialNumber(double lambda) {
	double y = (double)rand()/(double)RAND_MAX; //parametro pseudo-aleatorio da acumulativa da exponencial
	double x = log(1-y)/(-lambda); //funcao inversao da acumulativa da exponencial

	return x;
}

Event createEvent(char type, double lambda) {
	Event event;

	if (type == 'e') //o caractere 'e' indica que o evento segue uma distribuicao exponencial
		event.eventTime = generateExponentialNumber(lambda);
	else if (type == 'd') //o caractere 'd' indica que o evento segue uma distribuicao deterministica
		event.eventTime = 1/lambda;
	else
		printf("Parâmetro recebido não reconhecido!");

	return event;
}