#include<stdio.h>
#include<stdlib.h>
#include"esegui.h"

/*Lo stack di interi deve essere di 64 KB ovvero 2^16 B. Un intero è 2^2 Byte: dunque lo stack è composto da 2^16/2^2= 2^14 interi.
 Stack con 2^14 elementi, ovvero 16384*/
#define N_STACK 16384

#define N_MAX 2147483647 /*Numero intero massimo rappresentabile su 4 Byte (32 bit). 2147483647=2^31 - 1*/

/*Numero intero minimo rapresentabile su 4 Byte (32 bit). -2147483648=-2^31. */
#define N_MIN -2147483647 /*Numero intero minimo rapresentabile su 4 Byte (32 bit). -2147483648=-2^31.*/




/**************************STACK: PRIMITIVE*************************************************************************************************/

/* Definisco le primitive del mio stack. Lo stack deve essere di 64KB, ovvero 2^14 interi*/

tstack getempty(){/*funzione che ritorna uno stack vuoto*/
      tstack risp;
      risp.v=(int *)malloc(sizeof(int)*N_STACK);/*Alloco dinamicamente un vettore con 2^14=16384 elementi. Il campo v del mio stack punta
                                                a questo vettore*/
      /*controllare allocazione malloc*/
      risp.sp=0;/*stack vuoto*/
      return risp;
}

int isempty(tstack s){/*Funzione che mi dice se lo stack in input è vuoto*/
      return s.sp==0;
}

int push(tstack *ps,int elem){/*Funzione che mette dentro lo stack puntato da ps l'intero elem. Restituisce 1 se inserimento riuscito, 0 se
                                stack pieno*/
      if(ps->sp<16384){
            ps->v[ps->sp]=elem;/*Mette nella prima posizione libera dello stack elem*/
            ps->sp++;/*Prima posizione libera ora è la successiva*/
            return 1;
      }
      else /*stack pieno*/
            return 0;
}

int pop(tstack *ps,int *elem){/*Funzione che mette dentro la variabile puntata da elem l'ultimo elemento dello stack. Ritorna 1 se estrazione
                                riuscita, 0 se stack vuoto*/

    if(!isempty(*ps)){
          ps->sp--;/*Decremento sp. Ora la prima posizione libera dello stack è la precedente*/
          *elem=ps->v[ps->sp];/*Metto dentro la variabile puntata da elem l'ultimo elemento dello stack*/
          return 1;
    }
    else /*stack vuoto*/
          return 0;
}
/****************************************************************************************************************************************/

/* esegui è una funzione che dato un vettore di interi arr, di dimensione dim, contenente le istruzioni e i parametri delle istruzioni del programma in
   input, esegui queste istruzioni. */

void esegui(int *arr,int dim){

        tstack s=getempty();/*Creo il mio stack, per ora vuoto*/

        if(s.v!=NULL){/*Controllo che l'allocazione del vettore dinamico sia andata a buon fine*/

          unsigned int ip=0;/*INSTRUCTION POINTER: variabile che mi indica la posizione di dove sono arrivato sull'array arr*/

          int R[32];/*Array di 32 interi: sono i miei 32 registri. R è l'indirizzo di R0*/

          int error=0;/*Variabile che mi dice se ci sono stati errori (error=1) oppure no (error=0):*/

          while(arr[ip]!=0 && !error)/*continuo a eseguire le istruzioni finchè non raggiungo l'istruzione HALT (codificata con 0) e finchè
                                       non ho trovato errori*/

              if(!esegui_istruzione(arr,dim,&ip,&s,R))
                    error=1;
              /*esegui_istruzione è una funzione che esegue l'istruzione con indice ip (dell'array arr). L'eseguire un'istruzione può comportare la
                lettura/modifica di un registro (passo alla funzione R), o la lettura/mdofica dello stack (passo l'indirizzo dello stack s,
                così posso modificarlo da dentro la funzione). Devo inoltre modificare ip in modo che sia l'indice della prossima istruzione
                da eseguire : passo alla funzione l'indirizzo di ip, così posso modificarlo da dentro la funzione.

                esegui_istruzione inoltre ritorna 0 in caso di errore, 1 altrimenti*/


          free(s.v);/*Libero il vettore allocato dinamicamente*/
        }

        else/* Errore allocazione malloc*/
            printf("ERRORE: allocazione dinamica non andata a buon fine\n");
}


/*esegui_istruzione è una funzione che esegue un'istruzione, interagendo e modificando opportunamente ip, R e lo stack. Ritorna 0 in caso di
  errore, 1 altrimenti*/

