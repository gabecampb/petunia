#include "common.h"

Player* current_player;

Player::Player(std::string player_name) {
	name = player_name;
	health = 100;
}

void set_player(Player* player) {
	current_player = player;
}

Player* get_player() {
	return current_player;
}
