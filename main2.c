//gcc -Wall stack.c queue.c client.c exponentialEvent.c main2.c -o main.o -lm
//./main.o

#include "stack.h"
#include "queue.h"
#include "client.h"
#include "exponentialEvent.h"

#define SERVICE_RATE 1
#define ARRIVAL_RATE 0.1

int main() {
    int totalClients = 0, service = 0;
    double timeElapsed = 0;
    defineSeed(12);

    Queue *queueClients = createQueue(sizeof(Client), 1);
    Queue *queueClientsServiced = createQueue(sizeof(Client), 1);
 
    Client clientBeingServiced;
    
    ExponentialEvent arrivalEvent = createExponentialEvent(ARRIVAL_RATE); //gera evento de chegada

    ExponentialEvent endServiceEvent = createExponentialEvent(SERVICE_RATE);
    
    while(totalClients <= 5000) {
        //se o tempo de chegada eh menor que o tempo restante para o termino do servico(duracao do servico) ou se nao ha ninguem sendo servido, e se mais uma chegada nao superar o numero de clientes da rodada
        if(arrivalEvent.exponentialTime < endServiceEvent.exponentialTime || service == 0)  { 
            timeElapsed = timeElapsed + arrivalEvent.exponentialTime; //atualiza o tempo total transcorrido
            Client client = createClient(timeElapsed, timeElapsed, -1); //cria cliente que esta chegando

            if(service == 0) { //se ninguem esta sendo servido, entao a fila esta vazia
                service = 1;
                client.serviceStartTime = timeElapsed;
                clientBeingServiced = client; //se ninguem esta sendo servido, o cliente que chegou eh atendido imediatamente
            }
            else {
                endServiceEvent.exponentialTime = endServiceEvent.exponentialTime - arrivalEvent.exponentialTime; //atualiza o tempo transcorrido para o fim do servico
                queueInsert(queueClients, &client);
            }
            arrivalEvent = createExponentialEvent(ARRIVAL_RATE); //cria nova chegada

        }
        else {
            totalClients++;
            timeElapsed = timeElapsed + endServiceEvent.exponentialTime;
            clientBeingServiced.departureTime = timeElapsed;
            queueInsert(queueClientsServiced, &clientBeingServiced);
            arrivalEvent.exponentialTime = arrivalEvent.exponentialTime - endServiceEvent.exponentialTime;
            endServiceEvent = createExponentialEvent(SERVICE_RATE);

            if(queueClients->rear >= queueClients->front) { //se a fila nao esta vazia
                service = 1;
                queueRemove(queueClients, &clientBeingServiced);
                clientBeingServiced.serviceStartTime = timeElapsed;
            }
            else
                service = 0;
        }
    }

    // print

    double waitTime = 0;
    while(queueClientsServiced->rear >= queueClientsServiced->front) {
        Client client;
        queueRemove(queueClientsServiced, &client);
        waitTime = waitTime + client.departureTime-client.arrivalTime;
        printf("%lf : %lf : %lf ======== %lf\n", client.arrivalTime, client.serviceStartTime, client.departureTime, client.departureTime-client.arrivalTime);
    }
    printf("Tempo de espera medio: %lf\n", waitTime/totalClients);
    queueDestroy(queueClients);
    queueDestroy(queueClientsServiced);

    return 0;
}