//g++ -Wall stack.c queue.c client.c event.c metric.c simulation.c -o simulation.o -lm
//./simulation.o

#include "stack.h"
#include "queue.h"
#include "client.h"
#include "event.h"
#include "metric.h"

#define SERVICE_RATE 1
#define TOTAL_ROUNDS 3200
#define EVENT 'e'

void fcfsSimulation(int KMIN, float ARRIVAL_RATE) {
    
    int rounds = 0;
    double mean;
    int roundClient = 0;
    int totalClients = 0, service = 0;
    double timeElapsed = 0;
    int overlappingICs = 0;

    // Metricas da simulaçao, IC(lower e upper) e precisão do IC para media e variacia de W e Nq

    //Média da media das amostras de cada rodada de (Tempo de espera)
    double tStudentMeanLowerW, tStudentMeanUpperW, tStudentMeanPrecisionW, tStudentMeanCenterW ;

    
    //media da variancia das amostras de cada rodada de (Tempo de espera)   
    double tStudentVarianceLowerW, tStudentVarianceUpperW, tStudentVariancePrecisionW, tStudentVarianceCenterW;
    //variancia da variancia das amostras de cada rodada de (Tempo de espera)       
    double chiSquareVarianceLowerW, chiSquareVarianceUpperW, chiSquareVariancePrecisionW, chiSquareVarianceCenterW;

    
    //Média da media das amostras de cada rodada de (Numero de pessoas na fila de espera)   
    double tStudentMeanLowerNq, tStudentMeanUpperNq, tStudentMeanPrecisionNq, tStudentMeanCenterNq;
    
    
    //media da variancia das amostras de cada rodada de (Numero de pessoas na fila de espera)
    double tStudentVarianceLowerNq, tStudentVarianceUpperNq, tStudentVariancePrecisionNq, tStudentVarianceCenterNq;
    //variancia da variancia das amostras de cada rodada de (Numero de pessoas na fila de espera)
    double chiSquareVarianceLowerNq, chiSquareVarianceUpperNq, chiSquareVariancePrecisionNq, chiSquareVarianceCenterNq;
    

    double roundsVarianceEstimatorW[TOTAL_ROUNDS] = {0};
    double roundsMeanEstimatorW[TOTAL_ROUNDS] = {0};
    double roundsVarianceEstimatorNq[TOTAL_ROUNDS] = {0};
    double roundsMeanEstimatorNq[TOTAL_ROUNDS] = {0};

    FILE * kMetric, *roundMetric;


    kMetric = fopen ("C:\\Users\\diego\\Documents\\Simulador-AD\\Valores_Para_K.txt","w");
    roundMetric = fopen ("C:\\Users\\diego\\Documents\\Simulador-AD\\Valores_Por_Rodada.txt","w");
    

    defineSeed(12);

    while(overlappingICs == 0){
        overlappingICs = 1;

        // reiniciar simulação com k maior
        rounds = 0;
        roundClient = 0;
        totalClients = 0;
        service = 0;
        timeElapsed = 0;

        printf("Iniciando processo com valor de K = %d\n", KMIN);
        
        while(rounds < TOTAL_ROUNDS) {

            //Metricas do tempo na fila de espera
            SampleMetric metricW =  createSampleMetric();

            //Metricas do numero de pessoas na fila de espera
            SampleMetric metricNq =  createSampleMetric();
            
            Queue *queueClients = createQueue(sizeof(Client), 1);
            Client clientBeingServiced;
            
            Event arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //gera evento de chegada

            Event endServiceEvent = createEvent(EVENT, SERVICE_RATE);

            roundClient = 0;
            service = 0;

        
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


                    //atualizando estimadores
                    sampleEstimator(&metricW, (clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime), roundClient);
                    sampleEstimator(&metricNq, clientBeingServiced.waintingListSize, roundClient);

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
            // Atualizando metricas da rodada - W
            roundsMeanEstimatorW[rounds] = metricW.meanEstimator;
            roundsVarianceEstimatorW[rounds] = metricW.varianceEstimator;

            // Atualizando metricas da rodada - Nq
            roundsMeanEstimatorNq[rounds] = metricNq.meanEstimator;
            roundsVarianceEstimatorNq[rounds] = metricNq.varianceEstimator;


            fprintf (roundMetric, "%d,%d,%f,%f,%f,%f\n",rounds,KMIN,metricW.meanEstimator,metricW.varianceEstimator,metricNq.meanEstimator,metricNq.varianceEstimator);




            rounds++;        
            queueDestroy(queueClients);
        }    


        //Tempo medio de espera na fila
        meanIC(roundsMeanEstimatorW, TOTAL_ROUNDS, &tStudentMeanLowerW, &tStudentMeanUpperW, &tStudentMeanPrecisionW,&tStudentMeanCenterW);
        //varianceICChiSquare(roundsMeanEstimatorW, TOTAL_ROUNDS, KMIN, &tStudentVarianceLowerW, &tStudentVarianceLowerW, &tStudentVariancePrecisionW);

        //Variancia do tempo medio de espera na fila
        //TStudent
        meanIC(roundsVarianceEstimatorW, TOTAL_ROUNDS, &tStudentVarianceLowerW, &tStudentVarianceUpperW, &tStudentVariancePrecisionW, &tStudentVarianceCenterW);
        //ChiSquare
        varianceIC(roundsVarianceEstimatorW, TOTAL_ROUNDS, KMIN, &chiSquareVarianceLowerW, &chiSquareVarianceUpperW, &chiSquareVariancePrecisionW, &chiSquareVarianceCenterW);

        //Número médio de pessoas na fila de espera
        meanIC(roundsMeanEstimatorNq, TOTAL_ROUNDS, &tStudentMeanLowerNq, &tStudentMeanUpperNq, &tStudentMeanPrecisionNq,&tStudentMeanCenterNq);
        //varianceICChiSquare(roundsMeanEstimatorNq, TOTAL_ROUNDS, KMIN, &tStudentVarianceLowerNq, &tStudentVarianceUpperNq, &tStudentVariancePrecisionNq);

        //Variancia do numero de pessoas na fila de espera
        //tstudent    
        meanIC(roundsVarianceEstimatorNq, TOTAL_ROUNDS, &tStudentVarianceLowerNq, &tStudentVarianceUpperNq, &tStudentVariancePrecisionNq, &tStudentVarianceCenterNq);
        //Chiquare
        varianceIC(roundsVarianceEstimatorNq, TOTAL_ROUNDS, KMIN, &chiSquareVarianceLowerNq, &chiSquareVarianceUpperNq, &chiSquareVariancePrecisionNq, &chiSquareVarianceCenterNq);

        // Verifica se os ICs convergiram
        if(!(valueIsInsideInterval(chiSquareVarianceLowerW, chiSquareVarianceUpperW, tStudentVarianceCenterW) 
        && valueIsInsideInterval(tStudentVarianceLowerW,tStudentVarianceUpperW, chiSquareVarianceCenterW)) ){
            KMIN += 100;
            overlappingICs = 0;

        }

        fprintf (kMetric, "%d,[%lf;%lf],%lf,[%lf;%lf],%lf,[%lf;%lf],%lf,[%lf;%lf],%lf,[%lf;%lf],%lf,[%lf;%lf],%lf\n",KMIN,
        tStudentMeanLowerW,tStudentMeanUpperW,tStudentMeanPrecisionW,
        tStudentVarianceLowerW,tStudentVarianceUpperW,tStudentVariancePrecisionW,
        tStudentMeanLowerNq,tStudentMeanUpperNq,tStudentMeanPrecisionNq,
        tStudentVarianceLowerNq,tStudentVarianceUpperNq,tStudentVariancePrecisionNq,
        chiSquareVarianceLowerW,chiSquareVarianceUpperW,chiSquareVariancePrecisionW,
        chiSquareVarianceLowerNq,chiSquareVarianceUpperNq,chiSquareVariancePrecisionNq);



        
    } 
    fclose (kMetric);
    fclose (roundMetric);   
}

void lcfsSimulation(int KMIN, float ARRIVAL_RATE) {
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

int main(int argc, char *argv[]) {

    int kmin = 0;
    float arrival_rate = 0;

    if( argc == 3 ) {
      kmin = atoi(argv[1]);
      arrival_rate = atof(argv[2]);
    }
    else if( argc > 3 ) {
        printf("Argumentos demais para o programa, insira: %s <valor de k minimo inicial> <valor da taxa de chegada>\n", argv[0]);
        exit(0);
    }
    else {
        printf("Argumentos faltando para o programa, insira: %s <valor de k minimo inicial> <valor da taxa de chegada>\n", argv[0]);
        exit(0);
    }
    //lcfsSimulation();
    printf("Iniciando simulação com lambda = %f\n", arrival_rate);
    fcfsSimulation(kmin,arrival_rate);

    return 0;
}