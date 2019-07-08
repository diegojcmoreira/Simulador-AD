//g++ -Wall stack.c queue.c client.c event.c metric.c simulation.c -o simulation.o -lm
//./simulation.o

#include "stack.h"
#include "queue.h"
#include "client.h"
#include "event.h"
#include "metric.h"

#define SERVICE_RATE 1
#define ARRIVAL_RATE 2
#define TOTAL_ROUNDS 1
#define KMIN 100
#define EVENT 'd'

void fcfsSimulation() {
    int rounds = 0;
    double mean;
    int roundClient = 0;
    int totalClients = 0, service = 0;
    double timeElapsed = 0;
    FILE * fp;

    fp = fopen ("C:\\Users\\diego\\Documents\\Simulador-AD\\simulation.txt","w");

    //Metricas do tempo na fila de espera
    SampleMetric meanICW =  createSampleMetric();
    SampleMetric varianceICW =  createSampleMetric();

    //Metricas do numero de pessoas na fila de espera
    SampleMetric meanICNq =  createSampleMetric();
    SampleMetric varianceICNq =  createSampleMetric();


    defineSeed(12);
    while(rounds < TOTAL_ROUNDS) {
        rounds++;
        mean = 0;
        Queue *queueClients = createQueue(sizeof(Client), 1);
        Client clientBeingServiced;
        
        Event arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //gera evento de chegada

        Event endServiceEvent = createEvent(EVENT, SERVICE_RATE);

    
        while(roundClient < KMIN) {
            
            //se o tempo de chegada eh menor que o tempo restante para o termino do servico(duracao do servico) ou se nao ha ninguem sendo servido, e se mais uma chegada nao superar o numero de clientes da rodada
            if(arrivalEvent.eventTime < endServiceEvent.eventTime || service == 0)  { 

                //atualiza o tempo total transcorrido
                timeElapsed = timeElapsed + arrivalEvent.eventTime; 

                //cria cliente que esta chegando
                Client client = createClient(timeElapsed, timeElapsed, -1,queueClients -> size);

                //se ninguem esta sendo servido, entao a fila esta vazia
                if(service == 0) { 
                    service = 1;

                    

                    client.serviceStartTime = timeElapsed;

                    //printf("cliente: %d - Tempo inicio servico: %d\n",roundClient,timeElapsed);

                    //se ninguem esta sendo servido, o cliente que chegou eh atendido imediatamente
                    clientBeingServiced = client; 
                }
                else {
                    //atualiza o tempo transcorrido para o fim do servico
                    endServiceEvent.eventTime = endServiceEvent.eventTime - arrivalEvent.eventTime;
                    queueInsert(queueClients, &client);
                }
                arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //cria nova chegada
            }
            else {
                totalClients++;
                roundClient++;
                timeElapsed = timeElapsed + endServiceEvent.eventTime;

                

                //cliente termina de ser servido
                clientBeingServiced.departureTime = timeElapsed;

                // Medidas do tempo de espera na fila

                fprintf (fp, "%d;%lf;%lf;%lf;%d\n",roundClient,clientBeingServiced.arrivalTime, (clientBeingServiced.departureTime-clientBeingServiced.serviceStartTime),(clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime),clientBeingServiced.waintingListSize);


                mean = mean + (clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime)/KMIN;
                sampleEstimator(&meanICW, (clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime), totalClients);
                sampleEstimator(&varianceICW, (clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime), totalClients);
                sampleEstimator(&meanICNq, clientBeingServiced.waintingListSize, totalClients);
                sampleEstimator(&varianceICNq, clientBeingServiced.waintingListSize, totalClients);
                

                arrivalEvent.eventTime = arrivalEvent.eventTime - endServiceEvent.eventTime;

                endServiceEvent = createEvent(EVENT, SERVICE_RATE);


                //se a fila nao esta vazia
                if(queueClients->rear >= queueClients->front) { 
                    service = 1;
                    queueRemove(queueClients, &clientBeingServiced);

                    clientBeingServiced.serviceStartTime = timeElapsed;
                }
                else
                    service = 0;


            }
        }
        printf("[MEDIA]Rodada %d - EM: %f - EV: %f\n",rounds,meanICW.meanEstimator,meanICW.varianceEstimator);
        printf("[VARIA]Rodada %d - EM: %f - EV: %f\n",rounds,meanICNq.meanEstimator,meanICNq.varianceEstimator);
        //fprintf (fp, "%d;%lf;%lf\n",rounds,meanICW.meanEstimator, meanICW.varianceEstimator);
        //printf("\n%lf\n", mean);

        roundClient = 0;
        
        queueDestroy(queueClients);
    }    

    meanIC(&meanICW, totalClients);
    varianceIC(&varianceICW, totalClients);
    meanIC(&meanICNq, totalClients);
    varianceIC(&varianceICNq, totalClients);

    printf("[MEDIA(W) ]: REAL: %f -> [%f,%f]\n",meanICW.meanEstimator,meanICW.lower,meanICW.upper);
    printf("[VARIA(W) ]: REAL: %f -> [%f,%f]\n",varianceICW.varianceEstimator,varianceICW.lower,varianceICW.upper);
    printf("[MEDIA(Nq)]: REAL: %f -> [%f,%f]\n",meanICNq.meanEstimator,meanICNq.lower,meanICNq.upper);
    printf("[VARIA(Nq)]: REAL: %f -> [%f,%f]\n",varianceICNq.varianceEstimator,varianceICNq.lower,varianceICNq.upper);
    printf("Precicion ICMedia[W ]: %f\n",meanICW.precisionIC);
    printf("Precicion ICVaria[W ]: %f\n",varianceICW.precisionIC);
    printf("Precicion ICMedia[Nq]: %f\n",meanICNq.precisionIC);
    printf("Precicion ICVaria[Nq]: %f\n",varianceICNq.precisionIC);
    fclose (fp);
}

