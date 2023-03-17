#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <string>
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

Result selection(Node *racine, bt_t board);
void expansion(Node *selected, bt_t board);
bool simulation(bt_t board);
void backpropagation(Node *selected, bool simulation);

float UCB1(Node *noeud) {
  return (noeud->wins / noeud->visit) +
         (0.4 * sqrt(log(noeud->parent->visit) / noeud->visit));
}

Result selection(Node *racine, bt_t board) {
  if (board.endgame() != EMPTY) {
    board.play(racine->move);
    return {board, racine};
  }
  if (racine->visit == 0 and racine->parent != NULL) {
    board.play(racine->move);
    return {board, racine};
  }
  float max = -1;
  Node *best;
  for (auto i : racine->children) {
    if (i->visit == 0) {
      return {board, i};
    }
    int new_eval = UCB1(i);
    if (new_eval > max) {
      max = new_eval;
      best = i;
    }
  }
  board.play(best->move);
  return selection(best, board);
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
  bt_move_t m = B.mcts(0.95);
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
  Node* best_child;
  for (auto i: selected->children) {
    if (UCB1(i) > score) {
      best_child = i;
    }
  }
  return best_child->move;
}

bt_move_t bt_t::mcts(double _sec) {
  clock_t start_time = clock();
  clock_t run_time;
  Node* tree;
  tree->parent = NULL;
  tree->visit = 0;
  tree->wins = 0;
  bt_t cpy_B = B;

  do {
    Result selectedNode = selected(tree, cpy_B);
    expansion(selectedNode.noeud, selectedNode);
    backpropagation(selectedNode.noeud, simulation(selectedNode));
    run_time = clock() - start_time;
  } while (run_time < _sec)
  
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
