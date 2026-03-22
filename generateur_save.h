#ifndef GENERATEUR_SAVE_H
#define GENERATEUR_SAVE_H

#include <donnees_borne.h>
#include <memoire_borne.h>


#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

class Generateur_Save{
    private :
        entrees *io_g ;
    public :
        Generateur_Save();
        void generateur_save_fermer_contacteur();

        void generateur_save_ouvrir_contacteur() ;

        int  generateur_save_tension_DC() ;
        void generateur_save_generer_pwm(pwm signal) ;
};
#endif 