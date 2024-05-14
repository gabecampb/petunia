#ifndef NODE_H
#define NODE_H

#include "common.h"

class Node {
	public:
	Vec3 pos;
	Vec3 scale;
	Vec4 rot;
	Model* model;
	std::vector<Node*> children;

	Node() {
		pos		= Vec3(0,0,0);
		scale	= Vec3(1,1,1);
		rot		= Vec4(0,0,0,1);
		model	= 0;
	}
};

#endif
