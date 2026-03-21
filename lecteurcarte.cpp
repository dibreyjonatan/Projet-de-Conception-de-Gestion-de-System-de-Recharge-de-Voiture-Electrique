#include "lecteurcarte.h"

void  LecteurCarte::lecteurcarte_initialiser_lecteur(){

}
void  LecteurCarte::lecteurcarte_initialiser(){
   cout << "veuillez inserer votre de carte: \n";
}
int  LecteurCarte::lecteurcarte_lire_carte(){
  int num ;
  cin >> num ;
  return num ; 
}
