/**
 * @file bouton.h
 * @brief Déclaration de la classe Bouton — gestion des boutons physiques de la borne.
 *
 * @details
 * Ce fichier déclare la classe Bouton qui assure l'interface entre le logiciel
 * et les deux boutons physiques de la borne de recharge :
 * @li le bouton **CHARGE** : déclenché par le client pour démarrer la charge.
 * @li le bouton **STOP**   : déclenché pour interrompre la charge à tout moment.
 *
 * La classe s'appuie sur la structure @c entrees définie dans @c donnees_borne.h
 * pour accéder aux entrées matérielles de la borne via un pointeur mémoire
 * mappé depuis @c memoire_borne.h.
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

/**
 * @class Bouton
 * @brief Gère la lecture et la remise à zéro des boutons physiques de la borne.
 *
 * @details
 * La classe Bouton encapsule les accès aux boutons matériels de la borne
 * de recharge. Elle expose des méthodes de lecture d'état et de remise
 * à zéro logicielle pour chacun des deux boutons disponibles.
 *
 * En interne, elle utilise un pointeur @c io_b vers la structure @c entrees
 * qui représente le registre d'entrées matérielles de la borne, accessible
 * via un mapping mémoire défini dans @c memoire_borne.h.
 */
class Bouton {
    private :

        /**
         * @brief Pointeur vers la structure des entrées matérielles de la borne.
         *
         * @details
         * Ce pointeur est initialisé dans le constructeur via le mapping mémoire
         * défini dans @c memoire_borne.h. Il donne accès en lecture et en écriture
         * aux registres d'entrées physiques, notamment les états des boutons
         * CHARGE et STOP.
         */
        entrees *io_b ;

    public :

        /**
         * @brief Constructeur de la classe Bouton.
         *
         * @details
         * Initialise le pointeur @c io_b vers l'adresse mémoire correspondant
         * aux entrées matérielles de la borne, permettant ainsi l'accès aux
         * états physiques des boutons.
         */
        Bouton();

        /**
         * @brief Lit l'état du bouton CHARGE.
         *
         * @details
         * Interroge le registre d'entrées via @c io_b pour déterminer si
         * le bouton CHARGE a été appuyé par le client. Ce bouton est surveillé
         * dans @c etat0, où le client dispose de 60 secondes pour l'activer
         * afin de démarrer le cycle de charge.
         *
         * @return int
         * @retval 1 si le bouton CHARGE est appuyé.
         * @retval 0 si le bouton CHARGE n'est pas appuyé.
         */
        int bouton_appuie_button_charge();

        /**
         * @brief Lit l'état du bouton STOP.
         *
         * @details
         * Interroge le registre d'entrées via @c io_b pour déterminer si
         * le bouton STOP a été actionné. Ce bouton peut être utilisé depuis
         * les états 2, 3 et 4 pour interrompre la charge en cours et
         * rediriger le système vers @c etat5 (récupération du véhicule).
         *
         * @return int
         * @retval 1 si le bouton STOP est appuyé.
         * @retval 0 si le bouton STOP n'est pas appuyé.
         */
        int bouton_apppuie_button_stop();

        /**
         * @brief Remet à zéro logiciellement le bouton CHARGE.
         *
         * @details
         * Réinitialise l'état du bouton CHARGE dans le registre d'entrées
         * après sa prise en compte par le système. Appelée en début de
         * @c etat1 pour éviter une détection parasite lors du cycle suivant.
         *
         * @return void
         */
        void bouton_set_bouton_charge();

        /**
         * @brief Remet à zéro logiciellement le bouton STOP.
         *
         * @details
         * Réinitialise l'état du bouton STOP dans le registre d'entrées
         * après sa prise en compte par le système. Appelée après chaque
         * détection d'appui sur STOP dans les états 2, 3 et 4, ainsi qu'en
         * initialisation dans le @c main avant le démarrage de la boucle principale.
         *
         * @return void
         */
        void bouton_set_bouton_stop();
};

#endif