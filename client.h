#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double arrivalTime; //instante de chegada na fila
  double serviceStartTime; //instante de inicio do servico
  double departureTime; //instante de fim do servico
} Client;

//cria uma instancia da struct Client, de acordo com os parametros passados
//recebe o instante da chegada do cliente, o instante de inicio do servico, e o instante de saida do sistema
//retorna uma instancia do cliente
Client createClient(double arrival, double serviceStart, double departure);

#endif