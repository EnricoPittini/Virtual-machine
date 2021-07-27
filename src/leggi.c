#include<stdio.h>
#include<stdlib.h>
#include"leggi.h"


int control(char *buffer){ /*funzione che data la stringa contenente una riga acquisita dal file ,verifica se ci sono errori di formato. Se
                             sono presenti errori la funzione ritorna 1 altrimenti 0 */

int i=0; /* indice della stringa */
int flag=0; /* flag che indica se un ";" è stato trovato */
int flag1=0; /* flag che indica se un numero è stato trovato */
int err=0; /* variabile che segnala se c'è un errore di formato nella riga letta */

/* continua a controllare la stringa fino a quando o non è finita,o c'è un errore ,o
quando non trova un ";"  (è stato determinato che prima del ";" sia tutto corretto */

while(buffer[i]!='\0' && err==0 && flag==0){



        /* se ha trovato un ";" mette a 1 un flag che segnala che ha trovato un ";" */

        if(buffer[i]==';')
           flag=1;

        /* Se prima del ";" ha trovato un carattere che non è un numero,o uno spazio ,o un ";" ,o uno dei caratteri per
           andare a capo ,mette a 1 una variabile errore per segnalare che c'è un errore nel formato della riga */

        if((buffer[i]<'0'||buffer[i]>'9') && buffer[i]!=' ' && buffer[i]!=';' && buffer[i]!='\r' && buffer[i]!='\n')
          err=1;

        /*se ha trovato una cifra (prima di un ";") dopo che è già stata trovato un numero
          la variabile err che segnala l'errore di formato viene messa a 1 */

        if(buffer[i]>='0' && buffer[i]<='9' && flag1==1)
          err=1;

        /*se ha trovato una cifra (prima di un ";") e nessun numero è stato trovato prima, allora
          viene verificato quanto lungo è il numero (in modo che l'indice della stringa prosegua
          fino alla fine del numero) e viene messo a 1 un flag che indica che un numero è stato trovato */


        if(buffer[i]>='0' && buffer[i]<='9' && flag1==0){

          /* l'indice della stringa viene fatto scorrere fino alla fine del numero */

          while(buffer[i]>='0' && buffer[i]<='9')
               i++;

          flag1=1; /* il flag che indica che un numero è stato trovato (prima di un ";") viene messo a 1 */
          i--; /* l'indice viene decrementato in modo che con il prossimo incremento la posizione nella stringa non venga spostata più del dovuto */
          }


      i++; /* l'indice della stringa viene incrementato, per verificare poi il contenuto della successiva posizione della stringa al ciclo successivo */
     }

if(err==1) /* se c'è stato un errore di formato allora la funzione ritorna 1 */
  return 1;

return 0; /* se nessun errore di formato è stato riscontrato allora la funzione ritorna 0 */
}

/* Funzione leggi che apre il file in input s, contenete le istruzioni e i parametri delle istruzioni, e crea un vettore dinamico contenente
   le istruzioni e i loro parametri. Modifica la variabile del main arr, facendola puntare proprio all'array dinamico. Modifica la variabile
   del main dim mettendoci dentro la dimensione dell'array dinamico. Per modificore le variabili del main arr e dim da dentro la funzione, uso
   i loro indirizzi.
   leggi ritorna 1 in caso sia andato tutto bene (nessun errore); ritorna 0 altrimenti
*/

