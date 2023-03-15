/////////////////////////////////
/// TP recherche arborescente ///
/////////////////////////////////

objectif : ecrire le meilleur programme qui joue à breakthrough 
(quel algorithme utilisé, comment implémenter une version rapide, comment améliorer le programme)
(tous les précalculs sont autorisés mais un seul cpu est autorisé pendant le tournoi)
(l'allocation mémoire est limitée à 1Go par programme)

Nous jouerons probablement à breakthrough 6x4 mais cela peut évoluer...
 
Nous jouerons tous ensemble en mode tournoi à double élimination.
Quand nous auront 2 ou 3 groupes restant, nous les ferons jouer tous contre tous et le moins bon sera éliminé à chaque round.
En cas d'égalité, nous regarderons les scores, puis les temps de calcul utilisé.

Chaque groupe de TP pourra inscrire un programme au tournoi.
Le seeding round sera un premier tour sans élimination.

(exemple pour 12 équipes)
SEEDING ROUND   ROUND-1                 ROUND-2     ROUND-3
=============   =====================   ==========  ==========
G1 vs G2        Victoire1 vs Victoire2  ...         ...
G3 vs G4        Victoire3 vs Victoire4  ...         ... (dès qu'une équipe à 2 défaite, elle est éliminée)
G5 vs G6        Victoire5 vs Victoire6  ...         
G7 vs G8        Defaite1 vs Defaite2    (3 élmininations)
G9 vs G10       Defaite3 vs Defaite4   
G11 vs G12      Defaite5 vs Defaite6   

A breakthrough, toutes les pièces sont des pions

Fichiers fournis pour ce TP :
* mybt.h définit les structures bt_piece_t, bt_move_t et bt_t
* bt_piece_t qui modélise une piece
* bt_move_t qui modélise un coup
* bt_t qui modélise le plateau et ses contraintes
* rand_player.cpp est un joueur aléaoire qui supporte le breakthrough text protocol btp
* le protocol btp permet de controler un programme pour jouer a breakthrough
* game1.txt est un exemple de fichier de commandes btp
* run_many_games.pike est un programme pike permettant de faire jouer ensemble des programmes supportant le btp
* Makefile permet de compiler le rand_player
* mk_stats.sh permet de lancer plusieurs parties, stocker les logs et les stats

Pour le moment, on posera pour contrainte de répondre le coup à jouer en 1 sec
Au delà de 1 sec, l'absence de réponse sera considérée comme un abandon

Au dela de la competition, chaque groupe doit rendre :
+ les sources de son programme
+ un rapport en 1 page (ou +) présentant les particularités/optimisations de son programme 
