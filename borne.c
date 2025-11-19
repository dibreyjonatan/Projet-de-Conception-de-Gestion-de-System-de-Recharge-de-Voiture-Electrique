#include <memoire_borne.h>
#include <donnees_borne.h>

#include "lecteurcarte.h"
#include "baseclient.h"

int main()
{

    

    while (1)
    {
        lecteurcarte_initialiser();
        int numero=lecteurcarte_lire_carte();
        printf("numero lu est : %d\n",numero); 
        int found=baseclient_authentifier(numero);
        printf("resultat authentification : %d\n",found) ;
    }

}
