#include "queue.h"

Queue* createQueue(int memberSize, int capacity) {
  Queue *q = (Queue*)malloc(sizeof(Queue));
  
  q -> front = 0;
  q -> rear = -1;
  q -> memberSize = memberSize;
  q -> capacity = capacity;
  q -> data = malloc(capacity*memberSize);
  q -> size = 0;
  
  return q;
}

//dobra capacidade da fila
void expandQueue(Queue* q) {
  q -> data = realloc(q -> data, q -> capacity * 2 * q -> memberSize);
  q -> capacity *= 2;
}

//alinha a fila ao espaco de memoria representado pela variavel data
void queueOrganize(Queue *q) {
    if(q->front > 100) {
        void* source = (char*) q -> data + (q -> front * q -> memberSize);
        int queueSize = q->rear - q->front + 1;
        q->rear = q->rear - q->front;
        q->front = 0;
        memcpy(q->data, source, q->memberSize*queueSize);
    }
}

void queueInsert(Queue *q,  void *element) {
  //verifica se a fila esta cheia
  if (q -> rear == q -> capacity - 1)
    expandQueue(q); //aumenta capacidade da fila
  
  q -> rear++;
  q -> size++;
  //calcula posicao de inicio do novo elemento
  void* target = (char*) q -> data + (q -> rear*q -> memberSize);
  memcpy(target, element, q -> memberSize);
}

int queueRemove(Queue *q,  void *target) {
  q -> size--;
  if (q->rear == -1) {
    return 1;
  }

  //calcula posicao de inicio do primeiro elemento da fila
  void* source = (char*) q -> data + (q -> front * q -> memberSize);
  q->front++;
  memcpy(target, source, q->memberSize);
  //queueOrganize(q);
  return 0;
}

void queueDestroy(Queue *q) {
  free(q -> data);
  free(q);
}