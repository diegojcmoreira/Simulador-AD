#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  double eventTime; //instante de ocorrencia do evento
} Event;

//cria uma instancia da struct Event, de acordo com os parametros passados
//o evento pode ser exponencial, ou deterministico, indiado pela variavel type
//recebe a taxa lambda da distribuicao, em que o processo  baseia
//retorna uma instancia do Event
Event createEvent(char type, double lambda);

//define a seed escolhida para a geracao da distribuicao
void defineSeed(unsigned int seed);

#endif