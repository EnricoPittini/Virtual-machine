/**************************STACK: DEFINIZIONE DI TIPO********************************************/

/* Definisco lo stack come una struttura con un puntatore a interi, che punta all'array in memoria dinamica contenete i valori dello stack,
  e con un unsigned int che è l'indice che corrisponde alla prima posizione libera nello stack*/

struct stack{/*struttura stack: è formata da un puntatore e da un unsigned int*/

      int *v;/*Puntatore a interi che punta all'array dinamico contenente i valori dello stack*/

      unsigned int sp;/*STACK POINTER: è l'indice della prima posizione libera dello stack*/
};
typedef struct stack tstack;/*Definisco il tipo tstack*/

/**************************************************************************************************/

int overflow(int op,int p1,int p2);
int esegui_istruzione(int *arr,int dim,unsigned int *ip,tstack *ps,int *R);
void esegui(int *arr,int dim);
