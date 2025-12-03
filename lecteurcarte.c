#include <unistd.h>
#include "lecteurcarte.h"
#include "baseclient.h"
#include <donnees_borne.h>


int numero; // attrb de cette classe

void lecteurcarte_initialiser()
{
         initialisations_ports();
}

void lecteurcarte_lire_carte()
{
       
        printf("Inserez votre carte\n");
        attente_insertion_carte();
        numero = lecture_numero_carte();
        printf("Numéro lu:%d\n", numero);
        attente_retrait_carte();

        
        //liberation_ports();

        if (carte_inseree() ){
                baseclient_authentifier(numero);
        }
}

