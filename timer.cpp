#include "timer.h"

Timer::Timer(){
     int shmid_t ;
     io_t=acces_memoire(&shmid_t) ;
}
void Timer::timer_raz(){
       io_t->timer_sec=0;
       depart_timer=time(NULL) ;
}
int Timer::timer_valeur(){
     sleep(1) ;
    time_t now=time(NULL) ;
     
    int diff = now - depart_timer ; 
    depart_timer=now ;
    return diff ;
}
int Timer::timer_count_sec(){

    int timer_value = timer_valeur () ;
    io_t->timer_sec+= timer_value;
    return (timer_value) ;
}