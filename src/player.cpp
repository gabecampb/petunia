#include "common.h"

Player* current_player;

Player::Player(std::string player_name) {
	type = NodeType::TYPE_PLAYER;
	plr_name = player_name;
	health = 100;
}

Player::~Player() {
	if(current_player == this)
		current_player = 0;
}

void set_player(Player* player) {
	current_player = player;
}

Player* get_player() {
	return current_player;
}
