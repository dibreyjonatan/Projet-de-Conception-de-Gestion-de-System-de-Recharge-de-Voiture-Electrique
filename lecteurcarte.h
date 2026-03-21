#ifndef LECTEURCARTE_H
#define LECTEURCARTE_H
#include <lcarte.h>
#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std;
#endif
class LecteurCarte{
   public :
          void lecteurcarte_initialiser_lecteur();
          void lecteurcarte_initialiser();
          int lecteurcarte_lire_carte();  

} ;

#endif 