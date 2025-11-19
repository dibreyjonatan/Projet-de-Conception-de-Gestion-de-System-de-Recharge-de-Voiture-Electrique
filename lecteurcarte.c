#include <unistd.h>
#include "lecteurcarte.h"

void lecteurcarte_initialiser()
{
 printf("veuillez inserer un numero de carte: \n") ; 
}

int lecteurcarte_lire_carte()
{ int num ;
 scanf("%d",&num) ;
  return num ; 
}

