#ifndef EXPONENTIALEVENT_H
#define EXPONENTIALEVENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  double exponentialTime; //instante de ocorrencia do evento exponencial
} ExponentialEvent;

//cria uma instancia da struct ExponentialEvent, de acordo com os parametros passados
//recebe a taxa lambda da exponencial, em que o processo  baseia
//retorna uma instancia do ExponentialEvent
ExponentialEvent createExponentialEvent(double lambda);

//define a seed escolhida para a geracao da distribuicao exponecial
void defineSeed(unsigned int seed);

#endif