#ifndef TIMER_H
#define TIMER_H

#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>
#include <time.h>
#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

class Timer{
    private :
        entrees* io_t ;
        time_t depart_timer ;
    public:
       Timer();
       void timer_raz();
       int timer_valeur();
       int timer_count_sec(); 

};

#endif 