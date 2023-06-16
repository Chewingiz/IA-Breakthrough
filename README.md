# IA-Breakthrough
Intelligence artificielle en C++ pour jouer à Breakthrough avec MCTS (Monte Carlo Tree Search) avec une fonction d'évaluation.

# Paramètres
La fonction d'évaluation permet d'attribuer un score à la position des deux adversaires en fonction de l'état du plateau E. Les paramètres de la fonction d'évaluation peuvent être réglés pour influencer le comportement de l'IA. Par exemple :

   * Importance de la capture d'un adversaire : en accordant plus de points à la capture d'un adversaire, l'IA deviendra plus agressive en cherchant à éliminer les pièces adverses, ce qui peut diminuer leurs chances d'atteindre la promotion.
  *  Importance de la mobilité des pièces : en attribuant une importance élevée à la mobilité des pièces, l'IA sera encouragée à garder ses pièces mobiles et à les maintenir au centre du plateau pour avoir plus d'options stratégiques.
  *  Importance de la distance à la promotion : en accordant une importance élevée à la distance à la promotion, l'IA pourra évaluer quelle couleur est la plus proche de la victoire sur un plateau donné.
  *  Importance de la vengeance : si un pion est capturé, cette valeur permet de décider si l'IA doit chercher à le récupérer ou si elle peut le "sacrifier" en échange d'une meilleure position stratégique.

Ces paramètres peuvent être ajustés selon les besoins pour modifier le comportement de l'IA et adapter sa stratégie de jeu.


# Comment créé le joueur?
Faire make pour créé l'executatble CMBR_player qui correspond à notre joueur.

# Comment le faire fonctionner ?
executable joueur = J1 

executable joueur = J2

```
pike run_many_games.pike -f ./J1 -s ./J2 -v 1 -p 1 -l 6 -c 4
```
avec les options : 

```
//  "Options:\n"
//  "  -n, --number=NB_GAMES         the number of games to play\n"
//  "  -l, --nbl=NB_LINES            the number of lines on the board\n"
//  "  -c, --nbc=NB_COLS             the number of cols on the board\n"
//  "  -f, --first=COMMAND_LINE\n"
//  "  -s, --second=COMMAND_LINE     command lines to run the two engines with.\n\n"
//  "  -o, --outputdir=OUTPUT_DIRECTORY (default ouput is data)\n"
//  "      --help                    display this help and exit.\n"
//  "  -v, --verbose=LEVEL           1 - print moves, 2 and higher - draw boards.\n"
//  "  -p, --pause=SECONDS           1 - sleep(1), 2 and more - sleep(2 and more).\n";
```



