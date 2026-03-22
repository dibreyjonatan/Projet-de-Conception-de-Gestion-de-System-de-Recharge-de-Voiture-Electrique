/**
 * @file voyant.cpp
 * @brief Implémentation des méthodes de la classe Voyant.
 *
 * @details
 * Ce fichier contient l'implémentation des méthodes de pilotage des
 * voyants lumineux de la borne. Chaque méthode agit directement sur
 * les champs de la structure @c entrees via le pointeur @c io, initialisé
 * à la construction de l'objet par accès au segment de mémoire partagée.
 *
 * Les méthodes @c voyant_blink_charge() et @c voyant_blink_defaut() sont
 * les seules méthodes bloquantes de la classe — elles immobilisent le fil
 * d'exécution pendant 8 secondes via des appels successifs à @c sleep(1).
 */

#include "voyant.h"

/**
 * @details
 * Récupère l'adresse du segment de mémoire partagée via @c acces_memoire()
 * et l'affecte au pointeur @c io. À partir de ce moment, toutes les
 * interactions avec les registres matériels des voyants transitent par
 * ce pointeur sans nouvel accès au mécanisme de mémoire partagée.
 *
 * @par Variables internes utilisées :
 * @li **shmid** : identifiant du segment de mémoire partagée, passé
 *                 par adresse à @c acces_memoire() pour être initialisé.
 */
Voyant::Voyant() {
    int shmid ;
    io = acces_memoire(&shmid) ;
}

/**
 * @details
 * Affecte directement la valeur de @p dispo au champ @c led_dispo du
 * registre matériel via @c io, sans traitement supplémentaire côté logiciel.
 *
 * @note Appelée dans deux états distincts du cycle :
 * @li en @c etat1 : extinction (@c OFF) au démarrage du cycle de charge.
 * @li en @c etat6 : allumage (@c VERT) pour signaler la disponibilité
 *                   de la borne au client suivant.
 */
void Voyant::voyant_set_dispo(led dispo) {
    io->led_dispo = dispo;
}

/**
 * @details
 * Affecte directement la valeur de @p charge au champ @c led_charge du
 * registre matériel via @c io, sans traitement supplémentaire côté logiciel.
 *
 * @note Appelée dans trois états distincts du cycle :
 * @li en @c etat1 : allumage rouge (@c ROUGE) au démarrage de la charge.
 * @li en @c etat4 : allumage vert  (@c VERT)  en fin de charge.
 * @li en @c etat5 : extinction     (@c OFF)   lors de la récupération.
 */
void Voyant::voyant_set_charge(led charge) {
    io->led_charge = charge;
}

/**
 * @details
 * Produit 4 cycles d'allumage/extinction du voyant de charge, chaque
 * demi-période durant 1 seconde via @c sleep(1), pour un clignotement
 * total de 8 secondes. Le voyant alterne entre @c VERT et @c OFF à chaque
 * itération via des appels à @c voyant_set_charge().
 *
 * Ce clignotement signale au client qu'il a été authentifié avec succès
 * et qu'il dispose de 60 secondes pour appuyer sur le bouton CHARGE.
 *
 * @note Appelée en @c etat0 immédiatement après une authentification réussie,
 *       avant le démarrage de la temporisation de 60 secondes.
 *
 * @warning Cette méthode est bloquante pendant 8 secondes — aucune instruction
 *          du programme n'est exécutée durant son déroulement. La temporisation
 *          de 60 secondes ne démarre qu'après son retour.
 *
 * @par Variables internes utilisées :
 * @li **i** : compteur de boucle de type int, itère de 0 à 3 inclus
 *             pour produire les 4 cycles de clignotement.
 */
void Voyant::voyant_blink_charge() {
    for (int i = 0; i < 4; i++) {
        voyant_set_charge(VERT) ;
        sleep(1) ;
        voyant_set_charge(OFF) ;
        sleep(1) ;
    }
}

/**
 * @details
 * Affecte directement la valeur de @p defaut au champ @c led_defaut du
 * registre matériel via @c io, sans traitement supplémentaire côté logiciel.
 *
 * @note Utilisée en interne par @c voyant_blink_defaut() pour alterner
 *       l'état du voyant de défaut durant le clignotement. Peut également
 *       être appelée directement pour fixer un état de défaut persistant.
 */
void Voyant::voyant_set_defaut(led defaut) {
    io->led_defaut = defaut;
}

/**
 * @details
 * Produit 4 cycles d'allumage/extinction du voyant de défaut, chaque
 * demi-période durant 1 seconde via @c sleep(1), pour un clignotement
 * total de 8 secondes. Le voyant alterne entre @c ROUGE et @c OFF à chaque
 * itération via des appels à @c voyant_set_defaut().
 *
 * Ce clignotement signale au client que son authentification a échoué —
 * carte inconnue ou numéro absent de la base clients — avant de retourner
 * en @c etat0 pour une nouvelle tentative.
 *
 * @note Appelée en @c etat0 immédiatement après un échec d'authentification,
 *       avant le retour automatique en début de @c etat0.
 *
 * @warning Cette méthode est bloquante pendant 8 secondes — aucune instruction
 *          du programme n'est exécutée durant son déroulement.
 *
 * @par Variables internes utilisées :
 * @li **i** : compteur de boucle de type int, itère de 0 à 3 inclus
 *             pour produire les 4 cycles de clignotement.
 */
void Voyant::voyant_blink_defaut() {
    for (int i = 0; i < 4; i++) {
        voyant_set_defaut(ROUGE) ;
        sleep(1) ;
        voyant_set_defaut(OFF) ;
        sleep(1) ;
    }
}

/**
 * @details
 * Affecte directement la valeur de @p prise au champ @c led_prise du
 * registre matériel via @c io, sans traitement supplémentaire côté logiciel.
 *
 * @note Ce champ @c led_prise est partagé avec la classe Prise via le
 *       même segment de mémoire partagée — toute écriture ici est donc
 *       immédiatement visible depuis @c prise_set_prise() et inversement.
 */
void Voyant::voyant_set_prise(led prise) {
    io->led_prise = prise;
}

/**
 * @details
 * Affecte directement la valeur de @p trappe au champ @c led_trappe du
 * registre matériel via @c io, sans traitement supplémentaire côté logiciel.
 *
 * @note Ce champ @c led_trappe est partagé avec la classe Prise via le
 *       même segment de mémoire partagée — toute écriture ici est donc
 *       immédiatement visible depuis @c prise_verrouille_trappe() et
 *       @c prise_deverrouille_trappe() et inversement.
 */
void Voyant::voyant_set_trappe(led trappe) {
    io->led_trappe = trappe;
}