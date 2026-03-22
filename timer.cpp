/**
 * @file timer.cpp
 * @brief Implémentation des méthodes de la classe Timer.
 *
 * @details
 * Ce fichier contient l'implémentation des méthodes de temporisation
 * de la borne. La mesure du temps repose sur deux mécanismes combinés :
 * @li un registre matériel @c timer_sec accessible via @c io_t, mis à
 *     jour à chaque appel à @c timer_count_sec() pour refléter le temps
 *     écoulé côté hardware.
 * @li un horodatage logiciel via @c time() et @c depart_timer, qui sert
 *     de référence pour calculer les intervalles de temps entre deux appels.
 */

#include "timer.h"

/**
 * @details
 * Récupère l'adresse du segment de mémoire partagée via @c acces_memoire()
 * et l'affecte au pointeur @c io_t. À partir de ce moment, toutes les
 * interactions avec le registre timer matériel transitent par ce pointeur
 * sans nouvel accès au mécanisme de mémoire partagée.
 *
 * @par Variables internes utilisées :
 * @li **shmid_t** : identifiant du segment de mémoire partagée, passé
 *                   par adresse à @c acces_memoire() pour être initialisé.
 */
Timer::Timer() {
    int shmid_t ;
    io_t = acces_memoire(&shmid_t) ;
}

/**
 * @details
 * Effectue une double remise à zéro :
 * @li le registre matériel @c timer_sec est forcé à 0 via @c io_t,
 *     ce qui réinitialise le compteur côté hardware.
 * @li @c depart_timer est mis à jour avec l'instant courant via @c time(NULL),
 *     ce qui constitue le nouveau point de départ pour les calculs
 *     d'intervalles effectués par @c timer_valeur().
 *
 * @note Appelée en @c etat0 juste avant la boucle d'attente d'appui bouton,
 *       pour garantir que la temporisation de 60 secondes part bien de zéro
 *       à chaque nouveau cycle client.
 */
void Timer::timer_raz() {
    io_t->timer_sec = 0 ;
    depart_timer = time(NULL) ;
}

/**
 * @details
 * Attend une seconde via @c sleep(1), puis calcule la différence entre
 * l'instant courant et @c depart_timer. Cette différence représente le
 * temps réellement écoulé depuis le dernier appel à cette méthode.
 * @c depart_timer est ensuite mis à jour avec l'instant courant pour
 * préparer le prochain appel.
 *
 * @note L'appel à @c sleep(1) est intentionnel — il cadence la boucle
 *       d'attente dans @c etat0 à une itération par seconde, évitant
 *       ainsi une occupation CPU inutile durant la temporisation.
 *
 * @warning La valeur retournée peut légèrement dépasser 1 seconde en cas
 *          de charge système importante, @c sleep() n'étant pas une
 *          fonction temps réel garantie à la milliseconde près.
 *
 * @par Variables internes utilisées :
 * @li **now**  : horodatage de l'instant courant obtenu via @c time(NULL).
 * @li **diff** : différence en secondes entre @c now et @c depart_timer.
 *
 * @return int  Nombre de secondes écoulées depuis le dernier appel.
 */
int Timer::timer_valeur() {
    sleep(1) ;
    time_t now = time(NULL) ;
    int diff = now - depart_timer ;
    depart_timer = now ;
    return diff ;
}

/**
 * @details
 * Appelle @c timer_valeur() pour obtenir le nombre de secondes écoulées
 * depuis le dernier appel, puis cumule cette valeur dans le registre
 * matériel @c timer_sec via @c io_t, assurant ainsi la cohérence entre
 * le compteur logiciel et le compteur hardware.
 *
 * C'est cette méthode qui est appelée à chaque tour de la boucle d'attente
 * dans @c etat0 — sa valeur de retour est accumulée dans @c timer_secs
 * jusqu'à atteindre 60 secondes ou détecter un appui sur le bouton CHARGE.
 *
 * @par Variables internes utilisées :
 * @li **timer_value** : entier stockant temporairement la valeur retournée
 *                       par @c timer_valeur() avant cumul et retour.
 *
 * @return int  Nombre de secondes écoulées depuis le dernier appel à @c timer_valeur().
 */
int Timer::timer_count_sec() {
    int timer_value = timer_valeur() ;
    io_t->timer_sec += timer_value ;
    return (timer_value) ;
}