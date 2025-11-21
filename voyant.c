#include "voyant.h"

entrees *io ;
int shmid ;

void voyant_set_dispo(led dispo){
  io=acces_memoire(&shmid) ;
  io->led_dispo=dispo; 
}

void voyant_set_charge(led charge){
io=acces_memoire(&shmid) ;
  io->led_charge=charge; 

}
void voyant_blink_charge(){
  for(int i=0; i<4; i++){
   voyant_set_charge(VERT);
   sleep(1);
   voyant_set_charge(OFF);
   sleep(1);
}
}

void voyant_set_defaut(led defaut){
 io=acces_memoire(&shmid) ;
  io->led_defaut=defaut; 
}
void voyant_blink_defaut(){
    for(int i=0; i<4; i++){
   voyant_set_defaut(ROUGE);
   sleep(1);
   voyant_set_defaut(OFF);
   sleep(1);
}
}
void voyant_set_prise(led prise) {
 io=acces_memoire(&shmid) ;
  io->led_prise=prise; 
}
void voyant_set_trappe(led trappe){
io=acces_memoire(&shmid) ;
  io->led_trappe=trappe; 

}