int leggi(char *s,int **pa,int *pdim){/* s contiene il nome del file, pa l'indirizzo della variabile arr del main, pdim l'indirizzo della
                                        variabile dim del main */

        char *buffer=NULL; /* buffer dove metto la stringa letta da getline */
        size_t nBytes=0;   /* variabile che contiene il numero di Bytes letti da getline */
        int i=0; /* è l'indice dell'array di istruzioni , ed è la variabile che tiene conto del numero di istruzioni acquisite */
        int elem; /* variabile contenente il codice di un istruzione ad ogni acquisizione */
        int flag_err=0; /* flag che segnala se c'è stato un errore (messo a 1 in caso di errore,messo a 0 in caso non ci siano errori) */
        int flag_exit=0; /* flag che viene messo a 1, non appena la dimensione dell'array è stata trovata, per uscire dal ciclo */
        int line=0; /* variabile che indica a che linea vi è un errore di formato (se c'è) */
        ssize_t end=0; /* variabile che segnala se la fine del file è stata raggiunta */
        int error_type=-1; /* variabile che contiene il codice d'errore in caso si verifichi un errore */
        FILE *f; /* file */

        f=fopen(s,"r"); /* Apro il file s in modalità lettura */

        if(f){ /*file esiste*/

              /*acquisisce la dimensione dell'array di istruzioni*/

              do{

                   end=getline(&buffer,&nBytes,f); /* acquisisce una riga dal file */

                   if(end!=-1){ /* verifica che qualcosa sia stato letto */

                     if(control(buffer)){ /* controlla se ci sono errori di formato nella riga acquisita */
                         if(flag_err!=1){ /* controlla che non sia già stato segnalato un errore, in caso affermativo non modifico il codice d'errore
                                             cosi da poter segnalare solo l'errore trovato per primo */
                             error_type=0;
                             flag_err=1;
                           }
                         }

                     else{                                 /* se non ci sono errori di formato nella riga letta, viene acquisito il codice dell'istruzione (se presente) */
                          if(sscanf(buffer,"%d",&elem)>0){ /* se è presente il codice di un istruzione lo acquisisce */
                               *pdim=elem;  /* inserisce nella variabile puntatata da pdim (destinata a contere la dimesione dell'array di istruzioni) la dimensione acquisita  */
                               flag_exit=1; /* esco dal ciclo (poichè ho trovato la dimensione dell'array di istruzioni */
                            }
                         }
                      }

                    line++; /* incrementa la variabile in modo che sia aggiornata sulla riga che sta analizzando */

                }while(end!=-1 && flag_err==0 && flag_exit==0); /* continua ad analizzare le righe finchè o il file non è finito, o non è stato trovato un errore,o la dimensione non è stata acquisita */

             /*acquisice i numeri che indentificano le istruzioni asembly */

             if(*pdim>=0){ /* controlla che la dimensione dell'array di istruzioni acquisita sia >=0 (in caso contrario il resto del file non viene analizzato,
                              poichè non vi sono istruzioni da poter eseguire) */

               *pa=(int*)malloc(sizeof(int)*(*pdim)); /* viene allocata la memoria dinamica per l'array di istruzioni */

               if(*pa==NULL){ /* cotrolla se l'allocazione  va a buon fine o no */

                 if(flag_err!=1){ /* controlla che non sia già stato segnalato un errore, in caso affermativo non modifico il codice d'errore
                                            cosi da poter segnalare solo l'errore trovato per primo */
                     flag_err=1;
                     error_type=3;
                  }
                }

               while(getline(&buffer,&nBytes,f)!=-1 && flag_err==0){ /* continua ad analizzare le righe finchè o il file non è finito, o non
                                                                        è stato trovato un errore */

                   if(control(buffer)){ /* controlla se ci sono errori di formato nella riga acquisita */

                     if(flag_err!=1){ /* controlla che non sia già stato segnalato un errore, in caso affermativo non modifico il codice d'errore
                                         cosi da poter segnalare solo l'errore trovato per primo */
                         error_type=0;
                         flag_err=1;
                       }
                     }

                   else{                                  /* se non ci sono errori di formato nella riga letta viene acquisito il codice dell'istruzione (se presente) */
                        if(sscanf(buffer,"%d",&elem)>0){  /* se è presente il codice di un istruzione lo acquisisce */

                           if(i < *pdim) /* controlla che il numero di istruzioni acquisite non sia maggiore di quello dichiarato nel file */
                              (*pa)[i]=elem; /*inserisce nell'array di istruzioni il numero acquisito (che corrisponde ad una istruzione) */

                           else{
                               if(flag_err!=1){ /* controlla che non sia già stato segnalato un errore, in caso affermativo non modifica il codice d'errore
                                                   cosi da poter segnalare solo l'errore trovato per primo */
                                  flag_err=1;
                                 error_type=1;
                                }
                             }

                           i++; /* l'indice della stringa viene incrementato, per verificare poi il contenuto della successiva posizione della stringa al ciclo successivo */

                          }
                       }

                      line++; /* incrementa la variabile in modo che sia aggiornata sulla riga che sta analizzando */

                    }
                 }

             /*controlla che il numero di istruzioni acquisite non sia minore del numero di istruzioni dichiarato nel file , se è minore do errore */

             if(i < *pdim){ /* controlla che il numero di istruzioni acquisite non sia minore di quello dichiarato nel file */

               if(flag_err!=1){ /* controlla che non sia già stato segnalato un errore, in caso affermativo non modifico il codice d'errore
                                   cosi da poter segnalare solo l'errore trovato per primo */
                  flag_err=1;
                  error_type=2;
                }
              }


             /*libera la memoria allocata dinamicamente nel caso sia stata allocata e chiude il file*/
             if(buffer)
                free(buffer);
             fclose(f);

          }

         else{ /* da errore poichè il file  non è stato trovato o è impossibile da aprire */
              flag_err=1;
              error_type=4;
          }

         /*controlla il tipo di errore in caso si sia verificato almeno un errore */

         if(flag_err){

           /*fa corrispondere a ogni codice d'errore un messaggio che verrà stampato su terminale */

            switch(error_type){

                  case 0 : printf("ERRORE: Formato riga input %d non corretto\n",line);
                            break;

                  case 1 : printf("ERRORE: Numero istruzioni dichiarato (%d) e' minore di numero istruzioni acquisite (%d)\n",*pdim,i);
                            break;

                  case 2 : printf("ERRORE: Numero istruzioni dichiarato (%d) e' maggiore di numero istruzioni acquisite (%d)\n",*pdim,i);
                            break;

                  case 3 : printf("ERRORE: Memoria dinamica non allocata correttamente\n");
                           break;

                  case 4 : printf("ERRORE: File non trovato o impossibile da aprire\n");
                           break;

                  }
            }

return !flag_err; /*ritorna 0 in caso ci sia stato almeno un errore, ritorna 1 in caso non ci siano stati errori */
}
