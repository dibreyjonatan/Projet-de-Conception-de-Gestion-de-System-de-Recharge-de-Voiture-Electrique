#include <baseclient.h>

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