void lcfsSimulation() {
    int rounds = 3200;
    double mean;
    int totalClients = 0, service = 0;
    double timeElapsed = 0;
    defineSeed(12);

    Stack *stackClients = createStack(sizeof(Client), 1);
    Client clientBeingServiced;
    
    Event arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //gera evento de chegada

    Event endServiceEvent = createEvent(EVENT, SERVICE_RATE);
    
    while(totalClients <= 5000) {
        //se o tempo de chegada eh menor que o tempo restante para o termino do servico(duracao do servico) ou se nao ha ninguem sendo servido, e se mais uma chegada nao superar o numero de clientes da rodada
        if(arrivalEvent.eventTime < endServiceEvent.eventTime || service == 0)  { 

            //atualiza o tempo total transcorrido
            timeElapsed = timeElapsed + arrivalEvent.eventTime; 

            //cria cliente que esta chegando
            Client client = createClient(timeElapsed, timeElapsed, -1,stackClients -> size); 

            //se ninguem esta sendo servido, entao a fila esta vazia
            if(service == 0) { 
                service = 1;
                client.serviceStartTime = timeElapsed;

                //se ninguem esta sendo servido, o cliente que chegou eh atendido imediatamente
                clientBeingServiced = client; 
            }
            else {
                //atualiza o tempo transcorrido para o fim do servico
                endServiceEvent.eventTime = endServiceEvent.eventTime - arrivalEvent.eventTime;
                stackPush(stackClients, &client);
            }
            arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //cria nova chegada

        }
        else {
            totalClients++;
            timeElapsed = timeElapsed + endServiceEvent.eventTime;

            //cliente termina de ser servido
            clientBeingServiced.departureTime = timeElapsed;

            // --------- METRICAS DEVEM SER CALCULADAS AQUI -----------------
            //printf("Chegada: %lf Servico comeca: %lf Partida: %lf Tempo de servico: %lf Tempo na fila: %lf Tempo no sistema: %lf\n", clientBeingServiced.arrivalTime, clientBeingServiced.serviceStartTime, clientBeingServiced.departureTime, clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime, clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime,clientBeingServiced.departureTime-clientBeingServiced.arrivalTime);
            mean = mean + (clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime)/5000;
            printf("%lf\n", mean);

            // --------------------------------------------------------------

            arrivalEvent.eventTime = arrivalEvent.eventTime - endServiceEvent.eventTime;
            endServiceEvent = createEvent(EVENT, SERVICE_RATE);

            //se a fila nao esta vazia
            if(stackClients->top > 0) { 
                service = 1;
                stackPop(stackClients, &clientBeingServiced);
                clientBeingServiced.serviceStartTime = timeElapsed;
            }
            else
                service = 0;
        }
    }

    stackDestroy(stackClients);
}

int main() {
    //lcfsSimulation();
    fcfsSimulation();

    return 0;
}