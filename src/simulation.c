//g++ -Wall stack.c queue.c client.c event.c metric.c simulation.c -o simulation.o -lm
//./simulation.o


#include "stack.h"
#include "queue.h"
#include "client.h"
#include "event.h"
#include "metric.h"
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SERVICE_RATE 1
#define TOTAL_ROUNDS 3200
#define EVENT 'e'
#define ALPHA 0.05
#define FIM_FASE_TRANSIENTE 500

void Simulation(int seed, int KMIN, float ARRIVAL_RATE, char* const policy, FILE * fp) {

    
    //Calculando valores analiticos
    double EW, VW, ENq, VNq;
    
    ENq = pow(ARRIVAL_RATE,2)/(1-ARRIVAL_RATE);
    VNq = (pow(ARRIVAL_RATE,2)+pow(ARRIVAL_RATE,3)+pow(ARRIVAL_RATE,4))/(1-ARRIVAL_RATE);

    if(strcmp(policy,"FCFS") == 0){
        EW = ARRIVAL_RATE/(1-ARRIVAL_RATE);
        VW = (2*ARRIVAL_RATE-pow(ARRIVAL_RATE,2))/pow(ARRIVAL_RATE,2);

    }else{
        EW = ARRIVAL_RATE/(1-ARRIVAL_RATE);
        VW = (2*ARRIVAL_RATE-pow(ARRIVAL_RATE,2)-pow(ARRIVAL_RATE,3))/pow(ARRIVAL_RATE,3);

    }

    
    printf("-----Valores Analiticos-----\n");
    printf("E[W] = %f\n",EW);
    printf("V[W] = %f\n",VW);
    printf("E[Nq] = %f\n",ENq);
    printf("V[Nq] = %f\n",VNq);
    
    
    int rounds = 0;
    double mean;
    int roundClient = 0;
    int totalClients = 0, service = 0;
    double timeElapsed = 0;
    int overlappingICs = 0;
    int fimDaFaseTransiente = 0;

    Event arrivalEvent; //gera evento de chegada
    Event endServiceEvent;

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
    
    double total_time;
	clock_t start, end;

    defineSeed(seed);

    while(overlappingICs == 0){
        overlappingICs = 1;
	    start = clock();


        // reiniciar simulação com k maior
        rounds = 0;
        roundClient = 0;
        totalClients = 0;
        service = 0;
        timeElapsed = 0;
        double roundsVarianceEstimatorW[TOTAL_ROUNDS] = {0};
        double roundsMeanEstimatorW[TOTAL_ROUNDS] = {0};
        double roundsVarianceEstimatorNq[TOTAL_ROUNDS] = {0};
        double roundsMeanEstimatorNq[TOTAL_ROUNDS] = {0};

        Queue *queueClients;
        Stack *stackClients;

        if(strcmp(policy,"FCFS") == 0)
            queueClients = createQueue(sizeof(Client), 5);
        else
            stackClients = createStack(sizeof(Client), 5);

        

        arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //gera evento de chegada
        endServiceEvent = createEvent(EVENT, SERVICE_RATE);


        
        while(rounds <= TOTAL_ROUNDS) {

            //Metricas do tempo na fila de espera
            SampleMetric metricW =  createSampleMetric();

            //Metricas do numero de pessoas na fila de espera
            SampleMetric metricNq =  createSampleMetric();
            
            Client clientBeingServiced;
            
            roundClient = 0;
            service = 0;


        
            while(roundClient < KMIN) {
                
                //se o tempo de chegada eh menor que o tempo restante para o termino do servico(duracao do servico) ou se nao ha ninguem sendo servido, e se mais uma chegada nao superar o numero de clientes da rodada
                if(arrivalEvent.eventTime < endServiceEvent.eventTime || service == 0)  { 

                    //atualiza o tempo total transcorrido
                    timeElapsed = timeElapsed + arrivalEvent.eventTime; 

                    //cria cliente que esta chegando
                    Client client;
                    if(strcmp(policy,"FCFS") == 0)
                        client = createClient(timeElapsed, timeElapsed, -1,queueSize(queueClients),rounds);
                    else
                        client = createClient(timeElapsed, timeElapsed, -1,stackSize(stackClients),rounds);


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
                        if(strcmp(policy,"FCFS") == 0)
                            queueInsert(queueClients, &client);
                        else   
                            stackPush(stackClients, &client);

                    }
                    arrivalEvent = createEvent(EVENT, ARRIVAL_RATE); //cria nova chegada
                }
                else {
                    totalClients++;
                    roundClient++;
                    timeElapsed = timeElapsed + endServiceEvent.eventTime;


                    //cliente termina de ser servido
                    clientBeingServiced.departureTime = timeElapsed;


                    //atualizando estimadores(só coleta metricas de clientes da rodada[sistema de cores] e quando terminar a fase transiente)
                    if(clientBeingServiced.myRound == rounds && fimDaFaseTransiente != 0){
                        sampleEstimator(&metricW, (clientBeingServiced.serviceStartTime - clientBeingServiced.arrivalTime), roundClient);
                        sampleEstimator(&metricNq, clientBeingServiced.waintingListSize, roundClient);

                    }

                    arrivalEvent.eventTime = arrivalEvent.eventTime - endServiceEvent.eventTime;
                    endServiceEvent = createEvent(EVENT, SERVICE_RATE);

                    //se a fila nao esta vazia
                    if(strcmp(policy,"FCFS") == 0){    
                        if(queueClients->rear >= queueClients->front) { 
                            service = 1;
                            queueRemove(queueClients, &clientBeingServiced);

                            clientBeingServiced.serviceStartTime = timeElapsed;
                        }
                        else
                            service = 0;
                    }else {
                        if(stackClients->top > -1) { 
                            service = 1;
                            stackPop(stackClients, &clientBeingServiced);
                            
                            clientBeingServiced.serviceStartTime = timeElapsed;
                        }
                        else
                            service = 0;
                    }


                }
            }
            // Atualizando metricas da rodada - W
            roundsMeanEstimatorW[rounds] = metricW.meanEstimator;
            roundsVarianceEstimatorW[rounds] = metricW.varianceEstimator;

            // Atualizando metricas da rodada - Nq
            roundsMeanEstimatorNq[rounds] = metricNq.meanEstimator;
            roundsVarianceEstimatorNq[rounds] = metricNq.varianceEstimator;


            // Grava medidas das metricas para fins comparativos
            //fprintf (roundMetric, "%d,%d,%f,%f,%f,%f\n",rounds,KMIN,metricW.meanEstimator,metricW.varianceEstimator,metricNq.meanEstimator,metricNq.varianceEstimator);

            if(totalClients > FIM_FASE_TRANSIENTE && rounds == 0 && fimDaFaseTransiente == 0) {
                printf("TERMINOU A FASE TRANSIENTE\n");
                fimDaFaseTransiente = 1;
            }    

            rounds++; 
            //Reseta o numero de rodadas se ainda estiver no periodo transiente
            if(fimDaFaseTransiente == 0){
                rounds = 0;

            }

            
        }    


        //Tempo medio de espera na fila
        tStudentIC(roundsMeanEstimatorW, TOTAL_ROUNDS, &tStudentMeanLowerW, &tStudentMeanUpperW, &tStudentMeanPrecisionW,&tStudentMeanCenterW);

        //Variancia do tempo medio de espera na fila
        tStudentIC(roundsVarianceEstimatorW, TOTAL_ROUNDS, &tStudentVarianceLowerW, &tStudentVarianceUpperW, &tStudentVariancePrecisionW, &tStudentVarianceCenterW);
        chiSquareIC(roundsVarianceEstimatorW, TOTAL_ROUNDS, KMIN, &chiSquareVarianceLowerW, &chiSquareVarianceUpperW, &chiSquareVariancePrecisionW, &chiSquareVarianceCenterW);

        //Número médio de pessoas na fila de espera
        tStudentIC(roundsMeanEstimatorNq, TOTAL_ROUNDS, &tStudentMeanLowerNq, &tStudentMeanUpperNq, &tStudentMeanPrecisionNq,&tStudentMeanCenterNq);

        //Variancia do numero de pessoas na fila de espera
        tStudentIC(roundsVarianceEstimatorNq, TOTAL_ROUNDS, &tStudentVarianceLowerNq, &tStudentVarianceUpperNq, &tStudentVariancePrecisionNq, &tStudentVarianceCenterNq);
        chiSquareIC(roundsVarianceEstimatorNq, TOTAL_ROUNDS, KMIN, &chiSquareVarianceLowerNq, &chiSquareVarianceUpperNq, &chiSquareVariancePrecisionNq, &chiSquareVarianceCenterNq);

        
        
        // Verifica se os ICs convergiram
        // if(!(valueIsInsideInterval(chiSquareVarianceLowerW, chiSquareVarianceUpperW, tStudentVarianceCenterW) 
        // && valueIsInsideInterval(tStudentVarianceLowerW,tStudentVarianceUpperW, chiSquareVarianceCenterW)) ){
        //     KMIN += 100;
        //     overlappingICs = 0;

        // }
        end = clock();
        total_time = ((double) (end - start)) / CLK_TCK;

        //Imprimir metricas calculadas
        fprintf (fp, "%d,[%lf;%lf],%lf,%lf,[%lf;%lf],%lf,%lf,[%lf;%lf],%lf,%lf,[%lf;%lf],%lf,%lf,[%lf;%lf],%lf,%lf,[%lf;%lf],%lf,%lf,%f\n",KMIN,
        tStudentMeanLowerW,tStudentMeanUpperW,tStudentMeanCenterW, tStudentMeanPrecisionW,
        tStudentVarianceLowerW,tStudentVarianceUpperW,tStudentVarianceCenterW,tStudentVariancePrecisionW,
        tStudentMeanLowerNq,tStudentMeanUpperNq,tStudentMeanCenterNq,tStudentMeanPrecisionNq,
        tStudentVarianceLowerNq,tStudentVarianceUpperNq,tStudentVarianceCenterNq,tStudentVariancePrecisionNq,
        chiSquareVarianceLowerW,chiSquareVarianceUpperW,chiSquareVarianceCenterW,chiSquareVariancePrecisionW,
        chiSquareVarianceLowerNq,chiSquareVarianceUpperNq,chiSquareVarianceCenterNq,chiSquareVariancePrecisionNq,total_time);
       
       

        
        //Verificar precisao de 5%
        
        /* if((tStudentMeanPrecisionW > 5 || tStudentVariancePrecisionW >5 || chiSquareVariancePrecisionW > 5 || 
        tStudentMeanPrecisionNq > 5 || tStudentVariancePrecisionNq > 5 || chiSquareVariancePrecisionNq > 5) ||
        !valueIsInsideInterval(tStudentMeanLowerW,tStudentMeanUpperW,EW) || 
        !valueIsInsideInterval(tStudentVarianceLowerW,tStudentVarianceUpperW,VW) || 
        !valueIsInsideInterval(tStudentMeanLowerNq,tStudentMeanUpperNq,ENq) || 
        !valueIsInsideInterval(tStudentVarianceLowerNq,tStudentVarianceUpperNq,VNq) || 
        !valueIsInsideInterval(chiSquareVarianceLowerW,chiSquareVarianceUpperW,VW) ||         
        !valueIsInsideInterval(chiSquareVarianceLowerNq,chiSquareVarianceUpperNq,VNq)){
            KMIN += 100;
            overlappingICs = 0;
        } */
        

        // verifica se os ICs convergem, mantendo o valor analitico dentro do IC    
        /* if(!(valueIsInsideInterval(chiSquareVarianceLowerW, chiSquareVarianceUpperW, tStudentVarianceCenterW) 
        && valueIsInsideInterval(tStudentVarianceLowerW,tStudentVarianceUpperW, chiSquareVarianceCenterW)) ||
            !valueIsInsideInterval(tStudentMeanLowerW,tStudentMeanUpperW,EW) || 
            !valueIsInsideInterval(tStudentVarianceLowerW,tStudentVarianceUpperW,VW) || 
            !valueIsInsideInterval(tStudentMeanLowerNq,tStudentMeanUpperNq,ENq) || 
            !valueIsInsideInterval(tStudentVarianceLowerNq,tStudentVarianceUpperNq,VNq) || 
            !valueIsInsideInterval(chiSquareVarianceLowerW,chiSquareVarianceUpperW,VW) ||         
            !valueIsInsideInterval(chiSquareVarianceLowerNq,chiSquareVarianceUpperNq,VNq) ){
            KMIN += 100;
            overlappingICs = 0;

        } */

        printf("Simulacao para KMin = %d durou %f segundos\n",KMIN,total_time);

        
        //Não incrementar o K minimo, executar apenas o k de entrada
        
        //Não incrementa o K, para que durante os testes do professor ele possa executar para Ks unicos
        
        //KMIN += 5000;
        //overlappingICs = 0;

        


        if(strcmp(policy,"FCFS") == 0)
            queueDestroy(queueClients);   
        else
            stackDestroy(stackClients);
        
    } 
       
}

