/**
 * @file timer.h
 * @brief Déclaration de la classe Timer — gestion de la temporisation
 *        du système de borne de recharge.
 *
 * @details
 * Ce fichier déclare la classe Timer qui fournit les mécanismes de mesure
 * du temps écoulé utilisés dans la machine à états de la borne. Elle est
 * principalement sollicitée dans @c etat0, où le client dispose de 60
 * secondes pour appuyer sur le bouton CHARGE après authentification.
 *
 * La classe combine deux approches complémentaires :
 * @li un **accès aux registres matériels** via la mémoire partagée,
 *     pour lire la valeur du timer hardware de la borne.
 * @li un **horodatage logiciel** via @c time_t, pour mesurer le temps
 *     écoulé depuis la dernière remise à zéro.
 *
 * La classe s'appuie sur la structure @c entrees définie dans @c donnees_borne.h,
 * accessible via un segment de mémoire partagée fourni par @c memoire_borne.h,
 * ainsi que sur la bibliothèque standard @c time.h pour la gestion
 * de l'horodatage logiciel.
 */

#ifndef TIMER_H
#define TIMER_H

#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>
#include <time.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

/**
 * @class Timer
 * @brief Gère la temporisation et la mesure du temps écoulé dans la borne.
 *
 * @details
 * La classe Timer encapsule deux mécanismes de mesure du temps :
 * un accès au timer matériel via le pointeur @c io_t, et un horodatage
 * logiciel via le membre @c depart_timer de type @c time_t.
 *
 * Son utilisation typique dans la machine à états est la suivante :
 * @li **1.** @c timer_raz()       — remise à zéro au début de la temporisation.
 * @li **2.** @c timer_count_sec() — appelée en boucle pour accumuler
 *            les secondes écoulées jusqu'à expiration du délai.
 * @li **3.** @c timer_valeur()    — lecture directe de la valeur courante
 *            du timer matériel si nécessaire.
 *
 * @note La temporisation de 60 secondes dans @c etat0 est entièrement
 *       gérée par cette classe — c'est la seule contrainte temporelle
 *       explicite du cycle de charge côté logiciel.
 */
class Timer {
    private :

        /**
         * @brief Pointeur vers la structure des entrées matérielles de la borne.
         *
         * @details
         * Initialisé dans le constructeur via @c acces_memoire(), ce pointeur
         * donne accès en lecture aux registres matériels, notamment la valeur
         * courante du timer hardware de la borne.
         */
        entrees* io_t ;

        /**
         * @brief Horodatage de référence pour la mesure du temps écoulé.
         *
         * @details
         * Stocke l'instant de la dernière remise à zéro du timer, obtenu
         * via @c time(). La différence entre l'heure courante et cette
         * valeur de référence donne le temps écoulé en secondes.
         */
        time_t depart_timer ;

    public :

        /**
         * @brief Constructeur de la classe Timer.
         *
         * @details
         * Initialise le pointeur @c io_t vers le segment de mémoire partagée
         * contenant les registres matériels de la borne, via @c acces_memoire().
         */
        Timer();

        /**
         * @brief Remet à zéro le timer logiciel.
         *
         * @details
         * Enregistre l'instant courant dans @c depart_timer via @c time(),
         * constituant ainsi le point de départ de la temporisation. Doit
         * être appelée au début de chaque période de temporisation, notamment
         * en @c etat0 avant la boucle d'attente d'appui bouton de 60 secondes.
         *
         * @return void
         */
        void timer_raz();

        /**
         * @brief Lit la valeur courante du timer matériel.
         *
         * @details
         * Retourne directement la valeur du registre timer hardware de la
         * borne via le pointeur @c io_t, sans calcul supplémentaire.
         *
         * @return int  Valeur courante du timer matériel.
         */
        int timer_valeur();

        /**
         * @brief Calcule le nombre de secondes écoulées depuis la dernière remise à zéro.
         *
         * @details
         * Calcule et retourne la différence en secondes entre l'instant
         * courant et @c depart_timer. Cette valeur est accumulée dans la
         * variable @c timer_secs de @c borne.cpp à chaque tour de la boucle
         * d'attente, jusqu'à atteindre le seuil de 60 secondes ou détecter
         * un appui sur le bouton CHARGE.
         *
         * @return int  Nombre de secondes écoulées depuis le dernier appel à @c timer_raz().
         */
        int timer_count_sec();
};

#endif