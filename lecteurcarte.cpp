/**
 * @file lecteurcarte.cpp
 * @brief Implémentation des méthodes de la classe LecteurCarte.
 *
 * @details
 * Ce fichier contient l'implémentation des méthodes de gestion du lecteur
 * de cartes de la borne. Dans cette version, la lecture est simulée par
 * une saisie clavier via @c cin, ce qui permet de tester le comportement
 * de la machine à états sans matériel physique connecté.
 */

#include "lecteurcarte.h"

/**
 * @details
 * Dans cette version du système, le corps de la méthode est volontairement
 * laissé vide. L'initialisation du port de communication sera implémentée
 * lors de l'intégration du lecteur de cartes physique, via les primitives
 * de @c lcarte.h.
 *
 * @note Cette méthode est prévue pour accueillir la configuration du port
 *       série ou du bus de communication du lecteur lors du passage
 *       en environnement matériel réel.
 */
void LecteurCarte::lecteurcarte_initialiser_lecteur() {

}

/**
 * @details
 * Dans cette version simulée, l'initialisation se limite à afficher
 * un message invitant l'utilisateur à saisir son numéro de carte
 * au clavier. Sur le matériel réel, cette méthode sera chargée de
 * réinitialiser l'état interne du lecteur avant chaque nouvelle session.
 *
 * @note Appelée à chaque entrée en @c etat0, y compris après un retour
 *       depuis @c etat6 ou @c etat255.
 */
void LecteurCarte::lecteurcarte_initialiser() {
    cout << "veuillez inserer votre de carte: \n";
}

/**
 * @details
 * Dans cette version simulée, la lecture est effectuée par saisie clavier
 * via @c cin. Le numéro saisi est retourné directement sans traitement,
 * de la même façon que le ferait un lecteur physique.
 *
 * Sur le matériel réel, cette méthode sera remplacée par un appel aux
 * primitives de @c lcarte.h pour récupérer le numéro depuis le lecteur.
 *
 * @par Variables internes utilisées :
 * @li **num** : entier int stockant temporairement le numéro saisi
 *               avant retour à l'appelant.
 *
 * @note La valeur 255 saisie ici déclenche la transition vers @c etat255
 *       dans la machine à états — elle est réservée à l'opérateur et ne
 *       doit pas être attribuée à un client dans la base de données.
 */
int LecteurCarte::lecteurcarte_lire_carte() {
    int num ;
    cin >> num ;
    return num ;
}