int main(int argc, char *argv[]) {

    


    int kmin = 0;
    int seed = 0;
    float arrival_rate = 0;
    char* policy;
    char dir[PATH_MAX];

    //Montando caminho para o arquivo das metricas localmente
    if (getcwd(dir, sizeof(dir)))
        strcat(dir, "\\Metricas_Simulador.txt");


    //Verificar se o usuario digitou um valor para semente
    if( argc == 5 )
      seed = atoi(argv[4]);
    else
      seed = time(NULL);



    //Coleta os outros argumentos digitados pelo usuario
    if( argc >= 4 && argc <=5) {
    
      kmin = atoi(argv[1]);
    
      if (kmin < 5){
        printf("K minimo pequeno demais, insira kmim > 5\n");
        exit(0);    
      }

      arrival_rate = atof(argv[2]);
      policy = argv[3];
      
    }
    else if( argc > 5 ) {
        printf("Argumentos demais para o programa, insira: %s  <valor de k minimo inicial> <valor da taxa de chegada><politica de atendimento(FCFS ou LCFS)><(OPICIONAL)semente para numeros aleatorios>\n", argv[0]);
        exit(0);
    }
    else {
        printf("Argumentos faltando para o programa, insira: %s <valor de k minimo inicial> <valor da taxa de chegada><politica de atendimento(FCFS ou LCFS)><(OPICIONAL)semente para numeros aleatorios>\n", argv[0]);
        exit(0);
    }
    if(strcmp(policy,"FCFS") != 0  && strcmp(policy,"LCFS")  != 0) {
        printf("politica de atendimento invalida insira: %s <valor de k minimo inicial> <valor da taxa de chegada><politica de atendimento(FCFS ou LCFS)><(OPICIONAL)semente para numeros aleatorios>\n", argv[0]);
        exit(0);  
    }


    FILE *fp; 
    fp = fopen(dir,"w");


    printf("Iniciando execucao com lambda = %f\n", arrival_rate);

    Simulation(seed, kmin,arrival_rate, policy,fp);



    fclose (fp);


    return 0;
}