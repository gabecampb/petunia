#ifndef NODE_H
#define NODE_H

#include "common.h"



class Node {
	protected:
	Vec3 pos, size;
	Vec4 rot;
	Mat4 local, global;
	std::vector<Node*> children;
	Node* parent;

	void update_transform();

	public:
	Model* model;

	Node();
	Vec3 get_pos();
	Vec3 get_scale();
	Vec4 get_rot();
	Mat4 get_local();
	Mat4 get_global();
	Node* get_child(u32 idx);
	u32 get_child_count();
	void set_pos(Vec3 p);
	void set_scale(Vec3 s);
	void set_rot(Vec4 q);
	void add_child(Node* node);
};


#endif
