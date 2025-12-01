#include "bouton.h"

entrees *io_b ;
int shmid_b ;

int bouton_appuie_button_charge(){
    io_b= acces_memoire(&shmid_b) ;
    return ( io_b -> bouton_charge ) ;
}

int bouton_apppuie_button_stop() {

    io_b= acces_memoire(&shmid_b) ;
    return ( io_b -> bouton_stop ) ;
}


void bouton_set_bouton_charge(){
  io_b= acces_memoire(&shmid_b) ;
    io_b -> bouton_charge=0 ;
}

void bouton_set_bouton_stop(){
io_b= acces_memoire(&shmid_b) ;
    io_b -> bouton_stop=0 ;
    }


