/**
 * @file lecteurcarte.h
 * @brief Déclaration de la classe LecteurCarte — gestion du lecteur
 *        de cartes de la borne de recharge.
 *
 * @details
 * Ce fichier déclare la classe LecteurCarte qui assure l'interface entre
 * le logiciel et le lecteur de cartes physique de la borne. Elle
 * expose trois méthodes couvrant l'ensemble du cycle d'utilisation du
 * lecteur :
 * @li l'**initialisation du port** : effectuée une seule fois au démarrage
 *     du système, avant la boucle principale.
 * @li l'**initialisation de session** : effectuée à chaque passage en
 *     @c etat0, pour préparer une nouvelle lecture.
 * @li la **lecture de carte** : retourne le numéro lu depuis la carte
 *     présentée par le client ou l'opérateur.
 *
 * La classe s'appuie sur la bibliothèque @c lcarte.h qui fournit les
 * primitives bas niveau d'accès au lecteur de cartes.
 *
 * @note Le numéro 255 est une valeur réservée retournée par le lecteur
 *       lorsqu'une carte opérateur est détectée. Il déclenche alors
 *       la transition vers @c etat255 (mode administration).
 */

#ifndef LECTEURCARTE_H
#define LECTEURCARTE_H

#include <lcarte.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std;
#endif

/**
 * @class LecteurCarte
 * @brief Gère l'initialisation et la lecture du lecteur de cartes.
 *
 * @details
 * La classe LecteurCarte encapsule les interactions avec le lecteur de
 * cartes de la borne. Elle est utilisée exclusivement dans @c etat0 de
 * la machine à états, où elle constitue le point d'entrée obligatoire
 * de tout cycle de charge : sans carte valide présentée et reconnue,
 * aucune action n'est possible sur la borne.
 *
 * Le cycle d'utilisation typique est le suivant :
 * @li **1.** @c lecteurcarte_initialiser_lecteur() — appelée une fois dans
 *            le @c main avant la boucle principale pour ouvrir le port.
 * @li **2.** @c lecteurcarte_initialiser()         — appelée à chaque entrée
 *            en @c etat0 pour préparer une nouvelle session de lecture.
 * @li **3.** @c lecteurcarte_lire_carte()          — appelée après initialisation
 *            pour lire le numéro de la carte présentée.
 *
 * @warning Les méthodes @c lecteurcarte_initialiser() et @c lecteurcarte_lire_carte()
 *          ne doivent pas être appelées avant @c lecteurcarte_initialiser_lecteur(),
 *          sous peine d'un accès à un port non ouvert.
 */
class LecteurCarte {
    public :

        /**
         * @brief Initialise le port de communication du lecteur de cartes.
         *
         * @details
         * Ouvre et configure le port de communication utilisé par le lecteur
         * de cartes, via les primitives fournies par @c lcarte.h.
         * Cette initialisation est globale et ne doit être effectuée qu'une
         * seule fois, au démarrage du système, avant le lancement de la
         * boucle principale dans le @c main.
         *
         * @warning Appeler cette méthode plusieurs fois en cours d'exécution
         *          peut provoquer des conflits d'accès au port de communication.
         *
         * @return void
         */
        void lecteurcarte_initialiser_lecteur();

        /**
         * @brief Initialise une nouvelle session de lecture de carte.
         *
         * @details
         * Prépare le lecteur pour une nouvelle lecture en réinitialisant
         * son état interne. Doit être appelée à chaque entrée en @c etat0,
         * c'est-à-dire au début de chaque nouveau cycle client, y compris
         * après un retour depuis @c etat6 ou @c etat255.
         *
         * @return void
         */
        void lecteurcarte_initialiser();

        /**
         * @brief Lit le numéro de la carte présentée au lecteur.
         *
         * @details
         * Attend qu'une carte soit présentée et retourne son numéro
         * identifiant. Ce numéro est ensuite transmis à
         * @c baseclient_authentifier() pour vérification dans la base clients.
         *
         * Deux cas particuliers sont à noter :
         * @li **255** : carte opérateur détectée — déclenche la transition
         *              vers @c etat255 pour accéder au mode administration.
         * @li **tout autre numéro** : numéro client transmis à l'authentification.
         *
         * @return int  Numéro lu depuis la carte présentée.
         * @retval 255  Carte opérateur détectée.
         */
        int lecteurcarte_lire_carte();
};

#endif