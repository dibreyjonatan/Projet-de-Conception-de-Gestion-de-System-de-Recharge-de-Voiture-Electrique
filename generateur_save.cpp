#include "generateur_save.h"

Generateur_Save::Generateur_Save(){
int shmid_g ;
  io_g=acces_memoire(&shmid_g) ;
}
void Generateur_Save::generateur_save_fermer_contacteur(){
 io_g->contacteur_AC=1; 
}

void Generateur_Save::generateur_save_ouvrir_contacteur() {
 io_g->contacteur_AC=0; 
}

int  Generateur_Save::generateur_save_tension_DC() {
 return (io_g->gene_u); 
}
void Generateur_Save::generateur_save_generer_pwm(pwm signal) {
  io_g->gene_pwm=signal; 
}