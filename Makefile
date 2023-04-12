CC=g++
CFLAGS=-std=c++11 -Wall -O2

##### BREAKTHROUGH
CMBR_player: mybt.h mcts_player.cpp
	$(CC) $(CFLAGS) mcts_player.cpp -o $@

