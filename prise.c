#include "prise.h"


entrees *io_p ;
int shmid_p ;


void prise_verrouille_trappe() {

    io_p=acces_memoire(&shmid_p) ;
  io_p->led_trappe=OFF; 
}
void prise_deverrouille_trappe() {

    io_p=acces_memoire(&shmid_p) ;
  io_p->led_trappe=VERT; 
}

void prise_set_prise(led prise) {

    io_p=acces_memoire(&shmid_p) ;
  io_p->led_prise=prise; 
}