/**
 * @file bouton.cpp
 * @brief Implémentation des méthodes de la classe Bouton.
 *
 * @details
 * Ce fichier contient l'implémentation des méthodes de lecture et de
 * remise à zéro des boutons physiques de la borne. L'accès aux registres
 * matériels se fait via un segment de mémoire partagée, obtenu à
 * l'instanciation de l'objet grâce à la fonction @c acces_memoire().
 */

#include "bouton.h"

/**
 * @brief Constructeur de la classe Bouton.
 *
 * @details
 * Initialise le pointeur @c io_b en récupérant l'adresse du segment
 * de mémoire partagée via @c acces_memoire(). Ce segment contient
 * la structure @c entrees qui regroupe les états de tous les boutons
 * physiques de la borne.
 *
 * Une fois le constructeur exécuté, toutes les lectures et écritures
 * sur les boutons transitent directement par ce pointeur, sans nouvel
 * accès au mécanisme de mémoire partagée.
 *
 * @par Variables internes utilisées :
 * @li **shmid_b** : identifiant du segment de mémoire partagée, passé
 *                   par adresse à @c acces_memoire() pour être initialisé.
 */
Bouton::Bouton() {
    int shmid_b ;
    io_b = acces_memoire(&shmid_b) ;
}

/**
 * @brief Lit l'état du bouton CHARGE depuis le registre matériel.
 *
 * @details
 * Retourne directement la valeur du champ @c bouton_charge de la
 * structure @c entrees pointée par @c io_b. Aucun traitement n'est
 * effectué — la valeur reflète l'état physique instantané du bouton.
 *
 * @return int
 * @retval 1 si le bouton CHARGE est actuellement appuyé.
 * @retval 0 si le bouton CHARGE est au repos.
 */
int Bouton::bouton_appuie_button_charge() {
    return ( io_b -> bouton_charge ) ;
}

/**
 * @brief Lit l'état du bouton STOP depuis le registre matériel.
 *
 * @details
 * Retourne directement la valeur du champ @c bouton_stop de la
 * structure @c entrees pointée par @c io_b. Aucun traitement n'est
 * effectué — la valeur reflète l'état physique instantané du bouton.
 *
 * @return int
 * @retval 1 si le bouton STOP est actuellement appuyé.
 * @retval 0 si le bouton STOP est au repos.
 */
int Bouton::bouton_apppuie_button_stop() {
    return ( io_b -> bouton_stop ) ;
}

/**
 * @brief Remet à zéro logiciellement le bouton CHARGE.
 *
 * @details
 * Force la valeur du champ @c bouton_charge à 0 dans le registre
 * matériel via le pointeur @c io_b. Cette remise à zéro est nécessaire
 * après la prise en compte de l'appui, pour éviter qu'une lecture
 * ultérieure ne détecte à nouveau un appui fantôme sur le bouton.
 *
 * @note Appelée en début de @c etat1, juste après la confirmation
 *       de l'appui bouton détectée dans @c etat0.
 *
 * @return void
 */
void Bouton::bouton_set_bouton_charge() {
    io_b -> bouton_charge = 0 ;
}

/**
 * @brief Remet à zéro logiciellement le bouton STOP.
 *
 * @details
 * Force la valeur du champ @c bouton_stop à 0 dans le registre
 * matériel via le pointeur @c io_b. Comme pour le bouton CHARGE,
 * cette remise à zéro est indispensable après chaque détection
 * d'appui pour garantir qu'un seul événement est traité par appui physique.
 *
 * @note Appelée à deux moments distincts dans le système :
 * @li en initialisation dans le @c main, avant le démarrage de
 *     la boucle principale, pour partir d'un état propre.
 * @li après chaque détection d'appui STOP dans les états 2, 3 et 4,
 *     avant la transition vers @c etat5.
 *
 * @return void
 */
void Bouton::bouton_set_bouton_stop() {
    io_b -> bouton_stop = 0 ;
}