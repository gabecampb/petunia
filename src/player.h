#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

class Player : public Node {
	public:
	std::string plr_name;
	f32 health;

	Player(std::string player_name);
	~Player();
};

void set_player(Player*);
Player* get_player();

#endif
