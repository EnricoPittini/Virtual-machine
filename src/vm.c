#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"leggi.h"
#include"stampa.h"
#include"esegui.h"




int main(int argc,char *argv[]){
        /*Il programma deve prendere due parametri da linea di comando: il primo (argv[1]) dice l'azione da svolgere, il secondo (argv[2])
        è il nome del file contenete il programma da eseguire. I parametri da linea di comando (argv[1] e argv[2]) sono due stringhe.*/

      int *arr=NULL; /*Variabile puntatore che punterà all'array contenete le istruzioni e i parametri delle istruzioni*/

      int dim=0; /*Variabile intera che conterrà la dimensione dell'array arr*/

      if(argc!=3)/*Controllo che il numero di parametri da linea di comando sia giusto*/

          printf("ERRORE: il programma VM accetta due parametri da linea di comando\n");

      else{

        if(!strcmp(argv[1],"stampa")){
        /*Se la stringa argv[1] è uguale alla stringa "stampa" allora devo stampare il programma in input nel suo formato mnemonico/assembler.
          Uso la funzione di libreria strcmp per verificare che le stringhe siano uguali*/

              if(leggi(argv[2],&arr,&dim)){/*leggi mette le istruzioni e i parametri delle istruzioni, presenti nel file con nome argv[2],
                 dentro l'array di interi arr,creato dinamicamente sempre dentro la funzione. Passo alla funzione gli indirizzi di arr e dim
                 perchè così posso modificarli da dentro la funzione. Alla fine dell'esecuzione della funzione arr conterrà l'indirizzo
                 dell'array dinamico contente le istruzioni e dim conterrà la sua dimensione.
                 leggi ritorna 1 in caso di successo, 0 altrimenti (qualsiasi tipo di errore).
                 I vari tipi di errore che possono esserci in lettura li gestico dentro la funzione leggi.
                 Vedi leggi.c*/

                  stampa(arr,dim);/*La funzione "stampa" stampa il formato mnemonico delle istruzioni che sono dentro l'array arr. Vedi stampa.c*/

                  free(arr);/*arr è un array dinamico: prima del termine del programma VM devo liberare lo spazio allocato dinamicamente*/
              }

        }

        else if(!strcmp(argv[1],"esegui")){
        /*Se la stringa argv[1] è uguale alla stringa "esegui" allora devo eseguire il programma in input. Uso come prima strcmp*/

              if(leggi(argv[2],&arr,&dim)){/*Anche qua mi serve creare l'array con dentro le istruzioni e i loro parametri*/

                    esegui(arr,dim);/*La funzione esegui esegue le istruzioni presenti dentro l'array arr*/

                    free(arr);/*Libero il vettore dinamico arr*/

              }

        }
      }

        return 0;
}
