#include "voyant.h"

Voyant::Voyant(){
    int shmid ;
    io=acces_memoire(&shmid) ;
}
void Voyant::voyant_set_dispo(led dispo){
     io->led_dispo=dispo; 
}
void Voyant::voyant_set_charge(led charge){
     io->led_charge=charge;
}
void Voyant::voyant_blink_charge() {
    for(int i=0; i<4; i++){
   voyant_set_charge(VERT);
   sleep(1);
   voyant_set_charge(OFF);
   sleep(1);
}
}
void Voyant::voyant_set_defaut(led defaut){
     io->led_defaut=defaut; 
}
void Voyant::voyant_blink_defaut(){
     for(int i=0; i<4; i++){
   voyant_set_defaut(ROUGE);
   sleep(1);
   voyant_set_defaut(OFF);
   sleep(1);
}
}
void Voyant::voyant_set_prise(led prise) {
     io->led_prise=prise; 
}
void Voyant::voyant_set_trappe(led trappe){
  io->led_trappe=trappe; 
}