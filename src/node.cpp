#include "common.h"

Node::Node() {
	pos		= Vec3(0,0,0);
	size	= Vec3(1,1,1);
	rot		= Vec4(0,0,0,1);
	model	= 0;
	parent	= 0;
	update_transform();
}

void Node::update_transform() {
	local = translate(pos) * (quat_to_mat4(rot) * scale(size));

	if(parent)
		global = parent->global * local;
	else
		global = local;
	for(u32 i = 0; i < children.size(); i++)
		children.at(i)->update_transform();
}

Vec3 Node::get_pos()			{ return pos; }
Vec3 Node::get_scale()			{ return size; }
Vec4 Node::get_rot()			{ return rot; }
Mat4 Node::get_local()			{ return local; }
Mat4 Node::get_global()			{ return global; }
Node* Node::get_child(u32 idx)	{ return children.at(idx); }
u32 Node::get_child_count()		{ return children.size(); }

void Node::set_pos(Vec3 p) {
	pos = p;
	update_transform();
}

void Node::set_scale(Vec3 s) {
	size = s;
	update_transform();
}

void Node::set_rot(Vec4 q) {
	rot = q;
	update_transform();
}

void Node::add_child(Node* node) {
	if(!node || node->parent)
		return;
	children.push_back(node);
	node->parent = this;
	node->update_transform();
}
