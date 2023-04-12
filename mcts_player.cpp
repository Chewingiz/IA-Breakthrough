#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include "mybt.h"

bt_t B;
int boardwidth = 0;
int boardheight = 0;
bool white_turn = true;

#ifndef VERBOSE_RAND_PLAYER
#define VERBOSE_RAND_PLAYER
bool verbose = true;
bool showboard_at_each_move = false;
#endif

void print_int(int n);
void print_string(string truc);
void print_move(bt_move_t move);
void print_file(bt_t b);
Result selection(Node *racine, bt_t board);
void expansion(Node *selected, bt_t board);
bool simulation(bt_t board);
void backpropagation(Node *selected, bool simulation);
bool evaluate (bt_t board);

void print_string(string truc) {
  fstream my_file;
  my_file.open("my_file", fstream::app);
  my_file << truc << endl;
}

void print_int(int n) {
  fstream my_file;
  my_file.open("my_file", fstream::app);
  my_file << n << endl;
}

void print_move(bt_move_t move){
  fstream my_file;
  my_file.open("my_file", ios::out);
  my_file << move.line_i << " " << move.col_i << endl;
  my_file << move.line_f << " " << move.col_f << endl;
}

void print_file(bt_t b) {
  fstream my_file;
  my_file.open("my_file", ios::out);
  for (int i = 0; i < b.nbl; i++) {
    for (int j = 0; j < b.nbc; j++) {
      my_file << cboard[b.board[i][j]];

    }
    my_file << endl;
  }
}
float UCB1(Node *noeud) {
  return (noeud->wins / noeud->visit) +
         (0.4 * sqrt(log(noeud->parent->visit) / noeud->visit));
}

Result selection(Node *racine, bt_t board) {
  //fprintf(stderr, "coucou selecction");
  if (board.endgame() != EMPTY) {
    board.play(racine->move);
    return {board, racine};
  }
  if (racine->visit == 0 and racine->parent != NULL) {
    board.play(racine->move);
    return {board, racine};
  }
  float max = -1;
  Node *best = new Node;
  for (auto i : racine->children) {
    if (i->visit == 0) {
      board.play(i->move);
      return {board, i};
    }
    int new_eval = UCB1(i);
    if (new_eval > max) {

      max = new_eval;
      best = i;
    }
  }
  board.play(best->move);
  //fprintf(stderr, "end exp");
  return selection(best, board);
}
void expansion(Node *selected, bt_t board){
 // fprintf(stderr, "coucou exp");
  board.update_moves();
  for(int i=0;i<board.nb_moves;i++){
    Node *a = new Node;
    a->move=board.moves[i];
    a->wins=0;
    a->parent=selected;
    a->visit=0;
    selected->children.push_back(a);
  }
  board.nb_moves=0;
  fprintf(stderr, "end exp");
}
void help() {
  fprintf(stderr, "  quit\n");
  fprintf(stderr, "  echo ON | OFF\n");
  fprintf(stderr, "  help\n");
  fprintf(stderr, "  name <PLAYER_NAME>\n");
  fprintf(stderr, "  newgame <NBCOL> <NBLINE>\n");
  fprintf(stderr, "  genmove\n");
  fprintf(stderr, "  play <L0C0L1C1>\n");
  fprintf(stderr, "  showboard\n");
}
void name() {
  printf("= rand_player\n\n");
}
void newgame() {
  if((boardheight < 1 || boardheight > 10) && (boardwidth < 1 || boardwidth > 10)) {
    fprintf(stderr, "boardsize is %d %d ???\n", boardheight, boardwidth);
    printf("= \n\n");
    return;
  }
  B.init(boardheight, boardwidth);
  white_turn = true;
  if(verbose) fprintf(stderr, "ready to play on %dx%d board\n", boardheight, boardwidth);
  printf("= \n\n");
}
void showboard() {
  B.print_board(stderr);
  printf("= \n\n");
}
void genmove() {
  int ret = B.endgame();
  if(ret != EMPTY) {
    fprintf(stderr, "game finished\n");
    if(ret == WHITE) fprintf(stderr, "white player wins\n");
    else fprintf(stderr, "black player wins\n");
    printf("= \n\n");
    return;
  }
  bt_move_t m = B.mcts(999); // durée en millisecondes
  B.play(m);
  if(verbose) {
    m.print(stderr, white_turn, B.nbl);
    fprintf(stderr, "\n");
  }
  white_turn = !white_turn;
  printf("= %s\n\n", m.tostr(B.nbl).c_str());
}
void play(char a, char b, char c, char d) {
  bt_move_t m;
  m.line_i = boardheight-(a-'0');
  m.col_i = b-'a';
  m.line_f = boardheight-(c-'0');
  m.col_f = d-'a';
  if(B.can_play(m)) {
    B.play(m);
    if(verbose) {
      m.print(stderr, white_turn, B.nbl);
      fprintf(stderr, "\n");
    }
    white_turn = !white_turn;
  } else {
    fprintf(stderr, "CANT play %d %d %d %d ?\n", m.line_i, m.col_i, m.line_f, m.col_f);
  }
  if(showboard_at_each_move) showboard();
  printf("= \n\n");
}

