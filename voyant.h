/**
 * @file voyant.h
 * @brief Déclaration de la classe Voyant — gestion des voyants lumineux
 *        de la borne de recharge.
 *
 * @details
 * Ce fichier déclare la classe Voyant qui assure l'interface entre le
 * logiciel et les voyants lumineux physiques de la borne. Ces voyants
 * constituent le retour visuel principal pour le client et reflètent
 * à tout moment l'état courant du système :
 * @li le **voyant disponibilité** : indique si la borne est libre ou occupée.
 * @li le **voyant charge**        : signale l'état du cycle de charge
 *                                   (en attente, en cours, terminé).
 * @li le **voyant défaut**        : s'allume ou clignote en cas d'échec
 *                                   d'authentification ou d'anomalie.
 * @li le **voyant prise**         : indique l'état de la prise de charge.
 * @li le **voyant trappe**        : indique l'état du verrou de la trappe.
 *
 * La classe s'appuie sur la structure @c entrees définie dans @c donnees_borne.h,
 * accessible via un segment de mémoire partagée fourni par @c memoire_borne.h.
 *
 * @note Les méthodes @c voyant_blink_charge() et @c voyant_blink_defaut()
 *       produisent un clignotement pendant 8 secondes. Elles sont bloquantes
 *       durant toute la durée du clignotement.
 */

#ifndef VOYANT_H
#define VOYANT_H

#include <donnees_borne.h>
#include <memoire_borne.h>
#include <unistd.h>

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
using namespace std ;
#endif

/**
 * @class Voyant
 * @brief Pilote l'ensemble des voyants lumineux de la borne de recharge.
 *
 * @details
 * La classe Voyant encapsule le contrôle de tous les indicateurs lumineux
 * de la borne. Elle intervient à chaque transition de la machine à états
 * pour informer visuellement le client de l'évolution du cycle de charge :
 *
 * | Voyant        | Méthode                       | État associé        |
 * |---------------|-------------------------------|---------------------|
 * | Disponibilité | @c voyant_set_dispo(VERT)     | etat6               |
 * | Disponibilité | @c voyant_set_dispo(OFF)      | etat1               |
 * | Charge        | @c voyant_set_charge(ROUGE)   | etat1               |
 * | Charge        | @c voyant_set_charge(VERT)    | etat4               |
 * | Charge        | @c voyant_set_charge(OFF)     | etat5               |
 * | Charge        | @c voyant_blink_charge()      | etat0 (succès auth) |
 * | Défaut        | @c voyant_blink_defaut()      | etat0 (échec auth)  |
 *
 * En interne, elle s'appuie sur un pointeur @c io vers la structure
 * @c entrees mappée en mémoire partagée, donnant accès direct aux
 * registres matériels des voyants.
 */
class Voyant {
    private :

        /**
         * @brief Pointeur vers la structure des entrées matérielles de la borne.
         *
         * @details
         * Initialisé dans le constructeur via @c acces_memoire(), ce pointeur
         * donne accès en écriture aux registres matériels de tous les voyants
         * de la borne, regroupés dans la structure @c entrees.
         */
        entrees* io ;

    public :

        /**
         * @brief Constructeur de la classe Voyant.
         *
         * @details
         * Initialise le pointeur @c io vers le segment de mémoire partagée
         * contenant les registres matériels de la borne, via @c acces_memoire().
         */
        Voyant();

        /**
         * @brief Pilote le voyant de disponibilité de la borne.
         *
         * @details
         * Configure l'état du voyant indiquant si la borne est disponible
         * pour un nouveau client. Allumé en vert en @c etat6 pour signaler
         * que la borne est prête, éteint en @c etat1 dès le démarrage
         * d'un cycle de charge.
         *
         * @param dispo  Valeur de type @c led à appliquer sur le voyant
         *               de disponibilité (@c VERT ou @c OFF).
         *
         * @return void
         */
        void voyant_set_dispo(led dispo);

        /**
         * @brief Pilote le voyant de charge de la borne.
         *
         * @details
         * Configure l'état du voyant indiquant la progression du cycle
         * de charge. Les états utilisés dans le cycle sont :
         * @li @c ROUGE : charge en cours          — @c etat1.
         * @li @c VERT  : charge terminée          — @c etat4.
         * @li @c OFF   : cycle terminé            — @c etat5.
         *
         * @param charge  Valeur de type @c led à appliquer sur le voyant
         *                de charge (@c ROUGE, @c VERT ou @c OFF).
         *
         * @return void
         */
        void voyant_set_charge(led charge);

        /**
         * @brief Fait clignoter le voyant de charge pendant 8 secondes.
         *
         * @details
         * Produit un clignotement du voyant de charge pour signaler au client
         * qu'il a été authentifié avec succès et qu'il dispose de 60 secondes
         * pour appuyer sur le bouton CHARGE. Appelée en @c etat0 après une
         * authentification réussie.
         *
         * @warning Cette méthode est bloquante — elle monopolise le fil
         *          d'exécution pendant toute la durée du clignotement (8 secondes).
         *
         * @return void
         */
        void voyant_blink_charge();

        /**
         * @brief Pilote le voyant de défaut de la borne.
         *
         * @details
         * Configure l'état du voyant signalant une anomalie ou un échec.
         * Utilisé pour indiquer visuellement un état d'erreur persistant
         * distinct du clignotement temporaire géré par @c voyant_blink_defaut().
         *
         * @param defaut  Valeur de type @c led à appliquer sur le voyant
         *                de défaut.
         *
         * @return void
         */
        void voyant_set_defaut(led defaut);

        /**
         * @brief Fait clignoter le voyant de défaut pendant 8 secondes.
         *
         * @details
         * Produit un clignotement du voyant de défaut pour signaler au client
         * que son authentification a échoué — carte inconnue ou numéro non
         * reconnu dans la base clients. Appelée en @c etat0 après un échec
         * d'authentification.
         *
         * @warning Cette méthode est bloquante — elle monopolise le fil
         *          d'exécution pendant toute la durée du clignotement (8 secondes).
         *
         * @return void
         */
        void voyant_blink_defaut();

        /**
         * @brief Pilote le voyant lumineux de la prise de charge.
         *
         * @details
         * Configure l'état du voyant associé à la prise de charge,
         * indépendamment du voyant de charge principal. Permet un retour
         * visuel localisé directement au niveau de la prise.
         *
         * @param prise  Valeur de type @c led à appliquer sur le voyant
         *               de la prise.
         *
         * @return void
         */
        void voyant_set_prise(led prise);

        /**
         * @brief Pilote le voyant lumineux de la trappe de la prise.
         *
         * @details
         * Configure l'état du voyant associé à la trappe de la prise,
         * fournissant un retour visuel sur l'état du verrou mécanique
         * directement au niveau de la trappe.
         *
         * @param trappe  Valeur de type @c led à appliquer sur le voyant
         *                de la trappe.
         *
         * @return void
         */
        void voyant_set_trappe(led trappe);
};

#endif