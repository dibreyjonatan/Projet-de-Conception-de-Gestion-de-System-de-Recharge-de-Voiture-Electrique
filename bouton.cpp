#include "bouton.h"
Bouton::Bouton(){
    int shmid_b ;
    io_b= acces_memoire(&shmid_b) ;
}
int Bouton::bouton_appuie_button_charge(){
  return ( io_b -> bouton_charge ) ;
}
int Bouton::bouton_apppuie_button_stop() {
 return ( io_b -> bouton_stop ) ;
}

void Bouton::bouton_set_bouton_charge(){
  io_b -> bouton_charge=0 ;
}

void Bouton::bouton_set_bouton_stop(){
 io_b -> bouton_stop=0 ;
}
