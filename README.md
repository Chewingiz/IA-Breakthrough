# IA-Breakthrough
Intelligence artificielle en C++ pour jouer à Breakthrough avec MCTS (Monte Carlo Tree Search)

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


