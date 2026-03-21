/**
*@file voyant.h
*@brief Dans ce fichier on déclare les fonctions pour la gestion des voyants
*/
#ifndef VOYANT_H
#define VOYANT_H


#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

class Voyant{
    private :
        entrees* io ;
    public:
        Voyant();
        void voyant_set_dispo(led dispo);
        void voyant_set_charge(led charge);
        void voyant_blink_charge() ;
        void voyant_set_defaut(led defaut);
        void voyant_blink_defaut();
        void voyant_set_prise(led prise) ;
        void voyant_set_trappe(led trappe);

};

#endif 