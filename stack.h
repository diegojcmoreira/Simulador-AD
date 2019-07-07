#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int memberSize; //tamanha de cada dado que sera armazenado
  int capacity; //capacidade da pilha
  void* data; //dados armazenados na pilha 
  int top; //posicao do ultimo elemento da pilha
} Stack;

//aloca espaco de memoria para a pilha e retorna um ponteiro para ela
//recebe o tamanho do dado que a pilha armazena e a sua capacidade
//retorna uma instancia da pilha
Stack* createStack(int memberSize, int capacity);

//libera espaco de memoria da pilha
//recebe a pilha cujo espaco sera liberado
void stackDestroy(Stack *s);

//adiciona um elemento na pilha
//recebe a pilha onde o dado sera adicionado e o dado em si
void stackPush(Stack *s, void *data);

//remove o elemento que esta no topo da pilha, seu valor eh retornado na variavel target
//recebe a pilha de onde o dado sera retirado e a variavel onde sera armazenada este dado
//retorna 0 se a operacao foi bem sucedida e 1 se nao
int stackPop(Stack *s, void *target);

//retorna o numero de elementos na pilha
//recebe a pilha cujo tamanho eh desejado
int stackSize(Stack *s);

#endif