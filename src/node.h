#ifndef NODE_H
#define NODE_H

#include "common.h"

enum NodeType {
	TYPE_NODE,
	TYPE_CAMERA,
	TYPE_PLAYER
};

class Node {
	protected:
	Vec3 pos, size;
	Vec4 rot;
	Mat4 local, global;
	std::vector<Node*> children;
	Node* parent;
	NodeType type;

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
	NodeType get_type();
	void set_pos(Vec3 p);
	void set_scale(Vec3 s);
	void set_rot(Vec4 q);
	void add_child(Node* node);
	Node* remove_child(u32 idx);
	void queue_to_free();
	void free_children();
};

void node_cleanup();

#endif