bt_move_t best_move(Node* selected) {
  float score = 0;
  Node* best_child = new Node;
  for (auto i: selected->children) {
    if (UCB1(i) > score) {
      best_child = i;
    }
  }
  return best_child->move;
}

bt_move_t bt_t::mcts(double milli) {
  fprintf(stderr, "mcts\n");
  auto start_time = chrono::steady_clock::now();
  chrono::duration<double, std::milli> run_time;
  Node* tree = new Node;
  bool win;
  tree->parent = NULL;
  tree->visit = 0;
  tree->wins = 0;
  tree->children = {};
  expansion(tree, B);
  //int boucle = 0;
  //print_move(tree->children.at(0)->move);
  bt_t cpy_B = B;

  do {
    //boucle++;
    Result selectedNode = selection(tree, cpy_B);
    //print_string("selection");
    expansion(selectedNode.noeud, selectedNode.board);
    //print_string("expansion");
    win = evaluate(selectedNode.board);
    //print_string("simulation");
    backpropagation(selectedNode.noeud, win);
    //print_string("backpropagation");
    run_time = chrono::steady_clock::now() - start_time;
  } while (run_time.count() < milli);
  
  //print_int(boucle);
  //fprintf(stderr, "end mcts");
  return best_move(tree);
}

void backpropagation(Node* simulated, bool simulation) {
  if (simulation) {
    while (simulated != NULL) {
      simulated->visit += 1;
      simulated->wins += 1;
      simulated = simulated->parent;
    }
  } else {
    while (simulated != NULL) {
      simulated->visit += 1;
      simulated = simulated->parent;
    }
  }
}

bool simulation(bt_t board){
  int color = (board.turn%2==0)? WHITE: BLACK;
  int endgame_value = board.endgame();
  while(endgame_value == EMPTY){
    bt_move_t m = board.get_rand_move();
    board.play(m);
    endgame_value = board.endgame();
  }
  int winner_color = (board.turn%2 == 0)? WHITE: BLACK;
  return (winner_color == color)? true: false; // true for win, false for loss

}


