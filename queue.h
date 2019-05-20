#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int memberSize; //tamanha de cada dado que sera armazenado
  int capacity; //capacidade da fila
  void* data; //dados armazenados na fila 
  int front; //posicao do primeiro elemento da fila
  int rear; //posicao do ultimo elemento da fila
} Queue;

//aloca espaco de memoria para a fila e retorna um ponteiro para ela
//recebe o tamanho do dado que a fila armazena e a sua capacidade
//retorna uma instancia da fila
Queue* createQueue(int memberSize, int capacity);

//libera espaco de memoria da fila
//recebe a fila cujo espaco sera liberado
void queueDestroy(Queue *q);

//adiciona um elemento na fila
//recebe a fila onde o dado sera adicionado e o dado em si
void queueInsert(Queue *q, void *data);

//remove o primeiro elemento da fila, seu valor eh retornado na variavel target
//recebe a fila de onde o dado sera retirado e a variavel onde sera armazenada este dado
//retorna 0 se a operacao foi bem sucedida e 1 se nao
int queueRemove(Queue *q, void *target);

#endif