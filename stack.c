#include "stack.h"

Stack* createStack(int memberSize, int capacity) {
  Stack *s = malloc(sizeof(Stack));
  
  s -> top = -1;
  s -> memberSize = memberSize;
  s -> capacity = capacity;
  s -> data = malloc(capacity*memberSize);
  
  return s;
}

//dobra capacidade da pilha
void expandStack(Stack* s) {
  s -> data = realloc(s -> data, s -> capacity * 2 * s -> memberSize);
  s -> capacity *= 2;
}

void stackPush(Stack *s,  void *element) {
  //verifica se a pilha esta cheia
  if (s -> top == s -> capacity - 1)
    expandStack(s); //aumenta capacidade da pilha
  
  s -> top++;
  //calcula posicao de inicio do novo elemento
  void* target = (char*) s -> data + (s -> top*s -> memberSize);
  memcpy(target, element, s -> memberSize);
}

int stackPop(Stack *s,  void *target) {
  if (s->top == -1) {
    return 1;
  }

  //calcula posicao de inicio do ultimo elemento da pilha
  void* source = (char*) s -> data + (s -> top * s -> memberSize);
  s->top--;
  memcpy(target, source, s->memberSize);
  return 0;
}

void stackDestroy(Stack *s) {
  free(s -> data);
  free(s);
}