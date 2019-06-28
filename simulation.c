//g++ -Wall stack.c queue.c client.c exponentialEvent.c metric.c simulation.c -o simulation.o -lm
//./main.o

#include "stack.h"
#include "queue.h"
#include "client.h"
#include "exponentialEvent.h"
#include "metric.h"

#define SERVICE_RATE 1
#define ARRIVAL_RATE 0.9

void fcfsSimulation() {
    int rounds = 3200;
    double mean;
    int totalClients = 0, service = 0;
    double timeElapsed = 0;
    FILE * fp;

    fp = fopen ("C:\\Users\\Visagio\\Documents\\Simulador-AD\\simulationResults.txt","w");

    //Metricas do tempo na fila de espera
    SampleMetric meanICW =  createSampleMetric();
    SampleMetric varianceICW =  createSampleMetric();

    //Metricas do numero de pessoas na fila de espera
    SampleMetric meanICNq =  createSampleMetric();
    SampleMetric varianceICNq =  createSampleMetric();

    defineSeed(12);

    Queue *queueClients = createQueue(sizeof(Client), 1);
    Client clientBeingServiced;
    
    ExponentialEvent arrivalEvent = createExponentialEvent(ARRIVAL_RATE); //gera evento de chegada

    ExponentialEvent endServiceEvent = createExponentialEvent(SERVICE_RATE);
    
    while(totalClients <= 1550) {
        //se o tempo de chegada eh menor que o tempo restante para o termino do servico(duracao do servico) ou se nao ha ninguem sendo servido, e se mais uma chegada nao superar o numero de clientes da rodada
        if(arrivalEvent.exponentialTime < endServiceEvent.exponentialTime || service == 0)  { 

            //atualiza o tempo total transcorrido
            timeElapsed = timeElapsed + arrivalEvent.exponentialTime; 

            //cria cliente que esta chegando
            Client client = createClient(timeElapsed, timeElapsed, -1);

            // Medidas do numero de pessoas na fila

            //printf("Numero de pesssoas na fila agora: %f -> chegada: %d\n",queueClients->rear - queueClients->front,totalClients);
            //meanIC(&meanICNq, queueClients->rear - queueClients->front, totalClients);
            //varianceIC(&varianceICNq, queueClients->rear - queueClients->front, totalClients);
        

            //se ninguem esta sendo servido, entao a fila esta vazia
            if(service == 0) { 
                service = 1;
                client.serviceStartTime = timeElapsed;

                //se ninguem esta sendo servido, o cliente que chegou eh atendido imediatamente
                clientBeingServiced = client; 
            }
            else {
                //atualiza o tempo transcorrido para o fim do servico
                endServiceEvent.exponentialTime = endServiceEvent.exponentialTime - arrivalEvent.exponentialTime;
                queueInsert(queueClients, &client);
            }
            arrivalEvent = createExponentialEvent(ARRIVAL_RATE); //cria nova chegada

        }
        else {
            totalClients++;
            timeElapsed = timeElapsed + endServiceEvent.exponentialTime;

            //cliente termina de ser servido
            clientBeingServiced.departureTime = timeElapsed;


            printf("Valor de soma %f\n",meanICW.sumValuesSample);
            // Medidas do tempo de espera na fila
            printf("Tempo de saida: %f", clientBeingServiced.departureTime);
            printf("Tempo de entrada: %f", clientBeingServiced.serviceStartTime);

            printf("Tempo que o cliente esperou: %f para um total de %d \n", (clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime),totalClients);
            meanIC(&meanICW, (clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime), totalClients);
            
            fprintf (fp, "%d;%lf;%lf\n",totalClients,meanICW.lower, meanICW.upper);
            
            //varianceIC(&varianceICW, (clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime), totalClients);

            // --------- METRICAS DEVEM SER CALCULADAS AQUI -----------------
            //printf("Chegada: %lf Servico comeca: %lf Partida: %lf Tempo de servico: %lf Tempo na fila: %lf Tempo no sistema: %lf\n", clientBeingServiced.arrivalTime, clientBeingServiced.serviceStartTime, clientBeingServiced.departureTime, clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime, clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime,clientBeingServiced.departureTime-clientBeingServiced.arrivalTime);

            // --------------------------------------------------------------

            arrivalEvent.exponentialTime = arrivalEvent.exponentialTime - endServiceEvent.exponentialTime;
            endServiceEvent = createExponentialEvent(SERVICE_RATE);

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

    queueDestroy(queueClients);
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
    
    ExponentialEvent arrivalEvent = createExponentialEvent(ARRIVAL_RATE); //gera evento de chegada

    ExponentialEvent endServiceEvent = createExponentialEvent(SERVICE_RATE);
    
    while(totalClients <= 5000) {
        //se o tempo de chegada eh menor que o tempo restante para o termino do servico(duracao do servico) ou se nao ha ninguem sendo servido, e se mais uma chegada nao superar o numero de clientes da rodada
        if(arrivalEvent.exponentialTime < endServiceEvent.exponentialTime || service == 0)  { 

            //atualiza o tempo total transcorrido
            timeElapsed = timeElapsed + arrivalEvent.exponentialTime; 

            //cria cliente que esta chegando
            Client client = createClient(timeElapsed, timeElapsed, -1); 

            //se ninguem esta sendo servido, entao a fila esta vazia
            if(service == 0) { 
                service = 1;
                client.serviceStartTime = timeElapsed;

                //se ninguem esta sendo servido, o cliente que chegou eh atendido imediatamente
                clientBeingServiced = client; 
            }
            else {
                //atualiza o tempo transcorrido para o fim do servico
                endServiceEvent.exponentialTime = endServiceEvent.exponentialTime - arrivalEvent.exponentialTime;
                stackPush(stackClients, &client);
            }
            arrivalEvent = createExponentialEvent(ARRIVAL_RATE); //cria nova chegada

        }
        else {
            totalClients++;
            timeElapsed = timeElapsed + endServiceEvent.exponentialTime;

            //cliente termina de ser servido
            clientBeingServiced.departureTime = timeElapsed;

            // --------- METRICAS DEVEM SER CALCULADAS AQUI -----------------
            //printf("Chegada: %lf Servico comeca: %lf Partida: %lf Tempo de servico: %lf Tempo na fila: %lf Tempo no sistema: %lf\n", clientBeingServiced.arrivalTime, clientBeingServiced.serviceStartTime, clientBeingServiced.departureTime, clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime, clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime,clientBeingServiced.departureTime-clientBeingServiced.arrivalTime);
            mean = mean + (clientBeingServiced.departureTime - clientBeingServiced.serviceStartTime)/5000;
            printf("%lf\n", mean);

            // --------------------------------------------------------------

            arrivalEvent.exponentialTime = arrivalEvent.exponentialTime - endServiceEvent.exponentialTime;
            endServiceEvent = createExponentialEvent(SERVICE_RATE);

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