int esegui_istruzione(int *arr,int dim,unsigned int *ip,tstack *ps,int *R){

      int risp=0;/*variabile dove memorizzo la tipologia di errore incontrata. Se alla fine della funzione vale 0, allora non ci sono stati
                   errori. Se invece il suo valore è !=0, allora c'è stato un errore (identificato dal numero).
                   In caso di errore stampo sullo schermo il tipo di errore e lo segnalo al chiamante (così interrompo l'esecuzione)*/

      int istr=arr[*ip];/*istr è una variabile che contiene il codice intero dell'istruzione attuale, ovvero quella con indice ip*/

      /*Ora segue uno switch con vari casi per identificare il tipo di istruzione. Oltre ad eseguire l'istruzione, modifico opportunamente ip in modo
        che sia l'indice della prossima istruzione da eseguire.
        Inoltre, cerco di rilevare eventuali tipologie di errore*/


      switch(istr){

          case 1:{/*DISPLAY*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31)/*registro non valido*/
                  risp=7;

              else{
                  printf("%d\n",R[arr[(*ip)+1]]);/*Stampo il contenuto del registro*/

                  *ip=(*ip)+2;/*La prossima istruzione da eseguire è la successiva, ovvero due posizioni dopo nell'array (salto anche
                                il parametro)*/
              }

              break;
          }



          case 2:{/*PRINT_STACK*/

              int n=arr[(*ip)+1];/*Numero di elementi dello stack da stampare (è specificato come parametro)*/

              int i=1,empty=0;/*i mi serve per contare il numero di elementi stampati, empty mi segnala se ho già stampato tutti gli elementi disponibili
                                nello stack. empty==1 elementi dello stack finiti, empty==0 elementi dello stack non finiti.*/

              /*Ovviamente il caso in cui finiscano gli elementi dell'array può capitare solo nel caso in cui n (numero di elementi dello stack da
                stampare) sia maggiore di sp (indice della prima posizione libera dello stack o in modo equivalente numero di elementi effettivamente
                contenuti nello stack).
                Il caso in cui n>sp abbiamo deciso di gestirlo segnalando un avviso, ma senza interrompere il programma (come invece facciamo con gli
                errori). Stampiamo tutti gli elementi dello stack, diamo l'avviso e continuiamo a eseguire il programma*/

              while(i<=n && !empty){/*Continuo a stampare elementi finchè mi è richiesto (i<=n) e finchè gli elementi dello stack non sono finiti
                                      (empty!=0)*/

                  int index=ps->sp-i;/*Indice dell'elemento dello stack che sto per stampare*/

                  if(index>=0)/*Se l'indice è >=0 vuol dire che non sono finiti gli elementi dello stack*/

                      printf("%d: %d\n",index,ps->v[index]);/*Stampo l'indice e l'elemento dello stack*/

                  else{/*Elementi dello stack finiti*/

                      printf("ATTENZIONE: numero di elementi da stampare maggiore del numero di elementi nello stack\n");/*Stamp a schermo un avviso*/

                      empty=1;/*Elementi dello stack finiti: interrompo il ciclo*/
                  }

                  i++;/*Prossimo elemento da stampare*/
              }

              *ip=(*ip)+2;/*La prossima istruzione da eseguire è la successiva, ovvero due posizioni dopo nell'array (salto anche
                            il parametro)*/

              break;
          }


          case 10:{/*PUSH*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31)/*registro non valido*/
                  risp=7;

              else{

                  if(push(ps,R[arr[(*ip)+1]]))/*Metto nello stack il contenuto del registro che ho come parametro. La push restituisce
                                                1 in caso di corretto inserimento nello stack */

                      *ip=(*ip)+2;/*istruzione successiva (2 posizioni dopo)*/

                  else/*stack pieno*/
                       risp=1;
              }

              break;
          }



          case 11:{/*POP*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31)/*registro non valido*/
                  risp=7;

              else{

                  if(pop(ps,&R[arr[(*ip)+1]]))/*Tolgo dallo stack l'ultimo elemento inserito (LIFO) e lo metto nel registro che ho come
                                                parametro. La pop restituisce 1 in caso di stack non vuoto, 0 altrimenti*/

                      *ip=(*ip)+2;/*Istruzione successiva (2 posizioni dopo)*/

                  else/*stack vuoto*/
                      risp=2;
              }

              break;
          }



          case 12:{/*MOV*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31)/*resistro non valido.*/
                risp=7;

              else{
                R[arr[(*ip)+1]]=arr[(*ip)+2];/*Metto nel registro che ho come primo parametro il valore numerico che ho come secondo
                                               parametro*/

               *ip=(*ip)+3;/*Istruzione successiva (devo saltare due parametri)*/
              }

              break;
          }


          case 20:{/*CALL*/

              if(arr[*ip+1]>=0 && arr[*ip+1]<=dim-1){/*Indice di salto corretto (prossima istruzione da eseguire ha indice compreso tra 0 e dim-1,
                                                        ovvero ha indice dentro l'array)*/

                  if(push(ps,(*ip)+2))/*Metto nello stack l'indice corrispondente all'istruzione successiva. Così quando torno dalla Funzione
                                        al chiamante so dove tornare*/

                          *ip=arr[(*ip)+1];/*La prossima istruzione da eseguire non è la successiva ma quella con indice specificato come
                                              parametro*/

                  else/*stack pieno*/
                          risp=1;
              }
              else/*Indice di salto non corretto, fuori dall'array contenente il programma*/
                    risp=9;

              break;
          }



          case 21:{/*RET*/

              int temp;  /*serve perchè ip è unsigned int* mentre la pop come secondo parametro vuole un int* */

              if(pop(ps,&temp)){/*Tolgo dallo stack l'ultimo elemento (indice di ritorno dalla funzione) e lo metto dentro temp*/

                    if(temp>=0 && temp<=dim-1)/*Indice di salto corretto, dentro l'array*/

                             *ip=temp;/*Metto il valore appena preso dentro a ip, ovvero l'indice della prossima istruzione da eseguire*/

                    else/*Indice di salto non corretto, fuori dall'array*/
                              risp=9;
              }
              else/*stack vuoto*/
                  risp=2;

              break;
          }



          case 22:/*JMP*/

              if(arr[*ip+1]>=0 && arr[*ip+1]<=dim-1)/*Indice di salto corretto, dentro l'array*/

                      *ip=arr[(*ip)+1];/*Indice della prossima istruzione da eseguire è specificato come parametro. E' un salto incondizionato*/

              else/*Indice di salto nfuori dall'array*/
                      risp=9;

              break;



          case 23:{/*JZ*/

              if(arr[*ip+1]>=0 && arr[*ip+1]<=dim-1){/*Indice di salto corretto, dentro l'array*/

                  int elem;

                  if(pop(ps,&elem))/*Tolgo l'ultimo elemento dello stack (sempre cje non sia vuoto) e lo metto dentro elem*/

                        if(elem==0)/*se elem è uguale a 0 allora salto (indice prossima istruzione è quello specificato come parametro).*/
                                    /*Salto condizionato*/

                              *ip=arr[(*ip)+1];

                        else   /*Se elem è diverso da 0 non salto: la prossima istruzione da eseguire è la successiva*/

                              *ip=(*ip)+2;/*Metto dentro a ip indice prossima istruzione da eseguire*/

                  else/*stack vuoto*/
                        risp=2;
              }

              else/*Indice di salto fuori dall'array*/
                  risp=9;

              break;
          }



          case 24:{/*JPOS*/

              if(arr[*ip+1]>=0 && arr[*ip+1]<=dim-1){/*Indice di salto corretto, dentro l'array*/

                  int elem;

                  if(pop(ps,&elem))/*Tolgo l'ultimo elemento dello stack (sempre che non sia vuoto) e lo metto dentro a elem*/

                      if(elem>0)/*Se elem è maggiore di 0 allora salto. Salto condizionato*/

                          *ip=arr[(*ip)+1];/*Indice prossima istruzione da eseguire specificata come parametro*/

                      else   /*Se elem non è maggiore di 0 non salto: prossima istruzione da eseguire è la successiva*/

                          *ip=(*ip)+2;

                  else/*stack vuoto*/
                      risp=2;
              }

              else/*Indice di salto scorretto, fuori dall'array*/
                  risp=9;

              break;
          }



          case 25:{/*JNEG*/

              if(arr[*ip+1]>=0 && arr[*ip+1]<=dim-1){/*Indice di salto corretto, dentro l'array*/

                  int elem;

                  if(pop(ps,&elem))/*Tolgo l'ultimo elemento dello stack (sempre che non sia vuoto) e lo metto dentro a elem*/

                        if(elem<0)/*Se elem è minore di 0 allora salto. Salto condizionato*/

                            *ip=arr[(*ip)+1];/*Indice prossima istruzione da eseguire specificata come parametro*/

                        else /*Se elem non è minore di 0 non salto: prossima istruzione da eseguire è la successiva*/

                            *ip=(*ip)+2;

                  else/*stack vuoto*/
                        risp=2;
              }

              else/*Indice di salto scorretto, fuori dall'array*/
                  risp=9;

              break;
          }



          case 30:{/*ADD*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31 || arr[*ip+2]<0 || arr[*ip+2]>32)/*registro non valido*/
                  risp=7;

              else{

                  int p1=R[arr[*ip+1]];/*Primo operando (primo registro specificato come parametro)*/

                  int p2=R[arr[*ip+2]];/*Secondo operando (secondo registro specificato come parametro)*/

                  if(overflow(0,p1,p2))/*Chiamo la funzione overflow che ritorna 1 se c'è overflow.  Prende come parametri il codice
                                     dell'operazione (0 per somma, 1 per moltiplicazione) e i due parametri*/

                          risp=3;/*overflow somma*/

                  else{/*Non c'è overflow*/

                          int sum=p1+p2;/*somma tra gli operandi*/

                          if(push(ps,sum))/*Metto la somma nello stack (sempre che non sia pieno)*/

                                *ip=*ip+3;/*Prossima istruzione da eseguire è la successiva (2 parametri da saltare)*/

                          else/*stack pieno*/
                                risp=1;
                   }
              }

              break;
          }



          case 31:{/*SUB*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31 || arr[*ip+2]<0 || arr[*ip+2]>32)/*registro non valido*/
                    risp=7;

               else{

                    int p1=R[arr[*ip+1]];/*Primo operando*/

                    int p2=R[arr[*ip+2]];/*Secondo operando*/

                    if(overflow(0,p1,-p2))/*Chiamo la funzione overflow e controllo se c'è overflow. Come codice di operazione metto 0, ovvero somma.
                                    Controllare che p1-p2 non dia overflow è come controllare che p1+(-p2) non dia overflow*/

                            risp=4;/*overflow sottrazione*/

                    else{/*Non c'è overflow*/

                            int sub=p1-p2;/*Sottrazione tra i due operandi*/

                            if(push(ps,sub))/*Metto il risultato nello stack (sempre che non sia pieno)*/

                                  *ip=*ip+3;/*Prossima istruzione da eseguire è la successiva (2 parametri da saltare)*/

                            else/*stack pieno*/
                                  risp=1;
                    }
              }

              break;
          }



          case 32:{/*MUL*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31 || arr[*ip+2]<0 || arr[*ip+2]>32)/*registro non valido*/
                    risp=7;

              else{

                    int p1=R[arr[*ip+1]];/*Primo operando*/

                    int p2=R[arr[*ip+2]];/*Secondo operando*/

                    if(overflow(1,p1,p2))/* Chiamo la funzione overflow e controllo che non ci sia overflow. Codice di operazione 1 (moltiplicazione)*/

                        risp=5;/*overflow moltiplicazione*/

                    else{/*Non c'è overflow*/

                        int mul=p1*p2;/*Eseguo la moltiplicazione*/

                        if(push(ps,mul))/*Metto il risultato nello stack(sempre che non sia pieno)*/

                            *ip=*ip+3;/*Prossima istruzione da eseguire è la successiva (2 parametri da saltare)*/

                        else/*stack pieno*/
                            risp=1;
                    }
              }

              break;
          }



          case 33:{/*DIV*/

              if(arr[*ip+1]<0 || arr[*ip+1]>31 || arr[*ip+2]<0 || arr[*ip+2]>32)/*registro non valido*/
                    risp=7;

              else{

                    int p1=R[arr[*ip+1]];/*Primo operando*/

                    int p2=R[arr[*ip+2]];/*Secondo operando*/

                    if(p2!=0){/*COntrollo che il secondo operando non sia 0*/

                        int div=p1/p2;/*Eseguo l'operazione*/

                        if(push(ps,div))/*Metto il risultato nello stack (sempre che non sia pieno)*/

                            *ip=*ip+3;/*Prossima istruzione da eseguire è la successiva (2 parametri da saltare)*/

                        else/*stack pieno*/
                            risp=1;
                    }

                    else/*divisione per zero*/
                        risp=6;
              }

              break;
          }


          default:
              risp=8;/*istruzione non valida*/

              break;

      }


      /*Ora vedo se c'è stato qualche errore e in caso lo identifico e lo stampo a schermo.*/

      if(risp==1)
          printf("ERRORE: stack pieno\n");
      if(risp==2)
          printf("ERRORE: stack vuoto\n");
      if(risp==3)
          printf("ERRORE: overflow somma\n");
      if(risp==4)
          printf("ERRORE: overflow sottrazione\n");
      if(risp==5)
          printf("ERRORE: overflow moltiplicazione\n");
      if(risp==6)
          printf("ERRORE: divisione per zero\n");
      if(risp==7)
          printf("ERRORE: registro non valido\n");
      if(risp==8)
          printf("ERRORE: istruzione non valida\n");
      if(risp==9)
          printf("ERRORE: indirizzo(indice) di salto non corretto\n");


      return !risp;/*Ritorno 1 se non ci sono stati errori (risp==0), 0 altrimenti (risp!=0)*/
}


