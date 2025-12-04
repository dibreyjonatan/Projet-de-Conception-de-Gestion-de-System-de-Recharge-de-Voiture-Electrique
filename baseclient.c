/**
*@file baseclient.c 
*@brief Ici on a les fonctions qui permettent de faire la gestion des clients.
*/
#include <baseclient.h>
/**

*@brief Authentifier le client qui se presente.
*@details 
* Dans cette fonction on ouvre la base de données database.txt on vérifie si le numéro entrer par le client
* est présent dans la base de donnée.
*@param[in] num c'est l'id du client à faire valider 
*@return int 
*@retval 1 si le client est dans la base de données 
*@retval 0 si le client ne figure pas dans la base de donné

*/
int baseclient_authentifier(int num){

  FILE* fich = fopen("database.txt","r") ;
 if(fich==NULL) printf("erreur d'ouverture du fichier\n");
  else{
   int numero_r;
   while(fscanf(fich,"%d\n",&numero_r)==1)
{
   if(numero_r==num) {
    fclose(fich) ;
    return 1 ;
}

}
  
}
  fclose(fich);
  return 0 ;

}
/**

*@brief Ajout un client dans la base de donnée
*@details 
* Dans cette fonction on ouvre la base de données, on vérifie si le numéro client à ajouter
* n'est pas dans la base de donnée et après on l'ajoute.
* si le client est présent, on signal à l'opérateur que ce numéro est déjà présent dans la base de donnée.
*@param[in] k c'est l'id du client à ajouter 
*@return void 
*@par variables internes utilisées
*@li fich : pointeur de type FILE vers le fichier de la base de donnée
*/
void baseclient_ajoutclient(int k){
   if(k!=255){
  if(baseclient_authentifier(k)) printf("\nLe client que vous essayez d'ajouter figure déjà dans la liste\n");
  
  else{
FILE* fich=fopen("database.txt","a");
if(fich==NULL) printf("erreur d'ouverture du fichier\n");
else{
  fprintf(fich,"%d\n",k);
  fclose(fich);
}
}
   }
   else{
     printf("Vous ne pouvez pas ajouter ce numéro\n");
   }
}
/**

*@brief Supprime un client de la base de donnée
*@details 
* Dans cette fonction on créer un fichier temporaire et à la fin on supprime l'ancienne base de donnée et on renome le fichier temporaire en  notre fichier base de donnée.
* Ce fichier temporaire nous permet de transférer les numeros de clients qui sont différent du numéro à supprimer.
* Si le client à supprimer ne figure pas dans la base de donnée on le notifie à l'opérateur
* Si la suppression a reussit, on le signale à l'opérateur 
*@param[in] k c'est l'id du client à supprimer
*@return void 
*@par variables internes utilisées
*@li fich : pointeur de type FILE vers le fichier de la base de donnée
*@li tmp_data : pointeur de type FILE vers le fichier temporaire
*@li val : variable qui permet de lire les numéros dans la base de donnée
*@li found : variable qui permet de savoir si le client à supprimer est dans la base de donnée ou pas  

*/
void baseclient_supprimeclient(int k){

  FILE* fich=fopen("database.txt","r");
  FILE* tmp_data=fopen("temp.txt","w");
  int val ; 
  int found=0;

  if(fich==NULL || tmp_data==NULL) printf("Erreur d'ouverture de la base de donnée ou du fichier temporaire\n");
  else{
  while(fscanf(fich,"%d\n",&val)==1){
    if(val==k) found=1 ;
    if(val !=k){
      fprintf(tmp_data,"%d\n",val);
    }
  }
  printf("Suppression avec succès\n");
  fclose(fich);
  fclose(tmp_data);
  
  remove("database.txt");
  rename("temp.txt","database.txt");
  
  if(found==0) printf("Le client que vous voulez supprimer ne figure pas dans la base de données\n");
  }
}
