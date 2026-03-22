
#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>


#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

class Bouton{
    private :
    entrees *io_b ;
    public : 
    Bouton();
    int bouton_appuie_button_charge();
    int bouton_apppuie_button_stop() ;

    void bouton_set_bouton_charge();

    void bouton_set_bouton_stop();

};

#endif 