/*Funzione che restituisce 1 in caso di overflow, 0 altrimenti. Prende come parametri il codice numerico che rappresenta il tipo di
  operazione e i due operandi.*/

int overflow(int op,int p1,int p2){/*op codice operazione, p1 primo parametro, p2 secondo parametro*/

      if(op==0 && p1!=0 && p2!=0){/*overflow somma: op==0 (anche sottrazione è compresa). Se anche uno solo dei due operandi è 0 non testo
                                    neanche l'overflow perche sicuramente il risultato dell'operazione sarà rappresentabile*/

            int s1=(p1>0)?1:-1; /*In s1 metto il segno di p1*/

            int s2=(p2>0)?1:-1;/*In s2 metto il segno di p2*/

            if(s1*s2>0){/*Testo l'overflow solo se p1 e p2 hanno lo stesso segno: solo in questo caso può infatti verificarsi overflow*/

                if(s1>0){/* Caso in cui p1>0 && p2>0. L'overflow può verificarsi superando il massimo numero positivo (N_MAX)*/

                /*Devo testare se p1+p2>N_MAX senza però effettivamente svolgere l'operazione p1+p2 (se no appunto in caso di overflow il
                  risultato sarebbe sbagliato e verrebbe segnalato dal sistema C). Allora testo se p2>N_MAX-p1.*/

                    int temp=N_MAX-p1;/*Metto in una variabile temporanea N_MAX -p1*/

                    if(p2>temp)
                      return 1;/*overflow rilevato*/
                }

                else{/*Caso in cui p1<0 && p2<0. L'overflow può verificarsi se il risultato è minore del minore numero negativo (N_MIN)*/

                  /*Devo testare se p1+p2<N_MIN senza però effettivamente svolgere l'operazione p1+p2 (se no appunto in caso di overflow il
                    risultato sarebbe sbagliato e verrebbe segnalato dal sistema C). Allora testo se p2<N_MAX-p1.*/


                  int temp=N_MIN-1-p1;/*Metto in una variabile temporanea N_MIN-p1*/

                  if(p2<temp)
                     return 1;/*overflow rilevato*/
                }
            }
      }

      if(op==1 && p1!=0 && p2!=0 && p1!=1 && p2!=1){/*overflow moltiplicazione: op==1. Se anche uno solo dei due operandi è 0 o 1 non testo
                                                      neanche l'overflow perche sicuramente il risultato dell'operazione sarà rappresentabile*/

        int s1=(p1>0)?1:-1;/*In s1 metto il segno di p1*/

        int s2=(p2>0)?1:-1;/*In s2 metto il segno di p2*/

        if(s1*s2>0){/*(p1>0 && p2>0) || (p1<0 && p2<0): in entrambi i casi l'overflow può verificarsi superando il più grande numero positivo*/

                int temp;

                p1=p1*s1;/*valore assoluto di p1*/

                p2=p2*s2;/*valore assoluto di p2*/

                /*Devo testare se p1*p2>N_MAX senza però effettivamente svolgere l'operazione p1*p2 (se no appunto in caso di overflow il
                  risultato sarebbe sbagliato e verrebbe segnalato dal sistema C). Allora testo se p1>N_MAX/p2.*/


                temp=N_MAX/p2;/*Metto N_MAX/p2 in una variabile temporanea*/

                if(p1>temp)
                  return 1;/*overflow rilevato*/
        }

        else{/*(p1<0 && p2>0) || (p1>0 && p2<0): in entrambi i casi l'overflow può verificarsi se il risultato è minore del minimo numero
               negativo rappresentabile (N_MIN)*/

               int temp;

               p1=p1*s1;/*valore assoluto di p1*/

               p2=p2*s2;/*valore assoluto di p2*/

               /*Devo testare se p1*p2>-N_MIN senza però effettivamente svolgere l'operazione p1*p2 (se no appunto in caso di overflow il
                 risultato sarebbe sbagliato e verrebbe segnalato dal sistema C). Allora testo se p1>-N_MAX/p2.*/


               temp=(N_MIN-1)/p2;

               temp=-temp;/*attenzione, bisogna cambiare il segno. p1 e p2 sono entrambi valori assoluti*/

               if(p1>temp)
                  return 1;/*overflow rilevato*/
       }
     }

      return 0;/*nessun overflow trovato*/

}
