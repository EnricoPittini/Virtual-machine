#include<stdio.h>
#include"stampa.h"

/*Funzione stampa che dato un vettore di interi arr, di dimensione dim, contenente le istruzioni e i loro parametri, stampa su terminale il
  formato mnemonico di queste istruzioni.*/

void stampa(int *arr,int dim){

      int i=0;/*E' l'indice che mi dice dove sono arrivato a stampare sull'array arr*/

      int error=0;/*E' una variabile che mi dice se ci sono stati errori (error=1) oppure no (error=0)*/

      while(i<dim && !error){/*In ogni iterazione del ciclo while stampo un'istruzione. Termino o quando finisce l'array(i>=dim) o quando
                               rilevo un errore nel stampare un'istruzione*/

            printf("[%3d]  ",i);/*Stampo l'indice dell'istruzione*/

            if(!stampa_istruzione(arr,&i))
                  error=1;
            /* stampa_istruzione è una funzione che stampa l'istruzione con indice i. Siccome ogni istruzione ha un numero diverso di
               parametri, faccio in modo che la funzione modifichi l'indice i in modo che corrisponda alla posizione nell' array dell'ultimo
               parametro dell'istruzione attuale. Poichè modifico i da dentro la funzione, passo il suo indirizzo.

               Inoltre la funzione stampa_istruzione ritorna 0 in caso di errori, 1 altrimenti*/

            i++;/*Ora i corrisponde alla posizione nell'array della prossima istruzione*/
      }
}


/*Funzione che stampa l'istruzione con indice *i e modifica *i in modo che sia la posizione dell'ultimo parametro dell'istruzione attuale*/

int stampa_istruzione(int *arr,int *i){/* i punta alla variabile contenente l'indice dell'istruzione attuale*/

      int istr=arr[*i];/*istr è l'intero corrispondente all'istruzione attuale, ovvero quella con indice *i*/

      int risp=0;/*risp è una variabile intera dove salvo il tipo d'errore trovato. Se non trovo errori, alla fine della funzione risp varrà
                   0. Le tipologie di errori possibili sono: registro non valido (risp==1); istruzione non valida (risp==2)*/


      /*Ora segue uno switch con vari casi per identificare che tipo di istruzione è. Per ogni istruzione diversa dovrò stampare una stringa mnemonica
        diversa. In più, le istruzioni hanno numero e tipologia di parametri diversi.
        Oltre a stampare la stringa mnemonica, modifico *i in modo che sia la posizione dell'ultimo parametro dell'istruzione attuale.

        Inoltre, cerco di rilevare eventuali errori*/


      switch(istr){

          case 0:/*HALT*/
              printf("HALT\n");
              break;

          case 1:/*DISPLAY*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else
                  printf("DISPLAY R%d\n",arr[*i]);
              break;


          case 2:/*PRINT_STACK*/
              (*i)++;
              printf("PRINT_STACK %d\n",arr[*i]);
              break;


          case 10:/*PUSH*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else
                  printf("PUSH R%d\n",arr[*i]);
              break;


          case 11:/*POP*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else
                  printf("POP R%d\n",arr[*i]);
              break;


          case 12:/*MOV*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else{
                  printf("MOV R%d %d\n",arr[*i],arr[(*i)+1]);
                  (*i)++;
              }
              break;


          case 20:/*CALL*/
              (*i)++;
              printf("CALL %d\n",arr[*i]);
              break;


          case 21:/*RET*/
              printf("RET\n");
              break;

          case 22:/*JMP*/
              (*i)++;
              printf("JMP %d\n",arr[*i]);
              break;


          case 23:/*JZ*/
              (*i)++;
              printf("JZ %d\n",arr[*i]);
              break;


          case 24:/*JPOS*/
              (*i)++;
              printf("JPOS %d\n",arr[*i]);
              break;


          case 25:/*JNEG*/
              (*i)++;
              printf("JNEG %d\n",arr[*i]);
              break;


          case 30:/*ADD*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31 || arr[*i+1]<0 || arr[*i+1]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else{
                  printf("ADD R%d R%d\n",arr[*i],arr[(*i)+1]);
                  (*i)++;
              }
              break;


          case 31:/*SUB*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31 || arr[*i+1]<0 || arr[*i+1]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else{
                  printf("SUB R%d R%d\n",arr[*i],arr[(*i)+1]);
                  (*i)++;
              }
              break;


          case 32:/*MUL*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31 || arr[*i+1]<0 || arr[*i+1]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else{
                  printf("MUL R%d R%d\n",arr[*i],arr[(*i)+1]);
                  (*i)++;
              }
              break;


          case 33:/*DIV*/
              (*i)++;
              if(arr[*i]<0 || arr[*i]>31 || arr[*i+1]<0 || arr[*i+1]>31)/*resistro non valido. Ci sono 32 registri, che vanno da 0 a 31*/
                  risp=1;
              else{
                  printf("DIV R%d R%d\n",arr[*i],arr[(*i)+1]);
                  (*i)++;
              }
              break;


          default:/*istruzione non valida*/
              risp=2;
              break;

      }

      /*Ora vedo se c'è stato qualche errore e in caso lo stampo a schermo.*/
      if(risp==1)
          printf("ERRORE: registro non valido\n");
      if(risp==2)
          printf("ERRORE: istruzione non valida\n");

      return !risp;/*Ritorno 1 se non ci sono stati errori (risp==0), 0 altrimenti (risp!=0)*/

}
