/**
 * @file prise.cpp
 * @brief Implémentation des méthodes de la classe Prise.
 *
 * @details
 * Ce fichier contient l'implémentation des méthodes de contrôle de la
 * trappe mécanique et du voyant de la prise de charge. Chaque méthode
 * agit directement sur les registres matériels de la borne via le
 * pointeur @c io_p, initialisé à la construction de l'objet par accès
 * au segment de mémoire partagée.
 */

#include "prise.h"

/**
 * @details
 * Récupère l'adresse du segment de mémoire partagée via @c acces_memoire()
 * et l'affecte au pointeur @c io_p. À partir de ce moment, toutes les
 * interactions avec les actionneurs physiques de la prise transitent
 * par ce pointeur sans nouvel accès au mécanisme de mémoire partagée.
 *
 * @par Variables internes utilisées :
 * @li **shmid_p** : identifiant du segment de mémoire partagée, passé
 *                   par adresse à @c acces_memoire() pour être initialisé.
 */
Prise::Prise() {
    int shmid_p ;
    io_p = acces_memoire(&shmid_p) ;
}

/**
 * @details
 * Force le champ @c led_trappe à @c OFF dans le registre matériel via
 * @c io_p, ce qui provoque la fermeture physique du verrou de la trappe.
 *
 * @note Le choix d'utiliser le champ @c led_trappe pour piloter le verrou
 *       mécanique traduit une conception matérielle où l'actionneur de la
 *       trappe est commandé via le même registre que son voyant associé.
 *
 * @note Appelée dans deux états distincts du cycle :
 * @li en @c etat2 : verrouillage une fois le câble branché au véhicule.
 * @li en @c etat6 : verrouillage final avant remise en disponibilité de la borne.
 */
void Prise::prise_verrouille_trappe() {
    io_p->led_trappe = OFF;
}

/**
 * @details
 * Force le champ @c led_trappe à @c VERT dans le registre matériel via
 * @c io_p, ce qui provoque l'ouverture physique du verrou de la trappe
 * et signale visuellement au client qu'il peut accéder à la prise.
 *
 * @note Appelée dans trois états distincts du cycle :
 * @li en @c etat1 : déverrouillage pour permettre le branchement du câble.
 * @li en @c etat4 : déverrouillage en fin de charge pour la récupération.
 * @li en @c etat5 : déverrouillage pour permettre le débranchement du câble.
 */
void Prise::prise_deverrouille_trappe() {
    io_p->led_trappe = VERT;
}

/**
 * @details
 * Affecte directement la valeur de @p prise au champ @c led_prise du
 * registre matériel via @c io_p, ce qui configure l'état visuel du
 * voyant de la prise sans traitement supplémentaire côté logiciel.
 *
 * Les valeurs utilisées dans le cycle de charge :
 * @li @c VERT : prise active, câble correctement détecté     — @c etat1.
 * @li @c OFF  : prise inactive, fin de session ou récupération — @c etat5.
 *
 * @note Le champ @c led_prise est distinct du champ @c led_trappe —
 *       le voyant de la prise et l'actionneur de la trappe sont deux
 *       registres matériels indépendants.
 */
void Prise::prise_set_prise(led prise) {
    io_p->led_prise = prise;
}