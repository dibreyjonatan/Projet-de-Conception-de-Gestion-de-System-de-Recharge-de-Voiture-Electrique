#include "prise.h"
 Prise::Prise(){
     int shmid_p ;
     io_p=acces_memoire(&shmid_p) ;
 }
void Prise::prise_verrouille_trappe() {
      io_p->led_trappe=OFF; 
}
void Prise::prise_deverrouille_trappe() {
     io_p->led_trappe=VERT; 
}
void Prise::prise_set_prise(led prise) {
     io_p->led_prise=prise; 
}