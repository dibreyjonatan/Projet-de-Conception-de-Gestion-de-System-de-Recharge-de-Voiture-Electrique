#include "generateur_save.h"


entrees *io_g ;
int shmid_g ;


void generateur_save_fermer_contacteur() {
    io_g=acces_memoire(&shmid_g) ;
  io_g->contacteur_AC=1; 
}

void generateur_save_ouvrir_contacteur() {
 io_g=acces_memoire(&shmid_g) ;
  io_g->contacteur_AC=0; 
}

int generateur_save_tension_DC() {
    io_g=acces_memoire(&shmid_g) ;
  return (io_g->gene_u); 
   
}
void generateur_save_generer_pwm(pwm signal) {
     io_g=acces_memoire(&shmid_g) ;
  io_g->gene_pwm=signal; 
 
}