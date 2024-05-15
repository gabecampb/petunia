#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

class Player : public Node {
	public:
	std::string name;
	f32 health;

	Player(std::string player_name);
};

void set_player(Player*);
Player* get_player();

#endif