/*Les blancs décendent et les noirs montent*/
/* Fonction qui fait une evaluation heuristique pour essayer de determiner le joueur dans la meilleur position*/
int evaluate_board(bt_t board) {
  //fprintf(stderr, "coucou eval");
  int white_score = 0;
  int black_score = 0;

  int value_trade= 1;
  int value_danger = -2;
  int value_can_eat = 2;
  int value_can_move = 1;
  for (int i = 0; i < board.nbc; i++) {
    for (int j = 0; j < board.nbl; j++) {
      if (board.board[i][j] == WHITE) {          // Evaluation pour le joueur blanc
        white_score += (board.nbl - i);  // plus la pièce est proche de la promotion plus le score est grand
        if((i+1 < board.nbl)&&(j+1 < board.nbc)){ //test si on peu bouger
          white_score += value_can_move;
          if (board.board[i+1][j+1] == BLACK) {//test regarde si on peu manger 
            white_score += value_can_eat;
          }
        }
        if((i+1 < board.nbl)&&(j-1 > 0)){ //test si on peu bouger
          white_score += value_can_move;
          if (board.board[i+1][j-1] == BLACK) {//test regarde si on peu manger 
            white_score += value_can_eat;
          }
        }
        //regarde si on est trade si on se fait manger
       if((i-1  > 0)&&(j+1 < board.nbc)){ 
          if (board.board[i-1][j+1] == WHITE){//+ gagner
            white_score += value_trade;
          }
        }
        if((i-1 > 0)&&(j-1 > 0)){ //test si on peu bouger
           if (board.board[i-1][j-1] == WHITE){//+ gagner
             white_score += value_trade;
           }

        }    
        
      }else if (board.board[i][j] == BLACK) {    // Evaluation pour le joueur noir
        black_score += i+1;
        if((i-1 > 0) && (j+1 < board.nbc)){ //test si on peu bouger
          black_score += value_can_move;
          if (board.board[i-1][j+1] == WHITE) {//test regarde si on peu manger 
            black_score += value_can_eat;
          }
        }
        if((i-1 > 0) && (j-1 > 0)){ //test si on peu bouger
          black_score += value_can_move;
          if (board.board[i-1][j-1] == WHITE) {//test regarde si on peu manger 
            black_score += value_can_eat;
          }
        }
        //regarde si on est trade si on se fait manger
       if((i+1 < board.nbl) && (j+1 < board.nbc)){ 
          if (board.board[i+1][j+1] == BLACK){
            black_score += value_trade;
          }
        }
        if((i+1 < board.nbl) && (j-1 > 0)){ 
          if (board.board[i+1][j-1] == BLACK){
            black_score += value_trade;
          }
        }
      }
    }
  }

  // Renvois le joueur avec la meilleure position en fonction du score
  //fprintf(stderr, "end eval");
  return (white_score > black_score)? WHITE: BLACK;
}

/*egalité de score == defaite*/
bool evaluate (bt_t board){
  int color = (board.turn%2==0)? WHITE: BLACK;
  int best_position_color = evaluate_board(board);
  return (best_position_color == color)? true: false; // true for win, false for loss

}


int main(int _ac, char** _av) {
  bool echo_on = false;
  setbuf(stdout, 0);
  setbuf(stderr, 0);
  if(verbose) fprintf(stderr, "rand_player started\n");
  char a,b,c,d; // for play cmd
  for (std::string line; std::getline(std::cin, line);) {
    if(verbose) fprintf(stderr, "rand_player receive %s\n", line.c_str());
    if(echo_on) if(verbose) fprintf(stderr, "%s\n", line.c_str());
    if(line.compare("quit") == 0) { printf("= \n\n"); break; }
    else if(line.compare("echo ON") == 0) echo_on = true;
    else if(line.compare("echo OFF") == 0) echo_on = false;
    else if(line.compare("help") == 0) help();
    else if(line.compare("name") == 0) name();
    else if(sscanf(line.c_str(), "newgame %d %d\n", &boardheight, &boardwidth) == 2) newgame();
    else if(line.compare("genmove") == 0) genmove();
    else if(sscanf(line.c_str(), "play %c%c%c%c\n", &a,&b,&c,&d) == 4) play(a,b,c,d);
    else if(line.compare("showboard") == 0) showboard();
    else if(line.compare(0,2,"//") == 0) ; // just comments
    else fprintf(stderr, "???\n");
    if(echo_on) printf(">");
  }
  if(verbose) fprintf(stderr, "bye.\n");

  return 0;
}
