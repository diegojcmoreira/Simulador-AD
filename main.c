//gcc -Wall stack.c queue.c client.c main.c -o main.o
//./main.o

#include "stack.h"
#include "queue.h"
#include "client.h"

int main() {
    // Teste de Pilha
    Stack *stack = createStack(sizeof(int), 1);
    int b, i = 0;
    
    for(i = 0; i < 10; i ++) {
        stackPush(stack, &i);
        printf("%d\n", i);
    }

    printf("-------\n");

    while(stack->top >= 0) {
        stackPop(stack, &b);
        printf("%d\n", b);
    }
    stackDestroy(stack);

    // Teste de Fila
    Queue *queue = createQueue(sizeof(int), 1);
    int z;
    
    for(i = 0; i < 10; i ++) {
        queueInsert(queue, &i);
        printf("%d\n", i);
    }

    printf("-------\n");

    while(queue->rear >= queue -> front) {
        queueRemove(queue, &z);
        printf("%d\n", z);
    }
    queueDestroy(queue);

    //--------------
    Queue *fila = createQueue(sizeof(Client), 1);
    
    for(i = 0; i < 200; i ++) {
        Client client = createClient(i+1, i+2, i+3);
        queueInsert(fila, &client);

        printf("%lf, %lf, %lf\n", client.arrivalTime, client.serviceStartTime, client.departureTime);
    }

    printf("-------\n");

    while(fila->rear >= fila->front) {
        Client teste;
        queueRemove(fila, &teste);
        printf("%lf, %lf, %lf\n", teste.arrivalTime, teste.serviceStartTime, teste.departureTime);
    }
    queueDestroy(fila);

    return 0;
}