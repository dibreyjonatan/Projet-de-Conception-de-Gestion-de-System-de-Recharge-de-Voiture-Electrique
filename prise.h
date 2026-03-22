/**
 * @file prise.h
 * @brief Déclaration de la classe Prise — gestion de la trappe et
 *        du voyant de la prise de charge de la borne.
 *
 * @details
 * Ce fichier déclare la classe Prise qui assure l'interface entre le
 * logiciel et les actionneurs physiques de la prise de charge :
 * @li la **trappe mécanique** : verrouillée durant la charge pour
 *     sécuriser la connexion entre le câble et le véhicule, déverrouillée
 *     lors du branchement et du débranchement du câble.
 * @li le **voyant de la prise** : indique visuellement l'état de la prise
 *     au client (disponible, en cours de charge, éteinte).
 *
 * La classe s'appuie sur la structure @c entrees définie dans @c donnees_borne.h,
 * accessible via un segment de mémoire partagée fourni par @c memoire_borne.h.
 */

#ifndef PRISE_H
#define PRISE_H

#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

/**
 * @class Prise
 * @brief Pilote la trappe mécanique et le voyant de la prise de charge.
 *
 * @details
 * La classe Prise encapsule le contrôle des actionneurs physiques de la
 * prise de charge de la borne. Elle intervient à plusieurs étapes du
 * cycle de charge pour garantir la sécurité de la connexion :
 *
 * | Action                  | Méthode                     | État associé       |
 * |-------------------------|-----------------------------|--------------------|
 * | Déverrouillage trappe   | @c prise_deverrouille_trappe() | etat1, etat4, etat5 |
 * | Voyant prise allumé     | @c prise_set_prise(VERT)    | etat1              |
 * | Verrouillage trappe     | @c prise_verrouille_trappe()   | etat2, etat6       |
 * | Voyant prise éteint     | @c prise_set_prise(OFF)     | etat5              |
 *
 * En interne, elle s'appuie sur un pointeur @c io_p vers la structure
 * @c entrees mappée en mémoire partagée, donnant accès direct aux
 * registres matériels de la borne.
 *
 * @warning Le verrouillage de la trappe ne doit jamais être effectué
 *          avant que le câble soit correctement branché au véhicule,
 *          sous peine de bloquer mécaniquement la prise dans un état
 *          incohérent.
 */
class Prise {
    private :

        /**
         * @brief Pointeur vers la structure des entrées matérielles de la borne.
         *
         * @details
         * Initialisé dans le constructeur via @c acces_memoire(), ce pointeur
         * donne accès en lecture et en écriture aux registres matériels,
         * notamment la commande de la trappe et l'état du voyant de la prise.
         */
        entrees* io_p ;

    public :

        /**
         * @brief Constructeur de la classe Prise.
         *
         * @details
         * Initialise le pointeur @c io_p vers le segment de mémoire partagée
         * contenant les registres matériels de la borne, via @c acces_memoire().
         */
        Prise();

        /**
         * @brief Verrouille mécaniquement la trappe de la prise de charge.
         *
         * @details
         * Active le verrou mécanique de la trappe pour sécuriser la connexion
         * entre le câble de charge et le véhicule. Cette action empêche tout
         * débranchement accidentel ou volontaire durant la charge.
         *
         * @warning Ne pas verrouiller la trappe avant de s'être assuré que
         *          le câble est correctement inséré dans la prise du véhicule.
         *
         * @return void
         */
        void prise_verrouille_trappe();

        /**
         * @brief Déverrouille mécaniquement la trappe de la prise de charge.
         *
         * @details
         * Désactive le verrou mécanique de la trappe pour permettre au client
         * de brancher ou débrancher le câble de charge. Appelée avant toute
         * opération nécessitant un accès physique à la prise.
         *
         * @return void
         */
        void prise_deverrouille_trappe();

        /**
         * @brief Pilote le voyant lumineux de la prise de charge.
         *
         * @details
         * Configure l'état du voyant de la prise selon la valeur du paramètre
         * @p prise, de type @c led. Les états possibles sont :
         * @li @c VERT : prise active, câble correctement détecté — @c etat1.
         * @li @c OFF  : prise inactive, fin de session ou récupération
         *               du véhicule — @c etat5.
         *
         * @param prise  Valeur de type @c led indiquant l'état à appliquer
         *               sur le voyant de la prise.
         *
         * @return void
         */
        void prise_set_prise(led prise);
};

#endif