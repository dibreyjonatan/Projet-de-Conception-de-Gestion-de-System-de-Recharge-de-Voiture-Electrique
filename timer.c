#include "timer.h"

entrees *io_t ;
int shmid_t ;
time_t depart_timer ;

void timer_raz(){
    io_t=acces_memoire(&shmid_t) ;
     io_t->timer_sec=0;
    depart_timer=time(NULL) ;
     
}
 // calcul la difference de temps après 1s
int timer_valeur(){
      sleep(1) ;
    time_t now=time(NULL) ;
     
    int diff = now - depart_timer ; 
     // printf("la difference est : %d\n",diff) ;
    depart_timer=now ;
  return diff ;
}

int timer_count_sec(){
        
        int timer_value = timer_valeur () ;
     io_t=acces_memoire(&shmid_t) ;
//printf("timer system avant 1s: %d\n",io_t->timer_sec) ;
     io_t->timer_sec+= timer_value;
  //  printf("timer system apres 1s : %d\n",io_t->timer_sec) ;
           return (timer_value) ;
}


// brouillon 
/*
int timer_renvoie_sec(){
     timer_count_sec() ;
    
    io_t=acces_memoire(&shmid_t) ;
    printf("secondes retourner : %d\n",io_t->timer_sec) ;
     return (io_t->timer_sec); 

}*/